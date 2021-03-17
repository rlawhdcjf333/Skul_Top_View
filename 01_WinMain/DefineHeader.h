#pragma once
#define WINSTARTX 50
#define WINSTARTY 50
#define WINSIZEX 1280
#define WINSIZEY 720

#define TileSizeX 60
#define TileSizeY 30
#define StartX 610
#define StartY 100

//인자로 들어온 p를 삭제하고 NULL로 초기화 하는 매크로 함수, 안전하게 삭제
#define SafeDelete(p)\
{\
	if(p)\
	{\
		delete (p);\
		(p) = NULL;\
	}\
}

//싱글톤 매크로
#define Singleton(ClassName)\
public:\
static ClassName* GetInstance()\
{\
	static ClassName instance;\
	return &instance;\
}

#define Resources(Path) (wstring(L"../02_Resources/").append(Path))
#define ResourcesObject(Path) (wstring(L"../02_Resources/MapObject/").append(Path))
//금술
#define Synthesize(VarType,VarName,FuncName)\
protected: VarType VarName;\
public: inline VarType Get##FuncName(void)const{return VarName;}\
public: inline void Set##FuncName(VarType value){VarName = value;}

//라디안
#define radion(angle) angle/180.f * PI
//디그리
#define degree(angle) angle/PI *180.f