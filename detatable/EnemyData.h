#pragma once
#include "Element.h"

// @brief 敵のデータを表す構造体
// @details
// この構造体は、敵の基本的な属性を格納します

struct EnemyData
{
	int ID;
	const char Name[256];
	int HP;
	int ATK;
	int DEF;
	int SPD;
	int Gold;
	int EXP;
	int CriticalRate;
	Element Element;
};