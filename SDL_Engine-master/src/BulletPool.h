#pragma once

#include "Enemy.h"
#include <iostream>

class BulletPool
{
public:
	BulletPool(int);
	~BulletPool();

	Enemy* spawn();
	void despawn(Enemy*);

	std::vector<Enemy*> all; // keeps track of all bullets in pool
private:
	std::vector<Enemy*> inactive;
	std::vector<Enemy*> active;
};