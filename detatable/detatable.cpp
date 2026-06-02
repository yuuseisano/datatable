#include <iostream>
#include <iomanip>
#include <random>
#include <memory>
#include <vector>
#include "Enemy.h"
#include "EnemyFactory.h"
#include "EnemyData.h"

static const char* ElementToString(Element e)
{
	switch (e)
	{
	case Element::None:  return "None";
	case Element::Fire:  return "Fire";
	case Element::Water: return "Water";
	case Element::Earth: return "Earth";
	case Element::Wind:  return "Wind";
	case Element::Light: return "Light";
	case Element::Dark:  return "Dark";
	default:             return "Unknown";
	}
}

int main()
{
	int tableSize = EnemyFactory::GetEnemyTableSize();
	if (tableSize <= 0)
	{
		std::cout << "敵データが存在しません。" << std::endl;
		return 0;
	}

	// ランダムに4体生成（重複あり）。生成と同時に合計を計算する
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, tableSize - 1);

	std::vector<std::unique_ptr<Enemy>> enemies;
	enemies.reserve(4);

	int totalHP = 0;
	int totalATK = 0;

	for (int i = 0; i < 4; ++i)
	{
		int idx = dist(gen);
		const EnemyData* d = EnemyFactory::GetEnemyDataByIndex(idx);
		if (!d)
		{
			std::cout << "敵データの取得に失敗しました (index=" << idx << ")。" << std::endl;
			continue;
		}

		std::unique_ptr<Enemy> e = EnemyFactory::CreateEnemy(d->ID);
		if (!e)
		{
			std::cout << "敵インスタンスの生成に失敗しました (ID=" << d->ID << ")。" << std::endl;
			continue;
		}

		// 生成と同時に合計値に加算
		totalHP += e->Data.HP;
		totalATK += e->Data.ATK;

		enemies.push_back(std::move(e));
	}

	// 生成された敵を表示
	if (enemies.empty())
	{
		std::cout << "敵を生成できませんでした。" << std::endl;
	}
	else
	{
		std::cout << "=== 生成された敵（ランダム4体） ===" << std::endl;
		for (size_t i = 0; i < enemies.size(); ++i)
		{
			const Enemy& e = *enemies[i];
			std::cout << "[" << (i + 1) << "] ID: " << e.Data.ID
				<< "  Name: " << e.Data.Name
				<< "  HP: " << e.Data.HP
				<< "  ATK: " << e.Data.ATK
				<< "  DEF: " << e.Data.DEF
				<< "  SPD: " << e.Data.SPD
				<< "  Gold: " << e.Data.Gold
				<< "  EXP: " << e.Data.EXP
				<< "  Crit: " << e.Data.CriticalRate
				<< "  Element: " << ElementToString(e.Data.Element)
				<< std::endl;
		}

		// 生成された数で平均を計算
		double averageATK = 0.0;
		if (!enemies.empty())
		{
			averageATK = static_cast<double>(totalATK) / static_cast<double>(enemies.size());
		}

		std::cout << std::endl;
		std::cout << "合計 HP (生成された敵): " << totalHP << std::endl;
		std::cout << "平均 ATK (生成された敵): " << std::fixed << std::setprecision(2) << averageATK << std::endl;
	}

	// unique_ptr により自動解放
	return 0;
}

