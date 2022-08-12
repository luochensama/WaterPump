#include <windows.h>
#include <stdio.h>
//CALLBACK:o¡¥¨ºy2?¨º??¨°??¦Ì¡Â¨®?¦Ì?¡ê?¨º?¨®¨¦2¨´¡Á¡Â?¦Ì¨ª3¦Ì¡Â¨®?¦Ì?
LRESULT CALLBACK MyWndProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
);
//WindowProc¨º¦Ì???¨²WinMain??o¨®¡ê?¦Ì?¨º??¨²WinMain?D¨¦¨¨??¡ä¡ã?¨²¨¤¨¤¨º¡À¨°a¨®?¦Ì??¨¹¦Ì?????¡ê?D??¨²WinMain???¡ã¨¦¨´?¡ÂWindowProc


//1 ¨¨??¨²o¡¥¨ºy
int WINAPI WinMain(
  HINSTANCE hInstance,      // ¨º¦Ì¨¤y??¡À¨²
  HINSTANCE hPrevInstance,  //?¡ã¨°???¨º¦Ì¨¤y
  LPSTR lpCmdLine,          // ?¨¹¨¢?DD2?¨ºy
  int nCmdShow              // ?¡Â¡ä¡ã?¨²¦Ì???¨º?¡¤?¨º?
)
{
	char str[255];
	sprintf(str,"%d", nCmdShow);
	MessageBox(NULL,str,"¡ä¡ã?¨²¡Á¡ä¨¬?",0);

//2 ¡ä¡ä?¡§¡ä¡ã?¨²
//2.1 ¨¦¨¨??¡ä¡ã?¨²¨¤¨¤
	WNDCLASS MyWnd;
	//¡À?D????¨¢11¨¬?????¨º?D???¨¬?¨¦?¡ê??¡ä¨º12?D¨¨¨°a¨°2¨°a¨®?0?¨°null¨¬?¨¦?¡ê?2??¨¹?y¨¨¡¤¡¤????¨²¡ä?

	MyWnd.cbClsExtra = NULL;
	MyWnd.cbWndExtra = NULL;

	MyWnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//MyWnd.hbrBackground = (HBRUSH)COLOR_HIGHLIGHT; //¡À3?¡ã

	MyWnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	//1a¡À¨º
	MyWnd.hIcon = LoadIcon(NULL, IDI_QUESTION );
	//¨ª?¡À¨º

	MyWnd.hInstance = hInstance;
	//¦Ì¡À?¡ã¨®|¨®?3¨¬D¨°¦Ì?¨º¦Ì¨¤y??¡À¨²

	MyWnd.lpfnWndProc = MyWndProc;
	//¨®?MyWndProc?¨®¨º¨¹¡ä|¨¤¨ª???¡é

	MyWnd.lpszClassName = "Hello";
	//?¨°?¦Ì¨ª3¡Á¡é2¨¢¦Ì?¨¤¨¤??

	MyWnd.lpszMenuName = NULL;
	//2?¦Ì£¤¦Ì?ID
	MyWnd.style = CS_HREDRAW | CS_VREDRAW;
	//style ¡ä¡ã?¨²¨¤¨¤¦Ì?¨¬??¡Â¡ê?2?¨º?¡ä¡ã?¨²¦Ì?¨ªa1?¨¬??¡Â
	//		¡ä¡ã?¨²?????¨°¡ä1?¡À¡ä¨®D?¡¤¡é¨¦¨²¡À??¡¥¨º¡À¡ê??¨¢¨¨?¡ä¡ã?¨²¡À??a?TD¡ì


//2.2 ??¨¦¨¨??o?¦Ì?¡ä¡ã?¨²¨¤¨¤??DD¡Á¡é2¨¢
    RegisterClass(&MyWnd);

//2.3 ¡ä¡ä?¡§¡ä¡ã?¨²
	HWND hWnd;
	hWnd = CreateWindow("Hello",//¨¤¨¤??¡ê?¨®?¨¦???lpszClassName¨°???
						"Windows¡À¨¤3¨¬",//¡ä¡ã?¨²¡À¨º¨¬a??¡Á?
						WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, //¡ä¡ã?¨²¨ªa1??¨´¨º?  //CW_USEDEFAULT
						0,0,//?¨¤??????¡ä¡ã?¨²¡Á?¡À¨º
						800,600,//¡ä¡ã?¨²¡ä¨®D?
						NULL,//??¡ä¡ã?¨²
						NULL,//2?¦Ì£¤
						hInstance,
						"??¨®-¨¤¡ä¦Ì?Windows¡À¨¤3¨¬!"
						);

	 
//2.4 ??¨º??¡ã???¨¹D?¡ä¡ã?¨²
	ShowWindow(hWnd, SW_SHOW); 
	UpdateWindow(hWnd);


//3.???¡é?-?¡¤
//¨¨?3¨¬D¨°2??¨¢?2???¨¢¨º?
	MSG msg;
	while (GetMessage(&msg,NULL,0,0)) //¡ä¨®???¡é?¨®¨¢D?D??¨¨????¡é
	{
		TranslateMessage(&msg);    //???¡é?a¨º¨ª
    //¡¤¡é?¨ª???¡é
		DispatchMessage(&msg);     //?????¡é¡¤¡é?¨ª¦Ì?MyWndProco¡¥¨ºy
	}  

	return 0; 

} 

//4.?¡ã¡ä¡ã?¨²1y3¨¬o¡¥¨ºy?¡À¡ê¡§¡À¨¤D¡ä???¡é?¨¬¨®|¡ä¨²??¡ê?
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
		//2¨¦?¡äCreateWindowo¡¥¨ºy¦Ì?¡Á?o¨®¨°???2?¨ºy
		MessageBox(hwnd,(char*)(((LPCREATESTRUCT)lParam)->lpCreateParams),"???¡¥¡ä¡ã?¨²",0);
		return 0;  //¡¤¦Ì???¦Ì2??¨¹¨º?-1¡ê?¡¤??¨°CreateWindowo¡¥¨ºy¡¤¦Ì??0

	case WM_PAINT:
		//??¡ä?????¨º¡À?¨²¡ä¡ã?¨²¨¦?¡¤?¨º?3?helloworld

		HDC hpaintDC;
		PAINTSTRUCT ps;
		hpaintDC=BeginPaint(hwnd,&ps);
		TextOut(hpaintDC,255,50,"Hello World!",strlen("Hello World!"));
		EndPaint(hwnd,&ps);

/*		HDC hDC;//¨¦¨¨¡À??¨¨¨º?¡À¨ª
		hDC = GetDC(hwnd);//??¦Ì?¡ä¡ä?¡§¦Ì?¡ä¡ã?¨²¦Ì?¨¦¨¨¡À??¨¨¨º?¡À¨ª
		TextOut(hDC,255,100,"Hello World!",strlen("Hello World!"));
		ReleaseDC(hwnd,hDC);
		WM_CLOSE?T¡¤¡§?¨¬¨®| cpu??¨®D?¨º¡À???
		*/


		break;

	case WM_CHAR:
		char str[255];
		sprintf(str,"char is %d", wParam);
		MessageBox(hwnd,str,"¡ã¡ä?¨¹?¨¬¨®|",0);
		break;

	case WM_LBUTTONDOWN:
		//MessageBox(hwnd,"¨º¨®¡À¨º¡Á¨®?¨¹¡ã¡ä??","¨º¨®¡À¨º?¨¬¨®|",0);
		HDC hDC;//¨¦¨¨¡À??¨¨¨º?¡À¨ª
		hDC = GetDC(hwnd);//??¦Ì?¡ä¡ä?¡§¦Ì?¡ä¡ã?¨²¦Ì?¨¦¨¨¡À??¨¨¨º?¡À¨ª
		TextOut(hDC,255,100,"Hello World!",strlen("Hello World!"));
		ReleaseDC(hwnd,hDC);
		break;

	case WM_CLOSE:
		if (IDYES == MessageBox(hwnd,"??¦Ì?¨°a¨ª?3??e¡ê?","¨ª?3?", MB_YESNO))
			DestroyWindow(hwnd);
		break;

	
	case WM_DESTROY:
	//?¨¬2a¦Ì?WM_DESTROY(¡ä¡ã?¨²1?¡À?)¦Ì¡Â¨®?PostQuitMessage2??¨¹1?¦Ì?3¨¬D¨°
			PostQuitMessage(0);
		break;

	case WM_RBUTTONDOWN:
		//PostMessage(hwnd,WM_SETTEXT,0,(LPARAM)"Hello!");
		//SendMessage(FindWindow(NULL,"?T¡À¨º¨¬a - ??¨º?¡À?"),WM_SETTEXT,0,(LPARAM)"Hello!");	
		SendMessage(FindWindow("Notepad",NULL),WM_SETTEXT,0,(LPARAM)"Hello!");


		//SendMessage(hwnd,WM_SETTEXT,0,(LPARAM)"Hello!");//??¡À?¡ä¡ã?¨²¡À¨º¨¬a¨¤???¡Á?
		//FindWindow(?¨´?Y¨¤¨¤¦Ì???¡Á?,?¨´?Y¡À¨º¨¬a¨¤?¦Ì???¡Á?)2¨¦?¨°¡ä¡ã?¨²

		//SendMessage?????¡é?¡À?¨®¡¤¡é?¨ª??¡ä¡ã?¨²¡ê?2¡é¦Ì¡Â¨®???¡ä¡ã?¨²1y3¨¬??DD¡ä|¨¤¨ª?¡ê?¨²¡ä¡ã?¨²1y3¨¬?????¡é¡ä|¨¤¨ª¨ª¨º¡À?o¨®¡ê???o¡¥¨ºy2?¡¤¦Ì???¡ê
		//PostMessageo¡¥¨ºy?????¡é¡¤??¨®?¡ä¡ä?¡§¡ä¡ã?¦Ì???3¨¬?¨¤1?¨¢a¦Ì????¡é?¨®¨¢Do¨®?¡ä¡¤¦Ì???¡ê
		break;

	case WM_SETTEXT:
	//	SetWindowText(hwnd, (char*)lParam);
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	}
	
	return 0;
}
