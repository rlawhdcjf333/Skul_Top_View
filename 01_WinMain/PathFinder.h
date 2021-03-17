#pragma once
class PathFinder
{
	Singleton(PathFinder)
private:
	struct DummyTile
	{
		class Tile* Parent;
		bool IsOpen;
		bool IsClose;
		int CostTotal;
		int CostFromStart;
		int CostToEnd;

		DummyTile()
			:Parent(nullptr), IsOpen(false), IsClose(false),
			CostTotal(MAXINT), CostFromStart(MAXINT), CostToEnd(MAXINT) {}
	};

	vector<vector<DummyTile>> dummyList;
	
public:
	int CalcHeuristic(int idX1, int idY1, int idX2, int idY2);

	//해당 함수 실행하면 최단경로 타일들 찾아서 받아오기
	bool FindPath(const vector<vector<class Tile*>>& tileList, vector<Tile*>& output,
		int startIndexX, int startIndexY, int arrivalX, int arrivalY);
};

