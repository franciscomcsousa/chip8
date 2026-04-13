#include "display.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <unistd.h>
#include "memory.h"

const int cols = 64;
const int rows = 32;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
        std::exit(EXIT_FAILURE);
    }

    std::cout << argv[1] << " " << argv[2] << " " << argv[3];

    Display display{atoi(argv[1]), rows, cols};
    Memory memory{};

    // Checkerboard pattern

    while (!display.shouldQuit())
    {
        for (int y = 0; y < rows; y++)
            for (int x = 0; x < cols; x++)
                display(y, x) = (x + y) % ((rand() % 10) + 1) == 0;

        display.draw();
        sleep(1);
        system("clear");
        memory[rand() % 0x1000] = 0x16;
        memory.dump();
    }

    return 0;
}