#include<iostream>
#include<vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

//定义皇后数量
const int queenNum = 8;

//冲突检测次数
static int checkNum = 0;

//存放皇后的数组，下标为行数，取值为列数
static int* queenColumn = new int[queenNum];

//最大随机次数
static int maxRoundNum = 10;

/*
输入：存放皇后的数组，下标为行数，取值为列数
返回值： /
描述：执行打印功能，打印出皇后最终的摆放位置。
*/
void printResult(vector<int> eightQueenColumnResult, int checkNum);

/*
* 输入：
* 输出：存放皇后的数组，下标为行数，取值为列数
*随机摆放八个皇后的位置，保证不在同一行同一列
*/
vector<int> randomInitQueenColumn();

/*
*输入：存放皇后的数组
*输出：true:正确结果 false:错误结果
*/
bool checkQueenColumn(vector<int> queenColumn);

/*
*约束满足问题的回溯算法（找到一个结果即可）
*/

class Queen {
	vector<int> column;
	vector<int> vertical;//垂直方向的皇后数量
	vector<int> diagonal_1;//主对角线方向的皇后数量
	vector<int> diagonal_2;//反对角线方向的皇后数量
public:
	Queen(vector<int>& temp) {
		column = temp;
		checkNum = 0;
	}
	Queen() {
		column = randomInitQueenColumn();
		checkNum = 0;
	}
	int getConflictValue(int x,int y);
	void move(int x, int y);
	void update(int row);
	bool check();
	vector<int> vec();
};

void Queen::move(int x, int y) {
	column[x] = y;
}

void Queen::update(int col) {
	vector<int> temp1(queenNum, 0);\
	vector<int> temp2(2*queenNum-1, 0);
	vertical = temp1;
	diagonal_1 = temp2;
	diagonal_2 = temp2;
	for (int i = 0; i < queenNum; i++) {
		if (i == col) {
			continue;
		}
		if (column[i] >= 0 && column[i] < queenNum) {
			vertical[column[i]] += 1;
			diagonal_1[i - column[i] + queenNum - 1] += 1;
			diagonal_2[i + column[i]] += 1;
		}
	}
}

int Queen::getConflictValue(int x,int y) {
	checkNum += 1;
	return vertical[y] + diagonal_1[x - y + queenNum-1] + diagonal_2[x + y];
}

bool Queen::check() {
	return checkQueenColumn(column);
}

vector<int> Queen::vec() {
	return column;
}

vector<int> temp00(queenNum, -1);
Queen q00(temp00);

vector<int> bactrackDFS(int row=0, Queen q=q00 ) {
	if (row == 0) {
		Queen q(temp00);
	}
	vector<int> ans = q.vec();
	q.update(row);
	for (int j = 0; j < queenNum; j++) {
		if (q.getConflictValue(row, j) == 0) {
			q.move(row, j);
			if (row == queenNum - 1) {
				return q.vec();
			}
			ans=bactrackDFS(row + 1,q);
			if (checkQueenColumn(ans)) {
				return ans;
			}
		}
	}
	return ans;
}

/*
*约束满足问题的最小冲突检测算法（找到一个结果即可）
*/
vector<int> minConflicts() {
	/*
		在此实现八皇后问题的最小冲突检测算法，请在函数体最后调用printResult函数打印结果。
	*/
	Queen q;
	while (!q.check()) {
		for (int i = 0; i < queenNum; i++) {
			q.update(i);
			int temp = 100000000000;
			int ans = queenNum + 1;
			for (int j = 0; j < queenNum; j++) {
				int conf=q.getConflictValue(i, j);
				if (conf < temp) {
					temp = conf;
					ans = j;
				}
				if (conf == temp) {
					if (rand()%2) {
						ans = j;  //这一步随机选择很重要！！！！！
					}
				}
			}
			q.move(i, ans);
		}
	}
	printResult(q.vec(), checkNum);
	return q.vec();
}


int main() {
	while (true) {
		int type;
		cout << "请在此处输入调用的算法：" << endl;
		cout << "-----------------------------------------------------" << endl;
		cout << "1: 回溯法" << endl;
		cout << "2: 最小冲突检测法" << endl;
		cout << "0:退出" << endl;
		cout << "-----------------------------------------------------" << endl;
		cin >> type;
		while (type < 0 && type>2) {
			cout << "输入类型有误，请重新输入：" << endl;
			cin >> type;
			break;
		}
		cout << "----------------------START--------------------------" << endl;
		cout << endl;
		if (type == 1) {
			auto start = high_resolution_clock::now();
			vector<int> queenColumnResult = bactrackDFS();
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "Time taken by function bactrackDFS() is "
				<< duration.count()/1000 << " ms" << endl;
			bool result = checkQueenColumn(queenColumnResult);
			if (result) {
				cout << "正确结果" << endl;
				printResult(queenColumnResult,checkNum);
			}
			else {
				cout << "错误结果" << endl;
				printResult(queenColumnResult, checkNum);
			}
		}
		else if (type == 2) {
			auto start = high_resolution_clock::now();
			vector<int> queenColumnResult = minConflicts();
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "Time taken by function minConflicts() is "
				<< duration.count()/1000 << " ms" << endl;
			bool result = checkQueenColumn(queenColumnResult);
			if (result) {
				cout << "正确结果" << endl;
				printResult(queenColumnResult, checkNum);
			}
			else {
				cout << "错误结果" << endl;
				printResult(queenColumnResult, checkNum);
			}
		}
		else {
			return 0;
		}
		cout << "----------------------END ---------------------------" << endl;
		cout << endl;
	}
	return 0;
}

bool checkQueenColumn(vector<int> queenColumn) {
	if (queenColumn.size() != queenNum)
		return false;
	for (int i = 0; i < queenNum; i++) {
		for (int j = i + 1; j < queenNum; j++) {
			if (queenColumn[i] == queenColumn[j])
				return false;
			if (abs(i - j) == abs(queenColumn[i] - queenColumn[j]))
				return false;
		}
	}
	return true;
}

vector<int> randomInitQueenColumn() {
	vector<int> randomQueenColumn = vector<int>();
	for (int i = 0; i < queenNum; i++) {
		randomQueenColumn.push_back(i);
	}
	//初始化N个皇后对应的R数组为0~N-1的一个排列，即没有任意皇后同列，也没有任何皇后同行
	for (int i = 0; i <= queenNum - 2; i++) {
		/*srand((int)time(0));*/
		int randomNum = rand() % (queenNum - i) + i;
		int temp = randomQueenColumn[i];
		randomQueenColumn[i] = randomQueenColumn[randomNum];
		randomQueenColumn[randomNum] = temp;
	}
	return randomQueenColumn;
}

void printResult(vector<int> eightQueenColumnResult, int checkNum)
{
	cout << "---------------------皇后位置摆放结果如下-----------------" << endl;
	cout << endl;
	for (int i = 0; i < eightQueenColumnResult.size(); i++) {
		switch (eightQueenColumnResult[i]) {
		case 0:
			cout << "Q * * * * * * *" << endl;
			break;
		case 1:
			cout << "* Q * * * * * *" << endl;
			break;
		case 2:
			cout << "* * Q * * * * *" << endl;
			break;
		case 3:
			cout << "* * * Q * * * *" << endl;
			break;
		case 4:
			cout << "* * * * Q * * *" << endl;
			break;
		case 5:
			cout << "* * * * * Q * *" << endl;
			break;
		case 6:
			cout << "* * * * * * Q *" << endl;
			break;
		case 7:
			cout << "* * * * * * * Q" << endl;
			break;
		}

	}
	cout << "冲突检测次数："<< checkNum << endl;
}





/*
* https://blog.csdn.net/u013390476/article/details/50011261
* 
void MinConflict::MinConflictAlgorithm()
	While 没有找到最优解
		For row: 0 to QueenNum-1 do
			移开棋盘row这一行的皇后
			更新相应的三个保存皇后数量的vector
			For column: 0 to QueenNum do
				计算(row, column)位置的冲突值，常数时间
				If 比之前的 min 更小
					更新最小值 min, 更新此时的 columnMin
				Else if 和之前的最小值相等
					50%概率更新最小值min，更新此时的 columnMin
				End if
			End for
			放置皇后到冲突最小的地方(row, columnMin)
			更新相应的三个保存皇后数量的vector
			如果检测达到了最优状态，即找到了最优解，break For
		End for
	End while
	打印最优解
End
*/
