

#include <iostream>
using namespace std;

int numm = 0;//统计一个数独解的数量（变量）
int total = 0;//统计不同已知数字搭配的总数量，同时也是进度总数量（变量）

struct mask_1
{
	int rowdata;
	int valdata;
	int numdata;
}mask_1s[81];//一个储存已知数字相关信息的数组

struct mask_2
{
	int firstonerow;
	int firstoneval;
	int secondonerow;
	int secondoneval;
	int number;
}mask_2s[1000], result;
//一个储存去掉数字信息和其对应解数量的数组
//一个储存去掉数字信息和其对应解数量的结果的结构体

void GetCanFillNumber(int sudu[9][9], int row, int col, int mask[])
{
	for (int i = 0; i < 9; ++i)
	{
		int val = sudu[row][i];
		if (val == 0)
			continue;

		mask[val - 1] = 1;
	}
	for (int i = 0; i < 9; ++i)
	{
		int val = sudu[i][col];
		if (val == 0)
			continue;

		mask[val - 1] = 1;
	}

	int leftCol = col / 3 * 3;
	int topRow = row / 3 * 3;;
	for (int i = topRow; i < topRow + 3; ++i)
		for (int j = leftCol; j < leftCol + 3; ++j)
		{
			int val = sudu[i][j];
			if (val == 0)
				continue;

			mask[val - 1] = 1;
		}
}
//老师给的得到数独中可填数字的函数

bool IsFinished(int sudu[9][9])
{
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			if (sudu[i][j] == 0)
			{
				return false;
			}
		}

	return true;
}
//老师给的判断数独是否完成的函数

void PrintfSudu(int sudu[9][9])
{
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			printf("%d,", sudu[i][j]);
		}
		printf("\r\n");
	}
	printf("\r\n");
}
//老师给的打印数独的函数，但是在这道题实际上不会调用这个函数

void FillSudu(int sudu[9][9])
{

	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			if (sudu[i][j] == 0)
			{
				int mask[9] = { 0 };
				GetCanFillNumber(sudu, i, j, mask);

				for (int k = 0; k < 9; ++k)
				{
					if (mask[k] == 0)
					{
						sudu[i][j] = k + 1;
						FillSudu(sudu);
						sudu[i][j] = 0;
					}
				}
				return;
			}

		}


	if (IsFinished(sudu))
	{
		++numm;
		return;
	}
}
//老师给的完成数独的函数，做了一些修改，将打印数独结果修改成数独结果统计。

int main()
{
	int sudu[9][9];//定义了一个存储数独的二维数组
	char suduchar[9][9];//定义了一个用于存储数独字符形式的二维数组
	int row = 0, val = 0, t = 0;//初始化了行、列和统计变量的值
	int sumtotal = 0;//初始化了统计变量的值，下面会用到
	for (row = 0; row <= 8; ++row)
	{
		for (val = 0; val <= 8; ++val)
		{
			cin >> suduchar[row][val];
			sudu[row][val] = suduchar[row][val] - '0';
		}
	}//这里利用了输入字符转换为数值的方法，确保自由输入任意数独，而且支持换行
	for (row = 0; row <= 8; ++row)
	{
		for (val = 0; val <= 8; ++val)
		{
			if (sudu[row][val] == 0)
			{
				continue;
			}
			else
			{
				mask_1s[sumtotal].numdata = sudu[row][val];
				mask_1s[sumtotal].rowdata = row;
				mask_1s[sumtotal].valdata = val;
				++sumtotal;
			}
		}
	}//这里对于初始数独进行了扫描，将其中已知数字信息存储mask_1s中
	int i0 = 0, j0 = 0;
	for (i0 = 0; i0 <= sumtotal - 1; ++i0)
	{
		for (j0 = i0 + 1; j0 <= sumtotal - 1; ++j0)
		{
			++total;
		}
	}//这里事先统计了已知数字搭配的总数量，得到了总进度的大概数字。
	for (i0 = 0; i0 <= sumtotal - 1; ++i0)
	{
		for (j0 = i0 + 1; j0 <= sumtotal - 1; ++j0)
		{
			sudu[mask_1s[i0].rowdata][mask_1s[i0].valdata] = 0;
			sudu[mask_1s[j0].rowdata][mask_1s[j0].valdata] = 0;
			FillSudu(sudu);
			//求解数独
			mask_2s[t].number = numm;
			mask_2s[t].firstonerow = mask_1s[i0].rowdata;
			mask_2s[t].firstoneval = mask_1s[i0].valdata;
			mask_2s[t].secondonerow = mask_1s[j0].rowdata;
			mask_2s[t].secondoneval = mask_1s[j0].valdata;
			printf("完成进度：%d/%d:该进度计算出的数量%d\n", t + 1, total, numm);
			//打印已完成进度和总进度的关系，并且打印每次进度计算出的数量
			numm = 0;
			++t;
			sudu[mask_1s[i0].rowdata][mask_1s[i0].valdata] = mask_1s[i0].numdata;
			sudu[mask_1s[j0].rowdata][mask_1s[j0].valdata] = mask_1s[j0].numdata;
		}

	}//这里计算每种数字搭配的解的信息并存储到mask_2s中
	result.number = 0;
	for (int w = 0; w <= t - 1; ++w)
	{
		if (mask_2s[w].number > result.number)
		{
			result.number = mask_2s[w].number;
			result.firstonerow = mask_2s[w].firstonerow;
			result.firstoneval = mask_2s[w].firstoneval;
			result.secondonerow = mask_2s[w].secondonerow;
			result.secondoneval = mask_2s[w].secondoneval;
		}

	}//比较所有解中最大的那一个数字，并且将相关信息存储result中
	printf("已完成：最终结果:当去掉第%d行第%d列的数字和第%d行第%d列的数字时，可以得到最多%d个不同种类的数独终局", result.firstonerow + 1, result.firstoneval + 1, result.secondonerow + 1, result.secondoneval + 1, result.number);
	//打印结果
}
