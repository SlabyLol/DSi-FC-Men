#---------------------------------------------------------------------------------
export DEVKITPRO ?= /opt/devkitpro
export DEVKITARM ?= $(DEVKITPRO)/devkitARM

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO")
endif

ifneq ($(wildcard $(DEVKITPRO)/3ds_rules),)
include $(DEVKITPRO)/3ds_rules
else
include $(DEVKITPRO)/devkitARM/3ds_rules
endif

TARGET		:=	DSi-FC-Men
BUILD		:=	build
SOURCES		:=	source
INCLUDES	:=	source
ROMFS		:=	romfs

APP_TITLE		:=	DSi-FC-Men
APP_DESCRIPTION	:=	DSi FlashCard Menu
APP_AUTHOR		:=	SlabyLol

ARCH	:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
CFLAGS	:=	-g -Wall -O2 -mword-relocations -ffunction-sections $(ARCH)
CFLAGS	+=	$(INCLUDE) -D__3DS__
CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11
ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)
LIBS	:=	-lcitro2d -lcitro3d -lctru -lm

LIBDIRS	:=	$(CTRULIB) $(PORTLIBS)

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES := $(OFILES_SOURCES)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

# ALWAYS absolute path so sub-make in build/ finds the icon
export APP_ICON := $(CURDIR)/assets/icon.png

ifeq ($(strip $(NO_SMDH)),)
	export _3DSXFLAGS += --smdh=$(OUTPUT).smdh
endif
ifneq ($(ROMFS),)
	export _3DSXFLAGS += --romfs=$(CURDIR)/$(ROMFS)
endif

.PHONY: all clean cia

all: $(BUILD)

$(BUILD):
	@mkdir -p $@ assets
	@test -f assets/icon.png || python3 tools/gen_assets.py || true
	@test -f assets/icon.png || (echo "ERROR: assets/icon.png missing" && exit 1)
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@rm -fr $(BUILD) $(TARGET).3dsx $(TARGET).smdh $(TARGET).elf $(TARGET).cia $(TARGET).map

cia: all
	@echo "Building CIA..."
	@if command -v bannertool >/dev/null 2>&1; then \
		bannertool makebanner -i $(CURDIR)/assets/banner.png -a $(CURDIR)/assets/banner.wav -o $(BUILD)/banner.bin 2>/dev/null || \
		bannertool makebanner -i $(CURDIR)/assets/banner.png -o $(BUILD)/banner.bin ; \
		bannertool makesmdh -s "$(APP_TITLE)" -l "$(APP_DESCRIPTION)" -p "$(APP_AUTHOR)" -i $(CURDIR)/assets/icon.png -o $(TARGET).smdh ; \
	fi
	@if command -v makerom >/dev/null 2>&1; then \
		makerom -f cia -o $(TARGET).cia -rsf $(CURDIR)/assets/cia.rsf -target t -exefslogo \
			-elf $(OUTPUT).elf -icon $(TARGET).smdh -banner $(BUILD)/banner.bin 2>/dev/null || \
		makerom -f cia -o $(TARGET).cia -DAPP_ENCRYPTED=false -elf $(OUTPUT).elf -icon $(TARGET).smdh ; \
		ls -la $(TARGET).cia ; \
	fi

else

.PHONY: all
DEPENDS	:=	$(OFILES:.o=.d)

all: $(OUTPUT).3dsx

$(OUTPUT).3dsx: $(OUTPUT).elf $(OUTPUT).smdh
	@echo "built ... $(notdir $@)"

$(OUTPUT).elf: $(OFILES)
	@echo linking $(notdir $@)
	@$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

$(OUTPUT).smdh: $(APP_ICON)
	@echo "smdh from $(APP_ICON)"
	@smdhtool --create "$(APP_TITLE)" "$(APP_DESCRIPTION)" "$(APP_AUTHOR)" "$(APP_ICON)" $@ 2>/dev/null || \
	 bannertool makesmdh -s "$(APP_TITLE)" -l "$(APP_DESCRIPTION)" -p "$(APP_AUTHOR)" -i "$(APP_ICON)" -o $@ 2>/dev/null || \
	 (echo "WARNING: smdhtool/bannertool failed, touching empty smdh"; touch $@)

-include $(DEPENDS)

endif
