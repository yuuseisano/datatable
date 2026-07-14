#include "Enemy.h"
#include "EnemyFactory.h"
#include <memory>

const EnemyData EnemyFactory::EnemyTable[] = {
	{ 1, "Slime", 10, 5, 2, 1, 5, 10, 1, Element::Water },
	{ 2, "Goblin", 20, 10, 5, 2, 10, 20, 50, Element::Earth },
	{ 3, "Orc", 100, 50, 30, 10, 1000, 5000, 25, Element::Fire },
	{ 4, "Dragon", 1000, 200, 100, 50, 10000, 50000, 10, Element::Wind },
};

const int EnemyFactory::EnemyTableSize = sizeof(EnemyTable) / sizeof(EnemyData);

std::unique_ptr<Enemy> EnemyFactory::CreateEnemy(int ID)
{
	for (int i = 0; i < EnemyTableSize; ++i)
	{
		if (EnemyTable[i].ID == ID)
		{
			return std::make_unique<Enemy>(EnemyTable[i]);
		}
	}
	return nullptr;
}