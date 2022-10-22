#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*
굉장히 굉장히 굉장히 어려웠다고 생각하는 문제
모든 거울에 대한 정보를 담는 구조체를 만들어서 해결했다.
동서남북 출입 까지 고려해서 각 거울 당 8개의 정보가 있다.
한번이라도 지나간 경우가 있으면 그 정보는 1로 바꾼다.
모든 거울의 정보가 1이되면 -> 즉 4*거울의 수 만큼 빛이 이동했으면
모든 경로를 지났기 때문에 브레이크를 건다. (4인 이유는 한쪽에서 나간빛 = 다른쪽에서 들어온 빛)
벡터 라는 개념을 도입해 이후 빛이 어디로 이동할지를 판단한다.
순환이 되기 때문에 무조건 빛은 원위치로 돌아온다. 안돌아 오는 경우는 없다.
굉장히 어려웠고 그만큼 뿌듯했던 문제
*/

int* PlusOrder(int* list, int length)
// 오름차순 정렬
{
    int* NumCount = (int*)calloc(100000, sizeof(int));
    int index = 0;
    for (int i = 0; i < length; i++)
    {
        NumCount[list[i]] += 1;
    }

    for (int i = 0; i < 100000; i++)
    {
        if (NumCount[i] != NULL)
        {
            for (int j = 0; j < NumCount[i]; j++)
            {
                list[index] = i;
                index += 1;
            }
        }
        if (index == length - 1)
        {
            break;
        }
    }
    free(NumCount);
}

typedef struct
{
    int CaGNum;
    int CaGData[8];
    // 첫번째 부터 차례대로 동(출입) 서(출입) 남(출입) 북(출입)
} PositionData;

PositionData p[250000];
int result[300000] = { 0, };
// 각각의 거울에 대한 구조체와 결과를 담을 변수

void PrintLog(int OnOff, int c, int ci, int* po, int* v, int* list, int num)
// 로그를 프린트하여 오류를 잡아내려고 만든 함수
{
    if (OnOff)
    {
        printf("count : %d, CycleIndex : %d\n", c, ci);
        printf("position : %d, %d\n", po[0], po[1]);
        printf("Vector : %d, %d\n", v[0], v[1]);
        printf("CaGData : ");
        for (int i = 0; i < 8; i++)
        {
            printf("%d", list[i]);
        }
        printf("\n");
        printf("CaGNum : %d\n\n", num);
        _getch();
    }
}

int ComeVector(int* v)
// 들어오는 백테가 있을 때 그 백터가 해당되는 위치, 위치의 정의는 구조체에 적혀있다.
{
    if (v[0] == 1 && v[1] == 0)
    {
        return 3;
    }
    else if (v[0] == -1 && v[1] == 0)
    {
        return 1;
    }
    else if (v[0] == 0 && v[1] == -1)
    {
        return 5;
    }
    else if (v[0] == 0 && v[1] == 1)
    {
        return 7;
    }
}

int OutVector(int* v)
// 나가는 백테가 있을 때 그 백터가 해당되는 위치
{
    if (v[0] == 1 && v[1] == 0)
    {
        return 0;
    }
    else if (v[0] == -1 && v[1] == 0)
    {
        return 2;
    }
    else if (v[0] == 0 && v[1] == 1)
    {
        return 4;
    }
    else if (v[0] == 0 && v[1] == -1)
    {
        return 6;
    }
}

int* OutStart(int i)
// 백터의 위치 정보가 오면 그 위치에 맞는 나가는 백터를 리턴
{
    int v[2] = { 0,0 };
    switch (i)
    {
    case 0:
        v[0] = 1;
        break;
    case 2:
        v[0] = -1;
        break;
    case 4:
        v[1] = 1;
        break;
    case 6:
        v[1] = -1;
        break;
    }
    return v;
}

void PositionChange(int* p, int* v, int Mx, int My)
// 백터와 위치를 받아와서 다음 위치를 리턴함
{
    p[0] += v[0];
    p[1] += v[1];
    if (p[0] >= Mx)
    {
        p[0] = 0;
    }
    if (p[0] < 0)
    {
        p[0] = Mx - 1;
    }
    if (p[1] >= My)
    {
        p[1] = 0;
    }
    if (p[1] < 0)
    {
        p[1] = My - 1;
    }
}

void VectorChange(char m, int* v)
// 거울와 백터를 받아와서 다음 백터를 리턴함
{
    if (m == 'L')
    {
        int tmp = v[0];
        switch (tmp)
        {
        case 0:
            v[0] = v[1];
            v[1] = tmp;
            break;
        default:
            v[0] = -v[1];
            v[1] = -tmp;
            break;
        }
    }
    else if (m == 'R')
    {
        int tmp = v[0];
        switch (tmp)
        {
        case 0:
            v[0] = -v[1];
            v[1] = -tmp;
            break;
        default:
            v[0] = v[1];
            v[1] = tmp;
            break;
        }
    }
}

int* solution(const char* grid[], size_t grid_len) {
    // return 값은 malloc 등 동적 할당을 사용해주세요. 할당 길이는 상황에 맞게 변경해주세요
    for (int i = 0; i < 250000; i++)
    {
        p[i].CaGNum = 0;
    }

    int MaxY = grid_len;
    int MaxX = strlen(grid[0]);
    // 현재 거울 격자들의 가로세로 최대길이, 최대 길이를 넘을 때 반응하기 위한 변수
    int MaxCount = (MaxX * MaxY) * 4;
    // 빛이 최대로 튕길수 있는 횟수, 이 횟수로 모든 경로를 이동했는지 판별
    char mirror = grid[0][0];
    // 현재 거울, 백터와 반응하여 다음 백터를 만들어낼 변수
    int vector[2] = { 1,0 };
    // 현재 백터, 위치와 반응하여 다음 위치를 만들어낼 변수
    int position[2] = { 0,0 };
    // 현재 위치
    int count = 0;
    // 해당 싸이클에서 빛이 몇번 튕겼는지 알려주는 변수
    int TotalCount = 0;
    // 모든 싸이클에서 빛이 몇번 튕겼는지 알려주는 변수
    int CycleIndex = 0;
    // 싸이클 횟수
    int CaGindex = 0;
    // 구조체에서 CaGData 전용 인덱스
    int StartIndex = 0;
    // 연산량을 줄이기 위해 도입된 변수		

    while (1)
    {
        CaGindex = OutVector(vector);
        if (p[MaxX * position[1] + position[0]].CaGData[CaGindex] == 1)
        {
            result[CycleIndex] = count;
            TotalCount += count;
            if (TotalCount >= MaxCount)
            {
                PlusOrder(&result, CycleIndex + 1);
                int* answer = (int*)malloc(sizeof(int*) * (CycleIndex + 1));
                for (int i = 0; i < CycleIndex + 1; i++)
                {
                    answer[i] = result[i];
                }
                return answer;
            }
            else if (TotalCount < MaxCount)
            {
                count = 0;
                CycleIndex += 1;
                for (int i = StartIndex; i < 250000; i++)
                {
                    if (p[i].CaGNum < 8)
                    {
                        StartIndex = i;
                        for (int j = 0; j < 8; j += 2)
                        {
                            if (p[i].CaGData[j] == 0)
                            {
                                position[0] = i % MaxX;
                                position[1] = (i / MaxX);
                                vector[0] = OutStart(j)[0];
                                vector[1] = OutStart(j)[1];
                                p[i].CaGData[j] = 1;
                                p[i].CaGNum += 1;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
        else if (p[MaxX * position[1] + position[0]].CaGData[CaGindex] != 1)
        {
            p[MaxX * position[1] + position[0]].CaGData[CaGindex] = 1;
            p[MaxX * position[1] + position[0]].CaGNum += 1;
        }

        PrintLog(1, count, CycleIndex, position, vector, p[MaxX * position[1] + position[0]].CaGData, p[MaxX * position[1] + position[0]].CaGNum);

        PositionChange(&position, vector, MaxX, MaxY);

        CaGindex = ComeVector(vector);

        p[MaxX * position[1] + position[0]].CaGData[CaGindex] = 1;
        p[MaxX * position[1] + position[0]].CaGNum += 1;

        mirror = grid[position[1]][position[0]];

        VectorChange(mirror, &vector);

        count += 1;


    }
}

int main()
{
    char* test[2] = { "SSS","SSS" };
    int* r = solution(test, 2);
    for (int i = 0; i < 20; i++)
    {
        printf("%d,", r[i]);
    }

    _getch();
}