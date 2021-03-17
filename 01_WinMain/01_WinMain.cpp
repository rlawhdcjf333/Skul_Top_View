//API -> Application Programming Interface 라는 뜻

//미리 컴파일된 헤더 참조
#include "pch.h"
#include "MainGame.h"

HINSTANCE	_hInstance;		//APP 고유번호
//HWND : Handler Window
HWND		_hWnd;			//나중에 (그럴일은 없지만) WinAPI창을 여러개 생성했을 때 각 창을 컨트롤할 수 있는 Hanlder가 필요하다. 그게 바료 요 녀석

wstring _className = L"플밍26기";	//유니코드는 아스키코드랑 다르게 2byte씩 처리한다(한글등등 그런 언어들을 처리하기 위해서) 
//wchar라는 녀석을 쓰기 편하게 만든 클래스

POINT _mousePosition;	//마우스 좌표
POINT nonC_mousePosition; //마우스 절대좌표

//NULL : 숫자 0, 없는 메모리 주소를 0으로 표기하자는 개발자들의 약속
MainGame* _mainGame = NULL;

//윈도우 프록시
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	//API 창 띄우는 순서
	//1. 윈도우 구조체 초기화
	//2. 초기화한 윈도우 구조체로 레지스터 등록
	//3. 윈도우창 생성
	//4. 윈도우 창 화면에 보여지게 요청

	_hInstance = hInstance;

	//1. 윈도우 구조체 초기화
	WNDCLASS wndClass;			//윈도우 구조체(윈도우 창의 정보를 여기에 초기화 한다)

	wndClass.cbClsExtra = 0;										//클래스 여분 메모리 설정
	wndClass.cbWndExtra = 0;										//윈도우 여분 메모리 설정
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//윈도우 백그라운드
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//윈도우 커서 모양
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//윈도우 아이콘 모양
	wndClass.hInstance = hInstance;									//윈도우 인스턴스 번호
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						//윈도우 프로시져
	wndClass.lpszClassName = _className.c_str();					//윈도우 클래스 이름
	wndClass.lpszMenuName = NULL;									//윈도우 메뉴 이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//윈도우 창 스타일

	//2. 초기화한 윈도우 구조체 레지스터 등록
	RegisterClass(&wndClass); //윈도우 정보를 등록해준다
	//레지스터는 CPU가 요청을 처리하는 데 필요한 데이터를 일시적으로 저장하는 기억장치...

	//3. 윈도우 창 생성 
	_hWnd = CreateWindow(
		_className.c_str(),
		_className.c_str(),
		WS_OVERLAPPEDWINDOW,		//윈도우 창 스타일
		WINSTARTX,							//윈도우 시작 X좌표
		WINSTARTY,							//윈도우 시작 Y좌표
		WINSIZEX,							//윈도우 창 가로크기
		WINSIZEY,							//윈도우 창 세로크기
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	// {{ 윈도우 영역 재조정~
	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = WINSIZEX;
	windowRect.bottom = WINSIZEY;

	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	SetWindowPos(_hWnd, NULL, WINSTARTX, WINSTARTY,
		windowRect.right - windowRect.left, 
		windowRect.bottom - windowRect.top, SWP_NOZORDER | SWP_NOMOVE);
	// }}

	//4. 윈도우 창 화면에 보여지게 요청
	ShowWindow(_hWnd, cmdShow);	

	//윈도우 메시지 루프
	//GetMessage : 메시지가 올때까지 기다리며 메세지가 생기면 메세지 큐에서 메세지를 가져온다. 
	//PeekMessage : 메세지큐에서 메세지가 있는지 검사하고 있건없건 그냥 메세지를 리턴한다.
	MSG message;	//입력, 출력요청 등등 API에 어떤 요청이 들어올 때 마다 이 구조체에 정보를 담는다.

	/*
	TranslateMessage
	키보드 입력 메시지를 가공하여 프로그램에서 쉽게 쓸 수 있도록 해 준다.
	윈도우즈는 키보드의 어떤 키가 눌러졌다거나 떨어졌을 때 키보드 메시지를 발생시키는데
	이 함수는 키보드의 눌림(WM_KEYDOWN)과 떨어짐(WM_KEYUP)이 연속적으로 발생할 때
	문자가 입력되었다는 메시지(WM_CHAR)를 만드는 역할을 한다.
	예를 들어 A키를 누른 후 다시 A키를 떼면 A문자가 입력되었다는 메시지를 만들어 낸다.

	DispatchMessage
	시스템 메시지 큐에서 꺼낸 메시지를 프로그램의 메시지 처리 함수(WndProc)로 전달한다.
	*/
	HDC hdc = GetDC(_hWnd);

	_mainGame = new MainGame;
	_mainGame->Init();
	Time::GetInstance()->Start();


	//GetMessage : 어떤 메세지가 들어올 떄까지(즉, 입력, 출력등등 어떤 명령이 
	//들어올때까지) 내부적으로 중지를 걸어놓는 녀석

	//while (GetMessage(&message, 0, 0, 0))
	//{
	//	TranslateMessage(&message);		//키보드 입력하는 순간 그 메시지 처리 담당
	//	DispatchMessage(&message);		//실제로 윈도우에 메시지 전달 담당
	//}
	// PeekMessage , GetMessage 차이 조사
	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) == true)
		{
			if (message.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&message);
			DispatchMessage(&message);
		}	
		else
		{
			Time::GetInstance()->Update();
			SoundPlayer::GetInstance()->Update();
			Input::GetInstance()->Update();
			_mainGame->Update();
			_mainGame->Render(hdc);
		}
	}

	//여기까지 왔다면 프로그램이 끝났다는 얘기
	_mainGame->Release();
	delete _mainGame;

	return message.wParam;
}

//Proc함수 : 어떤 메세지가 들어올 때 마다 해당 함수가 호출된다.

//iMessage : 어떤 종류의 메세지인지 UINT로 보냄
//WPARAM(정수) : 메세지에 추가적인 내용을 담아 보낼 때 사용, 예를 들어 보통 키입력에 사용하는데 iMessage가 키가 입력되었다는 메세지가 나오면 WPARAM에는 어떤 키를
//눌렀는지에 대한 정보를 담아 보낸다.
//LPARAM : 보통 마우스 입력(이동)에 사용하는데 iMessage가 마우스가 이동되었다는 메세지를 보내면 LPARAM에 그 위치에 대한 정보를 담아 보낸다.
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	//없는 메모리 주소를 가르키고 있지 않다면 . . . 
	if (_mainGame != NULL)
	{
		return _mainGame->MainProc(hWnd, iMessage, wParam, lParam);
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
