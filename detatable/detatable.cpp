#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <memory>
#include "Enemy.h"
#include "EnemyFactory.h"

struct Hero
{
	int HP;
	int ATK;
	int DEF;
	int SPD;
	int CriticalRate; // パーセント
};

int main()
{
	// 全ての敵（ID 1〜4）を生成
	std::vector<std::unique_ptr<Enemy>> enemies;
	for (int id = 1; id <= 4; ++id)
	{
		auto e = EnemyFactory::CreateEnemy(id);
		if (e)
		{
			enemies.push_back(std::move(e));
		}
	}

	if (enemies.empty())
	{
		std::cout << "敵が見つかりませんでした。" << std::endl;
		return 1;
	}

	// 合計HP、平均ATKを計算
	int totalHP = 0;
	int totalATK = 0;
	for (const auto& e : enemies)
	{
		totalHP += e->Data.HP;
		totalATK += e->Data.ATK;
	}
	double averageATK = static_cast<double>(totalATK) / enemies.size();

	std::cout << "合計 HP: " << totalHP << std::endl;
	std::cout << "平均 ATK: " << averageATK << std::endl;

	// ランダムで1体選択
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, static_cast<int>(enemies.size() - 1));
	int idx = dist(mt);
	Enemy* enemy = enemies[idx].get();

	std::cout << "ランダムで選ばれた敵: " << enemy->Data.Name << " (HP:" << enemy->Data.HP << " ATK:" << enemy->Data.ATK << " DEF:" << enemy->Data.DEF << " SPD:" << enemy->Data.SPD << " CR:" << enemy->Data.CriticalRate << "%)" << std::endl;

	// 勇者を作成（必要なら値を調整してください）
	Hero hero{ 500, 80, 20, 15, 5 }; // HP, ATK, DEF, SPD, CriticalRate(%)

	// バトル準備
	int enemyHP = enemy->Data.HP;
	int heroHP = hero.HP;

	auto rollCritical = [&](int critRate) -> bool {
		if (critRate <= 0) return false;
		std::uniform_int_distribution<int> d(0, 99);
		return d(mt) < critRate;
	};

	std::cout << "バトル開始！ 勇者 vs " << enemy->Data.Name << std::endl;

	// 先攻判定（SPDが高い方）
	bool heroTurn = (hero.SPD >= enemy->Data.SPD);

	int turn = 1;
	while (heroHP > 0 && enemyHP > 0)
	{
		std::cout << "----- ターン " << turn << " -----" << std::endl;
		if (heroTurn)
		{
			int baseDamage = std::max(1, hero.ATK - enemy->Data.DEF);
			bool crit = rollCritical(hero.CriticalRate);
			int damage = crit ? baseDamage * 2 : baseDamage;
			enemyHP -= damage;
			if (enemyHP < 0) enemyHP = 0;
			std::cout << "勇者の攻撃！ ダメージ: " << damage << (crit ? " (会心)" : "") << " 残り敵HP: " << enemyHP << std::endl;
		}
		else
		{
			int baseDamage = std::max(1, enemy->Data.ATK - hero.DEF);
			bool crit = rollCritical(enemy->Data.CriticalRate);
			int damage = crit ? baseDamage * 2 : baseDamage;
			heroHP -= damage;
			if (heroHP < 0) heroHP = 0;
			std::cout << enemy->Data.Name << " の攻撃！ ダメージ: " << damage << (crit ? " (会心)" : "") << " 残り勇者HP: " << heroHP << std::endl;
		}

		heroTurn = !heroTurn;
		++turn;
	}

	// 勝敗表示
	if (heroHP > 0)
	{
		std::cout << "勇者の勝利！ 残りHP: " << heroHP << std::endl;
	}
	else
	{
		std::cout << enemy->Data.Name << " の勝利！ 残りHP: " << enemyHP << std::endl;
	}

	// メモリ解放は不要（unique_ptr が自動で解放）
	return 0;
}

