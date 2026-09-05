# Face Detection & Recognition in C++ Using OpenCV

A real-time **face detection and recognition** application built with **C++ and OpenCV**, using OpenCV's DNN-based **YuNet** face detector and **SFace** face recognizer. The app can enroll people from a webcam and then recognize them live, drawing labeled bounding boxes with a match confidence score.

> Note: earlier versions of this README described a Haar Cascade based detector. The project has since moved to OpenCV's YuNet/SFace DNN models, which are faster and significantly more accurate. This README now reflects the current code.

## 📌 Features

- Real-time face detection using OpenCV's **YuNet** DNN detector
- Face **recognition** against an enrolled database using OpenCV's **SFace** recognizer
- Enroll new people from the webcam (`enroll <name>`)
- List and delete enrolled people (`list`, `delete <name>`)
- Draws labeled bounding boxes (green = recognized, red = unknown) with match confidence
- Displays face count, real-time FPS, and camera resolution
- Detects and recognizes multiple faces simultaneously
- Downscaled detection pass for speed, with boxes rescaled back to full resolution

## 🛠️ Technologies Used

- **C++17**
- **OpenCV** (`objdetect`, DNN-based `FaceDetectorYN` and `FaceRecognizerSF`)
- **CMake**
- **CLion / MinGW**

## 📂 Project Structure

```
FaceDetection/
│
├── main.cpp                              # CLI parsing / dispatch only
├── config.h                              # shared constants (model paths, thresholds)
├── face_db.h / face_db.cpp               # KnownFace struct + CSV load/save/list/delete
├── matcher.h / matcher.cpp               # MatchResult struct + identify()
├── util.h / util.cpp                     # clampRect(), openCamera()
├── enroll.h / enroll.cpp                 # `enroll <name>` command
├── recognize.h / recognize.cpp           # default live recognition command
├── face_detection_yunet_2023mar.onnx     (download separately, see below)
├── face_recognition_sface_2021dec.onnx   (download separately, see below)
├── face_db.csv                           (created automatically when you enroll)
├── CMakeLists.txt
├── DEPENDENCIES.md
├── README.md
└── .gitignore
```

The code is split by responsibility: `config.h` holds shared constants, `face_db` owns all CSV persistence, `matcher` owns the cosine-similarity comparison, `util` holds small shared helpers, and `enroll`/`recognize` each implement one CLI command. `main.cpp` only parses arguments and wires the pieces together.

## ⚙️ How It Works

```
Webcam
   ↓
Capture Video Frame
   ↓
Downscale Frame (for faster detection)
   ↓
YuNet Face Detection
   ↓
Rescale Boxes to Full Resolution
   ↓
SFace: Align + Extract Embedding per Face
   ↓
Compare Embedding Against Enrolled Database (Cosine Similarity)
   ↓
Draw Bounding Boxes + Name + Confidence
   ↓
Display Face Count, FPS and Resolution
```

## 📋 Requirements

Before running the project, make sure you have installed:

- A C++17 compiler
- CMake
- OpenCV (built with the `objdetect` / DNN modules)
- CLion or another C++ IDE
- A webcam

## 🔧 OpenCV Setup

Make sure OpenCV is installed and configured correctly with your C++ compiler.

The project uses:

```cpp
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
```

If you are using **MinGW/MSYS2**, OpenCV can be installed with:

```
pacman -S mingw-w64-ucrt-x86_64-opencv
```

Make sure your compiler and OpenCV installation use compatible architectures and toolchains.

## 📥 Model Files (Required)

[//]: # (See DEPENDENCIES.md for direct download links.)

This project needs two ONNX model files that are **not included in the repo** (they're binary and fairly large):

```
face_detection_yunet_2023mar.onnx
face_recognition_sface_2021dec.onnx
```

See [`DEPENDENCIES.md`](DEPENDENCIES.md) for download links and instructions. Place both files in the project's working directory (same folder as the executable, or wherever you run it from):

```
FaceDetection/
├── main.cpp
├── face_detection_yunet_2023mar.onnx
├── face_recognition_sface_2021dec.onnx
└── CMakeLists.txt
```

## 🏗️ Build and Run

### 1. Clone the Repository

```
git clone https://github.com/aryanoff2112-art/face_detection.git
```

### 2. Open the Project

Open the project folder in CLion or your preferred C++ IDE.

### 3. Configure CMake

Use the following `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.20)

project(FaceDetection)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(FaceDetection main.cpp)

target_link_libraries(FaceDetection PRIVATE ${OpenCV_LIBS})

target_include_directories(FaceDetection PRIVATE ${OpenCV_INCLUDE_DIRS})
```

### 4. Reload CMake

Reload the CMake project after modifying `CMakeLists.txt`.

### 5. Build and Run

Build the project and run the executable from a directory containing the two `.onnx` model files.

## 🎮 Usage

```
FaceDetection                 Run live recognition against face_db.csv
FaceDetection enroll <name>   Capture samples for <name> into face_db.csv
FaceDetection list            List enrolled names and sample counts
FaceDetection delete <name>   Remove all samples for <name>
FaceDetection help            Show usage
```

### Enrolling someone

```
FaceDetection enroll Aryan
```

A window opens with a live preview. Press `c` to capture a sample whenever a face is detected (capture a few, from slightly different angles, for better accuracy), and `q` when you're done. Samples are appended to `face_db.csv`.

### Recognizing

```
FaceDetection
```

Opens the webcam and draws a labeled box around every detected face: green with a name and confidence percentage if it matches someone in `face_db.csv`, red and labeled "Unknown" otherwise.

| Key | Action                          |
| --- | -------------------------------- |
| `c` | Capture a sample (enroll mode)   |
| `q` | Quit / finish                    |

## 📊 Application Output

The application displays real-time information including:

- Number of detected faces
- Frames Per Second (FPS)
- Camera resolution
- Per-face name and match confidence

Example:

```
Faces: 1
FPS: 30
Resolution: 640x480
Aryan (87%)
```

## 🧠 Detection & Recognition Method

This project uses OpenCV's DNN-based **YuNet** detector and **SFace** recognizer, loaded via:

```cpp
Ptr<FaceDetectorYN> detector = FaceDetectorYN::create(
    DETECTOR_MODEL, "", Size(320, 320), 0.9f, 0.3f, 5000);

Ptr<FaceRecognizerSF> recognizer = FaceRecognizerSF::create(
    RECOGNIZER_MODEL, "");
```

Each frame is downscaled before detection for speed, and detected boxes are rescaled back to full resolution:

```cpp
detector->detect(smallFrame, faces);
if (faces.rows > 0)
    faces.colRange(0, 14) *= (1.0 / DETECTION_SCALE);
```

Each detected face is aligned and turned into a 128-d embedding, then compared against every enrolled embedding using cosine similarity; matches above `COSINE_THRESHOLD` (0.363, OpenCV's documented SFace threshold) are labeled with the closest name.

## ⚡ Performance Notes

- Detection runs on a downscaled frame (`DETECTION_SCALE = 0.5`) to keep FPS up; lower this further for more speed at the cost of missing small/distant faces.
- Frame buffers (`Mat` objects) are reused across the capture loop instead of being reallocated every frame.
- The webcam capture buffer is set to 1 frame where the backend supports it, to reduce latency.
- If your OpenCV build includes CUDA or OpenVINO, set `detector`/`recognizer` to use `DNN_BACKEND_CUDA`/`DNN_TARGET_CUDA` (or the OpenVINO equivalents) for a large FPS improvement — see the comment in `main()`.
- Recognition (embedding + comparison) is the expensive step, not detection — if you expect many enrolled people, consider only re-running recognition every N frames and tracking boxes in between.

## ⚠️ Troubleshooting

### `opencv2/opencv.hpp: No such file or directory`

This error usually means OpenCV is not configured correctly with your compiler.

Make sure:

- OpenCV is installed.
- OpenCV's include directory is available to the compiler.
- Your compiler is compatible with the OpenCV build.
- `find_package(OpenCV REQUIRED)` works correctly in CMake.

### `Error: Could not load face detector/recognizer model`

Make sure both `.onnx` files are present in the program's working directory (a relative path is recommended over a computer-specific absolute path). See [`DEPENDENCIES.md`](DEPENDENCIES.md).

### Webcam Does Not Open

Make sure:

- Your webcam is connected and enabled.
- Another application is not using the webcam.
- Your system allows the application to access the camera.

### Program crashes near frame edges / after enrolling

This used to happen when a detected face box extended slightly past the frame boundary. Boxes are now clamped to the frame (`clampRect`) before use — if you still see a crash, please open an issue with the exact steps to reproduce.

## 🚀 Future Improvements

- GUI interface instead of raw OpenCV windows
- Automatic face-attendance logging (timestamped CSV/DB of recognitions)
- Screenshot capture / video recording of sessions
- Batch import of enrollment photos instead of only live capture
- Optional GPU (CUDA/OpenVINO) backend toggle via a CLI flag

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Aryan**

---

⭐ If you found this project useful, consider giving the repository a star.

