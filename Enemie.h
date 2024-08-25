#pragma once
#include<SDL.h>
#include<random>
#include<vector>
extern std::random_device dv;
extern std::mt19937 gen;
class Enemie
{
public:
	int id,x,y;
	int enemieHp;
	int horizontalMoveStack;
	bool moveLeft;
	bool moveRight;
	int random,currentExplorsionAnimationId=0;
	int currentStackFall=0;
	bool alive,isExlorsion;
	SDL_Texture* enemieShape;
	SDL_Renderer* renderer;
	SDL_Rect shipRect,enemieRect;
	Enemie(const int x,const int y, SDL_Texture* enemieShape,SDL_Renderer* renderer,SDL_Rect shipRect);
	static bool onDamageEvent(SDL_Rect enemieRect,SDL_Rect shipRect);

};