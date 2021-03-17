#include "pch.h"
#include "PathFinder.h"

#include "Tile.h"
#include <algorithm>	//reverse�Լ� ��� ���İ��� �Լ��� �� �̾ȿ� ����

int PathFinder::CalcHeuristic(int idX1, int idY1, int idX2, int idY2)
{
	int dx = abs(idX1 - idX2);
	int dy = abs(idY1 - idY2);
	int result = dx + dy; //�ִ�� �ܼ�ȭ�� Manhattan Heuristic. ��� �̵� �޸���ƽ ��꿡�� ��������� iso������ ��� �̵��� �� �밢�̵��̴�.
	return result;
}

bool PathFinder::FindPath(const vector<vector<class Tile*>>& tileList, vector<Tile*>& output,
	int startIndexX, int startIndexY, int arrivalX, int arrivalY)
{
	vector<Tile*> result;	//����� ���� ���� 

	//�������� ������ ���ٸ� �߸�ȣ��� ��
	if (startIndexX == arrivalX && startIndexY == arrivalY) return false;
	
	int tileCountY = tileList.size();	//���� Ÿ�� �� 
	int tileCountX = tileList[0].size();	//���� Ÿ�� �� 

	// {{ �ش� �Լ��� ���� �߸� �־��ٸ� 
	if (startIndexX < 0 || startIndexX >= tileCountX)return false;
	if (startIndexY < 0 || startIndexY >= tileCountY)return false;
	if (arrivalX < 0 || arrivalX >= tileCountY)return false;
	if (arrivalY < 0 || arrivalY >= tileCountY)return false;
	// }} 

	//vector<vector<DummyTile>> dummyList;	// ���̽�Ÿ ���� ������ ���� Ÿ�� ����Ʈ
	// {{ 2���� ���� Ÿ�� ���� ~
	//ù��° ���ڷ� ���� �� ��ŭ �ι�° ���ڷ� ���� ���� �����ؼ� �ִ´� 
	dummyList.assign(tileCountY, vector<DummyTile>());
	for (int i = 0; i < dummyList.size(); ++i)
		dummyList[i].assign(tileCountX, DummyTile());
	// }} 

	dummyList[startIndexY][startIndexX].IsOpen = true;
	dummyList[startIndexY][startIndexX].IsClose = true;
	dummyList[startIndexY][startIndexX].CostFromStart = 0;
	dummyList[startIndexY][startIndexX].CostToEnd =
		CalcHeuristic(startIndexX, startIndexY, arrivalX, arrivalY);
	dummyList[startIndexY][startIndexX].CostTotal =
		dummyList[startIndexY][startIndexX].CostToEnd;

	//�켱���� ť�� �����ϸ� �� ������
	struct comp //costTotal�� ���� ���� ģ���� ���� �տ� �����ϴ� �񱳿�������... �̰� �� ���ٴ� �ȵ��� �ɱ��
	{
		bool operator() (Tile* a, Tile* b)
		{
			DummyTile first = PathFinder::GetInstance()->dummyList[a->GetIndexY()][a->GetIndexX()];
			DummyTile second = PathFinder::GetInstance()->dummyList[b->GetIndexY()][b->GetIndexX()];

			return first.CostTotal > second.CostTotal;
		}
	};
	priority_queue<Tile*, vector<Tile*>, comp> openList;
	//vector<Tile*> openList;	//�� �� �ִ� �� �ĺ����� ���� �༮

	Tile* startTile = tileList[startIndexY][startIndexX];
	Tile* arrivalTile = tileList[arrivalY][arrivalX];
	Tile* currentTile = startTile;

	// {{ ��ã�� ���� 
	while (true)
	{
		//���� �߸��Ǿ���??? 
		if (currentTile == nullptr)return false;
		int currentIndexX = currentTile->GetIndexX();
		int currentIndexY = currentTile->GetIndexY();
		// {{ �ֺ� 8�� Ÿ�� �˻�~

		for (int y = currentIndexY - 1; y <= currentIndexY + 1; ++y)
		{
			if (y < 0 || y >= tileCountY)continue;

			for (int x = currentIndexX - 1; x <= currentIndexX + 1; ++x)
			{
				if (x < 0 || x >= tileCountX)continue;

				if (x == currentIndexX && y == currentIndexY)continue;

				//�밢�� ���� ������ ���⿡ ���� �߰� ~

				if (dummyList[y][x].IsClose == true)continue;

				if (tileList[y][x]->GetType() == TileType::Block)
				{
					dummyList[y][x].IsClose = true;
					dummyList[y][x].IsOpen = true;
					continue;
				}
				//�˻��Ϸ��� Ÿ���� ó�� �˻��ϴ� Ÿ���̶�� 
				if (dummyList[y][x].IsOpen == false)
				{
					dummyList[y][x].IsOpen = true;
					dummyList[y][x].Parent = currentTile;
					dummyList[y][x].CostFromStart =
						dummyList[currentIndexY][currentIndexX].CostFromStart + 1;
					dummyList[y][x].CostToEnd =
						CalcHeuristic(x, y, arrivalX, arrivalY);
					dummyList[y][x].CostTotal =
						dummyList[y][x].CostFromStart + dummyList[y][x].CostToEnd;

					openList.push(tileList[y][x]);
				}
				//ó���� �ƴ϶�� ~ ������� Ȯ���ϴµ� �ٸ� ��θ� ���ؼ� Ȯ���� �Ǿ��ٴ� ��
				else
				{
					int newFromCost =
						dummyList[currentIndexY][currentIndexX].CostFromStart + 1;
					//���� ���� ���ؼ� �� ª�� ��Ʈ�� ����
					if (newFromCost < dummyList[y][x].CostFromStart)
					{
						dummyList[y][x].CostFromStart = newFromCost;
						dummyList[y][x].CostToEnd =
							dummyList[y][x].CostFromStart + dummyList[y][x].CostToEnd;
						dummyList[y][x].Parent = currentTile;
					}
				}
			}
		}
		// }} 

		// {{ openList���� ���� ���� ����� Ÿ�� �˻� ~
		Tile* tileMin = nullptr;

		if (openList.top() == currentTile)
		{
			openList.pop();
		}
		//if (tileMin == nullptr)
		//{
		//	tileMin = openList.top();
		//}
		if(!openList.empty()) //front() empty vector �̰� ���� ������ ����. ���ϸ� ƴƴ�� invalid comparator �̰ŵ� �� �� �ִ�.
		tileMin = openList.top();

		//for (int i = 0; i < openList.size(); ++i)
		//{
		//	if (openList[i] == currentTile)
		//	{
		//		openList.erase(openList.begin() + i);
		//		--i;
		//		continue;
		//	}
		//
		//	if (tileMin == nullptr)
		//	{
		//		tileMin = openList[i];
		//		continue;
		//	}
		//	if (dummyList[openList[i]->GetIndexY()][openList[i]->GetIndexX()].CostTotal <
		//		dummyList[tileMin->GetIndexY()][tileMin->GetIndexX()].CostToEnd)
		//	{
		//		tileMin = openList[i];
		//	}
		//
		//}


		
		//������� �Դٴ� ���� ���� ���� ����� Ÿ�� �����Ǿ��ٴ� ��
		if (tileMin != nullptr)
		{
			dummyList[tileMin->GetIndexY()][tileMin->GetIndexX()].IsClose = true;
			currentTile = tileMin;
		}
		//OpenList�� �ƹ��͵� ���ٴ� �� : �� �� �ִ� ���� ����.( Ž���� ������ ����)
		else
		{
			return false;
		}
		// {{ ���� ���� ã�Ҵ� ~!
		if (tileMin == arrivalTile)
		{
			Tile* temp = tileMin;
			result.push_back(temp);
			while (dummyList[temp->GetIndexY()][temp->GetIndexX()].Parent != nullptr)
			{
				temp = dummyList[temp->GetIndexY()][temp->GetIndexX()].Parent;
				result.push_back(temp);
			}

			reverse(result.begin(), result.end());

			output.assign(result.begin(), result.end());
			return true;
		}
		// }}
	}
	// }} 

	return false;
}
