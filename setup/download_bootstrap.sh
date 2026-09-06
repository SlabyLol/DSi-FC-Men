#!/bin/bash
# DSi-FC-Men – Download latest nds-bootstrap into setup/sd_files
set -e

OUT_DIR="$(dirname "$0")/sd_files/_nds/nds-bootstrap"
mkdir -p "$OUT_DIR"

echo "=== DSi-FC-Men: Fetching latest nds-bootstrap ==="

LATEST=$(curl -sL https://api.github.com/repos/DS-Homebrew/nds-bootstrap/releases/latest | grep -oP '"tag_name": "\K[^"]+' | head -1)

if [ -z "$LATEST" ]; then
  echo "Could not detect latest version, using known fallback..."
  LATEST="v1.6.1"
fi

echo "Latest version: $LATEST"

URL="https://github.com/DS-Homebrew/nds-bootstrap/releases/download/${LATEST}/nds-bootstrap.7z"
ALT_URL="https://github.com/DS-Homebrew/nds-bootstrap/releases/download/${LATEST}/nds-bootstrap.zip"

TMP=$(mktemp -d)
cd "$TMP"

echo "Downloading..."
if curl -sL -f -o bootstrap.7z "$URL" 2>/dev/null; then
  echo "Got 7z package"
  if command -v 7z >/dev/null; then
    7z x -y bootstrap.7z >/dev/null
  else
    echo "Please install p7zip (7z) to extract, or download manually."
    echo "URL: $URL"
    exit 1
  fi
elif curl -sL -f -o bootstrap.zip "$ALT_URL" 2>/dev/null; then
  echo "Got zip package"
  unzip -o bootstrap.zip >/dev/null
else
  echo "Automatic download failed."
  echo "Please download manually from:"
  echo "  https://github.com/DS-Homebrew/nds-bootstrap/releases/latest"
  echo "and extract into: $OUT_DIR"
  exit 1
fi

cp -v nds-bootstrap*.nds "$OUT_DIR/" 2>/dev/null || true
cp -v *.nds "$OUT_DIR/" 2>/dev/null || true
cp -v nds-bootstrap.ini "$OUT_DIR/" 2>/dev/null || true

echo ""
echo "Files now in $OUT_DIR:"
ls -la "$OUT_DIR"
echo ""
echo "Done. Copy the whole setup/sd_files/ contents to your microSD root."
rm -rf "$TMP"
