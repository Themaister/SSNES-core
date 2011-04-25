TARGET = libsnes.a
TEST_TARGET = main_test
HEADERS = $(wildcard */*.h)
INCDIRS = -I.
PIC =
CFLAGS += -O3 -g -Wall -std=gnu99 $(PIC)

SOURCES = $(wildcard */*.c)
OBJ = $(SOURCES:.c=.o)
TEST_OBJ = main_test.o

all: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) lib
	$(CC) -o $@ $(TEST_OBJ) $(TARGET)


%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCDIRS)

$(TARGET): $(OBJ)
	$(AR) rcs $@ $(OBJ) $(LDFLAGS)

lib: $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_OBJ) $(TEST_TARGET)

.PHONY: clean lib
