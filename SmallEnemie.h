#pragma once
#include"Enemie.h"
#include"Bullet.h"

/*
	For the first code, I want to create some method for describering how small enemies,
medium enemies, etc... fight, but now I just want all enemies use only a method for fighting
	It mean this class name is SmallEnemie but it can create Large and Medium enemies
*/

enum class EnemieSize {
	SMALL,
	MEDIUM,
	LARGE,
};


class SmallEnemie :public Enemie {
public:
	int enemieId,enemieHp;
	int enemieLostHp;
	int shotSize;
	bool isInit = false;
	EnemieSize enemieSize;
	SDL_Point enemieCenterPoint;
	SDL_Texture* shotTexture;
	std::vector<Bullet>* bullets;
	SmallEnemie(int enemieId,EnemieSize enemieSize,const int x,const int y,
		SDL_Texture* enemieShape, SDL_Renderer* renderer, SDL_Rect shipRect,
		std::vector<Bullet>& bullets);
	void execute(SDL_Rect spaceShipRect,std::vector<SmallEnemie>& enemies,
		int& shipHp);
	void init(SDL_Texture* shotTextures[]);
	bool operator==(const SmallEnemie& other) const {
		return id == other.id; 
	}
	
	void enemieExplorsionAnimation(std::vector<SmallEnemie>& enemies);
	void deleteEnemie(SmallEnemie& enemie, std::vector<SmallEnemie>& enemies);
};
