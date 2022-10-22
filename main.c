#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*
������ ������ ������ ������ٰ� �����ϴ� ����
��� �ſ￡ ���� ������ ��� ����ü�� ���� �ذ��ߴ�.
�������� ���� ���� ����ؼ� �� �ſ� �� 8���� ������ �ִ�.
�ѹ��̶� ������ ��찡 ������ �� ������ 1�� �ٲ۴�.
��� �ſ��� ������ 1�̵Ǹ� -> �� 4*�ſ��� �� ��ŭ ���� �̵�������
��� ��θ� ������ ������ �극��ũ�� �Ǵ�. (4�� ������ ���ʿ��� ������ = �ٸ��ʿ��� ���� ��)
���� ��� ������ ������ ���� ���� ���� �̵������� �Ǵ��Ѵ�.
��ȯ�� �Ǳ� ������ ������ ���� ����ġ�� ���ƿ´�. �ȵ��� ���� ���� ����.
������ ������� �׸�ŭ �ѵ��ߴ� ����
*/

int* PlusOrder(int* list, int length)
// �������� ����
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
    // ù��° ���� ���ʴ�� ��(����) ��(����) ��(����) ��(����)
} PositionData;

PositionData p[250000];
int result[300000] = { 0, };
// ������ �ſ￡ ���� ����ü�� ����� ���� ����

void PrintLog(int OnOff, int c, int ci, int* po, int* v, int* list, int num)
// �α׸� ����Ʈ�Ͽ� ������ ��Ƴ����� ���� �Լ�
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
// ������ ���װ� ���� �� �� ���Ͱ� �ش�Ǵ� ��ġ, ��ġ�� ���Ǵ� ����ü�� �����ִ�.
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
// ������ ���װ� ���� �� �� ���Ͱ� �ش�Ǵ� ��ġ
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
// ������ ��ġ ������ ���� �� ��ġ�� �´� ������ ���͸� ����
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
// ���Ϳ� ��ġ�� �޾ƿͼ� ���� ��ġ�� ������
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
// �ſ�� ���͸� �޾ƿͼ� ���� ���͸� ������
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
    // return ���� malloc �� ���� �Ҵ��� ������ּ���. �Ҵ� ���̴� ��Ȳ�� �°� �������ּ���
    for (int i = 0; i < 250000; i++)
    {
        p[i].CaGNum = 0;
    }

    int MaxY = grid_len;
    int MaxX = strlen(grid[0]);
    // ���� �ſ� ���ڵ��� ���μ��� �ִ����, �ִ� ���̸� ���� �� �����ϱ� ���� ����
    int MaxCount = (MaxX * MaxY) * 4;
    // ���� �ִ�� ƨ��� �ִ� Ƚ��, �� Ƚ���� ��� ��θ� �̵��ߴ��� �Ǻ�
    char mirror = grid[0][0];
    // ���� �ſ�, ���Ϳ� �����Ͽ� ���� ���͸� ���� ����
    int vector[2] = { 1,0 };
    // ���� ����, ��ġ�� �����Ͽ� ���� ��ġ�� ���� ����
    int position[2] = { 0,0 };
    // ���� ��ġ
    int count = 0;
    // �ش� ����Ŭ���� ���� ��� ƨ����� �˷��ִ� ����
    int TotalCount = 0;
    // ��� ����Ŭ���� ���� ��� ƨ����� �˷��ִ� ����
    int CycleIndex = 0;
    // ����Ŭ Ƚ��
    int CaGindex = 0;
    // ����ü���� CaGData ���� �ε���
    int StartIndex = 0;
    // ���귮�� ���̱� ���� ���Ե� ����		

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