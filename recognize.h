#pragma once

#include <opencv2/objdetect.hpp>

int runRecognize(const cv::Ptr<cv::FaceDetectorYN>& detector,
                  const cv::Ptr<cv::FaceRecognizerSF>& recognizer);