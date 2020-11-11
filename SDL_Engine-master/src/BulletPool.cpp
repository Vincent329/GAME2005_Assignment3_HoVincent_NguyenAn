#include "BulletPool.h"

BulletPool::BulletPool(int size)
{
	this->size = size;

	m_pBullets = new Enemy[size];
	std::cout << "Bullet pool created with size " << size << std::endl;
}

BulletPool::~BulletPool()
{

}

Enemy* BulletPool::spawn() {

	Enemy* bullet = NULL;
	if (index < size)
	{
		bullet = &m_pBullets[index];
		index++;
	}
	else {
		std::cout << "Max bullets spawned" << std::endl;
	}

	return bullet;
}


