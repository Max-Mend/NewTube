# NewTube

A lightweight NewTube player with autoplay videos in MPV player. Designed for personal use to make watching YouTube more convenient and efficient.

## Screenshot

![NewTube](screenshot.png)

## Features

### Player
- **Full YouTube experience** - Full YouTube viewing functionality
- **Session saving** - Saves cookies and login status
- **Quick navigation** - Back, Forward, Refresh buttons
- **URL bar** - Supports direct URL entry
- **Simple interface** - Minimalist design focused on content

### Video playback
- **MPV auto-launch** - Videos open in MPV player
- **High quality** - Video quality up to 1080p
- **Hardware acceleration** - GPU accelerated playback
- **Customizable controls** - Full MPV hotkey support
- **Stay open** - Player stays open after video ends

### Quick menu
- **Home** - YouTube Home
- **Subscriptions** - Your Subscribed Channels
- **History** - Watch History
- **Trends** - Trending Videos
- **Clear Cache** - Clear Cookies and Cache
- **About** - App Information

## Quick Start

### Requirements

**Ubuntu/Debian:**
```bash
sudo apt install qt6-webengine-dev libqt6webenginewidgets6 mpv yt-dlp
```

**Fedora:**
```bash
sudo dnf install qt6-qtwebengine-devel mpv yt-dlp
```

**Arch Linux:**
```bash
sudo pacman -S qt6-webengine mpv yt-dlp
```

### Build from Source

```bash
git clone https://github.com/Max-Mend/newtube.git
cd newtube
mkdir build && cd build
cmake ..
make
./app
```

### System-wide installation

```bash
chmod +x install.sh
./install.sh
```

Once installed, launch NewTube from your application menu.

## Usage

### Watch videos
1. Browse YouTube normally in the player
2. Click on any video thumbnail or link
3. The video will automatically open in the MPV player
4. Continue watching while watching

### MPV shortcuts
- **Space** - Play/Pause
- **Arrow keys** - Fast forward/rewind
- **[ ]** - Slow down/fasten playback speed
- **f** - Full screen
- **q** - Exit the player
- **9/0** - Volume down/up

### Navigation
- Use the **Back/Forward** buttons for history
- **Refresh** button to reload the page
- Type URLs directly into the address bar
- Use the **Menu** for quick access to YouTube sections

## Configuration

MPV settings are saved in:
```
~/.config/newtube/mpv.conf
```

Default settings:
```
ytdl=yes
ytdl-format=bestvideo[height<=?1080]+bestaudio/best
hwdec=auto
keep-open=yes
osc=yes
volume=50
```

Edit this file to adjust video quality, hardware acceleration, and other MPV settings.

## Technical Details

### Built with
- **Qt6** - Cross-platform application framework
- **QWebEngine** - Chromium-based web browser engine
- **MPV** - Powerful media player
- **yt-dlp** - YouTube video downloader
- **C++17** - C++ standards

### Architecture
- **CustomWebPage** - Intercepts clicks on video links
- **JavaScript Injection** - Captures YouTube video URLs
- **QProcess** - Manages MPV player instance
- **Resource System** - Embedded application icon

### Video Interception
The application uses JavaScript to intercept YouTube video links:
- Listens for clicks on video elements
- Prevents default navigation
- Extracts video URL
- Launches MPV from URL

## System Requirements

### Required
- **Qt6** (WebEngine module)
- **MPV** media player
- **yt-dlp** YouTube downloader
- **C++ compiler** (GCC, Clang)
- **CMake** 3.28+

### Recommended
- Hardware video acceleration support
- 2GB+ RAM
- Modern CPU for smooth 1080p playback

## Usage Options

Perfect for:
- Ad-free YouTube viewing
- Better video player control
- Background playback while browsing
- Save bandwidth (MPV uses less resources)
- Keyboard video control
- Privacy-conscious web browsing

## Known Limitations

- YouTube Shorts may not work properly
- Live streams require manual URL copying
- Playlists open videos separately
- No video download feature (by design)
- Active internet connection required

## Contribution

This is a personal project, but contributions are welcome! Feel free to:
- Report bugs via Issues
- Suggest new features
- Submit extension requests
- Improve documentation
- Share your experiences

## License

This project is licensed under the **GPL-3.0** license - see the [LICENSE](LICENSE) file for details.

## Author

**Max-Mend**
