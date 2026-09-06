# DSi-FC-Men
**DSi FlashCard Menu** – one CIA, auto-setup, sounds, BGM, nds-bootstrap

[![Build](https://github.com/SlabyLol/DSi-FC-Men/actions/workflows/build.yml/badge.svg)](https://github.com/SlabyLol/DSi-FC-Men/actions)

## Install (simple)
1. Download **DSi-FC-Men.cia** from [Releases](https://github.com/SlabyLol/DSi-FC-Men/releases)
2. Install with **FBI**
3. Launch once → it creates all folders + `config.ini` itself
4. Copy `.nds` ROMs to `/roms/nds/`
5. Copy [nds-bootstrap](https://github.com/DS-Homebrew/nds-bootstrap/releases/latest) into `/_nds/nds-bootstrap/`

Optional: extract `DSi-FC-Men-SD-Setup.zip` for sounds + template.

## Features
- **Single CIA** – no separate setupper
- Auto folder + config on first run
- DSi-style splash
- Modern UI sounds + looping menu BGM
- File browser → launches via nds-bootstrap
- Fully configurable via `/_nds/DSi-FC-Men/config.ini`
- SELECT = BGM toggle · START = exit

## Build
```bash
# needs devkitPro 3ds-dev, bannertool, makerom
make
make cia
./release.sh
```

## License
MIT
