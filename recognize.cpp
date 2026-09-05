#include "recognize.h"

#include <iostream>

#include "config.h"
#include "face_db.h"
#include "matcher.h"
#include "util.h"

using namespace cv;
using namespace std;

int runRecognize(const Ptr<FaceDetectorYN>& detector,
                  const Ptr<FaceRecognizerSF>& recognizer)
{
    vector<KnownFace> db = loadDatabase(config::DB_PATH);
    cout << "Loaded " << db.size() << " enrolled sample(s) from "
         << config::DB_PATH << endl;
    if (db.empty())
        cout << "Warning: database is empty. Run with 'enroll <name>' first." << endl;

    VideoCapture camera;
    if (!openCamera(camera)){
        cout << "Error: Could not open webcam!" << endl;
        return -1;
    }

    int frameWidth = (int)camera.get(CAP_PROP_FRAME_WIDTH);
    int frameHeight = (int)camera.get(CAP_PROP_FRAME_HEIGHT);

    cout << "Face recognition started..." << endl;
    cout << "Resolution: " << frameWidth << "x" << frameHeight << endl;
    cout << "Press 'q' to quit." << endl;

    Size smallSize(cvRound(frameWidth * config::DETECTION_SCALE),
                   cvRound(frameHeight * config::DETECTION_SCALE));
    detector->setInputSize(smallSize);

    Mat frame, smallFrame, faces, aligned, embedding;
    int frameCount = 0;
    double fps = 0.0;
    double startTime = (double)getTickCount();

    while (true){
        camera >> frame;
        if (frame.empty()){
            cout << "Error: Empty frame!" << endl;
            break;
        }

        resize(frame, smallFrame, smallSize);
        detector->detect(smallFrame, faces);
        if (faces.rows > 0)
            faces.colRange(0, 14) *= (1.0 / config::DETECTION_SCALE);

        for (int i = 0; i < faces.rows; i++){
            Rect box = clampRect(
                Rect((int)faces.at<float>(i, 0), (int)faces.at<float>(i, 1),
                     (int)faces.at<float>(i, 2), (int)faces.at<float>(i, 3)),
                frame.size());

            recognizer->alignCrop(frame, faces.row(i), aligned);
            recognizer->feature(aligned, embedding);

            MatchResult match = identify(recognizer, embedding, db);
            Scalar color = match.recognized ? Scalar(0, 255, 0) : Scalar(0, 0, 255);
            rectangle(frame, box, color, 2);

            char scoreText[64];
            snprintf(scoreText, sizeof(scoreText), "%s (%.0f%%)",
                     match.name.c_str(), match.score * 100.0);
            putText(frame, scoreText, Point(box.x, box.y - 10),
                    FONT_HERSHEY_SIMPLEX, 0.7, color, 2);
        }

        frameCount++;
        double elapsedTime = ((double)getTickCount() - startTime) / getTickFrequency();
        if (elapsedTime >= 1.0){
            fps = frameCount / elapsedTime;
            frameCount = 0;
            startTime = (double)getTickCount();
        }

        putText(frame, "Faces: " + to_string(faces.rows),
                Point(20, 40), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255), 2);
        putText(frame, "FPS: " + to_string((int)fps),
                Point(20, 75), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 0), 2);
        putText(frame, "Resolution: " + to_string(frame.cols) + "x" + to_string(frame.rows),
                Point(20, 110), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 255), 2);

        imshow("Face Recognition", frame);
        if (waitKey(1) == 'q')
            break;
    }

    camera.release();
    destroyAllWindows();
    return 0;
}