#pragma once

#include "Enemy.h"
#include <iostream>

class BulletPool
{
public:
	BulletPool(int);
	~BulletPool();
	
	int size;
	int index = 0;

	Enemy* spawn();
	//Enemy* despawn();

private:
	Enemy* m_pBullets;

	std::vector<Enemy> active;
	std::vector<Enemy> inactive;
};