#include "SmallEnemie.h"
#include "Enemie.h"
#include<SDL_image.h>
#include <random>
#include<string>
#include<iostream>


const std::string path = "image/enemies/animation/Explosions/";
const int ENEMIES_VERTICAL_SPEED_SLOWDOWN = 10;
const int ENEMIES_HORIZONTAL_SPEED_SLOWDOWN = 300;

const int HORIZONTAL_MOVE_STACK = 20;
const int HORIZONTAL_MOVE_SIZE = 10;

const int FREQUENCY_SHOT = 2000;
const int SHOT_SPEED = 7;//	HIGHER IT IS, SLOWER IT IS

const int ENEMIE_SMALL_SIZE = 80;
const int ENEMIE_MEDIUM_SIZE = 120;
const int ENEMIE_LARGE_SIZE = 150;
const int ENEMIE_SMALL_HP = 5000;
const int ENEMIE_MEDIUM_HP = 8000;
const int ENEMIE_LARGE_HP = 15000;
const int ENEMIE_SMALL_LOST_HP = 100;
const int ENEMIE_MEDIUM_LOST_HP = 150;
const int ENEMIE_LARGE_LOST_HP = 200;
const int ENEMIE_LOST_HP = 400;
const int SHOT_SMALL_SIZE = 70;
const int SHOT_MEDIUM_SIZE = 80;
const int SHOT_LARGE_SIZE = 100;

const int ENEMIE_SMALL_SCORE = 100;
const int ENEMIE_MEDIUM_SCORE = 200;
const int ENEMIE_LARGE_SCORE = 500;

const int MAX_EXPLOSION_ANIMATION_COUNT = 10;
SDL_Rect headEnemie;

std::uniform_int_distribution<> distance(1, 2);


SmallEnemie::SmallEnemie(int enemieId,EnemieSize enemieSize,const int x,const int y, SDL_Texture* enemieShape, SDL_Renderer* renderer, SDL_Rect shipRect,std::vector<Bullet>& bullets)
	:Enemie(x,y,enemieShape,renderer,shipRect),enemieId(enemieId),enemieSize(enemieSize),bullets(&bullets) {};



void SmallEnemie::init(SDL_Texture* shotTextures[]) {
	if (!isInit) {
		moveLeft = false;
		moveRight = false;
		horizontalMoveStack = 0;
		alive = true;
		isExlorsion = false;
		shotTexture = shotTextures[enemieId-1];
		std::uniform_int_distribution<> dis(0, x);
		random = dis(gen);
		id = random;
		int enemieIntSize=0;
		switch (enemieSize) {
			case EnemieSize::SMALL:
				enemieHp = ENEMIE_SMALL_HP;
				enemieIntSize = ENEMIE_SMALL_SIZE;
				enemieLostHp = ENEMIE_SMALL_LOST_HP;
				shotSize = SHOT_SMALL_SIZE;
				enemieScore = ENEMIE_SMALL_SCORE;
				break;
			case EnemieSize::MEDIUM:
				enemieHp = ENEMIE_MEDIUM_HP;
				enemieIntSize = ENEMIE_MEDIUM_SIZE;
				enemieLostHp = ENEMIE_MEDIUM_LOST_HP;
				shotSize = SHOT_MEDIUM_SIZE;
				enemieScore = ENEMIE_MEDIUM_SCORE;

				break;
			case EnemieSize::LARGE:
				enemieHp = ENEMIE_LARGE_HP;
				enemieIntSize = ENEMIE_LARGE_SIZE;
				enemieLostHp = ENEMIE_LARGE_LOST_HP;
				shotSize = SHOT_LARGE_SIZE;
				enemieScore = ENEMIE_LARGE_SCORE;

				break;
		}
		enemieRect = { random,-enemieIntSize,enemieIntSize,enemieIntSize };
		enemieCenterPoint = { enemieIntSize / 2,enemieIntSize / 2 };
		isInit = true;
	}
}
void SmallEnemie::execute(SDL_Rect spaceShipRect,std::vector<SmallEnemie>& enemies,
	int &shipHp,int &score) {
	if (alive) {
		currentStackFall++;

		if (currentStackFall % ENEMIES_VERTICAL_SPEED_SLOWDOWN == 0) enemieRect.y++;
		if (currentStackFall % ENEMIES_HORIZONTAL_SPEED_SLOWDOWN == 0) {
			if (moveLeft && horizontalMoveStack > 0) {
				enemieRect.x -= HORIZONTAL_MOVE_SIZE;
				horizontalMoveStack--;
			}
			if (moveRight && horizontalMoveStack > 0) {
				enemieRect.x += HORIZONTAL_MOVE_SIZE;
				horizontalMoveStack--;
			}
			if(horizontalMoveStack==0) {
				horizontalMoveStack = HORIZONTAL_MOVE_STACK;
				moveLeft = false;
				moveRight = false;
				switch (distance(gen)) {
					case 1:
						moveLeft = true;
						break;
					case 2:
						moveRight = true;
						break;
				}
			}
			if (enemieRect.x < 0) {
				enemieRect.x = 0;
				moveRight = true;
				moveLeft = false;
				horizontalMoveStack = HORIZONTAL_MOVE_STACK;
			}
			if (enemieRect.x > x) {
				enemieRect.x = x;
				moveLeft = true;
				moveRight = false;
				horizontalMoveStack = HORIZONTAL_MOVE_STACK;
			}
		}
		
		if (currentStackFall % FREQUENCY_SHOT == 0) {
			headEnemie = { enemieRect.x + enemieRect.w / 2 - shotSize/2,
				enemieRect.y+enemieRect.w-shotSize/2,
				shotSize,shotSize };
			
			bullets->push_back(Bullet(renderer, shotTexture,headEnemie,SHOT_SPEED,enemieId));
		}
		
		SDL_RenderCopyEx(renderer, enemieShape, NULL, &enemieRect,alpha,&enemieCenterPoint,flip);
		
		if (onDamageEvent(enemieRect, spaceShipRect)) {
			enemieHp -= ENEMIE_LOST_HP;
			shipHp-=enemieLostHp;
		}
		
		if (enemieHp <= 0) alive = false;
		if (!alive) {
			isExlorsion = true;
			score += enemieScore;
		}
	}
	if (isExlorsion) enemieExplorsionAnimation(enemies);
}

void SmallEnemie::enemieExplorsionAnimation(std::vector<SmallEnemie>& enemies) {
	currentExplorsionAnimationId++;
	std::string extends = "Ship" + std::to_string(enemieId) + "_Explosion" ;
	std::string filePath = path+ extends + "/" +std::to_string(currentExplorsionAnimationId) + ".png";
	SDL_Texture* currentAnimationPicture = IMG_LoadTexture(
		renderer, filePath.c_str());
	SDL_RenderCopy(renderer, currentAnimationPicture, NULL, &enemieRect);
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(currentAnimationPicture);
	if (currentExplorsionAnimationId == MAX_EXPLOSION_ANIMATION_COUNT) isExlorsion = false;
	if (currentExplorsionAnimationId == MAX_EXPLOSION_ANIMATION_COUNT) deleteEnemie(*this, enemies);
}
void SmallEnemie::deleteEnemie(SmallEnemie& enemie, std::vector<SmallEnemie>& enemies) {
	auto it = std::find(enemies.begin(), enemies.end(), enemie);
	if (it != enemies.end()) enemies.erase(it);
}