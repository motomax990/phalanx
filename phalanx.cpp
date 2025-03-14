#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
//Размеры поля 12x14
const int64_t CPH = 12;
const int64_t CPW = 14;
//Удобный перевод координат для вывода
char strLiter[CPH];
char colLiter[CPW];

int main(){
	for (int cnt = 0; cnt < CPH; cnt++)
	{
		strLiter[cnt] = 'l'-cnt;
	}
	for (int cnt = 0; cnt < CPW; cnt++)
	{
		strLiter[cnt] = 'A'+cnt;
	}

	return 0;
}