#include <windows.h>
#include <stdio.h>

TCHAR szAppName[] = TEXT("TEST");
char DESTOP_NAME[] = ("MY_DESTOP");
 
ATOM m_HotKeyId1;			//����Alt+F1  ,�л���ԭʼ����
ATOM m_HotKeyId2;			//����Alt+F2  ,�л����������� 
ATOM m_HotKeyId3;			//����Alt+F3  ,�˳����� 
ATOM atom; 
HDESK hDDesk;
 
HWINSTA  hWinStaThisProcess = GetProcessWindowStation();
HDESK hDeskOriignInput = OpenInputDesktop(0,FALSE,GENERIC_ALL);
void OnHotKey(WPARAM wParam,LPARAM lParam)
{
	static 
	LRESULT lRes = 0;
 
	if(wParam == m_HotKeyId1)//�ȼ�1������ 
	{
		// ?????????
		SwitchDesktop(hDeskOriignInput);	//�л���ԭʼ���� 
		//MessageBox(0,TEXT("F6"),0,0);
	}
	else if(wParam == m_HotKeyId2)//�ȼ�2������
	{
		SwitchDesktop(hDDesk);				//�л����������� 
		//MessageBox(0,TEXT("F7"),0,0);
	}
	else if(wParam == m_HotKeyId3)//�ȼ�3������
	{
		SwitchDesktop(hDeskOriignInput);
		GlobalDeleteAtom(atom);
		MessageBox(0,"�Ѿ��˳� ��������!","��ʾ",MB_ICONINFORMATION);
		PostQuitMessage(0);			//�˳����� 
		//MessageBox(0,TEXT("F7"),0,0);
	}
	return ;
}
LRESULT	CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		case WM_HOTKEY:
		OnHotKey(wParam,lParam);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
 
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)
{
	if(GlobalFindAtom("��������") != 0){
		::MessageBox(GetForegroundWindow(),"�������� ��������!","����",MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
		return 0;
	}
	atom = GlobalAddAtom("��������");
	HWND hwnd;
	MSG	 msg;
	WNDCLASS	wndclass;
 
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
 
	if(!RegisterClass(&wndclass))
	{
		MessageBox(GetForegroundWindow(),"��������ʧ��!","ERROR",MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(
		szAppName,szAppName,
		WS_OVERLAPPEDWINDOW,
		0,0,
		0,0,
		NULL,NULL,hInstance,NULL
		);
 
 
	hDDesk = CreateDesktop(
		DESTOP_NAME,
		0,0,0,
		GENERIC_ALL,
		NULL
		);
	//????????????? ???????
	SetThreadDesktop(hDDesk);
 
	//??STARTUPINFO ?????,????????????,????lpDesktop?????,
	STARTUPINFO sti = {
		sizeof(sti)
	};
	sti.lpDesktop = DESTOP_NAME;
	PROCESS_INFORMATION pi = {0};
	char szWindowsPath[128], szExpPath[128];
	GetEnvironmentVariable("windir", szWindowsPath, sizeof(szWindowsPath));
	sprintf(szExpPath, "%s\\explorer.exe", szWindowsPath);
	CreateProcess(
		szExpPath,NULL,NULL,NULL,TRUE,0,NULL,NULL,&sti,&pi
		);
 
	/****************************************************************
				�˳����� 
	******************************************************************/
 
	
	m_HotKeyId1 = GlobalAddAtomW(L"HotKey1") - 0xc000;

	RegisterHotKey(hwnd,m_HotKeyId1,MOD_ALT,VK_F1);
 

	m_HotKeyId2 = GlobalAddAtomW(L"HotKey2") - 0xc000;

	RegisterHotKey(hwnd,m_HotKeyId2,MOD_ALT ,VK_F2);
 
	m_HotKeyId3 = GlobalAddAtomW(L"HotKey3") - 0xc000;

	RegisterHotKey(hwnd,m_HotKeyId3,MOD_ALT,VK_F3);
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
 
	UnregisterHotKey(hwnd,m_HotKeyId1);
	GlobalDeleteAtom(m_HotKeyId1);
 
	UnregisterHotKey(hwnd,m_HotKeyId2);
	GlobalDeleteAtom(m_HotKeyId2);
 
	UnregisterHotKey(hwnd,m_HotKeyId3);
	GlobalDeleteAtom(m_HotKeyId3);
 
	CloseHandle(hDDesk);
	return msg.wParam;
}

