#include<SDL.h>
#include<SDL_mixer.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<iostream>
#include "Music.h"

using namespace std;

const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 780;
const int MAP_WIDTH = 2400;
const int MAP_HEIGHT = 1800;
const int TILE_SIZE = 800;
SDL_Rect musicRect = { 750,500,30,30 };
SDL_Rect newGameRect = { 700,300,150,50 };
SDL_Rect exitRect = { 700,400,150,50 };
void SDL_Init(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) cout << "ERROR";
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    IMG_Init(IMG_INIT_PNG);
    initMix();
}
void QuitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

SDL_Texture* loadTexture(const string &file,SDL_Renderer* renderer) {
    return IMG_LoadTexture(renderer, file.c_str());
}
bool onButtonClicked(SDL_Rect buttonRect, int mouseX, int mouseY) {
    return (mouseX > buttonRect.x && mouseX < buttonRect.x + buttonRect.w &&
        mouseY > buttonRect.y && mouseY < buttonRect.y + buttonRect.h);
}

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Init(window, renderer);
    
    SDL_Texture* texture = loadTexture("C:\\Users\\ADMIN\\Pictures\\Image\\SB.png", renderer);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = SCREEN_WIDTH;
    rect.h = SCREEN_HEIGHT;
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    
    SDL_Texture* musicOn = loadTexture("image/ui/music-on.png", renderer);
    SDL_Texture* musicOff = loadTexture("image/ui/music-off.png", renderer);
    SDL_Texture* newGame = loadTexture("image/ui/newgame.png", renderer);
    SDL_Texture* exitGame = loadTexture("image/ui/exit.png", renderer);
    SDL_Texture* spaceShip = loadTexture("image/Componant/spaceShip/ships/green.png", renderer);

    SDL_Rect spaceShipRect = { 300,300,300,300 };
    double angel = 45.0;
    SDL_Point spacePoint = { 150,150 };
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    
    renderMusicBackground(renderer, musicRect);

    SDL_RenderCopyEx(renderer, spaceShip, NULL, &spaceShipRect, angel, &spacePoint, flip);
    SDL_RenderCopy(renderer, musicOn, NULL, &musicRect);
    SDL_RenderCopy(renderer, newGame, NULL, &newGameRect);
    SDL_RenderCopy(renderer, exitGame, NULL, &exitRect);
    SDL_RenderPresent(renderer);

    playMusic();

    bool quit = false; SDL_Event e; bool openMusic = true;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (onButtonClicked(musicRect, mouseX, mouseY)) {
                    openMusic = !openMusic;
                    musicButtonAdapter(renderer, musicOn, musicOff, musicRect,openMusic);
                }
                if (onButtonClicked(exitRect, mouseX, mouseY)) quit = true;
            }
        }
    }
    QuitSDL(window, renderer);

    return 0;
}
