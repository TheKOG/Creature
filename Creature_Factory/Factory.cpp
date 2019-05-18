#include<bits/stdc++.h>
#include<windows.h>
#define FullWidth GetSystemMetrics(SM_CXSCREEN)
#define FullHeight GetSystemMetrics(SM_CYSCREEN)
#define Width 640
#define Height 480
#define EditMenu 28
#define ButtonMenu 297
using namespace std;
HWND Edit,Button;
HDC WinMap,dWinMap;
HBITMAP dbm;
HBRUSH black,white;
HPEN Black,White;
int height,width,Each=10;
bool cell[Width][Height];
void Init(HWND hwnd);
void Paint();
void save();
LRESULT CALLBACK WndProc(HWND hWnd,UINT Message,WPARAM wParam,LPARAM lParam){
	switch(Message){
		case WM_LBUTTONDOWN:{
			int x=LOWORD(lParam),y=HIWORD(lParam);
			if(y<Height){
				cell[x/Each][y/Each]^=1;
			}else{
				if(x>=Width-35)save();
			}
			break;
		}
		case WM_KEYDOWN:{
			switch(wParam){
				case 'Z':{exit(0);}
			}
		}
		case WM_DESTROY:{return PostQuitMessage(0),0;break;}
		default:return DefWindowProc(hWnd, Message, wParam, lParam);
	}
}
signed WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){
	HWND hwnd;WNDCLASSEX wc;MSG msg;
	memset(&wc,0,sizeof(wc));
	wc.cbSize=sizeof(WNDCLASSEX);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)(COLOR_WINDOW-5);
	wc.lpfnWndProc=WndProc;
	wc.hInstance=hInstance;
	wc.lpszClassName="WindowClass";
	wc.hIcon=(HICON)LoadImage(GetModuleHandle(0),"pictures\\Factory.ico",IMAGE_ICON,128,128,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	wc.hIconSm=(HICON)LoadImage(GetModuleHandle(0),"pictures\\Factory.ico",IMAGE_ICON,128,128,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(!RegisterClassEx(&wc))exit(0);
	hwnd=CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"WindowClass","Factory",
		WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		(FullWidth-Width)/2,(FullHeight-Height)/2,
		Width+20,Height+60,
		0,0,hInstance,0
		);
	if(hwnd==NULL)return MessageBox(NULL,"Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK),0;
	Init(hwnd);
	HINSTANCE EditHins;
	Edit=CreateWindow("edit","Untitled.mdl",WS_CHILD|WS_VISIBLE,0,Height,Width-35,20,hwnd,(HMENU)EditMenu,EditHins,NULL);
	Button=CreateWindow("static","save",WS_CHILD|WS_VISIBLE,Width-35,Height,35,20,hwnd,(HMENU)ButtonMenu,EditHins,NULL);
	while(msg.message!=WM_QUIT){
		if(PeekMessage(&msg,0,0,0,PM_REMOVE)){TranslateMessage(&msg),DispatchMessage(&msg);}
		else Paint();
	}
}
void Init(HWND hwnd){
		black=CreateSolidBrush(RGB(0,0,0));
	white=CreateSolidBrush(RGB(255,255,255));
	Black=CreatePen(PS_SOLID,0,RGB(0,0,0));
	White=CreatePen(PS_SOLID,0,RGB(255,255,255));
	WinMap=GetDC(hwnd);
	dWinMap=CreateCompatibleDC(WinMap);
	dbm=CreateCompatibleBitmap(WinMap,Width,Height);
	SelectObject(dWinMap,dbm);
	width=Width/Each;
	height=Height/Each;
}
void Paint(){
	SelectObject(dWinMap,black);
	SelectObject(dWinMap,Black);
	Rectangle(dWinMap,0,0,Width,Height);
	SelectObject(dWinMap,white);
	SelectObject(dWinMap,White);
	for(int x=0;x<Width/Each;x++)
	for(int y=0;y<Height/Each;y++)
		if(cell[x][y])Rectangle(dWinMap,x*Each,y*Each,x*Each+Each,y*Each+Each);
	BitBlt(WinMap,0,0,Width,Height,dWinMap,0,0,SRCCOPY);
}
void save(){
	int l=2147483647,r=0,u=2147483647,d=0;
	for(int x=0;x<width;x++)
	for(int y=0;y<height;y++){
		if(cell[x][y]){
			l=min(l,x),r=max(r,x);
			u=min(u,y),d=max(d,y);
		}
	}
	if(l==2147483647){MessageBox(NULL,"Çë·ÅÖÃ·½¿é!","Error!",MB_OK);return;}
	char FileName[256],str[256];
	SendMessage(Edit,WM_GETTEXT,256,(LPARAM)FileName);
	wsprintf(str,"models\\%s",FileName);
	freopen(str,"w",stdout);
	for(int y=u;y<=d;y++){
		for(int x=l;x<=r;x++)printf("%c",cell[x][y]?'*':' ');
		printf(",");
	}
	printf(".");
	freopen("CON","w",stdout);
	MessageBox(NULL,"±£´æ³É¹¦!","\\(^o^)/",MB_OK);
}
