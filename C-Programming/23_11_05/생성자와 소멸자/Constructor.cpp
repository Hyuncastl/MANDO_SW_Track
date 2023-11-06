//생성자를 정의할때 생성자의 이름이 클래스의 이름과 같다. 
//생성자도 함수와 같이 매개변수를 가질 수 있다. 그리고 반환형이 없다. 
//생성자의 또다른 특징에는 생성자도 함수 중 하나니 함수 오버로딩이 가능.
#include <iostream>

using namespace std;

class ExConstructor
{
public:
	ExConstructor()
	{
		cout << "ExConstructor() called!" << endl;
	}

	ExConstructor(int a)
	{
		cout << "ExConstructor(int a) called!" << endl;
	}

	ExConstructor(int a, int b)
	{
		cout << "ExConstructor(int a, int b) called!" << endl;
	}
};

int main()
{
	ExConstructor ec1;
	ExConstructor ec2(10);
	ExConstructor ec3(20, 10);

	return 0;
}
