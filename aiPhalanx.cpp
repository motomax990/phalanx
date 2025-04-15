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
const int dirOffset[8][2] = {
	{  0, -1 }, // UP (0)
	{  1, -1 }, // UPRT (1)
	{  1,  0 }, // RT (2)
	{  1,  1 }, // DWRT (3)
	{  0,  1 }, // DW (4)
	{ -1,  1 }, // DWLT (5)
	{ -1,  0 }, // LT (6)
	{ -1, -1 }  // UPLT (7)
};

//Размеры поля 12x14
const int64_t CPH = 12;
const int64_t CPW = 14;
//Удобный перевод координат для вывода
char strLiter[CPH];
char colLiter[CPW];
const int SEARCH_DEPTH = 2;

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
			if (mtr[i][j] == 0) cout << "  ";
			if (mtr[i][j] == 1) cout << "w ";
			if (mtr[i][j] == 2) cout << "b ";
		}
		cout << endl;
	}
}
int checkWin(vector < vector<int> >& mtr) {
	if (count(mtr[0].begin(), mtr[0].end(), 2) < count(mtr[0].begin(), mtr[0].end(), 1))
		return 1;
	if (count(mtr[11].begin(), mtr[11].end(), 1) < count(mtr[11].begin(), mtr[11].end(), 2))
		return 2;
	return 0;
}

bool checkMoveAbility(vector < vector<int> >& mtr, int crp, STMove move) {
	if (move.ex < 0 || move.ey < 0 || move.ex >= CPH || move.ey >= CPW) return false;
	if (move.dir == EDir::UP) {
		if (move.bx1 - move.ex > move.len) return false;
		for (int i = move.bx1 - 1; i > move.ex; i--) {
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
			while (cr1 >= 0 && mtr[cr1][move.ey] == drp) {
				crL++; cr1--;
			}
			if (crL > move.len) return false;
			return true;
		}
	}
	else if (move.dir == EDir::DW) {
		if (move.ex - move.bx1 > move.len) return false;
		for (int i = move.bx1 + 1; i < move.ex; i++) {
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
			while (cr1 < CPH && mtr[cr1][move.ey] == drp) {
				crL++; cr1++;
			}
			if (crL > move.len) return false;
			return true;
		}
	}
	else if (move.dir == EDir::RT) {
		if (move.ey - move.by1 > move.len) return false;
		for (int i = move.by1 + 1; i < move.ey; i++) {
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
			while (cr2 < CPW && mtr[move.ex][cr2] == drp) {
				crL++; cr2++;
			}
			if (crL > move.len) return false;
			return true;
		}
	}
	else if (move.dir == EDir::LT) {
		if (move.by1 - move.ey > move.len) return false;
		for (int i = move.by1 - 1; i > move.ey; i--) {
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
			while (cr2 >= 0 && mtr[move.ex][cr2] == drp) {
				crL++; cr2--;
			}
			if (crL > move.len) return false;
			return true;
		}
	}
	else if (move.dir == EDir::UPRT) {
		int cnt = 0;
		for (int i = move.bx1 - 1, j = move.by1 + 1; i > move.ex && j < move.ey; i--, j++) {
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
			while (crx1 >= 0 && cry1 < CPW && mtr[crx1][cry1] == drp) {
				crL++; crx1--; cry1++;
			}
			if (crL > move.len) return false;
			return true;
		}
	}
	else if (move.dir == EDir::UPLT) {
		int cnt = 0;
		for (int i = move.bx1 - 1, j = move.by1 - 1; i > move.ex && j > move.ey; i--, j--) {
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
			while (crx1 >= 0 && cry1 >= 0 && mtr[crx1][cry1] == drp) {
				crL++; crx1--; cry1--;
			}
			if (crL > move.len) return false;
			return true;
		}
	}
	else if (move.dir == EDir::DWRT) {
		int cnt = 0;
		for (int i = move.bx1 + 1, j = move.by1 + 1; i < move.ex && j < move.ey; i++, j++) {
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
			while (crx1 < CPH && cry1 < CPW && mtr[crx1][cry1] == drp) {
				crL++; crx1++; cry1++;
			}
			if (crL > move.len) return false;
			return true;
		}
	}
	else if (move.dir == EDir::DWLT) {
		int cnt = 0;
		for (int i = move.bx1 + 1, j = move.by1 - 1; i < move.ex && j > move.ey; i++, j--) {
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
			while (crx1 < CPH && cry1 >= 0 && mtr[crx1][cry1] == drp) {
				crL++; crx1++; cry1--;
			}
			if (crL > move.len) return false;
			return true;
		}
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
		if (colLiter[i] == st[1]) {
			res.second = i;
			break;
		}
	}
	return res;
}

string gsc(int x, int y) {
	string res = "";
	res += colLiter[x];
	res += strLiter[y];
	return res;
}

STMove convFromStr(string moveSt) {
	if (moveSt.size() == 5) {
		pair<int, int> cr = gcs(moveSt.substr(0, 2));
		pair<int, int> cr1 = gcs(moveSt.substr(3, 2));
		STMove mv;
		mv.bx1 = cr.first;
		mv.by1 = cr.second;
		mv.bx2 = cr.first;
		mv.by2 = cr.second;
		mv.ex = cr1.first;
		mv.ey = cr1.second;
		mv.len = 1;

		if (cr.first == cr1.first) {
			mv.dir = (cr.second > cr1.second) ? EDir::UP : EDir::DW;
		}
		else if (cr.second == cr1.second) {
			mv.dir = (cr.first < cr1.first) ? EDir::RT : EDir::LT;
		}
		else if (cr.first < cr1.first && cr.second > cr1.second) {
			mv.dir = EDir::UPRT;
		}
		else if (cr.first < cr1.first && cr.second < cr1.second) {
			mv.dir = EDir::DWRT;
		}
		else if (cr.first > cr1.first && cr.second > cr1.second) {
			mv.dir = EDir::UPLT;
		}
		else {
			mv.dir = EDir::DWLT;
		}
		return mv;
	}
	else if (moveSt.size() == 8) {
		pair<int, int> cr = gcs(moveSt.substr(0, 2));
		pair<int, int> cr1 = gcs(moveSt.substr(3, 2));
		pair<int, int> cr2 = gcs(moveSt.substr(6, 2));
		STMove mv;
		mv.bx1 = cr.first;
		mv.by1 = cr.second;
		mv.bx2 = cr1.first;
		mv.by2 = cr1.second;
		mv.ex = cr2.first;
		mv.ey = cr2.second;
		mv.len = (mv.dir == EDir::UP || mv.dir == EDir::DW) ? abs(cr.second - cr1.second) + 1 : abs(cr.first - cr1.first) + 1;

		if (cr.first == cr1.first) {
			mv.dir = (cr.second > cr1.second) ? EDir::UP : EDir::DW;
		}
		else if (cr.second == cr1.second) {
			mv.dir = (cr.first < cr1.first) ? EDir::RT : EDir::LT;
		}
		else if (cr.first < cr1.first && cr.second > cr1.second) {
			mv.dir = EDir::UPRT;
		}
		else if (cr.first < cr1.first && cr.second < cr1.second) {
			mv.dir = EDir::DWRT;
		}
		else if (cr.first > cr1.first && cr.second > cr1.second) {
			mv.dir = EDir::UPLT;
		}
		else {
			mv.dir = EDir::DWLT;
		}
		return mv;
	}

	return STMove();
}


string convToStr(STMove move) {
	if (move.len == 1) return string(gsc(move.bx1, move.by1) + "-" + gsc(move.ex, move.ey));
	else return string(gsc(move.bx1, move.by1) + "-" + gsc(move.bx2, move.by2) + "-" + gsc(move.ex, move.ey));
}

void makeMove(vector<vector<int>>& mtr, STMove move) {
	int p = mtr[move.bx1][move.by1];
	if (move.len == 1) {
		mtr[move.bx1][move.by1] = 0;
		mtr[move.ex][move.ey] = p;
		return;
	}

	if (move.dir == EDir::UP) {
		for (int i = 0; i < move.len; i++) mtr[move.bx1 - i][move.by1] = 0;
		for (int i = 0; i < move.len; i++) mtr[move.ex - i][move.ey] = p;
	}
	else if (move.dir == EDir::DW) {
		for (int i = 0; i < move.len; i++) mtr[move.bx1 + i][move.by1] = 0;
		for (int i = 0; i < move.len; i++) mtr[move.ex + i][move.ey] = p;
	}
	else if (move.dir == EDir::LT) {
		for (int i = 0; i < move.len; i++) mtr[move.bx1][move.by1 - i] = 0;
		for (int i = 0; i < move.len; i++) mtr[move.ex][move.ey - i] = p;
	}
	else if (move.dir == EDir::RT) {
		for (int i = 0; i < move.len; i++) mtr[move.bx1][move.by1 + i] = 0;
		for (int i = 0; i < move.len; i++) mtr[move.ex][move.ey + i] = p;
	}
	else if (move.dir == EDir::UPRT) {
		for (int i = 0; i < move.len; i++) mtr[move.bx1 - i][move.by1 + i] = 0;
		for (int i = 0; i < move.len; i++) mtr[move.ex - i][move.ey + i] = p;
	}
	else if (move.dir == EDir::UPLT) {
		for (int i = 0; i < move.len; i++) mtr[move.bx1 - i][move.by1 - i] = 0;
		for (int i = 0; i < move.len; i++) mtr[move.ex - i][move.ey - i] = p;
	}
	else if (move.dir == EDir::DWRT) {
		for (int i = 0; i < move.len; i++) mtr[move.bx1 + i][move.by1 + i] = 0;
		for (int i = 0; i < move.len; i++) mtr[move.ex + i][move.ey + i] = p;
	}
	else if (move.dir == EDir::DWLT) {
		for (int i = 0; i < move.len; i++) mtr[move.bx1 + i][move.by1 - i] = 0;
		for (int i = 0; i < move.len; i++) mtr[move.ex + i][move.ey - i] = p;
	}
}

int evaluateBoard(const vector<vector<int>>& mtr, int player) {
	int score = 0;
	int enemy = 3 - player;
	for (int i = 0; i < CPH; ++i) {
		for (int j = 0; j < CPW; ++j) {
			if (mtr[i][j] == player) {
				score += 10;
				score += (player == 1) ? (CPH - i) : i;
			}
			else if (mtr[i][j] == enemy) {
				score -= 10;
			}
		}
	}
	int enemyRow = (player == 1) ? 0 : CPH - 1;
	for (int j = 0; j < CPW; ++j) {
		if (mtr[enemyRow][j] == player) score += 100;
	}
	return score;
}

void genAllMoves(const vector<vector<int>>& mtr, int player, vector<STMove>& moves) {
	for (int y = 0; y < mtr.size(); ++y) {
		for (int x = 0; x < mtr[0].size(); ++x) {
			if (mtr[y][x] != player) continue;

			for (int d = 0; d < 8; ++d) {
				int dx = dirOffset[d][0];
				int dy = dirOffset[d][1];

				// Пытаемся собрать фалангу ПОД лидером, в сторону -dx, -dy
				int bx1 = x, by1 = y;
				int bx2 = x, by2 = y;
				int len = 1;

				int nx = x - dx;
				int ny = y - dy;

				while (nx >= 0 && ny >= 0 && nx < mtr[0].size() && ny < mtr.size() && mtr[ny][nx] == player) {
					bx2 = nx;
					by2 = ny;
					++len;
					nx -= dx;
					ny -= dy;
				}

				// Пробуем все возможные длины перемещения (от 1 до len)
				for (int step = 1; step <= len; ++step) {
					int ex = bx1 + dx * step;
					int ey = by1 + dy * step;

					if (ex < 0 || ey < 0 || ex >= mtr[0].size() || ey >= mtr.size())
						break;

					if (mtr[ey][ex] == player)
						break;

					STMove mv;
					mv.bx1 = bx1;
					mv.by1 = by1;
					mv.bx2 = bx2;
					mv.by2 = by2;
					mv.ex = ex;
					mv.ey = ey;
					mv.len = len;
					mv.dir = (EDir)d;
					moves.push_back(mv);

					// Если встретили не пустую клетку — дальше не идём
					if (mtr[ey][ex] != 0)
						break;
				}
			}
		}
	}
}


int minimax(vector<vector<int>>& mtr, int depth, int player, bool maximizingPlayer) {
	if (depth == 0 || checkWin(mtr) != 0) {
		return evaluateBoard(mtr, player);
	}

	vector<STMove> moves;
	genAllMoves(mtr, maximizingPlayer ? player : (3 - player), moves);
	if (moves.empty()) return evaluateBoard(mtr, player);

	int bestValue = maximizingPlayer ? INT_MIN : INT_MAX;
	for (const STMove& move : moves) {
		vector<vector<int>> temp = mtr;
		makeMove(temp, move);
		int eval = minimax(temp, depth - 1, player, !maximizingPlayer);
		if (maximizingPlayer)
			bestValue = max(bestValue, eval);
		else
			bestValue = min(bestValue, eval);
	}
	return bestValue;
}

STMove genBestMove(vector<vector<int>>& mtr, int player) {
	vector<STMove> moves;
	genAllMoves(mtr, player, moves);
	int bestValue = INT_MIN;
	STMove bestMove;
	for (const STMove& move : moves) {
		vector<vector<int>> temp = mtr;
		makeMove(temp, move);
		int value = minimax(temp, SEARCH_DEPTH - 1, player, false);
		if (value > bestValue) {
			bestValue = value;
			bestMove = move;
		}
	}
	return bestMove;
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
	buildBaseMtr(mtr);
	printMap(mtr);
	makeMove(mtr, genBestMove(mtr, 1));
	printMap(mtr);

	return 0;
}

/*
 * 1, EDir::UPLT, b 9 6, b2 10 7, e 7 4, 2
 *
 */