#include <stdio.h>
#include <tuple>
#include <iostream>
#include <string>
using namespace std;

int main()
{
	tuple<int, string, char>t1;
	t1 = make_tuple(21, "Nov", 'M');
	///////////tuple 원소 접근///////////
	cout << get<0>(t1) << endl;
	cout << get<1>(t1) << endl;
	cout << get<2>(t1) << endl;
	///////////tuple 원소 분해///////////
	int x;
	string y;
	char z;
	tie(x, y, z) = t1;
	cout << x << "" << y << "" << z << "";
	///////////두 개의tuple 연결///////////
	tuple<int, int>t2(1, 2);
	tuple<int, int>t3(3, 4);
	tuple<int, int, int, int>t4 = tuple_cat(t2,t3);
	///////////두 개의tuple 연결(auto 키워드)///////////
	auto t5 = tuple_cat(t2, t3);
	cout << get<0>(t4) << " " << get<1>(t4) << " " << get<2>(t4) << " " << get<3>(t4);
	cout << get<0>(t5) << " " << get<1>(t5) << " " << get<2>(t5) << " " << get<3>(t5);
	///////////서로 다른 튜플 변경 : swap///////////
	cout << "Before Swap" << endl;
	cout << "t1 tuple : ";
	cout << get<0>(t2) << " " << get<1>(t2) << endl;
	cout << "t2 tuple : ";
	cout << get<0>(t3) << " " << get<1>(t3) << endl;

	t2.swap(t3);
	cout << "After Swap" << endl;
	cout << "t1 tuple : ";
	cout << get<0>(t2) << " " << get<1>(t2) << endl;
	cout << "t2 tuple : ";
	cout << get<0>(t3) << " " << get<1>(t3) << endl;

	return 0;
}
