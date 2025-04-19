#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
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
pair<int, int> dirs[8]{ {-1,0},{1,0},{0,-1},{0,1},{-1,1},{-1,-1},{1,1},{1,-1} };

struct STMove {
    int bx1, bx2;
    int by1, by2;
    int ex, ey;
    int len;
    EDir dir;
    STMove(int _bx1 = 0, int _bx2 = 0, int _by1 = 0, int _by2 = 0, int _ex = 0, int _ey = 0, EDir _dir = EDir(0), int _len = 0)
            : bx1(_bx1), bx2(_bx2), by1(_by1), by2(_by2), ex(_ex), ey(_ey), dir(_dir), len(_len) {}
};

const int64_t CPH = 12;
const int64_t CPW = 14;
char strLiter[CPH];
char colLiter[CPW];
const int SEARCH_DEPTH = 2;
//ofstream out("output.txt");

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
	for (int i = 5; i < 9; ++i) mtr[9][i] = 1;
	for (int i = 0; i < 14; ++i) mtr[10][i] = 1;
	for (int i = 0; i < 5; ++i) mtr[11][i] = 1;
	for (int i = 9; i < 14; ++i) mtr[11][i] = 1;
}


int checkWin(vector < vector<int> >& mtr) {
	if (count(mtr[0].begin(), mtr[0].end(), 2) < count(mtr[0].begin(), mtr[0].end(), 1))
		return 1;
	if (count(mtr[11].begin(), mtr[11].end(), 1) < count(mtr[11].begin(), mtr[11].end(), 2))
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
		//Ef-Gh-Bc
		//01234567
		if (moveSt[0] != moveSt[6] and moveSt[1] != moveSt[7]) {
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
    if (move.ex < 0 || move.ey < 0 || move.ex >= CPH || move.ey >= CPW ||
        move.bx1 < 0 || move.by1 < 0 || move.bx1 >= CPH || move.by1 >= CPW ||
        move.bx2 < 0 || move.by2 < 0 || move.bx2 >= CPH || move.by2 >= CPW) {
        return false;
    }
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
        int x = move.bx1 + i * ( - dx);
        int y = move.by1 + i * ( - dy);
        if (x < 0 || y < 0 || x >= CPH || y >= CPW || mtr[x][y] != crp) {
            return false;
        }
    }

   
    for (int i = 1; i < move.len; ++i) {
        int x = move.ex - i * dx;
        int y = move.ey - i * dy;
        if (x < 0 || y < 0 || x >= CPH || y >= CPW || mtr[x][y] != 0) {
            return false;
        }
    }

    if (mtr[move.ex][move.ey] == crp) {
        return false;
    }

    if (mtr[move.ex][move.ey] == 3 - crp) {
        int enemyLen = 1;
        int cx = move.ex;
        int cy = move.ey;

        while (true) {
            cx += dx;
            cy += dy;
            if (cx < 0 || cy < 0 || cx >= CPH || cy >= CPW) break;
            if (mtr[cx][cy] != 3 - crp) break;
            enemyLen++;
        }

        if (move.len <= enemyLen) {
            return false;
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
	int ctr = mtr[move.bx1][move.by1];
	bool ff = false;
	for (int i = 0; i < move.len; ++i) {
		int x = move.ex - i * dx;
		int y = move.ey - i * dy;
		if (x == move.bx1 and y == move.by1) {
			ff = true;
		}
		if (x >= 0 && y >= 0 && x < CPH && y < CPW) {
			mtr[x][y] = ctr;
		}
	}

	for (int i = ff ? 1:0; i < move.len; ++i) {
		int x = move.bx1+ i * (-dx);
		int y = move.by1 + i * (-dy);
		if (x >= 0 && y >= 0 && x < CPH && y < CPW) {
			mtr[x][y] = 0;
		}
	}

	int cx = move.ex + dx;
	int cy = move.ey + dy;
	while (cx >= 0 && cy >= 0 && cx < CPH && cy < CPW && mtr[cx][cy] == 3 - mtr[move.bx1][move.by1]) {
		mtr[cx][cy] = 0;
		cx += dx;
		cy += dy;
	}
	if (mtr[move.ex][move.ey] == 3 - ctr) {
		int p = ctr;
		int dx = (move.ex != move.bx1) ? (move.ex > move.bx1 ? 1 : -1) : 0;
		int dy = (move.ey != move.by1) ? (move.ey > move.by1 ? 1 : -1) : 0;

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
                        if(i==10 and j == 0){
                            bool vvvv = false;
                        }
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




const int TDB = 2000;  
const int TPB = 100;    
const int ETP = -150;    


int evaluatePosition( vector<vector<int>>& mtr, int crp) {
	if (checkWin(mtr) == crp) return INT64_MAX;
	if (checkWin(mtr) == 3-crp) return INT64_MIN;
	const int targetRow = (crp == 1) ? CPH - 1:0;
	const int enemyRow = (crp == 1) ?  0: CPH - 1;
	int score = 0;

	// Критически важные показатели
	int myTarget = 0, enemyTarget = 0;
	for (int j = 0; j < CPW; ++j) {
		if (mtr[targetRow][j] == crp) myTarget++;
		if (mtr[enemyRow][j] == 3 - crp) enemyTarget++;
	}

	// Бонусы за доминирование на целевых линиях
	if (myTarget > enemyTarget) {
		score += TDB + (myTarget - enemyTarget) * 500;
	}

	score += myTarget * TPB;
	score += enemyTarget * ETP;

	// Дополнительные факторы
	int distanceScore = 0;
	int phalanxScore = 0;

	for (int i = 0; i < CPH; ++i) {
		for (int j = 0; j < CPW; ++j) {
			if (mtr[i][j] == crp) {
				// Расстояние до цели
				distanceScore += ((crp == 1) ? (CPH - 1 - i) : i) * 1000;

				// Проверка фаланг
				for (int d = 0; d < 8; ++d) {
					int ni = i + dirs[d].first;
					int nj = j + dirs[d].second;
					if (ni >= 0 && ni < CPH && nj >= 0 && nj < CPW && mtr[ni][nj] == crp) {
						phalanxScore += 30;
					}
				}
			}
		}
	}

	score += distanceScore;
	score += phalanxScore;
	for (int i = 0; i < CPH; ++i) {
		for (int j = 0; j < CPW; ++j) {
		
		}
	}
	return score;
}

int caclPrem(const vector<vector<int>>& mtr, int crp) {
	const int targetRow = (crp == 1) ? 0 : CPH - 1;
	int my = 0, enemy = 0;

	for (int j = 0; j < CPW; ++j) {
		if (mtr[targetRow][j] == crp) my++;
		if (mtr[targetRow][j] == 3 - crp) enemy++;
	}

	if (my > enemy) return 1000 + (my - enemy) * 500;
	if (my < enemy) return -800;
	return enemy * -100;
}

STMove findOptimalMove(vector<vector<int>>& mtr, int crp) {
	//const int MMA = 100;
	vector<STMove> moves;
	gemAllMoves(mtr, moves, crp);

	sort(moves.begin(), moves.end(), [crp](const STMove& a, const STMove& b) {
		return a.len > b.len; 
		});

	

	int bestScore = INT_MIN;
	STMove bestMove;

	struct {
		float current = 0.6f;
		float opponent = 0.3f;
		float dominance = 0.8f;
	} weights;

	for (STMove& move : moves) {
		vector<vector<int>> tmp = mtr;
		if (!checkMoveAbility(tmp, crp, move)) continue;
		makeMove(tmp, move);

		int crSc = evaluatePosition(tmp, crp);

		int worstEnemyScore = INT_MAX;
		vector<STMove> enemyMoves;
		gemAllMoves(tmp, enemyMoves, 3 - crp);

		for (STMove& enemyMove : enemyMoves) {
			vector<vector<int>> tmpEnemy = tmp;
			if (!checkMoveAbility(tmpEnemy, 3 - crp, enemyMove)) continue;
			makeMove(tmpEnemy, enemyMove);

			int emSc = evaluatePosition(tmpEnemy, crp); 
			worstEnemyScore = min(worstEnemyScore, emSc);
		}
		int dmS = caclPrem(tmp, crp);
		int finalScore =
			crSc * weights.current +
			(worstEnemyScore != INT_MAX ? worstEnemyScore * weights.opponent : 0) +
			dmS * weights.dominance;

		if (finalScore > bestScore) {
			bestScore = finalScore;
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
		//printMap(mtr);
		///out << endl;
		getline(cin, mvS);
		makeMove(mtr, convFromStr(mvS));
		///out << endl;
	}

	
	return 0;
}

/*
 * 1, EDir::UPLT, b 9 6, b2 10 7, e 7 4, 2
 *
 */
