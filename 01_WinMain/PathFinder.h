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

	//�ش� �Լ� �����ϸ� �ִܰ�� Ÿ�ϵ� ã�Ƽ� �޾ƿ���
	bool FindPath(const vector<vector<class Tile*>>& tileList, vector<Tile*>& output,
		int startIndexX, int startIndexY, int arrivalX, int arrivalY);
};

