#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "config.h"
#include "enroll.h"
#include "face_db.h"
#include "recognize.h"

using namespace cv;
using namespace std;

void printUsage()
{
    cout << "Usage:" << endl;
    cout << "  FaceDetection                 Run live recognition against face_db.csv" << endl;
    cout << "  FaceDetection enroll <name>   Capture samples for <name> into face_db.csv" << endl;
    cout << "  FaceDetection list            List enrolled names and sample counts" << endl;
    cout << "  FaceDetection delete <name>   Remove all samples for <name>" << endl;
    cout << "  FaceDetection help            Show this message" << endl;
}

int main(int argc, char** argv)
{

    vector<string> args(argv + 1, argv + argc);

    if (!args.empty() && (args[0] == "help" || args[0] == "-h" || args[0] == "--help"))
    {
        printUsage();
        return 0;
    }

    if (!args.empty() && args[0] == "list")
    {
        listDatabase(config::DB_PATH);
        return 0;
    }

    if (!args.empty() && args[0] == "delete")
    {
        if (args.size() < 2)
        {
            cout << "Error: 'delete' requires a name, e.g. 'FaceDetection delete Aryan'" << endl;
            printUsage();
            return -1;
        }
        int removed = deleteFromDatabase(config::DB_PATH, args[1]);
        if (removed == 0)
            cout << "No entries found for '" << args[1] << "'." << endl;
        else
            cout << "Removed " << removed << " sample(s) for '" << args[1] << "'." << endl;
        return 0;
    }

    if (!args.empty() && args[0] == "enroll" && args.size() < 2)
    {
        cout << "Error: 'enroll' requires a name, e.g. 'FaceDetection enroll Aryan'" << endl;
        printUsage();
        return -1;
    }

    if (!args.empty() && args[0] != "enroll")
    {
        cout << "Error: unknown command '" << args[0] << "'" << endl;
        printUsage();
        return -1;
    }

    Ptr<FaceDetectorYN> detector = FaceDetectorYN::create(
        config::DETECTOR_MODEL, "", Size(320, 320), 0.9f, 0.3f, 5000);
    if (detector.empty())
    {
        cout << "Error: Could not load face detector model ("
             << config::DETECTOR_MODEL << ")" << endl;
        return -1;
    }

    Ptr<FaceRecognizerSF> recognizer = FaceRecognizerSF::create(
        config::RECOGNIZER_MODEL, "");
    if (recognizer.empty())
    {
        cout << "Error: Could not load face recognizer model ("
             << config::RECOGNIZER_MODEL << ")" << endl;
        return -1;
    }

    if (!args.empty() && args[0] == "enroll")
        return runEnroll(args[1], detector, recognizer);

    return runRecognize(detector, recognizer);
}