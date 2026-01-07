<h1 align="center">
  <img src="./assets/MainIcon.png" alt="Chitr" width="40" />
  &nbsp;Chitr
</h1>

#### A cross-platform Media Player written in C++.

## Features

### Image Player
- Multiple format support
- Slideshow mode
- Folder navigation

### Video Player
- Playback controls (play, pause, seek)
- Volume control with mute
- Live playback timer
- Interactive seek bar
- Folder navigation

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

## Supported Media Formats

This project utilizes **wxWidgets** (C++) to provide cross-platform media viewing. Below is the breakdown of supported file extensions for Windows and macOS.

<table>
<tr><th colspan="2">Images</th></tr>
<tr><th width="500px">Platform</th><th width="500px">Supported Extensions</th></tr>
<tr><td align="center"><strong>Windows & Mac</strong></td><td align="center"><code>JPG</code>, <code>PNG</code>, <code>GIF</code>, <code>TIFF</code>, <code>PCX</code>, <code>PNM</code>, <code>XPM</code></td></tr>
</table>
<table>
<tr><th colspan="2">Video</th></tr>
<tr><th width="500px">Platform</th><th width="500px">Supported Extensions</th></tr>
<tr><td align="center"><strong>Windows & Mac</strong></td><td align="center"><code>mp4</code>, <code>m4v</code>, <code>mov</code>, <code>mpg</code></td></tr>
<tr><td align="center"><strong>Windows Only</strong></td><td align="center"><code>avi</code>, <code>mkv</code>, <code>wmv</code></td></tr>
<tr><td align="center"><strong>macOS Only</strong></td><td align="center"><em>Relies on common formats</em></td></tr>
</table>
<table>
<tr><th colspan="2">Audio</th></tr>
<tr><th width="500px">Platform</th><th width="500px">Supported Extensions</th></tr>
<tr><td align="center"><strong>Windows & Mac</strong></td><td align="center"><code>aac</code>, <code>aiff</code>, <code>au</code>, <code>flac</code>, <code>m4a</code>, <code>mp3</code>, <code>wav</code></td></tr>
<tr><td align="center"><strong>Windows Only</strong></td><td align="center"><code>wma</code></td></tr>
<tr><td align="center"><strong>macOS Only</strong></td><td align="center"><code>caf</code>, <code>ogg</code>, <code>opus</code></td></tr>
</table>

---
> **Note:** Format support is determined by the underlying OS media frameworks utilized by the C++ backend.

## Keyboard Shortcuts

<table>
<tr><th colspan="3">Image Player</th></tr>
<tr><th width="500px">Key</th><th width="500px">Action</th><th width="500px">Description</th></tr>
<tr><td align="center"><code>←</code></td><td align="center">Previous</td><td align="center">Load previous image</td></tr>
<tr><td align="center"><code>→</code></td><td align="center">Next</td><td align="center">Load next image</td></tr>
<tr><td align="center"><code>Space</code></td><td align="center">Slideshow</td><td align="center">Start/stop slideshow</td></tr>
<tr><td align="center"><code>O</code></td><td align="center">Open</td><td align="center">Select image file</td></tr>
</table>
<table>
<tr><th colspan="3">Video Player</th></tr>
<tr><th width="500px">Key</th><th width="500px">Action</th><th width="500px">Description</th></tr>
<tr><td align="center"><code>←</code></td><td align="center">Seek backward</td><td align="center">-5 seconds</td></tr>
<tr><td align="center"><code>→</code></td><td align="center">Seek forward</td><td align="center">+5 seconds</td></tr>
<tr><td align="center"><code>J</code></td><td align="center">Seek backward</td><td align="center">-10 seconds</td></tr>
<tr><td align="center"><code>L</code></td><td align="center">Seek forward</td><td align="center">+10 seconds</td></tr>
<tr><td align="center"><code>↑</code></td><td align="center">Volume up</td><td align="center">+1%</td></tr>
<tr><td align="center"><code>↓</code></td><td align="center">Volume down</td><td align="center">-1%</td></tr>
<tr><td align="center"><code>Space</code> / <code>K</code></td><td align="center">Play/Pause</td><td align="center">Toggle playback</td></tr>
<tr><td align="center"><code>M</code></td><td align="center">Mute</td><td align="center">Toggle audio</td></tr>
<tr><td align="center"><code>0-9</code></td><td align="center">Jump to position</td><td align="center">Seek to percentage (3 = 30%)</td></tr>
<tr><td align="center"><code>O</code></td><td align="center">Open</td><td align="center">Select video file</td></tr>
</table>

## Todo

* Tab Interface for multi-tab of either media players
* Integrate the native file directory for media discovery
* Playback status bar in video player and its interaction for reverse play or rewind.
* Add Support for more formats.