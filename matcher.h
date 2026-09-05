#pragma once

#include <opencv2/objdetect.hpp>
#include <string>
#include <vector>

#include "face_db.h"

struct MatchResult
{
    std::string name;
    double score;
    bool recognized;
};

MatchResult identify(const cv::Ptr<cv::FaceRecognizerSF>& recognizer,
                      const cv::Mat& liveEmbedding,
                      const std::vector<KnownFace>& db);