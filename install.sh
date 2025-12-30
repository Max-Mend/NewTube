#!/bin/bash

set -e

echo "Installing NewTube..."

if [ ! -f "cmake-build-debug/app" ]; then
    echo "Error: Binary not found. Please build the project first."
    exit 1
fi

if [ ! -f "assets/icon.svg" ]; then
    echo "Error: Icon not found at assets/icon.svg"
    exit 1
fi

sudo cp cmake-build-debug/app /usr/local/bin/newtube
echo "✓ Binary installed to /usr/local/bin/newtube"

sudo cp assets/icon.svg /usr/share/icons/newtube.svg
echo "✓ Icon installed to /usr/share/icons/newtube.svg"

cat > newtube.desktop << EOF
[Desktop Entry]
Type=Application
Name=NewTube
Comment=YouTube browser with MPV player
Icon=/usr/share/icons/newtube.svg
Exec=/usr/local/bin/newtube
Terminal=false
Categories=Network;AudioVideo;Video;
EOF

sudo cp newtube.desktop /usr/share/applications/
echo "✓ Desktop file installed to /usr/share/applications/newtube.desktop"

rm newtube.desktop

sudo update-desktop-database 2>/dev/null || true
echo "✓ Desktop database updated"

echo ""
echo "Installation complete!"
echo "You can now launch NewTube from your application menu."