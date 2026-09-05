# Dependencies

## OpenCV

This project requires OpenCV built with the `objdetect` module (which provides `FaceDetectorYN` and `FaceRecognizerSF`). Most recent OpenCV distributions (4.5.4+) include these out of the box.

- **MSYS2/MinGW:** `pacman -S mingw-w64-ucrt-x86_64-opencv`
- **Windows (vcpkg):** `vcpkg install opencv`
- **macOS (Homebrew):** `brew install opencv`
- **Linux (apt):** `sudo apt install libopencv-dev`

## Model Files

The face detector and recognizer are DNN models distributed by OpenCV's [opencv_zoo](https://github.com/opencv/opencv_zoo) project. They are **not** committed to this repository (they're binary blobs and don't belong in git history) — download them separately:

| File | Purpose | Source |
|---|---|---|
| `face_detection_yunet_2023mar.onnx` | Face detection | [opencv_zoo/models/face_detection_yunet](https://github.com/opencv/opencv_zoo/tree/main/models/face_detection_yunet) |
| `face_recognition_sface_2021dec.onnx` | Face recognition / embeddings | [opencv_zoo/models/face_recognition_sface](https://github.com/opencv/opencv_zoo/tree/main/models/face_recognition_sface) |

After downloading, place both files in the same directory you run the `FaceDetection` executable from (they are loaded via relative paths in `main.cpp`).

## Data File

`face_db.csv` stores enrolled face embeddings (one row per sample: `name,e0,e1,...,e127`). It is created automatically the first time you run `FaceDetection enroll <name>` and is intentionally excluded from git via `.gitignore`, since it can contain other people's biometric data.