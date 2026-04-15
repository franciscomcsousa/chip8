#include "display.h"

#include <iostream>

Display::Display(int scale) : scale(scale), pixels({})
{
    SDL_Init(SDL_INIT_VIDEO);
    window =
        SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         COLS * scale, ROWS * scale, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Display::~Display()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Accessed by (x,y)
bool &Display::operator()(int col, int row)
{
    return pixels[row][col];
}

void Display::draw()
{
    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLS; x++)
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

void Display::clear()
{
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            pixels[y][x] = 0;
}

bool Display::shouldQuit()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
        if (event.type == SDL_QUIT)
            return true;
    return false;
}