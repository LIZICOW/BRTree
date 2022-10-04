#include"BTree.h"
#include<fstream>
#include<time.h>　
#include<map>
const int maxn = 100000005;
int a[maxn];
int id[maxn];
int n;
int ans = 0;
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
	map<int, int> standard;
	//genData();
	fstream fp("text.txt", ios::in);
	n = 1000000;
	for (int i = 0; i < n; i++)
	{
		fp >> id[i] >> a[i];
		standard[id[i]] = a[i];
	}
	BTree<int, int> b;
	start = clock();
	for (int i = 0; i < n; i++)
		b.setData(id[i], a[i]);
	for (int i = 0; i < n; i++) {
		if (b[id[i]] != standard[id[i]]) {
			cout << i <<"id[i] = "<< id[i] << " standard[id[i]] = " << standard[id[i]] << " b[id[i]] = " << b.getData(id[i]) << '\n';
		}
	}

	for (int i = 0; i < n && !b.empty(); i++) {
		if(b.exist(id[i]))
			b.remove(id[i]);
		//cout << i << "****************************************\n";
		//ans=i;
	}
	cout << ans << '\n';
	end = clock();   //结束时间
	cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl;  //输出时间（单位：ｓ）
	;
}