CXX       = g++
CXXFLAGS  = -std=c++17 -Wall $(shell sdl2-config --cflags)
LIBS      = $(shell sdl2-config --libs)
SANITIZER = 
 
SRC       = src/main.cpp src/display.cpp src/memory.cpp
TARGET    = chip8

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SANITIZER) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
