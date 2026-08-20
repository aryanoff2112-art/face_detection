# Face Detection in C++ Using OpenCV

A basic **Face Detection** project built using **C++ and OpenCV**. The project uses a webcam to detect human faces in real time using OpenCV's Haar Cascade Classifier.

## 📌 Features

* Real-time face detection using a webcam
* Uses OpenCV Haar Cascade Classifier
* Draws a rectangle around detected faces
* Displays the number of detected faces
* Simple beginner-friendly C++ implementation

## 🛠️ Technologies Used

* **C++**
* **OpenCV**
* **Haar Cascade Classifier**
* **CMake**
* **CLion / MinGW**

## 📂 Project Structure

```text
FaceDetection/
│
├── main.cpp
├── haarcascade_frontalface_default.xml
└── CMakeLists.txt
```

## ⚙️ How It Works

The program follows these steps:

```text
Webcam
   ↓
Capture Video Frame
   ↓
Convert Frame to Grayscale
   ↓
Haar Cascade Face Detection
   ↓
Detect Faces
   ↓
Draw Bounding Boxes
   ↓
Display Result
```

## 📋 Requirements

Before running the project, install:

* C++ compiler
* CMake
* CLion or another C++ IDE
* OpenCV

## 🔧 OpenCV Setup

Make sure OpenCV is installed and configured with your compiler.

The project uses:

```cpp
#include <opencv2/opencv.hpp>
```

If you are using **MinGW/MSYS2**, OpenCV can be installed with:

```bash
pacman -S mingw-w64-ucrt-x86_64-opencv
```

Make sure your compiler and OpenCV installation use compatible architectures/toolchains.

## 📥 Haar Cascade File

The project requires:

```text
haarcascade_frontalface_default.xml
```

This file is provided by OpenCV and is used by the program to detect faces.

Place the file in the project directory:

```text
FaceDetection/
├── main.cpp
├── haarcascade_frontalface_default.xml
└── CMakeLists.txt
```

## ▶️ Running the Project

### 1. Clone the repository

```bash
git clone https://github.com/your-username/face-detection-cpp.git
```

### 2. Open the project in CLion

Open the project folder in CLion.

### 3. Configure CMake

Use the following `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.20)

project(face_detection)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(face_detection main.cpp)

target_link_libraries(face_detection PRIVATE ${OpenCV_LIBS})

target_include_directories(face_detection PRIVATE ${OpenCV_INCLUDE_DIRS})
```

### 4. Reload CMake

In CLion, reload the CMake project after modifying `CMakeLists.txt`.

### 5. Build and Run

Build the project and run the executable.

The webcam window should open and begin detecting faces.

Press:

```text
q
```

to exit the program.

## 💻 Example

When a face is detected, the program draws a bounding box around it:

```text
+---------------------------+
|                           |
|       ┌──────────┐        |
|       │   FACE   │        |
|       └──────────┘        |
|                           |
|       Faces: 1            |
+---------------------------+
```

## 🧠 Face Detection Method

This project uses the **Haar Cascade Classifier**.

The classifier is loaded using:

```cpp
CascadeClassifier faceCascade;

faceCascade.load(
    "haarcascade_frontalface_default.xml"
);
```

Faces are detected using:

```cpp
faceCascade.detectMultiScale(
    grayFrame,
    faces,
    1.1,
    5
);
```

## ⚠️ Troubleshooting

### `opencv2/opencv.hpp: No such file or directory`

This usually means OpenCV is not configured correctly with your compiler.

Make sure:

* OpenCV is installed.
* OpenCV's `include` directory is available to the compiler.
* Your compiler is compatible with the OpenCV build.
* `find_package(OpenCV REQUIRED)` works in CMake.

### `Could not load Haar Cascade file`

Make sure:

```text
haarcascade_frontalface_default.xml
```

is available in the program's working directory.

You can also provide the complete path:

```cpp
faceCascade.load(
    "D:/face_detection/haarcascade_frontalface_default.xml"
);
```

## 🚀 Possible Future Improvements

* Eye detection
* Smile detection
* Multiple face tracking
* Face recognition
* Save detected faces as images
* Real-time FPS counter
* Automatic face attendance system
* GUI interface

## 📄 License

This project is created for educational and learning purposes.

## 👨‍💻 Author

**Aryan**


⭐ If you found this project useful, consider giving the repository a star.
