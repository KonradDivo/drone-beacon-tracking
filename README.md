# 🛸 Visual Guidance & Target Tracking (OpenCV HSV Color Segmentation)

This repository implements a lightweight, high-speed visual servoing and landing target tracking system written in modern cross-platform C++ (C++17/20). Built specifically for autonomous robotic platforms, the software processes real-time camera frames to isolate a localized color landing beacon (e.g., fluorescent orange/red), computes its geometric contours, and extracts its exact 2D center coordinates to assist onboard flight microcontrollers during GPS-denied landing procedures.

---

## 🎯 Architectural Engineering Choices

*   **Pixel-Level Geometric Extraction:** Instead of using heavy, battery-draining deep learning layers, this module leverages optimized classic computer vision algorithms (HSV thresholding, morphological erosion/dilation, and contour mapping). This achieves near-zero latency execution directly on host edge CPUs.
*   **Encapsulated Logic (OOP):** The full image processing pipeline—color masking, noise filtering, area sorting, and spatial mass calculation—is completely self-contained within the modular `BeaconTracker` class.
*   **Universal CLI Portability:** Built without OS-specific window handlers, the codebase uses basic command-line parameters (`argc`/`argv`). This makes it natively portable between a Windows development machine and an autonomous Linux flight computer.

---

## 🧠 Core Mathematics: Spatial Moments Formula

When the target landing beacon is identified within the frame, its exact pixel coordinates are extracted using **Image Moments** ($M_{ij}$) based on raw contour pixel intensities:

$$\text{Target Center } (X, Y) = \left( \frac{M_{10}}{M_{00}}, \frac{M_{01}}{M_{00}} \right)$$

*   $M_{00}$: The raw area/mass of the largest detected target contour.
*   $M_{10}$: The first-order spatial moment along the X-axis (horizontal distribution).
*   $M_{01}$: The first-order spatial moment along the Y-axis (vertical distribution).

---

## 🗂️ Project Structure

*   `BeaconTracker.hpp` : Object definition layout containing HSV scalar thresholds and structural bindings.
*   `BeaconTracker.cpp` : Houses the logic for frame segmentation, contour extraction, and center-of-mass calculus.
*   `main.cpp` : Standard entry stream loop handling real-time frame buffering and programmatic output logging.

---

## ⚙️ IDE Setup & Local Compilation (Code::Blocks 25.03)

### 1. Requirements
*   **Compiler:** GCC / MinGW (32-bit `i686` or 64-bit `x86_64` aligned with your host toolkit).
*   **Library:** OpenCV 4.x (Built natively with the `-m32` parameter to match native Code::Blocks linker environments).

### 2. Project Properties Setup
Link your native OpenCV installation inside Code::Blocks:
1. Go to **Project** -> **Build options** -> **Search directories**.
2. **Compiler:** Add the path to `.../install/include/`
3. **Linker:** Add the path to `.../install/x86/mingw/lib/`
4. Go to **Linker settings** -> **Link libraries** and append the static archive dependencies in this **strict hierarchical order**:
   * `libopencv_highgui4120.a`
   * `libopencv_imgcodecs4120.a`
   * `libopencv_imgproc4120.a`
   * `libopencv_core4120.a` *(must remain at the bottom as the base binary block)*

---

## 🚀 Execution & Command-Line Testing

To feed a pre-recorded flight logging file (`.mp4`) directly into the analytical core loop via the console:

```bash
# Execute binary with standard argument passing
./drone_beacon_tracking.exe path/to/your/flight_video.mp4
```

### Simulating inputs inside Code::Blocks:
1. Go to the top menu: **Project** -> **Set programs' arguments...**
2. In the **Program arguments** input field, type your video asset filename (e.g., `landing_beacon_log.mp4`).
3. Press **F9** to build and boot the execution flow automatically.
