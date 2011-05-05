TARGET = libsnes.so
HEADERS = $(wildcard */*.h)
INCDIRS = -I.
PIC = -fPIC
CFLAGS += -O3 -g -Wall -std=c99 $(PIC)

SOURCES = $(wildcard */*.c)
OBJ = $(SOURCES:.c=.o)
TEST_OBJ = main_test.o

all: lib

lib: $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCDIRS)

$(TARGET): $(OBJ)
	$(CC) -shared -o $@ $(OBJ) $(LDFLAGS)


clean:
	rm -f $(OBJ) $(TARGET) $(TEST_OBJ) $(TEST_TARGET)

.PHONY: clean lib
