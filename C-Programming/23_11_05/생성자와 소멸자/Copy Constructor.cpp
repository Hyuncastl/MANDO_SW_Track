//복사 생성자는 자신과 같은 자료형의 객체를 인수로 전달하는 생성자.
//복사 생성자가 정의되어 있지 않다면, 디폴트 복사 생성자(Default Copy Constructor)가 생성된다.

#include <iostream>

using namespace std;

class MyClass
{
private:
	int num1;
	int num2;
public:
	MyClass(int a, int b)
	{
		num1 = a;
		num2 = b;
	}
	void ShowData()
	{
		cout << "num1: " << num1 << " num2: " << num2 << endl;
	}
};

int main()
{
	MyClass mc1(50, 40);
	MyClass mc2 = mc1;

	mc2.ShowData();
	return 0;
}
