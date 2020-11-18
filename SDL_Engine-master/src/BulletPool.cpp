#include "BulletPool.h"

BulletPool::BulletPool(int size)
{

	for (int i = 0; i < size; i++)
	{
		Enemy* bullet = new Enemy();
		inactive.push_back(bullet);
		all.push_back(bullet);
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
		std::cout << "Active Count: " << std::to_string(active.size()) << std::endl;

	}
	else {
		std::cout << "Max bullets spawned" << std::endl;
	}

	return bullet;
}

void BulletPool::despawn(Enemy* bullet) // set it to false and keep track of the inactive queue
{
	bullet->active = false;
	inactive.push_back(bullet);

	for (std::vector<Enemy*>::iterator myiter = active.begin(); myiter != active.end(); myiter++)
	{
		if (*myiter == bullet)
		{
			active.erase(myiter);
			std::cout << "Despawned" << std::endl;
			std::cout << "Active Count: " << std::to_string(active.size()) << std::endl;
			return;
		}
	}
}

