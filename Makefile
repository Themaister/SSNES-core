TARGET = libsnes.so
HEADERS = $(wildcard */*.h)
INCDIRS = -I.
PIC = -fPIC
CFLAGS += -O3 -g -Wall -std=c99 $(PIC)
CXXFLAGS += -O3 -g -Wall $(PIC)

CSOURCES = $(wildcard */*.c)
CXXSOURCES = $(wildcard */*.cpp)
OBJ = $(CSOURCES:.c=.o) $(CXXSOURCES:.cpp=.o)
TEST_OBJ = main_test.o

ifneq ($(DEBUG),)
   CFLAGS += -DSSNES_DEBUG
endif

all: lib

lib: $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCDIRS)

%.o: %.cpp $(HEADERS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(INCDIRS)

$(TARGET): $(OBJ)
	$(CC) -shared -o $@ $(OBJ) $(LDFLAGS)


clean:
	rm -f $(OBJ) $(TARGET) $(TEST_OBJ) $(TEST_TARGET)

.PHONY: clean lib
