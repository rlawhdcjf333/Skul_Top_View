#pragma once
#define WINSTARTX 50
#define WINSTARTY 50
#define WINSIZEX 1280
#define WINSIZEY 720

#define TileSizeX 60
#define TileSizeY 30
#define StartX 610
#define StartY 100

//���ڷ� ���� p�� �����ϰ� NULL�� �ʱ�ȭ �ϴ� ��ũ�� �Լ�, �����ϰ� ����
#define SafeDelete(p)\
{\
	if(p)\
	{\
		delete (p);\
		(p) = NULL;\
	}\
}

//�̱��� ��ũ��
#define Singleton(ClassName)\
public:\
static ClassName* GetInstance()\
{\
	static ClassName instance;\
	return &instance;\
}

#define Resources(Path) (wstring(L"../02_Resources/").append(Path))
#define ResourcesObject(Path) (wstring(L"../02_Resources/MapObject/").append(Path))
//�ݼ�
#define Synthesize(VarType,VarName,FuncName)\
protected: VarType VarName;\
public: inline VarType Get##FuncName(void)const{return VarName;}\
public: inline void Set##FuncName(VarType value){VarName = value;}

//����
#define radion(angle) angle/180.f * PI
//��׸�
#define degree(angle) angle/PI *180.f