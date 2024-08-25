#ifndef MUSIC_PROVIDER_H
#define MUSIC_PROVIDER_H
#include<string>

void initMix();
void closeMusic();
void shootPlay();
static Mix_Music* loadMusic(std::string path);
static Mix_Chunk* loadChunk(std::string path);
void playMusicBackground();
void renderMusicBackground(SDL_Renderer* renderer, SDL_Rect musicButtonRect);
void musicButtonAdapter(SDL_Renderer* renderer, SDL_Texture* musicOn, SDL_Texture* musicOff, SDL_Rect musicRect, bool openMusic);
#endif