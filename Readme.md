<h1 align="center">
  <img src="./assets/MainIcon.png" alt="Chitr" width="40" />
  &nbsp;Chitr
</h1>

#### A cross-platform Media Player written in C++.

## Features

* Image Viewer [for formats : [png, jpg, jpeg tiff, xpm, pcx, pnm, tiff]]
* Video Viewer [for formats : [mp4, mp3, mov]]

## Demo video

<p align="center">
  <a href="https://youtu.be/KM7Q8GF_5LU?si=XXWhheSZN4hlepyJ">
    <img src="./assets/thumbnail.png" width="40%" title="click to see demo" alt="Demo Thumbnail">
  </a>
</p>

## Install

```shell
conan install . --build=missing
```

## Build

```shell
conan build .
```

## Todo

* Tab Interface for multi-tab of either media players
* Integrate the native file directory for media discovery
* Playback status bar in video player and its interaction for reverse play or rewind.
* Add Support for more formats.

## Supported Media Formats

This project utilizes **wxWidgets** (C++) to provide cross-platform media viewing. Below is the breakdown of supported file extensions for Windows and macOS.

### Images
*Universal support across all platforms.*

| Platform | Supported Extensions |
| :--- | :--- |
| **Windows & Mac** | `JPG`, `PNG`, `GIF`, `TIFF`, `PCX`, `PNM`, `XPM` |

---

### Video Support

| Platform | Supported Extensions |
| :--- | :--- |
| **Windows & Mac** | `mp4`, `m4v`, `mov`, `mpg` |
| **Windows Only** | <kbd>avi</kbd> <kbd>mkv</kbd> <kbd>wmv</kbd> |
| **macOS Only** | *(Relies on common formats)* |

### Audio Support

| Platform | Supported Extensions |
| :--- | :--- |
| **Windows & Mac** | `aac`, `aiff`, `au`, `flac`, `m4a`, `mp3`, `wav` |
| **Windows Only** | <kbd>wma</kbd> |
| **macOS Only** | <kbd>caf</kbd> <kbd>ogg</kbd> <kbd>opus</kbd> |

---
> **Note:** Format support is determined by the underlying OS media frameworks utilized by the C++ backend.

