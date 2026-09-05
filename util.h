#pragma once

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

cv::Rect clampRect(const cv::Rect& r, const cv::Size& frameSize);

bool openCamera(cv::VideoCapture& camera);