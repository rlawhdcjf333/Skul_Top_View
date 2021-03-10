#include "pch.h"
#include "MainGame.h"

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		//�޼���ť�� WM_QUIT��� �޼����� ���� GetMessage�� false�� ��ȯ�ϰ� �����. �� ������ �����Ų��
		PostQuitMessage(0);
		break;
		//���콺 �������� �� ������ �޼���
	case WM_MOUSEMOVE:
		//lparam : ���콺 ��ǥ�� ���� �� �ش� ����(����) �ϳ��� x,y,���� ��� ������.
		//�տ� 32��Ʈ���� y��, �ڿ� 32��Ʈ���� x���� ����(<<,>> ����Ʈ �����ڷ�)
		_mousePosition.x = LOWORD(lParam);
		_mousePosition.y = HIWORD(lParam);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));

}
