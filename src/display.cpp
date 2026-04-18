#include "display.h"

#include <iostream>

Display::Display(int scale) : scale(scale), pixels({}), keys({})
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

Display::Keys Display::get_input()
{
    quit = false;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        int key_code = translate_input(event.key.keysym.sym);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
        {
            if (key_code <= 0xf)
            {
                keys[key_code] = 1;
            }
            break;
        }
        case SDL_KEYUP:
        {
            if (key_code <= 0xf)
                keys[key_code] = 0;
            break;
        }
        default:
            break;
        }
    }
    return keys;
}

u_int8_t Display::translate_input(int raw_key)
{
    switch (raw_key)
    {
    case SDLK_x:
        return 0x0;
    case SDLK_1:
        return 0x1;
    case SDLK_2:
        return 0x2;
    case SDLK_3:
        return 0x3;
    case SDLK_q:
        return 0x4;
    case SDLK_w:
        return 0x5;
    case SDLK_e:
        return 0x6;
    case SDLK_a:
        return 0x7;
    case SDLK_s:
        return 0x8;
    case SDLK_d:
        return 0x9;
    case SDLK_z:
        return 0xa;
    case SDLK_c:
        return 0xb;
    case SDLK_4:
        return 0xc;
    case SDLK_r:
        return 0xd;
    case SDLK_f:
        return 0xe;
    case SDLK_v:
        return 0xf;
    default:
        return 0xff;
    }
}

bool Display::should_quit()
{
    return quit;
}