#include <iostream>
#include <algorithm>
#include <stack>

using namespace std;

const int MAX = 100000;

// 2D 공간의 점을 나타내는 Point 클래스
class Graham_Scan
{
public:
    int x, y;
    int p, q;
    Graham_Scan() : Graham_Scan(0, 0, 1, 0) {}
    Graham_Scan(int x, int y) : Graham_Scan(x, y, 1, 0) {}
    Graham_Scan(int x1, int y1, int p1, int q1) : x(x1), y(y1), p(p1), q(q1) {}

    // 정렬을 위한 작은(less than) 연산자 오버로딩
    bool operator <(const Graham_Scan& O)
    {
        if (1LL * q * O.p != 1LL * p * O.q) return 1LL * q * O.p < 1LL * p * O.q;
        if (y != O.y) return y < O.y;
        return x < O.x;
    }
};

// 세 점의 CCW (반시계 방향) 크로스 프로덕트를 계산하는 함수
long long ccw(const Graham_Scan& A, const Graham_Scan& B, const Graham_Scan& C)
{
    return 1LL * (B.x - A.x) * (C.y - A.y) - 1LL * (B.y - A.y) * (C.x - A.x);
}

int main()
{
    int N;
    printf("N: ");
    scanf_s("%d", &N);
    Graham_Scan* p = new Graham_Scan[MAX];

    // 점들을 입력받음
    for (int i = 0; i < N; i++) {
        int x, y;
        scanf_s("%d %d", &x, &y);
        p[i] = Graham_Scan(x, y);
    }

    // 각도와 y 좌표에 기반하여 점들을 정렬
    sort(p + 1, p + N);

    // 가장 낮은 점을 기준으로 한 점들의 극좌표 설정
    for (int i = 1; i < N; i++)
    {
        p[i].p = p[i].x - p[0].x;
        p[i].q = p[i].y - p[0].y;
    }

    // 다시 극좌표에 기반하여 정렬
    sort(p + 1, p + N);

    stack<int> S;
    S.push(0);
    S.push(1);
    int next = 2;

    // Graham's Scan 알고리즘을 사용하여 볼록 껍질 찾기
    while (next < N)
    {
        while (S.size() >= 2)
        {
            int first, second;
            first = S.top();
            S.pop();
            second = S.top();
            if (ccw(p[second], p[first], p[next]) > 0)
            {
                S.push(first);
                break;
            }
        }
        S.push(next++);
    }

    // 볼록 껍질의 크기를 출력
    printf("%lld\n", S.size());

    delete[] p;

    return 0;
}
