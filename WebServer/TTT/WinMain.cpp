#include <windows.h>
#include <stdio.h>
//CALLBACK:o����y2?��??��??�̡¨�?��?��?��?����2������?�̨�3�̡¨�?��?
LRESULT CALLBACK MyWndProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
);
//WindowProc����???��WinMain??o����?��?��??��WinMain?D����??���?������������a��?��??����?????��?D??��WinMain???�㨦��?��WindowProc


//1 ��??��o����y
int WINAPI WinMain(
  HINSTANCE hInstance,      // ���̨�y??����
  HINSTANCE hPrevInstance,  //?�㨰???���̨�y
  LPSTR lpCmdLine,          // ?����?DD2?��y
  int nCmdShow              // ?�¡��?����???��?��?��?
)
{
	char str[255];
	sprintf(str,"%d", nCmdShow);
	MessageBox(NULL,str,"���?�����䨬?",0);

//2 ���?�����?��
//2.1 ����??���?������
	WNDCLASS MyWnd;
	//��?D????��11��?????��?D???��?��?��??�䨺12?D����a��2��a��?0?��null��?��?��?2??��?y������????����?

	MyWnd.cbClsExtra = NULL;
	MyWnd.cbWndExtra = NULL;

	MyWnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//MyWnd.hbrBackground = (HBRUSH)COLOR_HIGHLIGHT; //��3?��

	MyWnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	//1a����
	MyWnd.hIcon = LoadIcon(NULL, IDI_QUESTION );
	//��?����

	MyWnd.hInstance = hInstance;
	//�̡�?�㨮|��?3��D����?���̨�y??����

	MyWnd.lpfnWndProc = MyWndProc;
	//��?MyWndProc?��������|����???��

	MyWnd.lpszClassName = "Hello";
	//?��?�̨�3����2����?����??

	MyWnd.lpszMenuName = NULL;
	//2?�̣���?ID
	MyWnd.style = CS_HREDRAW | CS_VREDRAW;
	//style ���?��������?��??�¡�?2?��?���?����?��a1?��??��
	//		���?��?????����1?���䨮D?���騦����??��������??����?���?����??a?TD��


//2.2 ??����??o?��?���?������??DD����2��
    RegisterClass(&MyWnd);

//2.3 ���?�����?��
	HWND hWnd;
	hWnd = CreateWindow("Hello",//����??��?��?��???lpszClassName��???
						"Windows����3��",//���?��������a??��?
						WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, //���?����a1??����?  //CW_USEDEFAULT
						0,0,//?��??????���?����?����
						800,600,//���?���䨮D?
						NULL,//??���?��
						NULL,//2?�̣�
						hInstance,
						"??��-�����?Windows����3��!"
						);

	 
//2.4 ??��??��???��D?���?��
	ShowWindow(hWnd, SW_SHOW); 
	UpdateWindow(hWnd);


//3.???��?-?��
//��?3��D��2??��?2???����?
	MSG msg;
	while (GetMessage(&msg,NULL,0,0)) //�䨮???��?����D?D??��????��
	{
		TranslateMessage(&msg);    //???��?a����
    //����?��???��
		DispatchMessage(&msg);     //?????�顤��?����?MyWndProco����y
	}  

	return 0; 

} 

//4.?����?��1y3��o����y?���ꡧ����D��???��?����|�䨲??��?
LRESULT CALLBACK MyWndProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
)
{
	switch (uMsg)
	{
	case WM_CREATE:
		//2��?��CreateWindowo����y��?��?o����???2?��y
		MessageBox(hwnd,(char*)(((LPCREATESTRUCT)lParam)->lpCreateParams),"???�����?��",0);
		return 0;  //����???��2??����?-1��?��??��CreateWindowo����y����??0

	case WM_PAINT:
		//??��?????����?�����?����?��?��?3?helloworld

		HDC hpaintDC;
		PAINTSTRUCT ps;
		hpaintDC=BeginPaint(hwnd,&ps);
		TextOut(hpaintDC,255,50,"Hello World!",strlen("Hello World!"));
		EndPaint(hwnd,&ps);

/*		HDC hDC;//������??����?����
		hDC = GetDC(hwnd);//??��?���?����?���?����?������??����?����
		TextOut(hDC,255,100,"Hello World!",strlen("Hello World!"));
		ReleaseDC(hwnd,hDC);
		WM_CLOSE?T����?����| cpu??��D?����???
		*/


		break;

	case WM_CHAR:
		char str[255];
		sprintf(str,"char is %d", wParam);
		MessageBox(hwnd,str,"���?��?����|",0);
		break;

	case WM_LBUTTONDOWN:
		//MessageBox(hwnd,"������������?�����??","��������?����|",0);
		HDC hDC;//������??����?����
		hDC = GetDC(hwnd);//??��?���?����?���?����?������??����?����
		TextOut(hDC,255,100,"Hello World!",strlen("Hello World!"));
		ReleaseDC(hwnd,hDC);
		break;

	case WM_CLOSE:
		if (IDYES == MessageBox(hwnd,"??��?��a��?3??e��?","��?3?", MB_YESNO))
			DestroyWindow(hwnd);
		break;

	
	case WM_DESTROY:
	//?��2a��?WM_DESTROY(���?��1?��?)�̡¨�?PostQuitMessage2??��1?��?3��D��
			PostQuitMessage(0);
		break;

	case WM_RBUTTONDOWN:
		//PostMessage(hwnd,WM_SETTEXT,0,(LPARAM)"Hello!");
		//SendMessage(FindWindow(NULL,"?T������a - ??��?��?"),WM_SETTEXT,0,(LPARAM)"Hello!");	
		SendMessage(FindWindow("Notepad",NULL),WM_SETTEXT,0,(LPARAM)"Hello!");


		//SendMessage(hwnd,WM_SETTEXT,0,(LPARAM)"Hello!");//??��?���?��������a��???��?
		//FindWindow(?��?Y������???��?,?��?Y������a��?��???��?)2��?�����?��

		//SendMessage?????��?��?������?��??���?����?2��̡¨�???���?��1y3��??DD��|����?��?�����?��1y3��?????���|����������?o����???o����y2?����???��
		//PostMessageo����y?????�顤??��?���?�����?��???3��?��1?��a��????��?����Do��?�䡤��???��
		break;

	case WM_SETTEXT:
	//	SetWindowText(hwnd, (char*)lParam);
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	}
	
	return 0;
}
