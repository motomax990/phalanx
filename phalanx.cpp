#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

enum EDir{
    UP,
    DW ,
    LF,
    RT,
    UPRT,
    UPLT,
    DWRT,
    DWLT
};

//Размеры поля 12x14
const int64_t CPH = 12;
const int64_t CPW = 14;
//Удобный перевод координат для вывода
char strLiter[CPH];
char colLiter[CPW];

void buildBaseMtr(vector < vector<int> > &mtr){
    for (int i = 0; i < 3; ++i) mtr[0][i] = 2;
    for (int i = 5; i < 9; ++i) mtr[0][i] = 2;
    for (int i = 11; i < 14; ++i) mtr[0][i] = 2;
    for (int i = 0; i < 14; ++i) mtr[1][i] = 2;
    mtr[2][3]=2;
    mtr[2][4]=2;
    mtr[2][9]=2;
    mtr[2][10]=2;
    for (int i = 5; i < 10; ++i) mtr[9][i] = 1;
    for (int i = 0; i < 14; ++i) mtr[10][i] = 1;
    for (int i = 0; i < 5; ++i) mtr[11][i] = 1;
    for (int i = 9; i < 14; ++i) mtr[11][i] = 1;
}

void printMap(vector < vector<int> > &mtr){
    for (int i = 0; i < CPH; ++i) {
        for (int j = 0; j < CPW; ++j) {
            if(mtr[i][j]==0) cout << "  ";
            if(mtr[i][j]==1) cout << "w ";
            if(mtr[i][j]==2) cout << "b ";
        }
        cout << endl;
    }
}
int checkWin(vector < vector<int> > &mtr){
    if(count(mtr[0].begin(),mtr[0].end(),2)< count(mtr[0].begin(),mtr[0].end(),1))
        return 1;
    if(count(mtr[11].begin(),mtr[0].end(),1)< count(mtr[11].begin(),mtr[0].end(),2))
        return 2;
    return 0;
}

bool checkMoveAbility(vector < vector<int> > &mtr,int x1,int y1,int x2, int y2, int len){
    if()
    return false;
}


int main(){
    vector < vector<int> > mtr(CPH,vector<int>(CPW,0));
	for (int cnt = 0; cnt < CPH; cnt++)
	{
		strLiter[cnt] = 'l'-cnt;
	}
	for (int cnt = 0; cnt < CPW; cnt++)
	{
		strLiter[cnt] = 'A'+cnt;
	}
    buildBaseMtr(mtr);
    printMap(mtr);


	return 0;
}

/*
 *
 *
 */