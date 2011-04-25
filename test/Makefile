TARGET = test.sfc
ASM_OBJ = test.obj
ASM_SRC = test.asm
ASM_ALL_SRC = $(wildcard *.asm)
ASM_ALL_INC = $(wildcard *.inc)

WLA_65816 = wla-65816
WLA_LINK = wlalink
WLA_LDFILE = test.ld

all: $(TARGET)

$(TARGET): $(ASM_OBJ) $(WLA_LDFILE)
	$(WLA_LINK) -vr $(WLA_LDFILE) $@

%.obj: %.asm $(ASM_ALL_SRC) $(ASM_ALL_INC) $(TARGET_SPC)
	$(WLA_65816) -o $< $@

clean:
	rm -f $(TARGET)
	rm -f $(TARGET_SPC)
	rm -f $(ASM_OBJ)
	rm -f $(ASM_SPC_OBJ)

.PHONY: clean
