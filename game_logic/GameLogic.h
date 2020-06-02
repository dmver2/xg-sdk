#pragma once
#include "bitset2d.h"
//#include <SDL_rect.h>
#include <SDL.h>
#include <ostream>
#include <vector>
#include <map>
#include <string>
#include <tchar.h>
#include <stdlib.h> // rand

int absint(int k);
int absint2(int m);
void BestChoices(int n, std::multimap<int, SDL_Point, std::greater<int> >& choice_tree);

template <int N> const std::basic_string<TCHAR> RowAsString(const int r, const int matrix[N][N]);

template <int N> const std::basic_string<TCHAR> ColumnAsString(const int r, const int matrix[N][N]);

bool operator== (const SDL_Point& a, const SDL_Point& b);
bool operator!= (const SDL_Point& a, const SDL_Point& b);
std::basic_ostream<TCHAR>& operator << (std::basic_ostream<TCHAR>& ostream, const SDL_Point& pt);


template <int N> class CGameLogic
{
public:
	typedef int matrix[N][N];

	enum TTTField { EMPTY, ONE, TWO }; // ONE - PLAYER 1, TWO - PLAYER 2
	enum TTTOutcome { MOVE1 = 1, MOVE2, ONE_WIN, TWO_WIN, DRAW };
	static const SDL_Point NOVIABLEPATH;

private:
	int model[N][N];
	std::multimap<int, SDL_Point, std::greater<int> > choice_tree;
	SDL_Point lastmove;
	TTTOutcome outcome;
	int nmoves;
	int mode;
	SDL_LogPriority log_pr;

	void increment_move();

public:
	CGameLogic(bool debug = true);
	TTTOutcome CheckWin(bitset2d<N, N>& winline, TTTField player);
	SDL_Point Countermove();
	bool Move(const SDL_Point& cursor, TTTField player);
	std::basic_string<TCHAR> MatrixAsString() const;
	TTTOutcome Outcome() const;
	void Clear();
	const SDL_Point& Lastmove() const;
//	SetDebug(bool dbg = true) { this->debug = dbg;  };

private:
	SDL_Point CornerForkFix();
};

template <int N>
const SDL_Point CGameLogic<N>::NOVIABLEPATH = { -1, -1 };

template<int N>
inline void CGameLogic<N>::increment_move()
{
	if (nmoves < N * N) {
		++nmoves;
	}
}

template<int N>
inline const SDL_Point& CGameLogic<N>::Lastmove() const {
	return lastmove;
}

template <int N>
inline CGameLogic<N>::CGameLogic(bool debug) : model{ 0 }, mode(1)
, outcome(MOVE1)
, lastmove(NOVIABLEPATH)
, nmoves(0)
, log_pr(debug ? SDL_LOG_PRIORITY_DEBUG : SDL_LOG_PRIORITY_VERBOSE)
{
	::BestChoices(N, choice_tree);
}

template <int N>
inline typename CGameLogic<N>::TTTOutcome CGameLogic<N>::CheckWin(bitset2d<N, N>& winline, typename CGameLogic<N>::TTTField player)
{
	if (nmoves < 5)
		return outcome;

	if (::CheckWin<N>(lastmove, model, winline, player))
	{
		switch (outcome)
		{
		case MOVE1:
			outcome = TWO_WIN;
			break;
		case MOVE2:
			outcome = ONE_WIN;
			break;
		}
	}
	if (lastmove == NOVIABLEPATH)
	{
		outcome = DRAW;
	}
	else if (nmoves >= N * N - 1) {
		if (outcome < ONE_WIN) {
			if (NOVIABLEPATH == ::AttackTurn<N>(lastmove, model, ONE))
				outcome = DRAW;
		}
	}

	return outcome;
}

template<int N>
inline SDL_Point CGameLogic<N>::CornerForkFix()
{
	if (nmoves == 1) {
		const SDL_Point& last = lastmove;
		const int  key = (last.x == (N - 1) / 2 && last.y == (N - 1) / 2) ? 4 : (last.x == last.y || absint(last.x - last.y) == (N - 1)) ? 3 : 2;
		if (3 == key) // corner cell filled
		{
			return (RAND_MAX / rand() > 2) ?
				SDL_Point{ absint2((N - 1) - last.y), last.x }
			: SDL_Point{ last.y, absint2((N - 1) - last.x) };
		}
	}
	return NOVIABLEPATH;
}

template<int N>
inline SDL_Point CGameLogic<N>::Countermove()
{
	//if (eog != MOVE2)
	//{
	//	SDL_Log("ERROR: not your turn!");
	//	return CGameLogic<N>::NOVIABLEPATH;
	//}
	//else
	//{
	//	eog = MOVE1;
	//}

	CGameLogic<N>::TTTField player = (outcome == MOVE2) ? TWO : (outcome == MOVE1) ? ONE : EMPTY;

//	const SDL_Point fix = CornerForkFix();
//	const SDL_Point move = (fix != NOVIABLEPATH) ? fix : ::Countermove<N>(lastmove, model, player, choice_tree);
	const SDL_Point move = ::Countermove<N>(lastmove, model, player, choice_tree);

	lastmove = { move.y, move.x };

	if (move != NOVIABLEPATH) {
		int& ref = model[move.x][move.y];
		if (!ref) {
			ref = player;
			++nmoves;
			outcome = MOVE1;
			SDL_Log("counter move: {%d, %d}", move.x, move.y);
		}
		else {
			SDL_Log("counter move ERROR: cell already checked: {%d, %d} = %d", move.x, move.y, ref);
		}
	}
	SDL_Log("matix:\n%s", this->MatrixAsString().data());

	return lastmove;
}

template<int N>
inline typename bool CGameLogic<N>::Move(const SDL_Point& cursor, typename CGameLogic<N>::TTTField player)
{
	bool rc;
	SDL_Log("player: %d, move: {%d, %d}", player, cursor.y, cursor.x);
	lastmove = cursor;

	int& val = model[cursor.y][cursor.x];
	if (!val)
	{
		increment_move();
		val = player;
		outcome = (ONE == player) ? MOVE2 : MOVE1;
		rc = true;
	}
	else
	{
		rc = false;
		SDL_Log("ERROR: [%d, %d] = %d", cursor.y, cursor.x, val);
	}
	SDL_Log("matix:\n%s", this->MatrixAsString().data());

	return rc;
}

template<int N>
inline std::basic_string<TCHAR> CGameLogic<N>::MatrixAsString() const
{
	std::basic_string<TCHAR> s;
	s.reserve(0x20);
	for (int i = 0; i < N; ++i)
	{
		TCHAR tmp[0x10] = { 0 };
		for (int j = 0; j < N; ++j)
		{
			_itot_s(model[i][j], tmp, 10);
			s += tmp;
			s += ' ';
		}
		s += '\n';
	}
	return s;
}

template<int N>
inline typename CGameLogic<N>::TTTOutcome CGameLogic<N>::Outcome() const
{
	return outcome;
}

template<int N>
inline void CGameLogic<N>::Clear()
{
	SDL_memset(model, 0, sizeof(model));
	outcome = MOVE1;
	nmoves = 0;
}

template <int N>
SDL_Point DefenceTurn(const SDL_Point& cursor, int model[N][N], typename CGameLogic<N>::TTTField player)
{
	int colsum = 0;
	int rowsum = 0;
	int antagonist = CGameLogic<N>::TTTField::TWO + 1 - player;
	SDL_Point empty(CGameLogic<N>::NOVIABLEPATH);
	SDL_Point empty2 = CGameLogic<N>::NOVIABLEPATH;
	if (cursor.y == cursor.x)
	{
		int diagsum = 0;
		for (int i = 0; i < N; ++i)
		{
			diagsum += model[i][i];
			if (CGameLogic<N>::TTTField::EMPTY == model[i][i])
			{
				empty = { i,i };
			}
		}
		if ((N - 1) * antagonist == diagsum)
		{
			return empty;
		}
	}
	if (cursor.y == N - 1 - cursor.x)
	{
		int antidiagsum = 0;
		for (int i = 0; i < N; ++i)
		{
			antidiagsum += model[i][N - 1 - i];
			if (CGameLogic<N>::TTTField::EMPTY == model[i][N - 1 - i])
			{
				empty = { i, N - 1 - i };
			}
		}
		if ((N - 1) * antagonist == antidiagsum)
		{
			return empty;
		}
	}

	for (int i = 0; i < N; ++i)
	{
		rowsum += model[cursor.y][i];
		if (CGameLogic<N>::TTTField::EMPTY == model[cursor.y][i])
		{
			empty = { cursor.y, i };
		}

		colsum += model[i][cursor.x];
		if (CGameLogic<N>::TTTField::EMPTY == model[i][cursor.x])
		{
			empty2 = { i, cursor.x };
		}
	}
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "row[%d, *] { %s} sum %d", cursor.y, RowAsString<N>(cursor.y, model).data(), rowsum);
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "col[*, %d] { %s} sum %d", cursor.x, ColumnAsString<N>(cursor.x, model).data(), colsum);

	if ((N - 1) * antagonist == rowsum)
	{
		return empty;
	}

	if ((N - 1) * antagonist == colsum)
	{
		return empty2;
	}

	return  CGameLogic<N>::NOVIABLEPATH;
}

template <int N>
SDL_Point AttackTurn(const SDL_Point cursor, int model[N][N], typename CGameLogic<N>::TTTField player)
{
	if (cursor.y != N - 1 - cursor.x)
	{
		SDL_Point empty = CGameLogic<N>::NOVIABLEPATH;
		int antidiagsum = 0;
		for (int i = 0; i < N; ++i)
		{
			const int val = model[i][N - 1 - i];
			antidiagsum += val;
			if (CGameLogic<N>::TTTField::EMPTY == val) {
				empty = { i, N - 1 - i };
			}
		}
		if (empty != CGameLogic<N>::NOVIABLEPATH && antidiagsum == (N - 1) * player)
		{
			return empty;
		}
	}

	if (cursor.y != cursor.x)
	{
		SDL_Point empty = CGameLogic<N>::NOVIABLEPATH;
		int diagsum = 0;
		for (int i = 0; i < N; ++i)
		{
			const int val = model[i][i];
			diagsum += val;
			if (CGameLogic<N>::TTTField::EMPTY == val) {
				empty = { i,i };
			}
		}
		if (empty != CGameLogic<N>::NOVIABLEPATH && diagsum == (N - 1) * player)
		{
			return empty;
		}
	}

	for (int i = 0; i < N; ++i)
	{
		SDL_Point empty = CGameLogic<N>::NOVIABLEPATH;
		if (i != cursor.y)
		{
			int sum = 0;
			for (int j = 0; j < N; ++j)
			{
				const int val = model[i][j];
				sum += val;
				if (CGameLogic<N>::TTTField::EMPTY == val)
				{
					empty = { i, j };
				}
			}
			if (empty != CGameLogic<N>::NOVIABLEPATH && (N - 1) * player == sum)
			{
				return empty;
			}
		}

		empty = CGameLogic<N>::NOVIABLEPATH;
		if (i != cursor.x)
		{
			int sum = 0;
			for (int j = 0; j < N; ++j)
			{
				const int val = model[j][i];
				sum += model[j][i];
				if (CGameLogic<N>::TTTField::EMPTY == val)
				{
					empty = { j, i };
				}
			}
			if (empty != CGameLogic<N>::NOVIABLEPATH && (N - 1) * player == sum)
			{
				return empty;
			}

		}
	}

	return  CGameLogic<N>::NOVIABLEPATH;
}

template <int N>
SDL_Point MoveRandom(SDL_Point last, int model[N][N], const std::multimap<int, SDL_Point, std::greater<int> >& choice_tree) {
	SDL_Log("silly random tactics");
	std::multimap<int, SDL_Point, std::greater<int> >::key_type key = -1;
	std::vector<SDL_Point> free;
	for (auto it = choice_tree.begin(); it != choice_tree.end(); ++it)
	{
		if (key != it->first) {
			if (1 == free.size())
			{
				return free[0];
			}
			else if (1 < free.size())
			{
				const int dice = rand();
				const int randomn = dice / (RAND_MAX / free.size());
				std::multimap<int, SDL_Point, std::greater<int> >::mapped_type val = free[randomn];
				SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "chosen %d of %d", randomn, free.size());
				return { val.y, val.x };
			}
			free.clear();
			key = it->first;
		}
		if (!model[it->second.y][it->second.x])
		{
			free.push_back(it->second);
		}
	}
	return  CGameLogic<N>::NOVIABLEPATH;
}

template <int N>
SDL_Point MoveSilly(const SDL_Point& last, int model[N][N], const std::multimap<int, SDL_Point, std::greater<int> >& choice_tree) {
	if (true)
		return MoveRandom<N>(last, model, choice_tree);
	else
	{// silly stupid 
		SDL_Log("silly tactics");
		for (auto it = choice_tree.begin(); it != choice_tree.end(); ++it)
		{
			if (!model[it->second.y][it->second.x])
			{
				return { it->second.y, it->second.x };
			}
		}
	}
	return  CGameLogic<N>::NOVIABLEPATH;
}

inline bool operator== (const SDL_Point& a, const SDL_Point& b)
{
	return a.x == b.x && a.y == b.y;
}

inline bool operator!= (const SDL_Point& a, const SDL_Point& b)
{
	return !(a == b);
}

inline std::basic_ostream<TCHAR>& operator << (std::basic_ostream<TCHAR>& ostream, const SDL_Point& pt)
{
	return ostream << _T('{') << pt.x << _T(',') << pt.y << _T('}');
}

template<int N>
SDL_Point Countermove(const SDL_Point& turn, typename CGameLogic<N>::matrix model, typename CGameLogic<N>::TTTField player, const std::multimap<int, SDL_Point, std::greater<int> >& choice_tree)
{
	const SDL_Point attack = ::AttackTurn<N>(turn, model, player);

	if (attack != CGameLogic<N>::NOVIABLEPATH)
	{
		SDL_Log("attack");
		return attack;
	}
	else
	{
		const SDL_Point dfence = DefenceTurn<N>(turn, model, player);
		return (dfence != CGameLogic<N>::NOVIABLEPATH) ? dfence : ::MoveSilly<N>(turn, model, choice_tree);
	}
}

template <int N>
inline int CheckWin(const SDL_Point& cursor, int model[N][N], bitset2d<N, N>& winline, typename CGameLogic<N>::TTTField player)
{
	int colsum = 0;
	int rowsum = 0;
	for (int i = 0; i < N; ++i)
	{
		if (player == model[cursor.y][i])
		{
			++rowsum;
		}
		if (player == model[i][cursor.x])
		{
			++colsum;
		}
	}
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "checking player: %d column: [%d, *] { %s} sum: %d", player, cursor.x, ColumnAsString<N>(cursor.x, model).data(), colsum);
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "checking player: %d row   : [%d, *] { %s} sum: %d", player, cursor.y, RowAsString<N>(cursor.y, model).data(), rowsum);
	if (rowsum == N)
	{
		for (int j = 0; j < N; ++j)
		{
			winline(cursor.y, j) = 1;
		}
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%d WIN: row [%d, *] { %s} sum: %d", player, cursor.y, RowAsString<N>(cursor.y, model).data(), rowsum);
		return 1;
	}
	if (N == colsum)
	{
		for (int i = 0; i < N; ++i)
		{
			winline(i, cursor.x) = 1;
		}
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%d WIN: column [%d, *] { %s} sum %d", player, cursor.x, ColumnAsString<N>(cursor.x, model).data(), colsum);
		return 2;
	}

	if (cursor.y == cursor.x)
	{
		int diagsum = 0;
		for (int i = 0; i < N; ++i)
		{
			if (player == model[i][i]) {
				++diagsum;
			}
		}
		if (N == diagsum)
		{
			std::basic_string<TCHAR> s;
			for (int i = 0; i < N; ++i)
			{
				winline(i, i) = 1;
			}
			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%d WIN: diagonale sum: %d", player, diagsum);
			return 3;
		}
	}
	if (cursor.y == N - 1 - cursor.x)
	{
		int antidiagonal = 0;
		for (int i = 0; i < N; ++i)
		{
			if (player == model[N - 1 - i][i])
			{
				++antidiagonal;
			}
		}
		if (N == antidiagonal)
		{
			for (int i = 0; i < N; ++i)
			{
				winline(i, N - 1 - i) = 1;
			}
			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%d WIN: anti-diagonale sum: %d", player, antidiagonal);
			return 4;
		}
	}
	return 0;
}

template <int N>
const std::basic_string<TCHAR> RowAsString(const int r, const int matrix[N][N])
{
	std::basic_string<TCHAR> s;
	s.reserve(0x20);
	TCHAR tmp[0x10];
	for (int i = 0; i < N; ++i)
	{
		if (!_itot_s(matrix[r][i], tmp, 10))
		{
			s += tmp;
		}
		else
		{
			s += "NaN";
		}
		s += ' ';
	}
	return s;
}

template <int N>
const std::basic_string<TCHAR> ColumnAsString(const int c, const int matrix[N][N])
{
	std::basic_string<TCHAR> s;
	s.reserve(0x20);
	TCHAR tmp[0x10];
	for (int i = 0; i < N; ++i)
	{
		if (!_itot_s(matrix[i][c], tmp, 10))
		{
			s += tmp;
		}
		else {
			s += "NaN";
		}
		s += ' ';
	}
	return s;
}
