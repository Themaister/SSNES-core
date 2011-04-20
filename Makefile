TARGET = libsnes.so
HEADERS = $(wildcard */*.h)
INCDIRS = -Icpu -Ismp -Idsp -Ippu -Isystem -I.
PIC = -fPIC
CFLAGS += -O3 -g -Wall -std=gnu99 $(PIC)
LDFLAGS += -shared

SOURCES = $(wildcard */*.c)
OBJ = $(SOURCES:.c=.o)

all: $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCDIRS)

$(TARGET): $(OBJ)
	$(CC) -o $@ -shared $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean
