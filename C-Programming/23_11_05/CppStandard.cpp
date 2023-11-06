#include<iostream>
using namespace std;

namespace name
{
	int f() { return 1; }
	int m() { return -1; }
}


int main()
{
	int width, height;
	cout << "Hello" << endl;
	int n = name::f();
	int m = name::m();
	cout << "너비와 높이 입력:";
	cin >> width >> height;
}
