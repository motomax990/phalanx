#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

enum EDir {
	UP = 0,
	DW = 1,
	LT = 2,
	RT = 3,
	UPRT = 4,
	UPLT = 5,
	DWRT = 6,
	DWLT = 7
};

struct STMove {
	int bx1, bx2;
	int by1, by2;
	int ex, ey;
	int len;
	EDir dir;
	STMove(int _bx1 = 0, int _bx2 = 0, int _by1 = 0, int _by2 = 0, int _ex = 0, int _ey = 0, EDir _dir = EDir(0), int _len = 0) : bx1(_bx1), bx2(_bx2), by1(_by1), by2(_by1), ex(_ex), ey(_ey), dir(_dir), len(_len)
	{
	}
};


//Размеры поля 12x14
const int64_t CPH = 12;
const int64_t CPW = 14;
//Удобный перевод координат для вывода
char strLiter[CPH];
char colLiter[CPW];

void buildBaseMtr(vector < vector<int> >& mtr) {
	for (int i = 0; i < 3; ++i) mtr[0][i] = 2;
	for (int i = 5; i < 9; ++i) mtr[0][i] = 2;
	for (int i = 11; i < 14; ++i) mtr[0][i] = 2;
	for (int i = 0; i < 14; ++i) mtr[1][i] = 2;
	mtr[2][3] = 2;
	mtr[2][4] = 2;
	mtr[2][9] = 2;
	mtr[2][10] = 2;
	for (int i = 5; i < 10; ++i) mtr[9][i] = 1;
	for (int i = 0; i < 14; ++i) mtr[10][i] = 1;
	for (int i = 0; i < 5; ++i) mtr[11][i] = 1;
	for (int i = 9; i < 14; ++i) mtr[11][i] = 1;
}

void printMap(vector < vector<int> >& mtr) {
	for (int i = 0; i < CPH; ++i) {
		for (int j = 0; j < CPW; ++j) {
			if (mtr[i][j] == 0) cout << "#";
			if (mtr[i][j] == 1) cout << "w";
			if (mtr[i][j] == 2) cout << "b";
		}
		cout << endl;
	}
}
int checkWin(vector < vector<int> >& mtr) {
	if (count(mtr[0].begin(), mtr[0].end(), 2) < count(mtr[0].begin(), mtr[0].end(), 1))
		return 1;
	if (count(mtr[11].begin(), mtr[0].end(), 1) < count(mtr[11].begin(), mtr[0].end(), 2))
		return 2;
	return 0;
}

bool checkMoveAbility(vector < vector<int> >& mtr, int crp, STMove move) {
	if (move.ex<0 or move.ey <0 or move.ex > CPH or move.ey > CPW) return false;
	if (move.dir == EDir::UP) {
		if (move.bx1 - move.ex > move.len) return false;
		for (size_t i = move.bx1 - 1; i > move.ex; i--)
		{
			if (mtr[i][move.ey] != 0) return false;
		}
		if (mtr[move.ex][move.ey] == crp) return false;
		else if (mtr[move.ex][move.ey] == 0) {
			return true;
		}
		else {
			int drp = 3 - crp;
			int cr1 = move.ex;
			int crL = 0;
			while (cr1 >= 0 and mtr[cr1][move.ey] == drp) { crL++; cr1--; }
			if (crL > move.len) return false;
		}
		return true;
	}
	else if (move.dir == EDir::DW) {
		if (move.ex - move.bx1 > move.len) return false;
		for (size_t i = move.bx1 + 1; i < move.ex; i++)
		{
			if (mtr[i][move.ey] != 0) return false;
		}
		if (mtr[move.ex][move.ey] == crp) return false;
		else if (mtr[move.ex][move.ey] == 0) {
			return true;
		}
		else {
			int drp = 3 - crp;
			int cr1 = move.ex;
			int crL = 0;
			while (cr1 <= CPH and mtr[cr1][move.ey] == drp) {
				crL++; cr1++;
			}
			if (crL > move.len) return false;
		}
		return true;
	}
	else if (move.dir == EDir::RT) {
		if (move.ey - move.by1 > move.len) return false;
		for (size_t i = move.by1 + 1; i < move.ey; i++)
		{
			if (mtr[move.ex][i] != 0) return false;
		}
		if (mtr[move.ex][move.ey] == crp) return false;
		else if (mtr[move.ex][move.ey] == 0) {
			return true;
		}
		else {
			int drp = 3 - crp;
			int cr2 = move.ey;
			int crL = 0;
			while (cr2 <= CPW and mtr[move.ex][cr2] == drp) {
				crL++; cr2++;
			}
			if (crL > move.len) return false;
		}
		return true;
	}
	else if (move.dir == EDir::LT) {
		if (move.by1 - move.ey > move.len) return false;
		for (size_t i = move.by1 - 1; i > move.ey; i--)
		{
			if (mtr[move.ex][i] != 0) return false;
		}
		if (mtr[move.ex][move.ey] == crp) return false;
		else if (mtr[move.ex][move.ey] == 0) {
			return true;
		}
		else {
			int drp = 3 - crp;
			int cr2 = move.ey;
			int crL = 0;
			while (cr2 >= 0 and mtr[move.ex][cr2] == drp) {
				crL++; cr2--;
			}
			if (crL > move.len) return false;
		}
		return true;
	}
	else if (move.dir == EDir::UPRT) {
		int cnt = 0;
		for (size_t i = move.bx1 - 1, j = move.by1 + 1; i > move.ex and j < move.ey; i--, j++)
		{
			if (mtr[i][j] != 0) return false;
			cnt++;
		}
		if (cnt > move.len) return false;
		if (mtr[move.ex][move.ey] == crp) return false;
		else if (mtr[move.ex][move.ey] == 0) {
			return true;
		}
		else {
			int drp = 3 - crp;
			int crx1 = move.ex;
			int cry1 = move.ey;
			int crL = 0;
			while (crx1 >= 0 and cry1 < CPW and mtr[crx1][cry1] == drp) { crL++; crx1--; cry1++; }
			if (crL > move.len) return false;
		}
		return true;
	}
	else if (move.dir == EDir::UPLT) {
		int cnt = 0;
		for (size_t i = move.bx1 - 1, j = move.by1 - 1; i > move.ex and j > move.ey; i--, j--)
		{

			if (mtr[i][j] != 0) return false;
			cnt++;


		}
		if (cnt > move.len) return false;
		if (mtr[move.ex][move.ey] == crp) return false;
		else if (mtr[move.ex][move.ey] == 0) {
			return true;
		}
		else {
			int drp = 3 - crp;
			int crx1 = move.ex;
			int cry1 = move.ey;
			int crL = 0;
			while (crx1 >= 0 and cry1 >= 0 and mtr[crx1][cry1] == drp) { crL++; crx1--; cry1--; }
			if (crL > move.len) return false;
		}
		return true;
	}
	else if (move.dir == EDir::DWRT) {
		int cnt = 0;
		for (size_t i = move.bx1 + 1, j = move.by1 + 1; i < move.ex and j < move.ey; i++, j++)
		{

			if (mtr[i][j] != 0) return false;
			cnt++;


		}
		if (cnt > move.len) return false;
		if (mtr[move.ex][move.ey] == crp) return false;
		else if (mtr[move.ex][move.ey] == 0) {
			return true;
		}
		else {
			int drp = 3 - crp;
			int crx1 = move.ex;
			int cry1 = move.ey;
			int crL = 0;
			while (crx1 < CPH and cry1 < CPW and mtr[crx1][cry1] == drp) { crL++; crx1++; cry1++; }
			if (crL > move.len) return false;
		}
		return true;

	}
	else if (move.dir == EDir::DWLT) {
		int cnt = 0;
		for (size_t i = move.bx1 + 1, j = move.by1 - 1; i < move.ex and j > move.ey; i++, j--)
		{
			if (mtr[i][j] != 0) return false;
			cnt++;

		}
		if (cnt > move.len) return false;
		if (mtr[move.ex][move.ey] == crp) return false;
		else if (mtr[move.ex][move.ey] == 0) {
			return true;
		}
		else {
			int drp = 3 - crp;
			int crx1 = move.ex;
			int cry1 = move.ey;
			int crL = 0;
			while (crx1 < CPH and cry1 >= 0 and mtr[crx1][cry1] == drp) { crL++; crx1++; cry1--; }
			if (crL > move.len) return false;
		}
		return true;
	}
	return false;
}

/*
Aa - Nl
l
k
j
i
h
g
f
e
d
c
b
a
  A B C D E F G H I L K L M N
*/

pair<int, int> gcs(string st) {
	pair<int, int> res;
	for (size_t i = 0; i < CPW; i++)
	{
		if (colLiter[i] == st[0]) {
			res.first = i;
			break;
		}
	}
	for (size_t i = 0; i < CPH; i++)
	{
		if (strLiter[i] == st[1]) {
			res.second = i;
			break;
		}
	}
	return res;
}

string gsc(int x, int y) {
	string res = "";
	res += colLiter[y];
	res += strLiter[x];
	return res;
}

STMove convFromStr(string moveSt) {
	if (moveSt.size() == 5) {
		if (moveSt[0] != moveSt[3] and moveSt[1] != moveSt[4]) {
			if (moveSt[0] < moveSt[3]) {
				if (moveSt[1] < moveSt[4]) {
					STMove mv;
					mv.dir = EDir::UPRT;
					pair<int, int> cr = gcs(moveSt.substr(0, 2));
					mv.bx1 = cr.first;
					mv.by1 = cr.second;
					mv.bx2 = cr.first;
					mv.by2 = cr.second;
					pair<int, int> ed = gcs(moveSt.substr(3, 2));
					mv.ex = ed.first;
					mv.ey = ed.second;
					mv.len = 1;
					swap(mv.bx1, mv.by1);
					swap(mv.bx2, mv.by2);
					swap(mv.ex, mv.ey);
					return mv;
				}
				else {
					STMove mv;
					mv.dir = EDir::DWRT;
					pair<int, int> cr = gcs(moveSt.substr(0, 2));
					mv.bx1 = cr.first;
					mv.by1 = cr.second;
					mv.bx2 = cr.first;
					mv.by2 = cr.second;
					pair<int, int> ed = gcs(moveSt.substr(3, 2));
					mv.ex = ed.first;
					mv.ey = ed.second;
					mv.len = 1;
					swap(mv.bx1, mv.by1);
					swap(mv.bx2, mv.by2);
					swap(mv.ex, mv.ey);
					return mv;
				}
			}
			else {
				if (moveSt[1] < moveSt[4]) {
					STMove mv;
					mv.dir = EDir::UPLT;
					pair<int, int> cr = gcs(moveSt.substr(0, 2));
					mv.bx1 = cr.first;
					mv.by1 = cr.second;
					mv.bx2 = cr.first;
					mv.by2 = cr.second;
					pair<int, int> ed = gcs(moveSt.substr(3, 2));
					mv.ex = ed.first;
					mv.ey = ed.second;;
					mv.len = 1;
					swap(mv.bx1, mv.by1);
					swap(mv.bx2, mv.by2);
					swap(mv.ex, mv.ey);
					return mv;
				}
				else {
					STMove mv;
					mv.dir = EDir::DWLT;
					pair<int, int> cr = gcs(moveSt.substr(0, 2));
					mv.bx1 = cr.first;
					mv.by1 = cr.second;
					mv.bx2 = cr.first;
					mv.by2 = cr.second;
					pair<int, int> ed = gcs(moveSt.substr(3, 2));
					mv.ex = ed.first;
					mv.ey = ed.second;
					mv.len = 1;
					swap(mv.bx1, mv.by1);
					swap(mv.bx2, mv.by2);
					swap(mv.ex, mv.ey);
					return mv;
				}
			}
		}
		else if (moveSt[0] == moveSt[3] and moveSt[1] != moveSt[4]) {
			if (moveSt[1] < moveSt[4]) {
				STMove mv;
				mv.dir = EDir::UP;
				pair<int, int> cr = gcs(moveSt.substr(0, 2));
				mv.bx1 = cr.first;
				mv.by1 = cr.second;
				mv.bx2 = cr.first;
				mv.by2 = cr.second;
				pair<int, int> ed = gcs(moveSt.substr(3, 2));
				mv.ex = ed.first;
				mv.ey = ed.second;
				mv.len = 1;
				swap(mv.bx1, mv.by1);
				swap(mv.bx2, mv.by2);
				swap(mv.ex, mv.ey);
				return mv;
			}
			else {
				STMove mv;
				mv.dir = EDir::DW;
				pair<int, int> cr = gcs(moveSt.substr(0, 2));
				mv.bx1 = cr.first;
				mv.by1 = cr.second;
				mv.bx2 = cr.first;
				mv.by2 = cr.second;
				pair<int, int> ed = gcs(moveSt.substr(3, 2));
				mv.ex = ed.first;
				mv.ey = ed.second;
				mv.len = 1;
				swap(mv.bx1, mv.by1);
				swap(mv.bx2, mv.by2);
				swap(mv.ex, mv.ey);
				return mv;
			}
		}
		else if (moveSt[0] != moveSt[3] and moveSt[1] == moveSt[4]) {
			if (moveSt[0] < moveSt[3]) {
				STMove mv;
				mv.dir = EDir::RT;
				pair<int, int> cr = gcs(moveSt.substr(0, 2));
				mv.bx1 = cr.first;
				mv.by1 = cr.second;
				mv.bx2 = cr.first;
				mv.by2 = cr.second;
				pair<int, int> ed = gcs(moveSt.substr(3, 2));
				mv.ex = ed.first;
				mv.ey = ed.second;
				mv.len = 1;
				swap(mv.bx1, mv.by1);
				swap(mv.bx2, mv.by2);
				swap(mv.ex, mv.ey);
				return mv;
			}
			else {
				STMove mv;
				mv.dir = EDir::LT;
				pair<int, int> cr = gcs(moveSt.substr(0, 2));
				mv.bx1 = cr.first;
				mv.by1 = cr.second;
				mv.bx2 = cr.first;
				mv.by2 = cr.second;
				pair<int, int> ed = gcs(moveSt.substr(3, 2));
				mv.ex = ed.first;
				mv.ey = ed.second;
				mv.len = 1;
				swap(mv.bx1, mv.by1);
				swap(mv.bx2, mv.by2);
				swap(mv.ex, mv.ey);
				return mv;
			}
		}
	}
	else if (moveSt.size() == 8) {
		if (moveSt[0] != moveSt[3] and moveSt[6] != moveSt[7]) {
			if (moveSt[0] < moveSt[6]) {
				if (moveSt[1] < moveSt[7]) {
					STMove mv;
					mv.dir = EDir::UPRT;
					pair<int, int> cr = gcs(moveSt.substr(0, 2));
					pair<int, int> cr1 = gcs(moveSt.substr(3, 2));
					mv.bx1 = cr.first;
					mv.by1 = cr.second;
					mv.bx2 = cr1.first;
					mv.by2 = cr1.second;
					pair<int, int> ed = gcs(moveSt.substr(6, 2));
					mv.ex = ed.first;
					mv.ey = ed.second;
					mv.len = abs(cr.second - cr1.second) + 1;
					swap(mv.bx1, mv.by1);
					swap(mv.bx2, mv.by2);
					swap(mv.ex, mv.ey);
					return mv;
				}
				else {
					STMove mv;
					mv.dir = EDir::DWRT;
					pair<int, int> cr = gcs(moveSt.substr(0, 2));
					pair<int, int> cr1 = gcs(moveSt.substr(3, 2));
					mv.bx1 = cr.first;
					mv.by1 = cr.second;
					mv.bx2 = cr1.first;
					mv.by2 = cr1.second;
					pair<int, int> ed = gcs(moveSt.substr(6, 2));
					mv.ex = ed.first;
					mv.ey = ed.second;
					mv.len = abs(cr.second - cr1.second) + 1;
					swap(mv.bx1, mv.by1);
					swap(mv.bx2, mv.by2);
					swap(mv.ex, mv.ey);
					return mv;
				}
			}
			else {
				if (moveSt[1] < moveSt[7]) {
					STMove mv;
					mv.dir = EDir::UPLT;
					pair<int, int> cr = gcs(moveSt.substr(0, 2));
					pair<int, int> cr1 = gcs(moveSt.substr(3, 2));
					mv.bx1 = cr.first;
					mv.by1 = cr.second;
					mv.bx2 = cr1.first;
					mv.by2 = cr1.second;
					pair<int, int> ed = gcs(moveSt.substr(6, 2));
					mv.ex = ed.first;
					mv.ey = ed.second;
					mv.len = abs(cr.second - cr1.second) + 1;
					swap(mv.bx1, mv.by1);
					swap(mv.bx2, mv.by2);
					swap(mv.ex, mv.ey);
					return mv;
				}
				else {
					STMove mv;
					mv.dir = EDir::DWLT;
					pair<int, int> cr = gcs(moveSt.substr(0, 2));
					pair<int, int> cr1 = gcs(moveSt.substr(3, 2));
					mv.bx1 = cr.first;
					mv.by1 = cr.second;
					mv.bx2 = cr1.first;
					mv.by2 = cr1.second;
					pair<int, int> ed = gcs(moveSt.substr(6, 2));
					mv.ex = ed.first;
					mv.ey = ed.second;
					mv.len = abs(cr.second - cr1.second) + 1;
					swap(mv.bx1, mv.by1);
					swap(mv.bx2, mv.by2);
					swap(mv.ex, mv.ey);
					return mv;
				}
			}
		}
		else if (moveSt[0] == moveSt[6] and moveSt[1] != moveSt[7]) {
			if (moveSt[1] < moveSt[7]) {
				STMove mv;
				mv.dir = EDir::UP;
				pair<int, int> cr = gcs(moveSt.substr(0, 2));
				pair<int, int> cr1 = gcs(moveSt.substr(3, 2));
				mv.bx1 = cr.first;
				mv.by1 = cr.second;
				mv.bx2 = cr1.first;
				mv.by2 = cr1.second;
				pair<int, int> ed = gcs(moveSt.substr(6, 2));
				mv.ex = ed.first;
				mv.ey = ed.second;
				mv.len = abs(cr.second - cr1.second) + 1;
				swap(mv.bx1, mv.by1);
				swap(mv.bx2, mv.by2);
				swap(mv.ex, mv.ey);
				return mv;
			}
			else {
				STMove mv;
				mv.dir = EDir::DW;
				pair<int, int> cr = gcs(moveSt.substr(0, 2));
				pair<int, int> cr1 = gcs(moveSt.substr(3, 2));
				mv.bx1 = cr.first;
				mv.by1 = cr.second;
				mv.bx2 = cr1.first;
				mv.by2 = cr1.second;
				pair<int, int> ed = gcs(moveSt.substr(6, 2));
				mv.ex = ed.first;
				mv.ey = ed.second;
				mv.len = abs(cr.second - cr1.second) + 1;
				swap(mv.bx1, mv.by1);
				swap(mv.bx2, mv.by2);
				swap(mv.ex, mv.ey);
				return mv;
			}
		}
		else if (moveSt[0] != moveSt[6] and moveSt[1] == moveSt[7]) {
			if (moveSt[0] < moveSt[6]) {
				STMove mv;
				mv.dir = EDir::RT;
				pair<int, int> cr = gcs(moveSt.substr(0, 2));
				pair<int, int> cr1 = gcs(moveSt.substr(3, 2));
				mv.bx1 = cr.first;
				mv.by1 = cr.second;
				mv.bx2 = cr1.first;
				mv.by2 = cr1.second;
				pair<int, int> ed = gcs(moveSt.substr(6, 2));
				mv.ex = ed.first;
				mv.ey = ed.second;
				mv.len = abs(cr.first - cr1.first) + 1;
				swap(mv.bx1, mv.by1);
				swap(mv.bx2, mv.by2);
				swap(mv.ex, mv.ey);
				return mv;
			}
			else {
				STMove mv;
				mv.dir = EDir::LT;
				pair<int, int> cr = gcs(moveSt.substr(0, 2));
				pair<int, int> cr1 = gcs(moveSt.substr(3, 2));
				mv.bx1 = cr.first;
				mv.by1 = cr.second;
				mv.bx2 = cr1.first;
				mv.by2 = cr1.second;
				pair<int, int> ed = gcs(moveSt.substr(6, 2));
				mv.ex = ed.first;
				mv.ey = ed.second;
				mv.len = abs(cr.first - cr1.first) + 1;
				swap(mv.bx1, mv.by1);
				swap(mv.bx2, mv.by2);
				swap(mv.ex, mv.ey);

				return mv;
			}
		}
	}


	return STMove();
}

string convToStr(STMove move) {
	if (move.len == 1) return string(gsc(move.bx1, move.by1) + "-" + gsc(move.ex, move.ey));
	else return string(gsc(move.bx1, move.by1) + "-" + gsc(move.bx2, move.by2) + "-" + gsc(move.ex, move.ey));
}

void makeMove(vector < vector<int> >& mtr, STMove move) {
	if (move.len == 1) {
		int p = mtr[move.bx1][move.by1];
		mtr[move.bx1][move.by1] = 0;
		mtr[move.ex][move.ey] = p;
	}
	if (move.dir == EDir::UP) {
		int p = mtr[move.bx1][move.by1];
		for (size_t i = move.bx1; i < move.bx2; i++) mtr[i][move.by1] = 0;
		for (size_t i = 0; i < move.len; i++) mtr[move.ex+i][move.ey] = p;
	}
	else if (move.dir == EDir::DW) {
		int p = mtr[move.bx1][move.by1];
		for (size_t i = move.bx2; i < move.bx1; i++) mtr[i][move.by1] = 0;
		for (size_t i = 0; i < move.len; i++) mtr[move.ex - i][move.ey] = p;
	}
	else if (move.dir == EDir::RT) {
		int p = mtr[move.bx1][move.by1];
		for (size_t i = move.by1; i < move.by2; i++) mtr[move.bx1][i] = 0;
		for (size_t i = 0; i < move.len; i++) mtr[move.ex ][move.ey - i] = p;
	}
	else if (move.dir == EDir::LT) {
		int p = mtr[move.bx1][move.by1];
		for (size_t i = move.by2; i < move.by1; i++) mtr[move.bx1][i] = 0;
		for (size_t i = 0; i < move.len; i++) mtr[move.ex][move.ey+1] = p;
	}
	else if (move.dir == EDir::UPRT) {
		int p = mtr[move.bx1][move.by1];
		for (size_t i = move.by1; i < move.by2; i++) for (size_t j = move.bx1; j < move.bx2; j++) mtr[i][j] = 0;
		for (size_t i = 0; i < move.len; i++) mtr[move.ex + i][move.ey - i] = p;
	}
	else if (move.dir == EDir::UPLT) {
		int p = mtr[move.bx1][move.by1];
		for (size_t i = move.by2; i < move.by1; i++) for (size_t j = move.bx1; j < move.bx2; j++) mtr[i][j] = 0;
		for (size_t i = 0; i < move.len; i++) mtr[move.ex - i][move.ey + i] = p;
	}
	else if (move.dir == EDir::DWRT) {
		int p = mtr[move.bx1][move.by1];
		for (size_t i = move.by1; i < move.by2; i++) for (size_t j = move.bx2; j < move.bx1; j++) mtr[i][j] = 0;
		for (size_t i = 0; i < move.len; i++) mtr[move.ex + i][move.ey - i] = p;
	}
	else if (move.dir == EDir::DWLT) {
		int p = mtr[move.bx1][move.by1];
		for (size_t i = move.by2; i < move.by1; i++) for (size_t j = move.bx2; j < move.bx1; j++) mtr[i][j] = 0;
		for (size_t i = 0; i < move.len; i++) mtr[move.ex - i][move.ey + i] = p;
	}
}

void genMove(vector < vector<int> >& mtr, int crp) {
	
}


int main() {
	vector < vector<int> > mtr(CPH, vector<int>(CPW, 0));
	for (int cnt = 0; cnt < CPH; cnt++)
	{
		strLiter[cnt] = 'l' - cnt;
		cout << strLiter[cnt] << " ";
	}
	cout << endl;
	for (int cnt = 0; cnt < CPW; cnt++)
	{
		colLiter[cnt] = 'A' + cnt;
		cout << colLiter[cnt] << " ";
	}
	cout << endl;
	STMove mv = convFromStr("Bk-Cl-Aj");
	cout << convToStr(mv) << endl;
	buildBaseMtr(mtr);
	printMap(mtr);


	return 0;
}

/*
 * 1, EDir::UPLT, b 9 6, b2 10 7, e 7 4, 2
 *
 */