# DSi-FC-Men
**DSi FlashCard Menu**

A clean, fast and fully configurable menu for Nintendo DS / DSi / 3DS flashcards.  
Inspired by TWiLight Menu++ • English • citro2d • Sounds • One-CIA Setupper

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Build](https://github.com/SlabyLol/DSi-FC-Men/actions/workflows/build.yml/badge.svg)](https://github.com/SlabyLol/DSi-FC-Men/actions/workflows/build.yml)
[![Status](https://img.shields.io/badge/Status-Foundation-orange.svg)]()

---

## Features
- **DSi-style splash screen**
- **Fully configurable** via `config.ini` on the microSD
- File browser with folders-first sorting
- Theme + Sound support
- Soft-reset ready
- **3DS CIA Setupper** – prepares the microSD in seconds
- Ready-to-copy **SD Setup** package (folders + config + placeholders)
- GitHub Actions automatic CIA / 3dsx builds + Nightly releases

## Quick Start

### 1. Setupper CIA (recommended)
1. Download the latest **Nightly** from [Releases](https://github.com/SlabyLol/DSi-FC-Men/releases)
2. Install `DSi-FC-Men-Setupper.cia` with FBI
3. Launch it → press **A**
4. All folders + `config.ini` are created automatically

### 2. Or use the SD-Setup Zip
1. Download `DSi-FC-Men-SD-Setup.zip`
2. Extract it to the **root** of your microSD
3. You get:
   ```
   /_nds/DSi-FC-Men/     ← config, themes, sounds, boxart
   /_nds/nds-bootstrap/  ← place nds-bootstrap here
   /roms/nds/            ← put your .nds ROMs here
   /saves/
   ```

### 3. Add games & bootstrap
- Copy `.nds` files → `/roms/nds/`
- Download [nds-bootstrap](https://github.com/DS-Homebrew/nds-bootstrap/releases) → `/_nds/nds-bootstrap/`

## Project Structure
```
DSi-FC-Men/
├── .github/workflows/build.yml   ← automatic CIA builds
├── setupper/                     ← 3DS CIA that prepares microSD
├── source/                       ← Main menu (citro2d)
├── setup/
│   ├── sd_files/                 ← Ready microSD structure
│   └── INSTALL.txt
├── config/
├── assets/
└── docs/
```

## Configuration
Edit on the microSD:
```
/_nds/DSi-FC-Men/config.ini
```

## Building locally
Requires [devkitPro](https://devkitpro.org) (`3ds-dev`).

```bash
cd setupper && make
cd ../source && make
```

GitHub Actions builds automatically on every push to `main` and publishes a **Nightly** release.

## Credits
- Inspired by **TWiLight Menu++** (Rocket Robz & DS-Homebrew)
- citro2d by fincs
- nds-bootstrap

## License
MIT
