#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    CascadeClassifier faceCascade;

    if (!faceCascade.load("haarcascade_frontalface_default.xml"))
    {
        cout << "Error: Could not load Haar Cascade file!" << endl;
        return -1;
    }

    VideoCapture camera(0);

    if (!camera.isOpened())
    {
        cout << "Error: Could not open webcam!" << endl;
        return -1;
    }

    int frameWidth = (int)camera.get(CAP_PROP_FRAME_WIDTH);
    int frameHeight = (int)camera.get(CAP_PROP_FRAME_HEIGHT);

    cout << "Face detection started..." << endl;
    cout << "Resolution: " << frameWidth << "x" << frameHeight << endl;
    cout << "Press 'q' to quit." << endl;

    Mat frame, grayFrame;

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

        cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

        equalizeHist(grayFrame, grayFrame);

        vector<Rect> faces;

        faceCascade.detectMultiScale(
            grayFrame,
            faces,
            1.1,
            5,
            0,
            Size(30, 30)
        );

        for (const Rect& face : faces)
        {
            rectangle(
                frame,
                face,
                Scalar(0, 255, 0),
                2
            );

            putText(
                frame,
                "Face",
                Point(face.x, face.y - 10),
                FONT_HERSHEY_SIMPLEX,
                0.8,
                Scalar(0, 255, 0),
                2
            );
        }

        frameCount++;

        double elapsedTime =
            ((double)getTickCount() - startTime) / getTickFrequency();

        if (elapsedTime >= 1.0)
        {
            fps = frameCount / elapsedTime;

            frameCount = 0;
            startTime = (double)getTickCount();
        }

        putText(
            frame,
            "Faces: " + to_string(faces.size()),
            Point(20, 40),
            FONT_HERSHEY_SIMPLEX,
            0.8,
            Scalar(0, 0, 255),
            2
        );

        putText(
            frame,
            "FPS: " + to_string((int)fps),
            Point(20, 75),
            FONT_HERSHEY_SIMPLEX,
            0.8,
            Scalar(255, 255, 0),
            2
        );

        putText(
            frame,
            "Resolution: " +
            to_string(frame.cols) + "x" +
            to_string(frame.rows),
            Point(20, 110),
            FONT_HERSHEY_SIMPLEX,
            0.8,
            Scalar(255, 255, 255),
            2
        );

        imshow("Face Detection", frame);

        if (waitKey(1) == 'q')
        {
            break;
        }
    }

    camera.release();
    destroyAllWindows();

    return 0;
}