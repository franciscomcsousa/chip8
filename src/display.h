#pragma once
#include <SDL2/SDL.h>
#include <array>
#include <map>

class Display
{
public:
    Display(int scale);
    ~Display();

    using Keys = std::array<bool, 16>;

    bool &operator()(int row, int col);

    static constexpr int ROWS = 32;
    static constexpr int COLS = 64;

    void draw();
    void clear();
    Keys get_input();
    bool should_quit();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    int scale;
    bool quit;

    std::array<std::array<bool, COLS>, ROWS> pixels;
    Keys keys;

    u_int8_t translate_input(int raw_key);
};