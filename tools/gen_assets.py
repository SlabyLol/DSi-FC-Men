#!/usr/bin/env python3
import struct, zlib
from pathlib import Path

def png(w, h, rgb_fn):
    raw = b""
    for y in range(h):
        raw += b"\x00"
        for x in range(w):
            r, g, b = rgb_fn(x, y)
            raw += bytes([r & 255, g & 255, b & 255])
    def chunk(tag, data):
        return struct.pack(">I", len(data)) + tag + data + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)
    ihdr = struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0)
    return b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", ihdr) + chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b"")

Path("assets").mkdir(exist_ok=True)
if not Path("assets/icon.png").exists():
    Path("assets/icon.png").write_bytes(
        png(48, 48, lambda x, y: (10, 30, 60) if 12 < x < 36 and 12 < y < 36 else (8, 12, 28))
    )
    print("generated icon.png")
if not Path("assets/banner.png").exists():
    Path("assets/banner.png").write_bytes(
        png(256, 128, lambda x, y: (0, 180, 255) if y > 100 else (8, 12, 28))
    )
    print("generated banner.png")
if not Path("assets/banner.wav").exists():
    import wave
    with wave.open("assets/banner.wav", "w") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(44100)
        w.writeframes(b"\x00\x00" * 1000)
    print("generated banner.wav")
if not Path("assets/cia.rsf").exists():
    Path("assets/cia.rsf").write_text(
        "BasicInfo:\n"
        "  Title: \"DSi-FC-Men\"\n"
        "  CompanyCode: \"00\"\n"
        "  ProductCode: \"CTR-P-DSIFC\"\n"
        "  ContentType: Application\n"
        "  Logo: Nintendo\n"
        "TitleInfo:\n"
        "  UniqueId: 0x445343\n"
        "  Category: Application\n"
        "Option:\n"
        "  UseOnSD: true\n"
        "  FreeProductCode: true\n"
        "  EnableCompress: true\n"
        "AccessControlInfo:\n"
        "  CoreVersion: 2\n"
        "  MemoryType: Application\n"
        "  SystemMode: 64MB\n"
        "  IdealProcessor: 0\n"
        "  Priority: 16\n"
        "SystemControlInfo:\n"
        "  SaveDataSize: 0KB\n"
        "  StackSize: 0x40000\n"
    )
    print("generated cia.rsf")
print("assets ready")
