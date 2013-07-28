#include "PEExplorer.h"
#include "resource.h"

#define FONT_HEIGHT (16)
#define HALF_FONT_HEIGHT (10)
#define STRING_LEN (190)

HINSTANCE	m_hInstance;
HWND		m_hWnd;
char		m_szWndClassName[]="PEExplorerWnd";
char		m_szTitle[]="PEExplorer";
char        msg_INFO[]="请将EXE文件拖拽到本窗体";
char        msg_EXE[3][256]={"刚才的EXE文件位数未知","刚才的EXE文件为32位","刚才的文件EXE为64位"};

//===================================================================================================================================================================================
LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	//static HWND hButton=NULL;
	MINMAXINFO *MinMaxinfo=NULL;
	HDROP hDrop=NULL;
	static PEExplorer myATSPeExplorer;
	//int wmId,wmEvent;
	static int exeBit=0;

	RECT rect;
	HDC dc=NULL;
	PAINTSTRUCT ps;
	GetClientRect(hWnd, &rect);

	RECT lbl1;
	lbl1.left = (rect.right-STRING_LEN)/2;
	lbl1.top = (rect.bottom-FONT_HEIGHT)/2-HALF_FONT_HEIGHT;
	lbl1.right = (rect.right-STRING_LEN)/2+STRING_LEN;
	lbl1.bottom = (rect.bottom-FONT_HEIGHT)/2-HALF_FONT_HEIGHT+FONT_HEIGHT;

	RECT lbl2;
	lbl2.left = (rect.right-STRING_LEN)/2;
	lbl2.top = (rect.bottom-FONT_HEIGHT)/2+HALF_FONT_HEIGHT;
	lbl2.right = (rect.right-STRING_LEN)/2+STRING_LEN;
	lbl2.bottom = (rect.bottom-FONT_HEIGHT)/2+HALF_FONT_HEIGHT+FONT_HEIGHT;

	switch(message)
	{
	case WM_CREATE:
		DragAcceptFiles(hWnd,true);
		//hButton=CreateWindow("BUTTON","HI",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_VCENTER|BS_CENTER,0,0,100,100,hWnd,(HMENU)IDC_OPNE_DCM,m_hInstance,(LPCREATESTRUCT)lParam);
		break;

	case WM_COMMAND:
		//wmId=LOWORD(wParam);
		//wmEvent=HIWORD(wParam);

		//switch(wmId)
		//{
		//case IDC_OPNE_DCM:
		//	myATSDump.DumpImage();
		//	myATSDump.Dump2XML();
		//	break;
		//default:
		//	return DefWindowProc(hWnd,message,wParam,lParam);
		//	break;
		//}
		return DefWindowProc(hWnd,message,wParam,lParam);
		break;

	case WM_PAINT:
		dc = BeginPaint(hWnd, &ps);
		::DrawText(dc,msg_INFO,-1,&lbl1,DT_LEFT);
		::DrawText(dc,msg_EXE[exeBit],-1,&lbl2,DT_LEFT);
		EndPaint(hWnd, &ps);
		break;

	case WM_GETMINMAXINFO:
		MinMaxinfo=(MINMAXINFO *)lParam;
		if(NULL!=MinMaxinfo)
		{
			MinMaxinfo->ptMinTrackSize.x=210;
			MinMaxinfo->ptMinTrackSize.y=120;
			MinMaxinfo->ptMaxTrackSize.x=210;
			MinMaxinfo->ptMaxTrackSize.y=120;
		}
		break;

	case WM_DROPFILES:
		hDrop=(HDROP)wParam;
		if(hDrop!=NULL)
		{
			int nNum=DragQueryFile(hDrop,0xFFFFFFFF,NULL,0);
			char cExe[MAX_PATH];
			for(int i=0;i<nNum;i++)
			{
				int nSize=DragQueryFile(hDrop,i,cExe,MAX_PATH);
				if(nSize>0)
				{
					int nRet=myATSPeExplorer.GetExeBit(cExe);
					if(nRet==myATSPeExplorer.EXE_BIT_32)
					{
						exeBit=1;
					}
					else if(nRet==myATSPeExplorer.EXE_BIT_64)
					{
						exeBit=2;
					}
					else
					{
						exeBit=0;
					}
				}
			}
			DragFinish(hDrop);
			InvalidateRect(hWnd,NULL,TRUE);
			UpdateWindow(hWnd);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd,message,wParam,lParam);
		break;
	}

	return 0;
}

//===================================================================================================================================================================================
BOOL InitApplication(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.style		=CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		=(WNDPROC)WndProc;
	wc.cbClsExtra		=0;
	wc.cbWndExtra		=0;
	wc.hInstance		=hInstance;
	wc.hIcon		=LoadIcon(hInstance,MAKEINTRESOURCE(MAIN_ICON));
	wc.hCursor		=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground	=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		="";
	wc.lpszClassName	=m_szWndClassName;

	return RegisterClass(&wc);
}

//===================================================================================================================================================================================
BOOL InitInstance(HINSTANCE hInstance,int nCmdShow)
{
	m_hInstance=hInstance;
	m_hWnd=CreateWindow(m_szWndClassName,m_szTitle,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInstance,NULL);
	if(NULL==m_hWnd)return FALSE;

	ShowWindow(m_hWnd,nCmdShow);
	UpdateWindow(m_hWnd);

	return TRUE;
}

//===================================================================================================================================================================================
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER(lpCmdLine);

	if(!hPrevInstance)
	{
		if(!InitApplication(hInstance))
		{
			return FALSE;
		}
	}

	if(!InitInstance(hInstance,nCmdShow))
	{
		return FALSE;
	}

	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

//===================================================================================================================================================================================
