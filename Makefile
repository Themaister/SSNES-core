TARGET = libsnes.so
HEADERS = $(wildcard */*.h)
INCDIRS = -Icpu -Ismp -Ippu -Isystem
PIC = -fPIC
CFLAGS += -O3 -g -Wall -std=gnu99 $(PIC)
LDFLAGS += -shared

OBJ = cpu/cpu.o apu/apu.o smp/smp.o dsp/dsp.o system/system.o


all: $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCDIRS)

$(TARGET): $(OBJ)
	$(CC) -o $@ -shared $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean
