# Building DSi-FC-Men

## Requirements
- [devkitPro](https://devkitpro.org/wiki/Getting_Started) with `3ds-dev` package
- citro2d / citro3d (usually included)
- Optional: makerom for CIA packaging

## Build Steps
```bash
cd source
make
```

This produces `DSi-FC-Men.3dsx` (and .smdh).

### Creating a CIA
You can use tools such as:
- CBuilder3DS
- makerom + bannertool
- Or convert via GodMode9 later

## Current Status
This is the **foundation**:
- Config system fully working (ini parser)
- DSi-style splash
- Basic UI skeleton with citro2d
- File browser skeleton
- One-minute SD Setup Tool

Next development steps:
1. Complete file browser + launch via nds-bootstrap
2. Real theme loading (backgrounds, colors from SD)
3. Boxart support
4. Soft-reset implementation
5. Pure NDS (libnds) version for classic flashcards
6. Automatic download of latest nds-bootstrap in the Setup Tool
