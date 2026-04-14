#include "display.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include "memory.h"

const int cols = 64;
const int rows = 32;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n"
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::filesystem::path rom_path = argv[3];

    if (!std::filesystem::exists(rom_path))
    {
        std::cout << "ROM file does not exist." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    Display display{atoi(argv[1]), rows, cols};
    Memory memory{};
    memory.load_rom(rom_path);

    memory.dump();

    while (!display.shouldQuit())
    {
        for (int y = 0; y < rows; y++)
            for (int x = 0; x < cols; x++)
                display(y, x) = (x + y) % ((rand() % 10) + 1) == 0;

        display.draw();
        sleep(1);
    }

    return 0;
}