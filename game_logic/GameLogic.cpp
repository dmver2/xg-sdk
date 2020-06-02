#include "pch.h"
#include "GameLogic.h"

//void test() {
//	CGameLogic<3> gamelo;
//	gamelo.Move({ 1,1 }, CGameLogic<3>::ONE);
//	gamelo.Countermove(CGameLogic<3>::TWO);
//}

void BestChoices(int n, std::multimap<int, SDL_Point, std::greater<int> >& choice_tree) {
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j) {
			const int  key = (i == (n - 1) / 2 && j == (n - 1) / 2) ? 4 : (i == j || absint(i - j) == (n - 1)) ? 3 : 2;
			choice_tree.insert({ key, SDL_Point{i, j} });
		}
	}
	//for (auto it = choice_tree.begin(); it != choice_tree.end(); ++it)
	//{
	//	SDL_Log("%d -> { %d, %d }", it->first, it->second.x, it->second.y);
	//}
}
