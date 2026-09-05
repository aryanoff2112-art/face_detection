#pragma once

#include <opencv2/core.hpp>
#include <string>
#include <vector>

struct KnownFace
{
    std::string name;
    cv::Mat embedding;
};

std::vector<KnownFace> loadDatabase(const std::string& path);

void appendToDatabase(const std::string& path, const KnownFace& kf);

int deleteFromDatabase(const std::string& path, const std::string& name);

void listDatabase(const std::string& path);
