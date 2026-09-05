#include "enroll.h"

#include <iostream>

#include "config.h"
#include "face_db.h"
#include "util.h"

using namespace cv;
using namespace std;

int runEnroll(const string& name,
              const Ptr<FaceDetectorYN>& detector,
              const Ptr<FaceRecognizerSF>& recognizer)
{
    VideoCapture camera;
    if (!openCamera(camera)){
        cout << "Error: Could not open webcam!" << endl;
        return -1;
    }

    Mat frame;
    camera >> frame;
    if (frame.empty()){
        cout << "Error: Could not read from webcam!" << endl;
        return -1;
    }

    Size smallSize(cvRound(frame.cols * config::DETECTION_SCALE),
                   cvRound(frame.rows * config::DETECTION_SCALE));
    detector->setInputSize(smallSize);

    cout << "Enrolling '" << name << "'." << endl;
    cout << "Press 'c' to capture a sample, 'q' when done." << endl;

    Mat smallFrame, faces, aligned, embedding;
    int samples = 0;

    while (true){
        camera >> frame;
        if (frame.empty()) break;

        resize(frame, smallFrame, smallSize);
        detector->detect(smallFrame, faces);
        if (faces.rows > 0)
            faces.colRange(0, 14) *= (1.0 / config::DETECTION_SCALE);

        Rect box;
        bool haveFace = faces.rows > 0;
        if (haveFace){
            box = clampRect(
                Rect((int)faces.at<float>(0, 0), (int)faces.at<float>(0, 1),
                     (int)faces.at<float>(0, 2), (int)faces.at<float>(0, 3)),
                frame.size());
            rectangle(frame, box, Scalar(0, 255, 0), 2);
        }

        putText(frame, "Samples captured: " + to_string(samples),
                Point(20, 40), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255), 2);
        putText(frame, "[c] capture [q] finish",
                Point(20, 75), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);

        imshow("Enroll", frame);
        int key = waitKey(1);

        if (key == 'c' && haveFace){
            recognizer->alignCrop(frame, faces.row(0), aligned);
            recognizer->feature(aligned, embedding);

            KnownFace kf;
            kf.name = name;
            kf.embedding = embedding.clone();
            appendToDatabase(config::DB_PATH, kf);
            samples++;
            cout << "Captured sample " << samples << " for " << name << endl;
        }
        else if (key == 'q'){
            break;
        }
    }

    camera.release();
    destroyAllWindows();

    if (samples == 0)
        cout << "No samples captured -- nothing saved." << endl;
    else
        cout << "Done. " << samples << " sample(s) saved to " << config::DB_PATH << endl;

    return 0;
}