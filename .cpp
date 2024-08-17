
#include "SDL_mixer.h"
#include "SDL.h"

Mix_Music* music = nullptr;
void initMix() {
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}
Mix_Music* loadMusic() {
    return Mix_LoadMUS("C:\\Users\\ADMIN\\Downloads\\Music\\FF.mp3");
}
void closeMusic() {
    Mix_FreeMusic(music);
	Mix_Quit();
}
void renderMusicBackground(SDL_Renderer* renderer, SDL_Rect musicButtonRect) {
    SDL_SetRenderDrawColor(renderer, 0, 230, 30, 255);
    SDL_RenderFillRect(renderer, &musicButtonRect);
}
void playMusic() {
    music = loadMusic();
    Mix_PlayMusic(music, -1);
}
void musicButtonAdapter(SDL_Renderer* renderer, SDL_Texture* musicOn, SDL_Texture* musicOff,SDL_Rect musicRect, bool openMusic) {
    if (!openMusic) {
        Mix_PauseMusic();
        renderMusicBackground(renderer, musicRect);
        SDL_RenderCopy(renderer, musicOff, NULL, &musicRect);
        SDL_RenderPresent(renderer);
    }
    else {
        Mix_ResumeMusic();
        renderMusicBackground(renderer, musicRect);
        SDL_RenderCopy(renderer, musicOn, NULL, &musicRect);
        SDL_RenderPresent(renderer);
    }
}