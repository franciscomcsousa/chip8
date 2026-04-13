#pragma once
#include <SDL2/SDL.h>
#include <vector>

class Display
{
public:
    Display(int scale, int rows, int cols);
    ~Display();

    char &operator()(int row, int col);

    void draw();
    bool shouldQuit();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    int scale;
    int rows;
    int cols;
    std::vector<std::vector<char>> pixels;
};