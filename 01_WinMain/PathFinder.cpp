#include "pch.h"
#include "PathFinder.h"

#include "Tile.h"
#include <algorithm>	//reverse함수 등등 정렬관련 함수들 다 이안에 있음

int PathFinder::CalcHeuristic(int idX1, int idY1, int idX2, int idY2)
{
	int dx = abs(idX1 - idX2);
	int dy = abs(idY1 - idY2);
	int result = dx + dy; //최대로 단순화한 Manhattan Heuristic. 사방 이동 휴리스틱 계산에만 사용하지만 iso에서는 사방 이동이 곧 대각이동이다.
	return result;
}

bool PathFinder::FindPath(const vector<vector<class Tile*>>& tileList, vector<Tile*>& output,
	int startIndexX, int startIndexY, int arrivalX, int arrivalY)
{
	vector<Tile*> result;	//결과값 담을 벡터 

	//시작점이 끝점과 같다면 잘못호출된 것
	if (startIndexX == arrivalX && startIndexY == arrivalY) return false;
	
	int tileCountY = tileList.size();	//세로 타일 수 
	int tileCountX = tileList[0].size();	//가로 타일 수 

	// {{ 해당 함수에 값을 잘못 넣었다면 
	if (startIndexX < 0 || startIndexX >= tileCountX)return false;
	if (startIndexY < 0 || startIndexY >= tileCountY)return false;
	if (arrivalX < 0 || arrivalX >= tileCountY)return false;
	if (arrivalY < 0 || arrivalY >= tileCountY)return false;
	// }} 

	//vector<vector<DummyTile>> dummyList;	// 에이스타 연산 도와줄 더미 타일 리스트
	// {{ 2차원 더미 타일 세팅 ~
	//첫번째 인자로 들어온 수 만큼 두번째 인자로 들어온 값을 복사해서 넣는다 
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

	//우선순위 큐로 수정하면 더 빨라짐
	struct comp //costTotal이 가장 작은 친구를 가장 앞에 정렬하는 비교연산조건... 이거 왜 람다는 안들어가는 걸까요
	{
		bool operator() (Tile* a, Tile* b)
		{
			DummyTile first = PathFinder::GetInstance()->dummyList[a->GetIndexY()][a->GetIndexX()];
			DummyTile second = PathFinder::GetInstance()->dummyList[b->GetIndexY()][b->GetIndexX()];

			return first.CostTotal > second.CostTotal;
		}
	};
	priority_queue<Tile*, vector<Tile*>, comp> openList;
	//vector<Tile*> openList;	//갈 수 있는 길 후보들을 담을 녀석

	Tile* startTile = tileList[startIndexY][startIndexX];
	Tile* arrivalTile = tileList[arrivalY][arrivalX];
	Tile* currentTile = startTile;

	// {{ 길찾기 루프 
	while (true)
	{
		//뭔가 잘못되었다??? 
		if (currentTile == nullptr)return false;
		int currentIndexX = currentTile->GetIndexX();
		int currentIndexY = currentTile->GetIndexY();
		// {{ 주변 8개 타일 검사~

		for (int y = currentIndexY - 1; y <= currentIndexY + 1; ++y)
		{
			if (y < 0 || y >= tileCountY)continue;

			for (int x = currentIndexX - 1; x <= currentIndexX + 1; ++x)
			{
				if (x < 0 || x >= tileCountX)continue;

				if (x == currentIndexX && y == currentIndexY)continue;

				//대각선 막고 싶으면 여기에 조건 추가 ~

				if (dummyList[y][x].IsClose == true)continue;

				if (tileList[y][x]->GetType() == TileType::Block)
				{
					dummyList[y][x].IsClose = true;
					dummyList[y][x].IsOpen = true;
					continue;
				}
				//검사하려는 타일이 처음 검사하는 타일이라면 
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
				//처음이 아니라면 ~ 여기까지 확장하는데 다른 경로를 통해서 확장이 되었다는 뜻
				else
				{
					int newFromCost =
						dummyList[currentIndexY][currentIndexX].CostFromStart + 1;
					//이전 비용과 비교해서 더 짧은 루트를 선택
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

		// {{ openList에서 가장 적은 비용의 타일 검사 ~
		Tile* tileMin = nullptr;

		if (openList.top() == currentTile)
		{
			openList.pop();
		}
		//if (tileMin == nullptr)
		//{
		//	tileMin = openList.top();
		//}
		if(!openList.empty()) //front() empty vector 이거 보기 싫으면 막자. 안하면 틈틈이 invalid comparator 이거도 볼 수 있다.
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


		
		//여기까지 왔다는 것은 가장 적은 비용의 타일 선정되었다는 것
		if (tileMin != nullptr)
		{
			dummyList[tileMin->GetIndexY()][tileMin->GetIndexX()].IsClose = true;
			currentTile = tileMin;
		}
		//OpenList에 아무것도 없다는 뜻 : 갈 수 있는 길이 없다.( 탐색의 여지가 없음)
		else
		{
			return false;
		}
		// {{ 도착 지점 찾았다 ~!
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
