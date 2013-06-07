/****************************************************************************

	PROGRAM: Horse.c

	PURPOSE: Horse template for Windows applications

	FUNCTIONS:

	WinMain() - calls initialization function, processes message loop
	HorseInit() - initializes window data and registers window
	HorseWndProc() - processes messages
	About() - processes messages for "About" dialog box

****************************************************************************/
#define _WINDOWS 1
#include <string.h>
#include <stdlib.h>

#define  NOMINMAX	1
#include "windows.h"			/* required for all Windows applications */
#include "Horse.h"			/* specific to this program			 */
#include "Horsedat.h"

#if defined(WIN32)
	#define GetInstance(hWnd)	((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE))
#else
	#define GetInstance(hWnd)	((HINSTANCE)GetWindowWord(hWnd, GWW_HINSTANCE))
#endif

	
#define	APPNAME	"Horse"

#define BLACK		RGB(0,0,0)

#define GREEN		RGB(0,255,0)
#define	ORANGE		RGB(0,127,127)
#define BLUE		RGB(0,0,255)
#define RED			RGB(255,0,0)
#define BLUEGREEN	RGB(96,160,0)

/*lint -e534 */

	int	InstNum;
	const DWORD	ColorTable[5]=
	{
				GREEN,
				ORANGE,
				BLUE,
				BLUEGREEN,
				RED
	};

// put Word value in its own data segment, and share it with all instances
// this requires /section:HorseShared,rws in the project link options
#pragma data_seg( "HorseShared" )
	int g_nInstNum=-1;
#pragma data_seg()


static BOOL HorseInit(HANDLE);
static void DrawBitmap(HDC hDC, short xStart, short yStart, HBITMAP hBitmap);
static BOOL FAR PASCAL About(HWND, unsigned, WORD, LONG);
static void DrawHorse(HWND hWnd,int swtch);

int  PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
long FAR PASCAL HorseWndProc(HWND, unsigned, WPARAM, LPARAM);

/****************************************************************************

	FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)

	PURPOSE: calls initialization function, processes message loop

	COMMENTS:

	This will initialize the window class if it is the first time this
	application is run.  It then creates the window, and processes the
	message loop until a PostQuitMessage is received.  It exits the
	application by returning the value passed by the PostQuitMessage.

****************************************************************************/

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
/* 	    	   	    current instance   previous instance     command line   show-window type (open/icon) */
{
	HWND 	hWnd;			 /* window handle		 */
	MSG 	msg;			 /* message			 */
	HMENU 	hlMenu;			 /* local handle to the System menu  */
	int	counter=0;
	char	szSTR[12];
	int	wX,wY;

#if defined(WIN32)
		if (!HorseInit(hInstance))	
			return (0);   	/* Exits if unable to initialize	 */

		InstNum = g_nInstNum;
		g_nInstNum++;
#else
	if (!hPrevInstance)			/* Has application been initialized? */
	{
		if (!HorseInit(hInstance))	
			return (0);   	/* Exits if unable to initialize	 */
		else
			InstNum=-1;
	}
	else
	{
		GetInstanceData(hPrevInstance,(NPSTR) &InstNum,sizeof(InstNum));
	}

#endif
	InstNum++;


	if ( (hPrevInstance == NULL) &&
	     (GetProfileString("Galloping Horse","Position","",szSTR,12)))
	{
		wX = atoi(strtok(szSTR,","));
		wY = atoi(strtok(NULL,","));
	}
	else
	{
		wX = CW_USEDEFAULT;
		wY = CW_USEDEFAULT;
	}


	if (InstNum==0)
		strcpy(szSTR,"Horse");
	else
		wsprintf(szSTR,"Horse %d",InstNum+1);

	hWnd = CreateWindow(APPNAME,  /* window class		 */
	 szSTR,  			/* window name		 */
	(WS_TILED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX),
				  	/* window style		 */
	wX,				  /* x position			 */
	wY,				  /* y position			 */
	135,				  /* width			 */
	70,				  /* height			 */
	NULL,				  /* parent handle		 */
	0,				  /* menu or child ID		 */
	hInstance,			  /* instance			 */
	NULL);				  /* additional info		 */

	if (!hWnd)					  /* Was the window created? */
		return (0);

	hlMenu = GetSystemMenu(hWnd,FALSE);
	ChangeMenu(hlMenu,0, 0, 	   0,	      MF_APPEND);
	AppendMenu(hlMenu,MF_STRING,IDM_ABOUT,"&About...");
	
	ShowWindow(hWnd, nCmdShow);	  /* Shows the window		 */
	UpdateWindow(hWnd);		  /* Sends WM_PAINT message  */

	if (SetTimer(hWnd,1,100,NULL)) 
	{
	   while (GetMessage(&msg,	   	/* message structure			*/  
 		NULL,		   	/* handle of window receiving the message */
 		0,		   	/* lowest message to examine		 */
 		0))
	   {	   	/* highest message to examine		 */
 			TranslateMessage(&msg);	/* Translates virtual key codes		 */
 			DispatchMessage(&msg);	/* Dispatches message to window		 */
 		}
	   KillTimer(hWnd,1);
	}	   
	else
	{
	   while (msg.message!=WM_QUIT)
	   {
			if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else 
			{
				counter++;
				if (counter==10)
				{
					counter=0;
					DrawHorse(hWnd,FALSE);
				}
			}

		}
	}


	return (msg.wParam);	   	/* Returns the value from PostQuitMessage */
}


/****************************************************************************

	FUNCTION: HorseInit(HANDLE)

	PURPOSE: Initializes window data and registers window class

****************************************************************************/

static BOOL HorseInit(HANDLE hInstance)
         	   /* current instance  */
{
	HANDLE 		hMemory;				   /* handle to allocated memory */
	PWNDCLASS	pWndClass;			   /* structure pointer		 */
	BOOL 		bSuccess;				   /* RegisterClass() result	 */

	hMemory   = LocalAlloc(LPTR, sizeof(WNDCLASS));
	pWndClass = (PWNDCLASS) LocalLock(hMemory);

	pWndClass->style          = 0;
	pWndClass->lpfnWndProc    = HorseWndProc;
	pWndClass->hInstance      = hInstance;
	pWndClass->hIcon          = NULL;       //LoadIcon(NULL, IDI_APPLICATION);
	pWndClass->hCursor        = LoadCursor(NULL, IDC_ARROW);
	pWndClass->hbrBackground  = NULL;   	//CreateSolidBrush(BLACK_BRUSH);
	pWndClass->lpszMenuName   = NULL;
	pWndClass->lpszClassName  = (LPSTR) APPNAME;

	bSuccess = RegisterClass(pWndClass);

	LocalUnlock(hMemory);	  		/* Unlocks the memory	*/
	LocalFree(hMemory);	  		/* Returns it to Windows */

	return (bSuccess);		 /* Returns result of registering the window */
}

/****************************************************************************

	FUNCTION: HorseWndProc(HWND, unsigned, WORD, LONG)

	PURPOSE:  Processes messages

	MESSAGES:

	WM_CREATE	- create window
	WM_DESTROY	- destroy window
	WM_COMMAND	- menu selections and others

	COMMENTS:


****************************************************************************/

long FAR PASCAL HorseWndProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
/* 			     window handle  type of message      additional information  */
{
	PAINTSTRUCT	ps;
	FARPROC 	lpProcAbout;
	char		szPosition[24];

	switch (message) {

	    case WM_CREATE:		/* message: window being created */
		break;

	   case WM_SIZE:
		DrawHorse(hWnd,TRUE);
		break;

	    case WM_PAINT:
		BeginPaint(hWnd,&ps);

		DrawHorse(hWnd,FALSE);
		
		EndPaint(hWnd,&ps);
		break;

	    case WM_TIMER:
		DrawHorse(hWnd,FALSE);
		break;

	    case WM_DESTROY:		/* message: window being destroyed */
		PostQuitMessage(0);
		break;

	    case WM_MOVE:
		if (InstNum==0) {
			wsprintf(szPosition,"%d,%d",LOWORD(lParam),
				HIWORD(lParam)-GetSystemMetrics(SM_CYCAPTION));
			WriteProfileString("Galloping Horse","Position",szPosition);
			}
		break;

            case WM_SYSCOMMAND: 
				if (wParam==IDM_ABOUT)
				{
					lpProcAbout = MakeProcInstance(About, GetWindowWord(hWnd,GWW_HINSTANCE));
					DialogBox(GetInstance(hWnd), "AboutBox", hWnd, lpProcAbout);
					FreeProcInstance(lpProcAbout);
				}
		    	else
	                return (DefWindowProc(hWnd, message, wParam, lParam));
				break;

	    	default:			  /* Passes it on if unproccessed	*/

		return (DefWindowProc(hWnd, message, wParam, lParam));
	    }

	return (0L);
}

/*************************************************************************/
/*                                                           DRAWHORSE   */
/*  o   Summary:                                                         */
/*                                                                       */
/*                                                                       */
/*  o   Description:                                                     */
/*                                                                       */
/*                                                                       */
/*  o   Return Value:                                                    */
/*                                                                       */
/*                                                                       */
/*************************************************************************/

void DrawHorse(HWND hWnd, int swtch)
{

	static	int	horsenum=0;
	static	int	iconic=TRUE;

		HBITMAP	hBitmap;
		HDC	hDC;
		HBRUSH	hBrush1;
		RECT	rect;
		HANDLE	hInst;
//		HANDLE  hResource;
		LPCSTR	lpData;

	if (swtch)
		iconic = !iconic;

	hDC=GetDC(hWnd);
	hBrush1=CreateSolidBrush(ColorTable[InstNum % 5]);
	SelectObject(hDC,hBrush1);

	GetClientRect(hWnd,&rect);
	Rectangle(hDC, 0, 0, rect.right,rect.bottom);

	SetTextColor(hDC,ColorTable[InstNum & 3]);

	SetBkColor(hDC,BLACK);
	horsenum++;
	horsenum = horsenum % 5;

	hInst=(HINSTANCE) GetInstance(hWnd);

	lpData=horse[horsenum];

	hBitmap=CreateBitmap(48,29,1,1,lpData);
	if (iconic)
		DrawBitmap(hDC,-7,5,hBitmap);
	else
		DrawBitmap(hDC,40,10,hBitmap);

	ReleaseDC(hWnd,hDC);

	DeleteObject(hBitmap);
	DeleteObject(hBrush1);

}


/*************************************************************************/
/*                                                          DRAWBITMAP   */
/*  o   Summary:                                                         */
/*                                                                       */
/*                                                                       */
/*  o   Description:                                                     */
/*                                                                       */
/*                                                                       */
/*  o   Return Value:                                                    */
/*                                                                       */
/*                                                                       */
/*************************************************************************/

void DrawBitmap(HDC hDC, short xStart, short yStart, HBITMAP hBitmap)
{
	BITMAP	bm;
	HDC	hMemDC;
	POINT	pt;


	hMemDC = CreateCompatibleDC(hDC);
	SelectObject(hMemDC, hBitmap);
	SetMapMode(hMemDC, GetMapMode(hDC));

	GetObject(hBitmap, sizeof(BITMAP), (LPSTR) &bm);

	pt.x = bm.bmWidth;
	pt.y = bm.bmHeight;

	DPtoLP(hDC,&pt,1);

	BitBlt(hDC,xStart,yStart, pt.x, pt.y, hMemDC, 0,0,SRCCOPY);

	DeleteDC(hMemDC);

}
/*************************************************************************/
/*                                                               ABOUT   */
/*  o   Summary:                                                         */
/*                                                                       */
/*                                                                       */
/*  o   Description:                                                     */
/*                                                                       */
/*                                                                       */
/*  o   Return Value:                                                    */
/*                                                                       */
/*                                                                       */
/*************************************************************************/

BOOL FAR PASCAL About(HWND hDlg, unsigned int message, WORD wParam, LONG lParam)
{
    switch (message) {
	case WM_INITDIALOG:
	    return (TRUE);

	case WM_COMMAND:
	    if (wParam == IDOK) {
		EndDialog(hDlg, TRUE);
		return (TRUE);
	    }
	    break;

	default: break;
    }
    return (FALSE);
}


