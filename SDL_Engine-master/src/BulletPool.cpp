#include "BulletPool.h"

BulletPool::BulletPool(int size)
{

	for (int i = 0; i < size; i++)
	{
		inactive.push_back(new Enemy());
	}

	std::cout << "Bullet pool created with size " << size << std::endl;
}

BulletPool::~BulletPool()
{

}

Enemy* BulletPool::spawn() {

	Enemy* bullet = NULL;

	if (inactive.size() > 0) {
		bullet = inactive.back();
		bullet->Reset();
		bullet->active = true;
		inactive.pop_back();
		active.push_back(bullet);
		std::cout << "Spawn Bullet" << std::endl;
	}
	else {
		std::cout << "Max bullets spawned" << std::endl;
	}

	return bullet;
}

void BulletPool::despawn(Enemy* bullet)
{
	bullet->Reset();
	inactive.push_back(bullet);

	for (std::vector<Enemy*>::iterator myiter = active.begin(); myiter != active.end(); myiter++)
	{
		if (*myiter == bullet)
		{
			active.erase(myiter);
			return;
		}
	}
}

