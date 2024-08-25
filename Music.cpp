#include "SDL_mixer.h"
#include "SDL.h"
#include "Music.h"
#include<string>
Mix_Music* music = nullptr;
Mix_Chunk* shoot = nullptr;
std::string BACKGROUND_MUSIC_PATH = "music/FF.mp3";
std::string SHOOT_SOUND_PATH = "music/shot.flac";
void initMix() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    music = loadMusic(BACKGROUND_MUSIC_PATH);
    shoot = loadChunk(SHOOT_SOUND_PATH);
}
static Mix_Music* loadMusic(std::string path) {
    return Mix_LoadMUS(path.c_str());
}
static Mix_Chunk* loadChunk(std::string path) {
    return Mix_LoadWAV(path.c_str());
}
void closeMusic() {
    Mix_FreeMusic(music);
    Mix_FreeChunk(shoot);
    Mix_Quit();
}
void renderMusicBackground(SDL_Renderer* renderer, SDL_Rect musicButtonRect) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &musicButtonRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}
void playMusicBackground() {
    Mix_PlayMusic(music, -1);
}

void shootPlay() {
    Mix_PlayChannel(-1,shoot, 0);
}
void musicButtonAdapter(SDL_Renderer* renderer, SDL_Texture* musicOn, SDL_Texture* musicOff, SDL_Rect musicRect, bool openMusic) {
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