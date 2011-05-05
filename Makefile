TARGET = libsnes.so
TEST_TARGET = main_test
HEADERS = $(wildcard */*.h)
INCDIRS = -I.
PIC = -fPIC
CFLAGS += -O3 -g -Wall -std=c99 $(PIC)

SOURCES = $(wildcard */*.c)
OBJ = $(SOURCES:.c=.o)
TEST_OBJ = main_test.o

all: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) lib
	$(CC) -o $@ $(TEST_OBJ) $(TARGET)


%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCDIRS)

$(TARGET): $(OBJ)
	$(CC) -shared -o $@ $(OBJ) $(LDFLAGS)

lib: $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_OBJ) $(TEST_TARGET)

.PHONY: clean lib
