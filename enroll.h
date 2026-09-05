#pragma once

#include <opencv2/objdetect.hpp>
#include <string>

int runEnroll(const std::string& name,
              const cv::Ptr<cv::FaceDetectorYN>& detector,
              const cv::Ptr<cv::FaceRecognizerSF>& recognizer);