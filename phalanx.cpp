#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

enum EDir{
    UP=0,
    DW=1,
    LT=2,
    RT=3,
    UPRT=4,
    UPLT=5,
    DWRT=6,
    DWLT=7
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

bool checkMoveAbility(vector < vector<int> > &mtr,int crp, EDir dir, int bx1,int by1,int bx2, int by2, int e1, int e2, int len){
    if (e1<0 or e2 <0 or e1 > CPH or e2 > CPW) return false;
    if (dir == EDir::UP) {
        if (bx1-e1 > len) return false;
        for (size_t i = bx1-1; i > e1; i--)
        {
            if (mtr[i][e2] != 0) return false;
        }
        if (mtr[e1][e2] == crp) return false;
        else if (mtr[e1][e2] == 0) {
            return true;
        }
        else {
            int drp = 3 - crp;
            int cr1 = e1;
            int crL = 0;
            while (cr1 >= 0 and mtr[cr1][e2] == drp) { crL++; cr1--; }
            if (crL > len) return false;
        }
        return true;
    }
    else if (dir == EDir::DW) {
        if (e1 - bx1 > len) return false;
        for (size_t i = bx1+1; i < e1; i++)
        {
            if (mtr[i][e2] != 0) return false;
        }
        if (mtr[e1][e2] == crp) return false;
        else if (mtr[e1][e2] == 0) {
            return true;
        }
        else {
            int drp = 3 - crp;
            int cr1 = e1;
            int crL = 0;
            while (cr1 <= CPH and mtr[cr1][e2] == drp) {
                crL++; cr1++;
        }
            if (crL > len) return false;
        }
        return true;
    }
    else if (dir == EDir::RT) {
            if (e2 - by1 > len) return false;
            for (size_t i = by1+1; i < e2; i++)
            {
                if (mtr[e1][i] != 0) return false;
            }
            if (mtr[e1][e2] == crp) return false;
            else if (mtr[e1][e2] == 0) {
                return true;
            }
            else {
                int drp = 3 - crp;
                int cr2 = e2;
                int crL = 0;
                while (cr2 <= CPW and mtr[e1][cr2] == drp) {
                    crL++; cr2++;
                }
                if (crL > len) return false;
            }
            return true;
    }
    else if (dir == EDir::LT) {
        if (by1 - e2 > len) return false;
        for (size_t i = by1-1; i > e2; i--)
        {
            if (mtr[e1][i] != 0) return false;
        }
        if (mtr[e1][e2] == crp) return false;
        else if (mtr[e1][e2] == 0) {
            return true;
        }
        else {
            int drp = 3 - crp;
            int cr2 = e2;
            int crL = 0;
            while (cr2 >= 0 and mtr[e1][cr2] == drp) {
                crL++; cr2--;
            }
            if (crL > len) return false;
        }
        return true;
    }
    else if (dir == EDir::UPRT) {
        int cnt = 0;
        for (size_t i = bx1 - 1, j = by1 + 1; i > e1 and j < e2; i--, j++)
        {
                if (mtr[i][j] != 0) return false;
                cnt++;
        }
        if (cnt > len) return false;
        if (mtr[e1][e2] == crp) return false;
        else if (mtr[e1][e2] == 0) {
            return true;
        }
        else {
            int drp = 3 - crp;
            int crx1 = e1;
            int cry1 = e2;
            int crL = 0;
            while (crx1 >= 0 and cry1 < CPW and mtr[crx1][cry1] == drp) { crL++; crx1--; cry1++;}
            if (crL > len) return false;
        }
        return true;
    }
    else if (dir == EDir::UPLT) {
        int cnt = 0;
        for (size_t i = bx1 - 1, j = by1 - 1; i > e1 and j > e2; i--, j--)
        {
            
                if (mtr[i][j] != 0) return false;
                cnt++;

            
        }
        if (cnt > len) return false;
        if (mtr[e1][e2] == crp) return false;
        else if (mtr[e1][e2] == 0) {
            return true;
        }
        else {
            int drp = 3 - crp;
            int crx1 = e1;
            int cry1 = e2;
            int crL = 0;
            while (crx1 >= 0 and cry1 >= 0 and mtr[crx1][cry1] == drp) { crL++; crx1--; cry1--; }
            if (crL > len) return false;
        }
        return true;
    }
    else if (dir == EDir::DWRT) {
        int cnt = 0;
        for (size_t i = bx1 + 1, j = by1 + 1; i < e1 and j < e2; i++,j++)
        {
            
                if (mtr[i][j] != 0) return false;
                cnt++;

            
        }
        if (cnt > len) return false;
        if (mtr[e1][e2] == crp) return false;
        else if (mtr[e1][e2] == 0) {
            return true;
        }
        else {
            int drp = 3 - crp;
            int crx1 = e1;
            int cry1 = e2;
            int crL = 0;
            while (crx1 < CPH and cry1 < CPW and mtr[crx1][cry1] == drp) { crL++; crx1++; cry1++; }
            if (crL > len) return false;
        }
        return true;
        
    }
    else if (dir == EDir::DWLT) {
        int cnt = 0;
        for (size_t i = bx1 + 1, j = by1 - 1; i < e1 and  j > e2; i++, j--)
        {
                if (mtr[i][j] != 0) return false;
                cnt++;

        }
        if (cnt > len) return false;
        if (mtr[e1][e2] == crp) return false;
        else if (mtr[e1][e2] == 0) {
            return true;
        }
        else {
            int drp = 3 - crp;
            int crx1 = e1;
            int cry1 = e2;
            int crL = 0;
            while (crx1 < CPH and cry1 >= 0 and mtr[crx1][cry1] == drp) { crL++; crx1++; cry1--; }
            if (crL > len) return false;
        }
        return true;
    }
    return false;
}


int main(){
    vector < vector<int> > mtr(CPH,vector<int>(CPW,0));
	for (int cnt = 0; cnt < CPH; cnt++)
	{
		strLiter[cnt] = 'l'-cnt;
        cout << strLiter[cnt] << " ";
	}
    cout << endl;
	for (int cnt = 0; cnt < CPW; cnt++)
	{
        colLiter[cnt] = 'A'+cnt;
        cout << colLiter[cnt] << " ";
    }
    cout << endl;
    buildBaseMtr(mtr);
    printMap(mtr);


	return 0;
}

/*
 * 1, EDir::UPLT, b 9 6, b2 10 7, e 7 4, 2 
 *
 */