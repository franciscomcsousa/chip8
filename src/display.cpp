#include "display.h"

#include <iostream>

Display::Display(int scale, int rows, int cols)
    : scale(scale), rows(rows), cols(cols), pixels(rows, std::vector<char>(cols, 0))
{
    SDL_Init(SDL_INIT_VIDEO);
    window =
        SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         cols * scale, rows * scale, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Display::~Display()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

char &Display::operator()(int row, int col)
{
    return pixels[row][col];
}

void Display::draw()
{
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            SDL_Rect rect = {x * scale, y * scale, scale, scale};
            if (pixels[y][x])
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer);
}

bool Display::shouldQuit()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
        if (event.type == SDL_QUIT)
            return true;
    return false;
}