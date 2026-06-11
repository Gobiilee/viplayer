
# ViPlayer

A high-performance, modern cross-platform video player developed using **C++**, **Qt 6 (Qt Quick/QML)**, and the **MVVM (Model-View-ViewModel)** architectural pattern. Designed with a sleek, minimalist user interface inspired by the Windows 11 Fluent Design, ViPlayer leverages multi-threading and modern hardware rendering interfaces to achieve smooth, robust media playback.

---

## 🛠 Features

* **Advanced MVVM Architecture:** Total separation of presentation logic (QML) and core business logic (C++).
* **Smart Media Controls:** Full control bar featuring playback toggles (Play/Pause), volume adjustments, and continuous seek/progression timelines.
* **Intelligent Playlist Management:** Supports bulk-loading files or scanning entire folders seamlessly.
* **Asynchronous Folder Scanning:** Leverages `QtConcurrent` and dedicated worker threads to traverse disk directories without freezing the main UI thread.
* **Thread-Safe Core:** Utilizes `QMutex` and state-machine locking concepts to mitigate race conditions during high-frequency user interactions (e.g., fast forward/backward/skipping).
* **Immersive Fullscreen:** True double-click or layout-button fullscreen transitions powered by the Qt Quick window management pipeline.

---

## 📂 Project Architecture & Directory Structure

This project enforces an industry-standard corporate structure, utilizing segregated sub-modules decoupled dynamically through **CMake**.

```text
ViPlayer/
├── CMakeLists.txt             # Main top-level CMake configuration
├── src/                       # Backend Core Engine (C++)
│   ├── CMakeLists.txt         # Sub-build management for C++ modules
│   ├── main.cpp               # Application entry-point & style initialization
│   ├── models/                # Pure data storage & observation layer
│   │   ├── VideoModel.cpp
│   │   └── VideoModel.h
│   └── viewmodels/            # Binding controller & asynchronous orchestrator
│       ├── PlayerViewModel.cpp
│       └── PlayerViewModel.h
└── ui/                        # Frontend Application Layer (QML)
    ├── CMakeLists.txt         # Resource compilation & QML module registration
    ├── Main.qml               # Master Layout shell
    └── components/            # Reusable UI component modules
        ├── ControlBar.qml
        ├── IconButton.qml
        └── VideoPlayerView.qml

```

### Architectural Data Flow (MVVM)

1. **Model (`VideoModel`)**: Manages individual media attributes, metadata extraction, and tracks actual data boundaries safely as a `QObject` leveraging the **Observer Pattern**.
2. **ViewModel (`PlayerViewModel`)**: Exposes native C++ parameters to the UI using the `Q_PROPERTY` system, dispatches change markers via Signals/Slots, and processes input interactions safely across asynchronous borders.
3. **View (`Main.qml`, Components)**: Renders the hardware-accelerated presentation pipeline, completely unaware of low-level C++ thread instances or disk state structures.

---

## 🚀 Key Technical Insights & Takeaways

Developing **ViPlayer** provided profound engineering insights into building real-time desktop environments:

* **Qt 6 Graphics Hardware Interface (RHI)**: Learned to master how raw frames are pushed from decoding pools into rendering targets. Solved complex GPU race conditions (`failed to get textures for frame; format: 172`) by overriding backends dynamically via `QSG_RHI_BACKEND=opengl` and structuring stateful delays during stream source transitions.
* **Data Concurrency & Multi-threading**: Gained practical proficiency with `QtConcurrent::run` to offload blocking file-system operations from the Main GUI Thread, safely shifting cross-thread execution contexts back via `QMetaObject::invokeMethod`.
* **Thread Safety**: Implemented explicit thread synchronization structures (`QMutex`, `QMutexLocker`) to defend critical indexes and memory chunks against volatile concurrent execution vectors.
* **Component-Driven QML**: Mastered advanced Qt Quick customization techniques, circumventing native styling restrictions via the `Basic` controls ecosystem to construct bespoke, performant UI widgets.

---

## 🏗️ Prerequisites & Build Guide

### Requirements

* **Compiler**: MSVC 2019/2022 or MinGW 64-bit supporting **C++17** or higher.
* **Framework**: **Qt 6.5.3+** (Modules required: `Gui`, `Qml`, `Multimedia`, `Concurrent`).
* **Build System**: **CMake 3.16+**.

### Compilation

1. Launch **Qt Creator**.
2. Open the top-level `CMakeLists.txt` file.
3. Select your designated Kit (e.g., `Desktop Qt 6.5.3 MinGW 64-bit`).
4. Execute **Run CMake**, followed by a **Clean** project refresh.
5. Press `Ctrl + R` to compile and launch the media player application.

---

## 📄 License

This project is licensed under the **MIT License** - see the details below:

```text
MIT License

Copyright (c) 2026 ViPlayer Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

```
