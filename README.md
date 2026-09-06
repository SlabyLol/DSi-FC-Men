# DSi-FC-Men
**DSi FlashCard Menu**

A clean, fast and fully configurable menu for Nintendo DS / DSi / 3DS flashcards.  
Inspired by TWiLight Menu++ • English • citro2d • Sounds • One-CIA Setupper

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-Foundation-orange.svg)]()

---

## Features
- **DSi-style splash screen**
- **Fully configurable** via `config.ini` on the microSD (no recompilation needed)
- File browser with folders-first sorting
- Theme system (colors + later backgrounds)
- **Sound support** (menu move / select / back / splash)
- Soft-reset ready
- **3DS CIA Setupper** – run it once on your 3DS and the microSD is prepared in seconds
- Designed to feel modern and light

## Quick Start

### 1. Install the Setupper CIA
1. Put `DSi-FC-Men-Setupper.cia` on your 3DS SD card
2. Install it with FBI / GodMode9
3. Launch **DSi-FC-Men Setupper**
4. Press **A** → it creates all folders + config.ini automatically
5. Done

### 2. Add your games
- Copy `.nds` ROMs to `/roms/nds/`
- (Optional) Put sound files into `/_nds/DSi-FC-Men/sounds/`

### 3. Launch the menu
Use your flashcard’s autoboot or launch the main DSi-FC-Men application.

---

## Project Structure
```
DSi-FC-Men/
├── setupper/          ← 3DS CIA that prepares the microSD
│   └── source/
├── source/            ← Main menu (citro2d)
├── assets/
│   └── sounds/        ← Sound placeholders
├── config/            ← Default config.ini
├── themes/
├── sd_template/       ← Exact SD layout
└── docs/
```

## Configuration
Everything lives in:
```
/_nds/DSi-FC-Men/config.ini
```

You can change:
- Colors, theme, splash
- Launcher (nds-bootstrap / kernel)
- Paths, sounds, battery/clock display
- Soft-reset, sorting, etc.

## Sounds
Place these files in `/_nds/DSi-FC-Men/sounds/`:
- `menu_move.bcwav` / `.wav`
- `menu_select.bcwav`
- `menu_back.bcwav`
- `splash.bcwav`

(Enable/disable in config with `EnableSounds = true`)

## Building
Requires **devkitPro** (`3ds-dev`).

```bash
# Setupper CIA
cd setupper
make

# Main menu
cd ../source
make
```

To create a proper `.cia` use `makerom` + bannertool or tools like CBuilder3DS.

## Credits
- Inspired by **TWiLight Menu++** (Rocket Robz & DS-Homebrew)
- citro2d by fincs
- nds-bootstrap

## License
MIT

---

**Status:** Active Foundation  
We are building a real, clean alternative step by step.
