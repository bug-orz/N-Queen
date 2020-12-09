#include<iostream>
#include<vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

//����ʺ�����
const int queenNum = 8;

//��ͻ������
static int checkNum = 0;

//��Żʺ�����飬�±�Ϊ������ȡֵΪ����
static int* queenColumn = new int[queenNum];

//����������
static int maxRoundNum = 10;

/*
���룺��Żʺ�����飬�±�Ϊ������ȡֵΪ����
����ֵ�� /
������ִ�д�ӡ���ܣ���ӡ���ʺ����յİڷ�λ�á�
*/
void printResult(vector<int> eightQueenColumnResult, int checkNum);

/*
* ���룺
* �������Żʺ�����飬�±�Ϊ������ȡֵΪ����
*����ڷŰ˸��ʺ��λ�ã���֤����ͬһ��ͬһ��
*/
vector<int> randomInitQueenColumn();

/*
*���룺��Żʺ������
*�����true:��ȷ��� false:������
*/
bool checkQueenColumn(vector<int> queenColumn);

/*
*Լ����������Ļ����㷨���ҵ�һ��������ɣ�
*/

class Queen {
	vector<int> column;
	vector<int> vertical;//��ֱ����Ļʺ�����
	vector<int> diagonal_1;//���Խ��߷���Ļʺ�����
	vector<int> diagonal_2;//���Խ��߷���Ļʺ�����
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
*Լ�������������С��ͻ����㷨���ҵ�һ��������ɣ�
*/
vector<int> minConflicts() {
	/*
		�ڴ�ʵ�ְ˻ʺ��������С��ͻ����㷨�����ں�����������printResult������ӡ�����
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
						ans = j;  //��һ�����ѡ�����Ҫ����������
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
		cout << "���ڴ˴�������õ��㷨��" << endl;
		cout << "-----------------------------------------------------" << endl;
		cout << "1: ���ݷ�" << endl;
		cout << "2: ��С��ͻ��ⷨ" << endl;
		cout << "0:�˳�" << endl;
		cout << "-----------------------------------------------------" << endl;
		cin >> type;
		while (type < 0 && type>2) {
			cout << "���������������������룺" << endl;
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
				cout << "��ȷ���" << endl;
				printResult(queenColumnResult,checkNum);
			}
			else {
				cout << "������" << endl;
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
				cout << "��ȷ���" << endl;
				printResult(queenColumnResult, checkNum);
			}
			else {
				cout << "������" << endl;
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
	//��ʼ��N���ʺ��Ӧ��R����Ϊ0~N-1��һ�����У���û������ʺ�ͬ�У�Ҳû���κλʺ�ͬ��
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
	cout << "---------------------�ʺ�λ�ðڷŽ������-----------------" << endl;
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
	cout << "��ͻ��������"<< checkNum << endl;
}





/*
* https://blog.csdn.net/u013390476/article/details/50011261
* 
void MinConflict::MinConflictAlgorithm()
	While û���ҵ����Ž�
		For row: 0 to QueenNum-1 do
			�ƿ�����row��һ�еĻʺ�
			������Ӧ����������ʺ�������vector
			For column: 0 to QueenNum do
				����(row, column)λ�õĳ�ͻֵ������ʱ��
				If ��֮ǰ�� min ��С
					������Сֵ min, ���´�ʱ�� columnMin
				Else if ��֮ǰ����Сֵ���
					50%���ʸ�����Сֵmin�����´�ʱ�� columnMin
				End if
			End for
			���ûʺ󵽳�ͻ��С�ĵط�(row, columnMin)
			������Ӧ����������ʺ�������vector
			������ﵽ������״̬�����ҵ������Ž⣬break For
		End for
	End while
	��ӡ���Ž�
End
*/
