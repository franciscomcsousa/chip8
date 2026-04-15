#pragma once
#include <SDL2/SDL.h>
#include <array>

class Display
{
public:
    Display(int scale);
    ~Display();

    bool &operator()(int row, int col);

    static constexpr int ROWS = 32;
    static constexpr int COLS = 64;

    void draw();
    void clear();
    bool shouldQuit();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    int scale;

    std::array<std::array<bool, COLS>, ROWS> pixels;
};