#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

static const string DETECTOR_MODEL   = "face_detection_yunet_2023mar.onnx";
static const string RECOGNIZER_MODEL = "face_recognition_sface_2021dec.onnx";
static const string DB_PATH          = "face_db.csv";

static const double COSINE_THRESHOLD = 0.363;

static const double DETECTION_SCALE = 0.5;

struct KnownFace
{
    string name;
    Mat embedding; 
};

vector<KnownFace> loadDatabase(const string& path)
{
    vector<KnownFace> db;
    ifstream file(path);
    if (!file.is_open())
        return db; 

    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;

        getline(ss, token, ',');
        KnownFace kf;
        kf.name = token;

        vector<float> values;
        while (getline(ss, token, ','))
            values.push_back(stof(token));

        if (values.empty()) continue;

        kf.embedding = Mat(1, (int)values.size(), CV_32F);
        for (size_t i = 0; i < values.size(); i++)
            kf.embedding.at<float>(0, (int)i) = values[i];

        db.push_back(kf);
    }
    return db;
}

void appendToDatabase(const string& path, const KnownFace& kf)
{
    ofstream file(path, ios::app);
    file << kf.name;
    for (int i = 0; i < kf.embedding.cols; i++)
        file << "," << kf.embedding.at<float>(0, i);
    file << "\n";
}

struct MatchResult
{
    string name;
    double score;
    bool recognized;
};

MatchResult identify(const Ptr<FaceRecognizerSF>& recognizer,
                      const Mat& liveEmbedding,
                      const vector<KnownFace>& db)
{
    MatchResult best{ "Unknown", -1.0, false };

    for (const KnownFace& kf : db)
    {
        double score = recognizer->match(liveEmbedding, kf.embedding,
                                          FaceRecognizerSF::FR_COSINE);
        if (score > best.score)
        {
            best.score = score;
            best.name  = kf.name;
        }
    }

    if (best.score > COSINE_THRESHOLD)
        best.recognized = true;
    else
    {
        best.name = "Unknown";
    }

    return best;
}

int runEnroll(const string& name,
              const Ptr<FaceDetectorYN>& detector,
              const Ptr<FaceRecognizerSF>& recognizer)
{
    VideoCapture camera(0);
    if (!camera.isOpened())
    {
        cout << "Error: Could not open webcam!" << endl;
        return -1;
    }

    Mat frame;
    camera >> frame;

    Size smallSize(cvRound(frame.cols * DETECTION_SCALE),
                    cvRound(frame.rows * DETECTION_SCALE));
    detector->setInputSize(smallSize);

    cout << "Enrolling '" << name << "'." << endl;
    cout << "Press 'c' to capture a sample, 'q' when done." << endl;

    int samples = 0;

    while (true)
    {
        camera >> frame;
        if (frame.empty()) break;

        Mat smallFrame;
        resize(frame, smallFrame, smallSize);

        Mat faces;
        detector->detect(smallFrame, faces);

        if (faces.rows > 0)
            faces.colRange(0, 14) *= (1.0 / DETECTION_SCALE);

        Rect box;
        bool haveFace = faces.rows > 0;
        if (haveFace)
        {
            box = Rect((int)faces.at<float>(0, 0), (int)faces.at<float>(0, 1),
                       (int)faces.at<float>(0, 2), (int)faces.at<float>(0, 3));
            rectangle(frame, box, Scalar(0, 255, 0), 2);
        }

        putText(frame, "Samples captured: " + to_string(samples),
                Point(20, 40), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255), 2);
        putText(frame, "[c] capture   [q] finish",
                Point(20, 75), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);

        imshow("Enroll", frame);
        int key = waitKey(1);

        if (key == 'c' && haveFace)
        {
            Mat aligned, embedding;
            recognizer->alignCrop(frame, faces.row(0), aligned);
            recognizer->feature(aligned, embedding);

            KnownFace kf;
            kf.name = name;
            kf.embedding = embedding.clone();
            appendToDatabase(DB_PATH, kf);

            samples++;
            cout << "Captured sample " << samples << " for " << name << endl;
        }
        else if (key == 'q')
        {
            break;
        }
    }

    camera.release();
    destroyAllWindows();

    if (samples == 0)
        cout << "No samples captured — nothing saved." << endl;
    else
        cout << "Done. " << samples << " sample(s) saved to " << DB_PATH << endl;

    return 0;
}

int runRecognize(const Ptr<FaceDetectorYN>& detector,
                  const Ptr<FaceRecognizerSF>& recognizer)
{
    vector<KnownFace> db = loadDatabase(DB_PATH);
    cout << "Loaded " << db.size() << " enrolled sample(s) from "
         << DB_PATH << endl;
    if (db.empty())
        cout << "Warning: database is empty. Run with 'enroll <name>' first." << endl;

    VideoCapture camera(0);
    if (!camera.isOpened())
    {
        cout << "Error: Could not open webcam!" << endl;
        return -1;
    }

    int frameWidth  = (int)camera.get(CAP_PROP_FRAME_WIDTH);
    int frameHeight = (int)camera.get(CAP_PROP_FRAME_HEIGHT);

    cout << "Face recognition started..." << endl;
    cout << "Resolution: " << frameWidth << "x" << frameHeight << endl;
    cout << "Press 'q' to quit." << endl;

    Mat frame;
    Size smallSize(cvRound(frameWidth * DETECTION_SCALE),
                    cvRound(frameHeight * DETECTION_SCALE));
    detector->setInputSize(smallSize);

    int frameCount = 0;
    double fps = 0.0;
    double startTime = (double)getTickCount();

    while (true)
    {
        camera >> frame;
        if (frame.empty())
        {
            cout << "Error: Empty frame!" << endl;
            break;
        }

        Mat smallFrame;
        resize(frame, smallFrame, smallSize);

        Mat faces;
        detector->detect(smallFrame, faces);

        if (faces.rows > 0)
            faces.colRange(0, 14) *= (1.0 / DETECTION_SCALE);

        for (int i = 0; i < faces.rows; i++)
        {
            Rect box((int)faces.at<float>(i, 0), (int)faces.at<float>(i, 1),
                     (int)faces.at<float>(i, 2), (int)faces.at<float>(i, 3));

            Mat aligned, embedding;
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
        if (elapsedTime >= 1.0)
        {
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

int main(int argc, char** argv)
{
    Ptr<FaceDetectorYN> detector = FaceDetectorYN::create(
        DETECTOR_MODEL, "", Size(320, 320), 0.9f, 0.3f, 5000);

    if (detector.empty())
    {
        cout << "Error: Could not load face detector model ("
             << DETECTOR_MODEL << ")" << endl;
        return -1;
    }

    Ptr<FaceRecognizerSF> recognizer = FaceRecognizerSF::create(
        RECOGNIZER_MODEL, "");

    if (recognizer.empty())
    {
        cout << "Error: Could not load face recognizer model ("
             << RECOGNIZER_MODEL << ")" << endl;
        return -1;
    }

    if (argc >= 3 && string(argv[1]) == "enroll")
    {
        return runEnroll(argv[2], detector, recognizer);
    }

    return runRecognize(detector, recognizer);
}