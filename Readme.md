<h1 align="center">
  <img src="./assets/MainIcon.png" alt="" width="40" />
  &nbsp;Chitr
</h1>

<p align="center">
  A cross-platform media player written in C++.
</p>

Chitr plays images, video and audio in a native window on macOS, Windows and
Linux. It uses the media framework each operating system already provides, so
there is no browser engine inside it and nothing extra to install alongside it.
Open one file and the rest of the folder comes with it.

<p align="center">
  <a href="https://youtu.be/8d4SarF3NFY">
    <img src="./assets/thumbnail.png" width="60%" alt="">
  </a>
  <br />
  <a href="https://youtu.be/8d4SarF3NFY"><b>▶&nbsp; Watch the demo</b></a>
</p>

## Contents

- [Features](#features)
- [Download](#download)
- [First launch on macOS](#first-launch-on-macos)
- [Supported formats](#supported-formats)
- [Keyboard shortcuts](#keyboard-shortcuts)
- [Build from source](#build-from-source)
- [Landing page](#landing-page)
- [Roadmap](#roadmap)
- [License](#license)

## Features

The window opens with two tabs.

**Image Player**

- Opens JPG, PNG, GIF, TIFF, PCX, PNM and XPM
- Slideshow mode, started and stopped with one key
- Steps through the folder the file came from

**Video Player**

- Play, pause and seek, from the control bar or the keyboard
- Seek bar you can drag, and number keys that jump to a position
- Volume control with mute
- Elapsed and total time, updating as it plays
- Steps through the folder the file came from

## Download

Builds for macOS, Windows and Linux are attached to every release.

**[Download the latest release](https://github.com/jamal474/chitr/releases/latest)**

Each download is a zip archive. Unpack it and run the application inside; there
is no installer.

| Platform | File | Notes |
| --- | --- | --- |
| macOS | `Chitr-macos-arm64-v<version>.zip` | Apple silicon. See [First launch on macOS](#first-launch-on-macos). |
| Windows | `Chitr-windows-x86_64-v<version>.zip` | Run `Chitr.exe`. |
| Linux | `Chitr-linux-x86_64-v<version>.zip` | Needs wxWidgets 3.2 installed — see [Build from source](#build-from-source). |

## First launch on macOS

The macOS builds are not signed with an Apple Developer ID, so the first time
you open one, macOS refuses:

> "Chitr" cannot be opened because it is from an unidentified developer.

This is expected for an unsigned open-source build. Either of these clears it,
once per download:

**Run one command in Terminal**

```shell
xattr -dr com.apple.quarantine /path/to/Chitr.app
```

Then double-click `Chitr.app` as normal.

**Or open it from the menu** — right-click (or Control-click) `Chitr.app` in
Finder, choose **Open**, and confirm the dialog. macOS remembers the choice for
that copy of the app.

The same notice is included in every macOS release zip as `README-macOS.txt`.

## Supported formats

Chitr hands files to the media framework of the operating system it is running
on, so the lists differ slightly by platform. These have been verified on
macOS 26 Tahoe and Windows 10; other formats may work but are untested.

**Images**

| Platform | Formats |
| --- | --- |
| Windows, macOS and Linux | `JPG` `PNG` `GIF` `TIFF` `PCX` `PNM` `XPM` |

**Video**

| Platform | Formats |
| --- | --- |
| Windows and macOS | `mp4` `m4v` `mov` `mpg` |
| Windows only | `avi` `mkv` `wmv` |

**Audio**

| Platform | Formats |
| --- | --- |
| Windows and macOS | `aac` `aiff` `au` `flac` `m4a` `mp3` `wav` |
| Windows only | `wma` |
| macOS only | `caf` `ogg` `opus` |

## Keyboard shortcuts

**Image Player**

| Key | Action |
| --- | --- |
| <kbd>←</kbd> | Previous image |
| <kbd>→</kbd> | Next image |
| <kbd>Space</kbd> | Start or stop the slideshow |
| <kbd>O</kbd> | Open an image file |

**Video Player**

| Key | Action |
| --- | --- |
| <kbd>Space</kbd> or <kbd>K</kbd> | Play or pause |
| <kbd>←</kbd> | Back 5 seconds |
| <kbd>→</kbd> | Forward 5 seconds |
| <kbd>J</kbd> | Back 10 seconds |
| <kbd>L</kbd> | Forward 10 seconds |
| <kbd>0</kbd>–<kbd>9</kbd> | Jump to that point in the file (<kbd>3</kbd> is 30% in) |
| <kbd>↑</kbd> | Volume up 1% |
| <kbd>↓</kbd> | Volume down 1% |
| <kbd>M</kbd> | Mute |
| <kbd>Alt</kbd> + <kbd>←</kbd> | Previous video |
| <kbd>Alt</kbd> + <kbd>→</kbd> | Next video |
| <kbd>O</kbd> | Open a video file |

On macOS, <kbd>Option</kbd> is the <kbd>Alt</kbd> key.

## Build from source

Chitr builds with CMake. Its dependencies are installed by Conan, a package
manager for C++.

### 1. Install Conan

```shell
# Windows
scoop install conan

# macOS
brew install conan

# Linux
pip install "conan>=2.0"
```

Then let Conan detect your compiler:

```shell
conan profile detect --force
```

### 2. Install wxWidgets (Linux only)

On Windows and macOS, Conan provides wxWidgets and there is nothing to install.

On Linux, Chitr uses the wxWidgets package from your system instead, because the
one in Conan Center has not been updated for Conan 2. Install version 3.2 with
media support:

```shell
# Ubuntu 24.04, Debian 13 and newer
sudo apt-get install libwxgtk3.2-dev libwxgtk-media3.2-dev

# Fedora, RHEL
sudo dnf install wxGTK3-devel gstreamer1-plugins-base-devel
```

Ubuntu 22.04 ships wxWidgets 3.0, so add the official repository first:

```shell
sudo apt-get install ca-certificates curl gnupg
curl -fsSL https://repos.codelite.org/CodeLite.asc \
  | sudo gpg --dearmor -o /etc/apt/keyrings/codelite.gpg
echo "deb [signed-by=/etc/apt/keyrings/codelite.gpg] https://repos.codelite.org/wx3.2.5/ubuntu/ jammy universe" \
  | sudo tee /etc/apt/sources.list.d/wxwidgets.list
sudo apt-get update
sudo apt-get install libwxgtk3.2-dev libwxgtk-media3.2-dev
```

### 3. Build and run

```shell
conan install . --build=missing
conan build .
```

The result lands in `build/Release`:

```shell
./build/Release/Chitr           # Linux
.\build\Release\Chitr.exe       # Windows
open ./build/Release/Chitr.app  # macOS
```

## Landing page

The page for Chitr at [sabo.sh/chitr](https://sabo.sh/chitr/) lives in
[`website/`](./website) — what the app is, the demo, and the downloads. It has
its own [readme](./website/README.md) covering how to run and deploy it, and is
not part of the application build.

## Roadmap

- Tabs, so several players can be open at once
- Browsing the file system from inside the app
- A playback status bar with reverse play and rewind
- Support for more formats

Bug reports and feature requests are welcome in
[Issues](https://github.com/jamal474/chitr/issues).

## License

[GNU General Public License v3.0](./LICENSE)
