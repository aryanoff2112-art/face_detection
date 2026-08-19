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

    cout << "Face detection started..." << endl;
    cout << "Press 'q' to quit." << endl;

    Mat frame, grayFrame;

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

        putText(
            frame,
            "Faces: " + to_string(faces.size()),
            Point(20, 40),
            FONT_HERSHEY_SIMPLEX,
            1,
            Scalar(0, 0, 255),
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