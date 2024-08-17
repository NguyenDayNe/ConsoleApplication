#ifndef MUSIC_PROVIDER_H
#define MUSIC_PROVIDER_H

void initMix();
void closeMusic();
void playMusic();
void renderMusicBackground(SDL_Renderer* renderer, SDL_Rect musicButtonRect);
void musicButtonAdapter(SDL_Renderer* renderer, SDL_Texture* musicOn, SDL_Texture* musicOff, SDL_Rect musicRect, bool openMusic);
#endif