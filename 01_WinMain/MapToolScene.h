#pragma once
#include "Scene.h"
enum class Layer : int {
	Tile,
	Object,
	End
};
class Tile;
class Button;
class Pallete;
class ICommand;
class MapToolScene  : public Scene
{
	Image* mImage;

	vector <vector<Tile*>> mTileList;
	vector <vector<Pallete*>> mPalleteList;
	vector <class MapObject*> mMapObjectList;
	vector <vector<class MapObjectPallete*>> mMapObjectPallete;

	Tile* mCurrentTile;
	Pallete* mCurrentPallete;
	//MapObject* mCurrentMapObject;
	MapObjectPallete* mCurrentMapObjectPallete;

	Button* mSave;
	Button* mLoad;
	Button* mUndo;
	Button* mRedo;
	Button* mNext;
	Button* mChangeLayer;

	stack <ICommand*>  mCommandList;
	stack <ICommand*> mRedoList;
	stack <ICommand*> mObjectCommandList;
	stack <ICommand*> mObjectRedoList;

	RECT mPalletRc;
	RECT mMenuRc;
	Layer mCurrentLayer = Layer::Tile;
	
	bool mTabKey = false;
	bool mRenderToggle = false;
private:
	void RegisterCommand(ICommand* command);
	void Save();
	void Load();
	void Undo();
	void Redo();
	void ChangeLayer();
	void stackClear(stack<ICommand*>* stack);
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc) override;
};