#include"semantics.h"
char SrcFilePath[MAX_CHARS];
static FILE *InFile;
static char Name[]="Complier";
static int PrepareWindow(HINSTANCE,HINSTANCE,int);
static int CheckSrcFile(LPSTR);
static void CloseFile();
static LRESULT CALLBACK WindProc(HWND,UINT,WPARAM,LPARAM);
extern FILE *yyin;
extern int yyparse();
int APIENTRY WinMain(HINSTANCE hIstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	MSG Msg;
	strcpy(SrcFilePath,lpCmdLine);
	//在这里填写输入文件的路径。
	strcpy(lpCmdLine,"E:\\data9.in");
	if(PrepareWindow(hIstance,hPrevInstance,nCmdShow)!=1)
	{
		MessageBox(NULL,"error 1","error",MB_OK);
		return 1;
	}
	if(!CheckSrcFile(lpCmdLine)) return 1;
	yyparse();
	CloseFile();
	while(GetMessage(&Msg,NULL,0,0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
 }
 int PrepareWindow(HINSTANCE hInst,HINSTANCE hPrevInstance,int nCmdShow)
 {
 	HWND hWnd;
 	WNDCLASS W;
 	memset(&W,0,sizeof(WNDCLASS));
 	W.style=CS_HREDRAW|CS_VREDRAW;
 	W.lpfnWndProc=WindProc;
 	W.hInstance=hInst;
 	W.hCursor=LoadCursor(NULL,IDC_ARROW);
 	W.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
 	W.lpszClassName=Name;
 	RegisterClass(&W);
 	hWnd=CreateWindow(Name,Name,WS_OVERLAPPEDWINDOW,0,0,1200,2000,NULL,NULL,hInst,NULL);
 	if(hWnd==NULL)return 0;
 	ShowWindow(hWnd,nCmdShow);
 	UpdateWindow(hWnd);
 	SetCursor(LoadCursor(hInst,IDC_ARROW));
 	hDC=GetDC(hWnd);
 	return 1;
 }
 int CheckSrcFile(LPSTR lpszCmdParam)
 {
 	InFile=NULL;
 	if(strlen(lpszCmdParam)==0)
 	{
 		MessageBox(NULL,"error 2","error",MB_OK);
 		return 0;
	 }
	 if((InFile=fopen(lpszCmdParam,"r"))==NULL)
	 {
	 	MessageBox(NULL,"error 3","error",MB_OK);
	 	MessageBox(NULL,lpszCmdParam,"file",MB_OK);
	 	return 0;
	 }
	 yyin=InFile;
	 return 1;
 }
static void  CloseFile(){ if(InFile!=NULL) fclose(InFile); }
LRESULT CALLBACK WindProc(HWND hWnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
    switch(Message)
    {
    case WM_DESTROY:
        {
        ReleaseDC(hWnd,hDC);
        PostQuitMessage(0);
        return 0;
        break;
        }
    case WM_PAINT:
        {
            PAINTSTRUCT pt;
            BeginPaint(hWnd,&pt);
            EndPaint(hWnd,&pt);
        }
    default:return DefWindowProc(hWnd,Message,wParam,lParam);
    }
}

