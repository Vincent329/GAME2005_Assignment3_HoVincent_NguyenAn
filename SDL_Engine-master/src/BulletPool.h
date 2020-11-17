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

	std::vector<Enemy*> active;

private:
	std::vector<Enemy*> inactive;
};