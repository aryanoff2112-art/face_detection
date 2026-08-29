# Face Detection in C++ Using OpenCV

A real-time **Face Detection** project built using **C++ and OpenCV**. The application uses a webcam to detect human faces using OpenCV's **Haar Cascade Classifier**.

## 📌 Features

* Real-time face detection using a webcam
* Uses OpenCV Haar Cascade Classifier
* Draws bounding boxes around detected faces
* Detects multiple faces simultaneously
* Displays the number of detected faces
* Displays real-time FPS
* Displays camera resolution
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
├── CMakeLists.txt
├── README.md
├── .gitignore
│
└── screenshots/
    └── demo.png
```

## ⚙️ How It Works

The application follows this process:

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
Display Face Count, FPS and Resolution
```

## 📋 Requirements

Before running the project, make sure you have installed:

* A C++ compiler
* CMake
* OpenCV
* CLion or another C++ IDE
* A webcam

## 🔧 OpenCV Setup

Make sure OpenCV is installed and configured correctly with your C++ compiler.

The project uses:

```cpp
#include <opencv2/opencv.hpp>
```

If you are using **MinGW/MSYS2**, OpenCV can be installed with:

```bash
pacman -S mingw-w64-ucrt-x86_64-opencv
```

Make sure your compiler and OpenCV installation use compatible architectures and toolchains.

## 📥 Haar Cascade File

The project requires the following file:

```text
haarcascade_frontalface_default.xml
```

This file is provided by OpenCV and is used to detect faces.

Place it in the project directory:

```text
FaceDetection/
├── main.cpp
├── haarcascade_frontalface_default.xml
└── CMakeLists.txt
```

## 🏗️ Build and Run

### 1. Clone the Repository

```bash
git clone https://github.com/YOUR-USERNAME/YOUR-REPOSITORY.git
```

Replace `YOUR-USERNAME` and `YOUR-REPOSITORY` with your GitHub username and repository name.

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

Build the project and run the executable.

The webcam window should open and begin detecting faces in real time.

## 🎮 Controls

| Key | Action               |
| --- | -------------------- |
| `q` | Quit the application |

## 📊 Application Output

The application displays real-time information including:

* Number of detected faces
* Frames Per Second (FPS)
* Camera resolution

Example:

```text
Faces: 1
FPS: 30
Resolution: 640x480
```

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
|       FPS: 30             |
|       Resolution: 640x480 |
+---------------------------+
```

## 🧠 Face Detection Method

This project uses OpenCV's **Haar Cascade Classifier**.

The classifier is loaded using:

```cpp
CascadeClassifier faceCascade;

if (!faceCascade.load("haarcascade_frontalface_default.xml"))
{
    cout << "Error: Could not load Haar Cascade file!" << endl;
    return -1;
}
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

You can display a screenshot in this README using:

```md
![Face Detection Demo](screenshots/demo.png)
```

## ⚠️ Troubleshooting

### `opencv2/opencv.hpp: No such file or directory`

This error usually means OpenCV is not configured correctly with your compiler.

Make sure:

* OpenCV is installed.
* OpenCV's include directory is available to the compiler.
* Your compiler is compatible with the OpenCV build.
* `find_package(OpenCV REQUIRED)` works correctly in CMake.

### `Could not load Haar Cascade file`

Make sure:

```text
haarcascade_frontalface_default.xml
```

is available in the program's working directory.

A relative path is recommended instead of a computer-specific absolute path.

### Webcam Does Not Open

Make sure:

* Your webcam is connected and enabled.
* Another application is not using the webcam.
* Your system allows the application to access the camera.

## 🚀 Future Improvements

Possible improvements include:

* Eye detection
* Smile detection
* Multiple face tracking
* Face recognition
* Save detected faces as images
* Automatic face attendance system
* GUI interface
* Screenshot capture
* Video recording
* Improved performance and accuracy

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Aryan**

---

⭐ If you found this project useful, consider giving the repository a star.

