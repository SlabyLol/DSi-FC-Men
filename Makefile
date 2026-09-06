#---------------------------------------------------------------------------------
# DSi-FC-Men – 3DS Makefile
#---------------------------------------------------------------------------------
export DEVKITPRO ?= /opt/devkitpro
export DEVKITARM ?= $(DEVKITPRO)/devkitARM

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO. export DEVKITPRO=/opt/devkitpro")
endif

ifneq ($(wildcard $(DEVKITPRO)/3ds_rules),)
  include $(DEVKITPRO)/3ds_rules
else ifneq ($(wildcard $(DEVKITPRO)/devkitARM/3ds_rules),)
  include $(DEVKITPRO)/devkitARM/3ds_rules
else
  $(error "3ds_rules not found under $(DEVKITPRO). Run: dkp-pacman -S 3ds-dev")
endif

TARGET		:=	DSi-FC-Men
BUILD		:=	build
SOURCES		:=	source
INCLUDES	:=	source
ROMFS		:=	romfs

APP_TITLE		:=	DSi-FC-Men
APP_DESCRIPTION	:=	DSi FlashCard Menu
APP_AUTHOR		:=	SlabyLol

LIBCTRU_INC	:=	$(DEVKITPRO)/libctru/include
LIBCTRU_LIB	:=	$(DEVKITPRO)/libctru/lib
PORTLIBS	:=	$(DEVKITPRO)/portlibs/3ds

ARCH	:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
CFLAGS	:=	-g -Wall -O2 -mword-relocations -ffunction-sections $(ARCH)
CFLAGS	+=	$(INCLUDE) -D__3DS__
CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11
ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)
LIBS	:=	-lcitro2d -lcitro3d -lctru -lm

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

export OFILES_SOURCES 	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES := $(OFILES_SOURCES)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(LIBCTRU_INC) \
			-I$(PORTLIBS)/include \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
			-L$(LIBCTRU_LIB) \
			-L$(PORTLIBS)/lib

export APP_ICON := $(TOPDIR)/assets/icon.png
export _3DSXFLAGS := --smdh=$(CURDIR)/$(TARGET).smdh
ifneq ($(ROMFS),)
export _3DSXFLAGS += --romfs=$(CURDIR)/$(ROMFS)
endif

.PHONY: all clean cia release

all: $(BUILD) $(OUTPUT).3dsx

$(BUILD):
	@mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

$(OUTPUT).3dsx	:	$(OUTPUT).elf $(OUTPUT).smdh
$(OUTPUT).elf	:	$(OFILES)

cia: all
	@echo "Building CIA..."
	@mkdir -p $(BUILD)
	@if command -v bannertool >/dev/null 2>&1; then \
		bannertool makebanner -i assets/banner.png -a assets/banner.wav -o $(BUILD)/banner.bin 2>/dev/null || \
		bannertool makebanner -i assets/banner.png -o $(BUILD)/banner.bin ; \
		bannertool makesmdh -s "$(APP_TITLE)" -l "$(APP_DESCRIPTION)" -p "$(APP_AUTHOR)" -i assets/icon.png -o $(TARGET).smdh ; \
	fi
	@if command -v makerom >/dev/null 2>&1; then \
		makerom -f cia -o $(TARGET).cia -rsf assets/cia.rsf -target t -exefslogo \
			-elf $(OUTPUT).elf -icon $(TARGET).smdh -banner $(BUILD)/banner.bin 2>/dev/null || \
		makerom -f cia -o $(TARGET).cia -DAPP_ENCRYPTED=false -elf $(OUTPUT).elf -icon $(TARGET).smdh ; \
		echo "-> $(TARGET).cia" ; \
	else \
		echo "makerom not found" ; \
	fi

release: cia
	@mkdir -p release
	@cp -v $(TARGET).cia $(TARGET).3dsx release/ 2>/dev/null || true
	@(cd setup/sd_files && zip -r ../../release/DSi-FC-Men-SD-Setup.zip .)

clean:
	@rm -fr $(BUILD) $(TARGET).3dsx $(OUTPUT).smdh $(TARGET).elf $(TARGET).cia $(TARGET).map release

else

DEPENDS	:=	$(OFILES:.o=.d)
$(OUTPUT).3dsx	:	$(OUTPUT).elf $(OUTPUT).smdh
$(OUTPUT).elf	:	$(OFILES)
-include $(DEPENDS)

endif
