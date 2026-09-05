#include "util.h"

using namespace cv;

Rect clampRect(const Rect& r, const Size& frameSize)
{
    return r & Rect(0, 0, frameSize.width, frameSize.height);
}

bool openCamera(VideoCapture& camera)
{
    camera.open(0);
    if (!camera.isOpened())
        return false;
    camera.set(CAP_PROP_BUFFERSIZE, 1);
    return true;
}