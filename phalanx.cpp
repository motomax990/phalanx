#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <random>
#include <cstdio>
#include <climits>

using namespace std;

ofstream out("output.txt");

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

pair<int, int> dirs[8]{ {1,0},{-1,0},{0,-1},{0,1},{1,1},{1,-1},{-1,1},{-1,-1} };

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


const int64_t CPH = 12;
const int64_t CPW = 14;

char strLiter[CPH];
char colLiter[CPW];
unordered_map<uint64_t, int> evaluationCache;

vector<vector<vector<uint64_t>>> zobristTable(CPH, vector<vector<uint64_t>>(CPW, vector<uint64_t>(3)));
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
			if (mtr[i][j] == 0) out << "#";
			if (mtr[i][j] == 1) out << "w";
			if (mtr[i][j] == 2) out << "b";
		}
		out << endl;
	}
}
int checkWin(vector < vector<int> >& mtr) {
	if (count(mtr[0].begin(), mtr[0].end(), 2) < count(mtr[0].begin(), mtr[0].end(), 1))
		return 1;
	if (count(mtr[11].begin(), mtr[0].end(), 1) < count(mtr[11].begin(), mtr[0].end(), 2))
		return 2;
	return 0;
}

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

bool checkMoveAbility(vector<vector<int>>& mtr, int crp, STMove move) {
	if (move.ex < 0 || move.ey < 0 || move.ex >= CPH || move.ey >= CPW)
		return false;

	int dx = 0, dy = 0;
	switch (move.dir) {
	case EDir::UP:    dx = -1; break;
	case EDir::DW:    dx = 1;  break;
	case EDir::LT:    dy = -1; break;
	case EDir::RT:    dy = 1;  break;
	case EDir::UPRT:  dx = -1; dy = 1; break;
	case EDir::UPLT:  dx = -1; dy = -1; break;
	case EDir::DWRT:  dx = 1; dy = 1; break;
	case EDir::DWLT:  dx = 1; dy = -1; break;
	}

	for (int i = 0; i < move.len; ++i) {
		int x = move.bx1 + i * dx;
		int y = move.by1 + i * dy;
		if (x < 0 || y < 0 || x >= CPH || y >= CPW || mtr[x][y] != crp)
			return false;
	}

	int dist = (dx && dy) ? abs(move.ex - move.bx1) :
		max(abs(move.ex - move.bx1), abs(move.ey - move.by1));

	for (int i = 1; i <= dist; ++i) {
		int x = move.bx1 + i * dx;
		int y = move.by1 + i * dy;
		if (x < 0 || y < 0 || x >= CPH || y >= CPW || mtr[x][y] != 0)
			return false;
	}

	if (mtr[move.ex][move.ey] == crp)
		return false;
	if (mtr[move.ex][move.ey] == 3 - crp) {
		int enemyLen = 0;
		int cx = move.ex;
		int cy = move.ey;

		// Считаем ВСЕХ врагов в линии атаки
		while (true) {
			if (cx < 0 || cy < 0 || cx >= CPH || cy >= CPW) break;
			if (mtr[cx][cy] != 3 - crp) break;
			enemyLen++;
			cx += dx;
			cy += dy;
		}

		cx = move.ex - dx;
		cy = move.ey - dy;
		while (true) {
			if (cx < 0 || cy < 0 || cx >= CPH || cy >= CPW) break;
			if (mtr[cx][cy] != 3 - crp) break;
			enemyLen++;
			cx -= dx;
			cy -= dy;
		}

		// Решающее условие: длина атакующей > защищающейся
		if (move.len <= enemyLen) {
			return false; // Захват невозможен
		}
	}

	return true;
}
void makeMove(vector<vector<int>>& mtr, const STMove& move) {
	if (move.len == 1) {
		int p = mtr[move.bx1][move.by1];
		mtr[move.bx1][move.by1] = 0;
		mtr[move.ex][move.ey] = p;
		return;
	}

	const int dx = (move.ex != move.bx1) ? (move.ex > move.bx1 ? 1 : -1) : 0;
	const int dy = (move.ey != move.by1) ? (move.ey > move.by1 ? 1 : -1) : 0;

	for (int i = 0; i < move.len; ++i) {
		int x = move.bx1 + i * dx;
		int y = move.by1 + i * dy;
		if (x >= 0 && y >= 0 && x < CPH && y < CPW) {
			mtr[x][y] = 0;
		}
	}

	for (int i = 0; i < move.len; ++i) {
		int x = move.ex - i * dx;
		int y = move.ey - i * dy;
		if (x >= 0 && y >= 0 && x < CPH && y < CPW) {
			mtr[x][y] = mtr[move.bx1][move.by1];
		}
	}

	int cx = move.ex + dx;
	int cy = move.ey + dy;
	while (cx >= 0 && cy >= 0 && cx < CPH && cy < CPW && mtr[cx][cy] == 3 - mtr[move.bx1][move.by1]) {
		mtr[cx][cy] = 0;
		cx += dx;
		cy += dy;
	}
	if (mtr[move.ex][move.ey] == 3 - mtr[move.bx1][move.by1]) {
		int p = mtr[move.bx1][move.by1];
		int dx = (move.ex != move.bx1) ? (move.ex > move.bx1 ? 1 : -1) : 0;
		int dy = (move.ey != move.by1) ? (move.ey > move.by1 ? 1 : -1) : 0;

		// Удаляем ровно move.len вражеских шашек
		int captured = 0;
		int cx = move.ex;
		int cy = move.ey;

		while (captured < move.len &&
			cx >= 0 && cy >= 0 && cx < CPH && cy < CPW) {
			if (mtr[cx][cy] == 3 - p) {
				mtr[cx][cy] = 0;
				captured++;
			}
			cx += dx;
			cy += dy;
		}
	}
}

void gemAllMoves(vector < vector<int> >& mtr, vector<STMove>& mvs, int crp) {
	for (int i = 0; i < CPH; i++)
	{
		for (size_t j = 0; j < CPW; j++)
		{
			if (mtr[i][j] == crp) {
				for (int k = 0; k < 8; k++) {
					for (int p = 0; p < 14; p++) {
						int ni1 = i + (-dirs[k].first) * p, nj1 = j + (-dirs[k].second) * p;
						if (ni1 < 0 or nj1 < 0 or ni1 >= CPH or nj1 >= CPW) continue;
						if (mtr[ni1][nj1] != crp) continue;
						int ni = i + dirs[k].first * (p + 1), nj = j + dirs[k].second * (1 + p);
						if (ni < 0 or ni1 < 0 or nj < 0 or nj1 < 0 or ni >= CPH or ni1 >= CPH or nj >= CPW or nj1 >= CPW) continue;
						STMove mv;
						mv.bx1 = i;
						mv.by1 = j;
						mv.bx2 = ni1;
						mv.by2 = nj1;
						mv.ex = ni;
						mv.ey = nj;
						mv.len = p + 1;
						mv.dir = EDir(k);
						if (checkMoveAbility(mtr, crp, mv)) {
							mvs.emplace_back(mv);
						}

					}
				}
			}
		}
	}

}

int evaluatePosition(const vector<vector<int>>& mtr, int crp) {
	const int targetRow = crp == 1 ? 0 : CPH - 1;
	int score = 0;
	int frontLine[CPW] = { 0 };

	for (int i = 0; i < CPH; ++i) {
		for (int j = 0; j < CPW; ++j) {
			if (mtr[i][j] == crp) {
				int dist = abs(i - targetRow);
				score += (CPH - dist) * 100;

				if (j >= 4 && j < 10) score += 30;

				if (dist < 3) frontLine[j]++;
			}
		}
	}

	int maxConcentration = 0;
	for (int j = 0; j < CPW; ++j) {
		if (frontLine[j] > 2) score += 50;
		maxConcentration = max(maxConcentration, frontLine[j]);
	}
	score += maxConcentration * 40;

	return score;
}

STMove findOptimalMove(vector<vector<int>>& mtr, int crp) {
	vector<STMove> moves;
	gemAllMoves(mtr, moves, crp);

	if (moves.empty()) return STMove();

	sort(moves.begin(), moves.end(), [crp](const STMove& a, const STMove& b) {
		int aProgress = crp == 1 ? (a.bx1 - a.ex) : (a.ex - a.bx1);
		int bProgress = crp == 1 ? (b.bx1 - b.ex) : (b.ex - b.bx1);
		return aProgress > bProgress;
		});

	const int maxMoves = min(5, (int)moves.size());
	int bestScore = -INT_MAX;
	STMove bestMove = moves[0];

	for (int i = 0; i < maxMoves; ++i) {
		auto& move = moves[i];
		vector<vector<int>> newMtr = mtr;
		if (checkMoveAbility(newMtr, crp, move))
			makeMove(newMtr, move);
		else continue;

		int currentScore = evaluatePosition(newMtr, crp);

		vector<STMove> enemyMoves;
		gemAllMoves(newMtr, enemyMoves, 3 - crp);
		if (!enemyMoves.empty()) {
			int worstEnemyScore = INT_MAX;
			for (auto& eMove : enemyMoves) {
				vector<vector<int>> temp = newMtr;
				if (checkMoveAbility(newMtr, 3 - crp, eMove))
					makeMove(temp, eMove);
				else continue;
				worstEnemyScore = min(worstEnemyScore, evaluatePosition(temp, crp));
			}
			currentScore = 0.6 * currentScore + 0.4 * worstEnemyScore;
		}

		if (currentScore > bestScore) {
			bestScore = currentScore;
			bestMove = move;
		}
	}

	return bestMove;
}

int main() {
	std::ios::sync_with_stdio(true);

	int nn;
	cin >> nn;
	for (int cnt = 0; cnt < CPH; cnt++)
	{
		strLiter[cnt] = 'l' - cnt;
	}
	for (int cnt = 0; cnt < CPW; cnt++)
	{
		colLiter[cnt] = 'A' + cnt;
	}
	vector < vector<int> > mtr(CPH, vector<int>(CPW, 0));
	buildBaseMtr(mtr);
	int crp;
	cin >> crp;
	crp++;
	string mvS;
	bool f = true;
	if (crp == 2) {
		if (f) {
			cin.ignore();
			f = false;
		}
		getline(cin, mvS);
		makeMove(mtr, convFromStr(mvS));
	}
	for (size_t i = 0; i < nn; i++)
	{
		STMove mv = findOptimalMove(mtr, crp);
		makeMove(mtr, mv);
		cout << convToStr(mv) << endl;
		if (f) {
			cin.ignore(); f = false;
		}
		printMap(mtr);
		out << endl;
		getline(cin, mvS);
		makeMove(mtr, convFromStr(mvS));
		printMap(mtr);
		out << endl;
	}


	return 0;
}

/*
 * 1, EDir::UPLT, b 9 6, b2 10 7, e 7 4, 2
 *
 */