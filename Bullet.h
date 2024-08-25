#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<vector>

const SDL_RendererFlip flip = SDL_FLIP_NONE;

const float alpha = 90.0;
const int ANIMATION_COUNT = 100;
class Bullet
{
public:
	SDL_Texture* image;
	SDL_Renderer* renderer;
	SDL_Rect bulletRect;
	SDL_Point bulletPoint;
	int bulletDelay;
	int bulletCount=0;
	int id;
	int enemieId;
	int animationCount = ANIMATION_COUNT;
	bool isDestroy = false;
	Bullet(SDL_Renderer* renderer,SDL_Texture* image, SDL_Rect bulletRect,int bulletDelay,int enemieId);
	void bulletRender(int screenHeight, std::vector<Bullet>& bullets);
	void destroyBullet(std::vector<Bullet>& bullets);

	bool operator==(const Bullet& other) const {
		return id == other.id;
	}
};

