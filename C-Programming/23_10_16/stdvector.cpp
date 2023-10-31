#include <algorithm>
#include <vector>
#include <iostream>
#include <stdio.h>
using namespace std;

vector<int> v1;
vector<int> v2(4);
vector<int> v3 = { 1,2,3 };
vector<int> v4[] = { {1,2},{3,4} };
vector<vector<int>> v5;
vector<int> v6 = { 1,2,3,4,5 };


class A 
{
private:
    int num;
    std::string name;
public:
    A(int i, std::string s) : num(i), name(s) {}
};


int main() 
{
    vector<int> v = { 1, 2, 3, 4 };
    for_each(v.begin(), v.end(), [&](int& n) 
        {
        cout << n << endl;        //output : 1 2 3 4
        });

    for_each(v.rbegin(), v.rend(), [&](int& n) 
        {
        cout << n << endl;        //output : 4 3 2 1
        });

    vector<int>::iterator itor = v.begin();
    for (; itor != v.end(); itor++)
        cout << *itor << endl;        //output : 1 2 3 4
    vector<int>::reverse_iterator itor2 = v.rbegin();
    for (; itor2 != v.rend(); itor2++)
        cout << *itor2 << endl;        //output : 4 3 2 1
    ////////////////////////////////////////////////////
    v6.assign(5, 10);
    for (int i = 0; i < v6.size(); i++) 
    {
        cout << v6[i] << endl;
    }
    ////////////////////////////////////////////////////
    vector<int> v7 = { 1, 2, 3, 4 };
    cout << v7.front() << endl;        //output : 1
    cout << v7.back() << endl;        //output : 4
    cout << v7.at(1) << endl;        //output : 2
    cout << v7[2] << endl;            //output : 3​
    ////////////////////////////////////////////////////
    vector<int> v8;
    v8.push_back(10);
    v8.push_back(20);        //v = { 10, 20 }
    v8.insert(v.begin() + 1, 100);     // v = { 10, 100, 20 }
    v8.pop_back();        // v = { 10, 100 }
    v8.emplace_back(1); //v = { 10, 100, 1 }
    v8.emplace_back(2);    //v = { 10, 100, 1, 2 }
    v8.emplace(v.begin() + 2, -50);    //v = { 1, 100, -50, 1, 2 }
    v8.erase(v.begin() + 1); // v = { 1, -50, 1, 2 }
    v8.resize(6);    // v = { 1, -50, 1, 2, 0, 0 }
    v8.clear();    // v = empty()     
    ////////////////////////////////////////////////////
    std::vector<A> v9;
    A a(1, "hwan");
    v9.push_back(a);
    //v9.push_back(1, "hi");   //error -> v.push_back(a);
    v9.emplace_back(1, "hi"); //ok!!
    ////////////////////////////////////////////////////
    vector<int> v10 = { 1, 2, 3, 4 };
    v10.clear();
    cout << v10.capacity() << endl;    //output : 10
    vector<int>().swap(v10);
    cout << v10.capacity() << endl;    //output : 0​
    ////////////////////////////////////////////////////
    vector<int>v11 = { 1, 2, 3, 4 };
    cout << v11.size() << endl;    //output : 4
    cout << v11.capacity() << endl; //output : 10 (컴파일 환경에 따라 달라질 수 있음)
    v11.reserve(6);
    cout << v11.capacity() << endl; //output : 6
    cout << v11.max_size() << endl; //output : 1073741823(시스템 성능에 따라 달라질 수 있음)
    v11.shrink_to_fit();
    cout << v11.capacity() << endl; //output : 4
    cout << v11.empty() << endl; //output : false
    v11.clear();
    cout << v11.empty() << endl; //output : true​
}
