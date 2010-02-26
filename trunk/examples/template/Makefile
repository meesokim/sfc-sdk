# Build path information.
SOURCE := src
BUILD  := build
VPATH  := $(foreach dir,$(SOURCE),$(CURDIR)/$(dir))

# Toolchain information.
TOOLCHAIN    := ../toolchain
AS           := $(TOOLCHAIN)/wla-65816
ASFLAGS      := -l
LD           := $(TOOLCHAIN)/wlalink
LDFLAGS      := -r
PREPROCESSOR := cpp

PROJECT  := template
ASMFILES := $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.asm)))
LIBFILES := $(ASMFILES:.asm=.lib)

# Build rules.
$(PROJECT).sfc : $(patsubst %,$(BUILD)/%, $(LIBFILES))
#	@rm -rf temp.asm
	del /F temp.asm
	@echo "linking $(PROJECT).sfc.."
	@$(LD) $(LDFLAGS) $(PROJECT).link $(PROJECT).sfc

$(BUILD)/main.lib : main.asm
	@echo "[as] $<"
	@$(PREPROCESSOR) -w -P -I$(SOURCE) $< > temp.asm
	@$(AS) -o temp.asm $@

$(BUILD)/%.lib : %.asm
	@echo "[as] $<"
	@$(PREPROCESSOR) -w -P -I$(SOURCE) $< > temp.asm
	@$(AS) $(ASFLAGS) temp.asm $@

clean :
#	@rm -rf $(BUILD)/*
	del /F $(BUILD)