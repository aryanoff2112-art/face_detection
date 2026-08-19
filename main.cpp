#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    // Load Haar Cascade for face detection
    CascadeClassifier faceCascade;

    if (!faceCascade.load("haarcascade_frontalface_default.xml"))
    {
        cout << "Error: Could not load Haar Cascade file!" << endl;
        return -1;
    }

    // Open webcam
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
        // Capture frame from webcam
        camera >> frame;

        if (frame.empty())
        {
            cout << "Error: Empty frame!" << endl;
            break;
        }

        // Convert frame to grayscale
        cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

        // Improve contrast
        equalizeHist(grayFrame, grayFrame);

        // Store detected faces
        vector<Rect> faces;

        // Detect faces
        faceCascade.detectMultiScale(
            grayFrame,
            faces,
            1.1,
            5,
            0,
            Size(30, 30)
        );

        // Draw rectangle around each detected face
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

        // Display number of detected faces
        putText(
            frame,
            "Faces: " + to_string(faces.size()),
            Point(20, 40),
            FONT_HERSHEY_SIMPLEX,
            1,
            Scalar(0, 0, 255),
            2
        );

        // Show webcam window
        imshow("Face Detection", frame);

        // Press q to exit
        if (waitKey(1) == 'q')
        {
            break;
        }
    }

    // Release webcam and close windows
    camera.release();
    destroyAllWindows();

    return 0;
}