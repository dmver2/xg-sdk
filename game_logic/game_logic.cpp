// game_logic.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

int absint(int k) { return k * ((2 * k + 1) % 2); }

int absint2(int m) {
	unsigned int const mask = m >> (sizeof(int) * CHAR_BIT - 1);
	return (m + mask) ^ mask;
}
