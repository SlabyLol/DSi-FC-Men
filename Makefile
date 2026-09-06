# DSi-FC-Men – single 3DS homebrew (3dsx + CIA)
ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO")
endif

include $(DEVKITPRO)/3ds_rules

TARGET   := DSi-FC-Men
BUILD    := build
SOURCES  := source
INCLUDES := source
ROMFS    := romfs

APP_TITLE       := DSi-FC-Men
APP_DESCRIPTION := DSi FlashCard Menu
APP_AUTHOR      := SlabyLol

ICON := assets/icon.png
BANNER_IMAGE := assets/banner.png

ARCH     := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
CFLAGS   := -g -Wall -O2 -mword-relocations -ffunction-sections $(ARCH) -D__3DS__
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11
ASFLAGS  := -g $(ARCH)
LDFLAGS  := -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(TARGET).map
LIBS     := -lcitro2d -lcitro3d -lctru -lm

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT := $(CURDIR)/$(TARGET)
export TOPDIR := $(CURDIR)
export VPATH  := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR:= $(CURDIR)/$(BUILD)

CFILES   := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))

ifeq ($(strip $(CPPFILES)),)
  export LD := $(CC)
else
  export LD := $(CXX)
endif

export OFILES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o)
export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) -I$(CURDIR)/$(BUILD)
export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

export _3DSXFLAGS := --smdh=$(CURDIR)/$(TARGET).smdh
ifneq ($(ROMFS),)
  export _3DSXFLAGS += --romfs=$(CURDIR)/$(ROMFS)
endif

.PHONY: all clean cia release

all: $(BUILD) $(OUTPUT).3dsx

$(BUILD):
	@mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

$(OUTPUT).3dsx: $(OUTPUT).elf $(OUTPUT).smdh
$(OUTPUT).elf: $(OFILES)

cia: all
	@bannertool makebanner -i $(BANNER_IMAGE) -a assets/banner.wav -o $(BUILD)/banner.bin 2>/dev/null || bannertool makebanner -i $(BANNER_IMAGE) -o $(BUILD)/banner.bin
	@bannertool makesmdh -s "$(APP_TITLE)" -l "$(APP_DESCRIPTION)" -p "$(APP_AUTHOR)" -i $(ICON) -o $(TARGET).smdh
	@makerom -f cia -o $(TARGET).cia -rsf assets/cia.rsf -target t -exefslogo -elf $(OUTPUT).elf -icon $(TARGET).smdh -banner $(BUILD)/banner.bin || makerom -f cia -o $(TARGET).cia -DAPP_ENCRYPTED=false -elf $(OUTPUT).elf -icon $(TARGET).smdh
	@echo "-> $(TARGET).cia"

release: cia
	@mkdir -p release
	@cp -v $(TARGET).cia $(TARGET).3dsx release/ 2>/dev/null || true
	@(cd setup/sd_files && zip -r ../../release/DSi-FC-Men-SD-Setup.zip .)
	@echo "Release files in release/"

clean:
	@rm -rf $(BUILD) $(TARGET).3dsx $(TARGET).elf $(TARGET).smdh $(TARGET).cia $(TARGET).map release

else
DEPENDS := $(OFILES:.o=.d)
%.3dsx: %.elf
	@$(SSTRIP) $<
	@3dsxtool $< $@ $(_3DSXFLAGS)
%.elf:
	@$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@
-include $(DEPENDS)
endif
