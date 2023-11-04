#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
using namespace std;

double euclideanDistance(int x1, int x2, int y1, int y2) 
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int main() 
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    const int numElements = 100;
    const int target1 = 30;
    const int target2 = 70;

    std::vector<int> A; 
    std::vector<int> B; 

    for (int i = 0; i < numElements; ++i) 
    {
        int random_number = std::rand() % (numElements + 1); // 0부터 100까지의 랜덤 수 생성

        double distanceToTarget1 = euclideanDistance(random_number, target1, 0, 0);

        double distanceToTarget2 = euclideanDistance(random_number, target2, 0, 0);

        if (distanceToTarget1 <= 10) 
        {
            A.push_back(random_number);
        }
        if (distanceToTarget2 <= 5) 
        {
            B.push_back(random_number);
        }
    }

    std::cout << "배열 A: ";
    for (int num : A)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cout << "배열 B: ";
    for (int num : B) 
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
