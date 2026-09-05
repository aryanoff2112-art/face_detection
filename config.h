#pragma once

#include <string>

namespace config
{
    inline const std::string DETECTOR_MODEL = "face_detection_yunet_2023mar.onnx";
    inline const std::string RECOGNIZER_MODEL = "face_recognition_sface_2021dec.onnx";
    inline const std::string DB_PATH = "face_db.csv";

    inline constexpr double COSINE_THRESHOLD = 0.363;

    inline constexpr double DETECTION_SCALE = 0.5;
}