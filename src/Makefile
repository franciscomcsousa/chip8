cat > Makefile << 'EOF'
CXX      = g++
CXXFLAGS = -std=c++17 -Wall $(shell sdl2-config --cflags)
LIBS     = $(shell sdl2-config --libs)

SRC      = src/main.cpp src/display.cpp
TARGET   = chip8

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
EOF