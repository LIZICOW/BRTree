#include"BTree.h"
#include<fstream>
#include <time.h>　

const int maxn = 100000005;
int a[maxn];
int id[maxn];
int n;

void genData() {
	fstream fp("text.txt", ios::out);
	for (int i = 0; i <= maxn; i++)
	{
		fp << rand()%1000000 << ' ' << rand() % 10000 << '\n';
	}
}

int main() {
	srand(0);
	clock_t start, end;
	
	//genData();
	fstream fp("text.txt", ios::in);
	n = 1000000;
	for (int i = 0; i < n; i++)
		fp >> id[i] >> a[i];
	BTree<int, int> b;
	start = clock();
	for (int i = 0; i < n; i++)
		b.setData(i, a[i]);
	b.setData(10, a[10]);
	for (int i = 0; i < n; i++) {
		if (b.getData(i) != a[i]) {
			cout << i << " a[i] = " << a[i] << " b[i] = " << b.getData(i) << '\n';
		}
	}
	end = clock();   //结束时间
	cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl;  //输出时间（单位：ｓ）
	;
}