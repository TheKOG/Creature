#include<bits/stdc++.h>
#include<windows.h>
#define FullWidth GetSystemMetrics(SM_CXSCREEN)
#define FullHeight GetSystemMetrics(SM_CYSCREEN)
#define Width 1000
#define Height 600
#define CmdMenu 28
#define EditMenu 297
#define ButtonMenu 29728
#define CmdWidth 150
#define EditHeight 20
using namespace std;
HWND Edit,Static;
HDC WinMap,dWinMap,CmdMap;
HBITMAP dbm,dbm2;
HBRUSH black,red,green,blue,white,grey,dark;
HPEN Black,White,Green,Red,Blue;
POINT LU;
int height,width,Each=2,Choose,SetModel,CmdUp,wait,start;
bool cell[Width][Height],cell2[Width][Height];
bool stop,up,dn,lf,rt;
struct Model{
	int cx,cy,mp[64][48];
	char FileName[128];
	Model(char *str){memcpy(FileName,str,(strlen(str)+1));memset(mp,0,sizeof(mp)),cx=cy=0;}
};
vector<Model>models;
void Init(HWND hwnd);
void Paint(HWND hwnd);
void Run(HWND hwnd);
int Get(int x,int y);
void EnumFiles(HWND hwnd);
int GetWait(HWND hwnd);
LRESULT CALLBACK WndProc(HWND hWnd,UINT Message,WPARAM wParam,LPARAM lParam){
	switch(Message){
		case WM_LBUTTONDOWN:{
			int x=LOWORD(lParam),y=HIWORD(lParam);
			if(x<=Width){
				SetModel=lParam;
			}else{
				Choose=(y+CmdUp)/CmdWidth;
			}
			break;
		}
		case WM_RBUTTONDOWN:{
			memset(cell,0,sizeof cell);
			break;
		}
		case WM_MOUSEWHEEL:{
			RECT rct;POINT pos;
			GetWindowRect(hWnd,&rct);
			GetCursorPos(&pos);
			int x=pos.x-rct.left,y=pos.y-rct.top;
			short dis=HIWORD(wParam);
			if(x<=Width){
				int pre=Each;
				Each+=dis/120;
				Each=min(50,Each);
				Each=max(2,Each);
			}else{
				CmdUp-=dis/6;
				CmdUp=max(CmdUp,0);
			}
			break;
		}
		case WM_KEYDOWN:{
			switch(wParam){
				case VK_SPACE:stop^=1;break;
				case 'W':{up=true;break;}
				case 'S':dn=true;break;
				case 'A':lf=true;break;
				case 'D':rt=true;break;
				case 'Z':exit(0);
				break;
			}
			break;
		}
		case WM_KEYUP:{
			switch(wParam){
				case 'W':up=false;break;
				case 'S':dn=false;break;
				case 'A':lf=false;break;
				case 'D':rt=false;break;
				break;
			}
			break;
		}
		case WM_DESTROY:{return PostQuitMessage(0),0;break;}
		default:return DefWindowProc(hWnd, Message, wParam, lParam);
	}
}
DWORD WINAPI Main(LPVOID lParam){
	HWND hwnd=*(HWND*)lParam;
	while(1){
		EnumFiles(hwnd);
		SetBkMode(CmdMap,TRANSPARENT);
		SetTextColor(CmdMap,RGB(255,255,255));
		SelectObject(CmdMap,Green);
		SelectObject(CmdMap,grey);
		Rectangle(CmdMap,0,0,CmdWidth,Height);
		int ny=-CmdUp;
		for(int i=0;i<models.size();i++){
			if(ny>Height)break;
			if(ny+CmdWidth<0){ny+=CmdWidth;continue;}
			SelectObject(CmdMap,black);
			SelectObject(CmdMap,Choose==i?Red:Green);
			Rectangle(CmdMap,0,ny,CmdWidth,ny+CmdWidth);
			SelectObject(CmdMap,white);
			Model mdl=models[i];
			RECT rct={5,ny+5,CmdWidth-5,ny+CmdWidth-5};
			DrawText(CmdMap,mdl.FileName,-1,&rct,DT_SINGLELINE|DT_BOTTOM|DT_RIGHT);
			int each=max(2,(CmdWidth-40)/max(1,max(mdl.cx,mdl.cy)));
			SelectObject(CmdMap,each>=5?Black:White);
			int fx=(CmdWidth-mdl.cx*each)/2,fy=ny+(CmdWidth-mdl.cy*each)/2;
			for(int Y=0;Y<mdl.cy;Y++)
			for(int X=0;X<mdl.cx;X++){
				if(!mdl.mp[X][Y])continue;
				int px=fx+X*each,py=fy+Y*each;
				Rectangle(CmdMap,px,py,px+each,py+each);
			}
			ny+=CmdWidth+4;
		}
		if(ny<=Height){
			SelectObject(CmdMap,dark);
			SelectObject(CmdMap,Blue);
			Rectangle(CmdMap,0,ny,CmdWidth,ny+CmdWidth);
			RECT rct={5,ny+5,CmdWidth-5,ny+CmdWidth-5};
			DrawText(CmdMap,"×Ô¶¨Òå",-1,&rct,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		}
		BitBlt(WinMap,Width,0,CmdWidth,Height-EditHeight,CmdMap,0,0,SRCCOPY);
		while(SetModel){
			int X=(LOWORD(SetModel)+LU.x)/Each,Y=(HIWORD(SetModel)+LU.y)/Each;
			SetModel=0;
			if(Choose>=models.size())break;
			Model mdl=models[Choose];
			for(int x=0;x<mdl.cx;x++)
			for(int y=0;y<mdl.cy;y++){
				int nx=X+x,ny=Y+y;
				if(nx>=width)nx-=width;
				if(ny>=height)ny-=height;
				cell[nx][ny]=mdl.mp[x][y]?1:0;
			}
		}
		if(Choose==models.size()){
			PROCESS_INFORMATION pi;
			STARTUPINFOA si={sizeof(pi)};
			si.wShowWindow=true;
			si.dwFlags=STARTF_USESHOWWINDOW;
			if(!CreateProcess(NULL,"Factory.exe",NULL,NULL,false,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi))
				MessageBox(NULL,"äÆ! \"Factory.exe\"²»¼ûÁË!","QAQ",MB_OK);
			Choose=19260817;
		}
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
	wc.hIcon=(HICON)LoadImage(GetModuleHandle(0),"pictures\\Creature.ico",IMAGE_ICON,128,128,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	wc.hIconSm=(HICON)LoadImage(GetModuleHandle(0),"pictures\\Creature.ico",IMAGE_ICON,128,128,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(!RegisterClassEx(&wc))exit(0);
	hwnd=CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"WindowClass","Creature",
		WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		(FullWidth-Width-CmdWidth-20)/2,(FullHeight-Height)/2,
		Width+CmdWidth+20,Height+40,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	if(hwnd==NULL)return MessageBox(NULL,"Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK),0;
	HINSTANCE Hins;
	Edit=CreateWindow("edit","0",WS_CHILD|WS_VISIBLE,Width+100,Height-EditHeight,CmdWidth-100,EditHeight,hwnd,(HMENU)EditMenu,Hins,NULL);
	Static=CreateWindow("static","¼ä¸ôÊ±¼ä(ms)",WS_CHILD|WS_VISIBLE,Width,Height-EditHeight,100,EditHeight,hwnd,(HMENU)ButtonMenu,Hins,NULL);
	Init(hwnd);
	HANDLE hThread=CreateThread(0,0,Main,&hwnd,0,0);
	while(msg.message!=WM_QUIT){
		if(PeekMessage(&msg,0,0,0,PM_REMOVE)){TranslateMessage(&msg),DispatchMessage(&msg);}
		else{
			Run(hwnd);
			Paint(hwnd);
		}
	}
}
void Init(HWND hwnd){
    black=CreateSolidBrush(RGB(0,0,0));
	red=CreateSolidBrush(RGB(255,0,0));
	green=CreateSolidBrush(RGB(0,255,0));
	blue=CreateSolidBrush(RGB(0,0,255));
	white=CreateSolidBrush(RGB(255,255,255));
	grey=CreateSolidBrush(RGB(100,100,100));
	dark=CreateSolidBrush(RGB(50,50,50));
	Black=CreatePen(PS_SOLID,0,RGB(0,0,0));
	White=CreatePen(PS_SOLID,0,RGB(255,255,255));
	Green=CreatePen(PS_SOLID,5,RGB(0,255,0));
	Red=CreatePen(PS_SOLID,5,RGB(255,0,0));
	Blue=CreatePen(PS_SOLID,5,RGB(0,0,255));
	WinMap=GetDC(hwnd);
	dWinMap=CreateCompatibleDC(WinMap);
	CmdMap=CreateCompatibleDC(WinMap);
	dbm=CreateCompatibleBitmap(WinMap,Width,Height);
	dbm2=CreateCompatibleBitmap(WinMap,CmdWidth,Height);
	SelectObject(dWinMap,dbm);
	SelectObject(CmdMap,dbm2);
	SelectObject(dWinMap,black);
	width=Width/Each;
	height=Height/Each;
	LU.x=LU.y=0;
}
void Paint(HWND hwnd){
	SelectObject(dWinMap,black);
	SelectObject(dWinMap,Black);
	Rectangle(dWinMap,0,0,Width,Height);
	SelectObject(dWinMap,white);
	SelectObject(dWinMap,White);
	for(int x=LU.x/Each;x<(LU.x+Width)/Each;x++)
	for(int y=LU.y/Each;y<(LU.y+Height)/Each;y++){
		if(cell[x][y]){
			Rectangle(dWinMap,x*Each-LU.x,y*Each-LU.y,x*Each+Each-LU.x,y*Each+Each-LU.y);
		}
	}
	BitBlt(WinMap,0,0,Width,Height,dWinMap,0,0,SRCCOPY);
}
void Run(HWND hwnd){
	if(up)LU.y-=Height/100;
	if(dn)LU.y+=Height/100;
	if(lf)LU.x-=Width/100;
	if(rt)LU.x+=Width/100;
	LU.y=max(LU.y,(long)0);
	LU.y=min(LU.y,(long)max(height*Each-Height,0));
	LU.x=max(LU.x,(long)0);
	LU.x=min(LU.x,(long)max(width*Each-Width,0));
	int wt=GetWait(hwnd);
	if(clock()>=start+wt)start+=wt;
	else return;
	if(stop||SetModel)return; 
	for(int x=0;x<width;x++)
	for(int y=0;y<height;y++){
		int num=Get(x,y);
		cell2[x][y]=false;
		if(num==2)cell2[x][y]=cell[x][y];
		if(num==3)cell2[x][y]=true;
	}
	for(int x=0;x<width;x++)
	for(int y=0;y<height;y++)
		cell[x][y]=cell2[x][y];
}
int Get(int x,int y){
	int re=0,a,b,c,d;
	a=x-1<0?width-1:x-1;
	b=x+1>=width?0:x+1;
	c=y-1<0?height-1:y-1;
	d=y+1>=height?0:y+1;
	re+=cell[a][c]+cell[a][y]+cell[a][d];
	re+=cell[x][c]+cell[x][d];
	re+=cell[b][c]+cell[b][y]+cell[b][d];
	return re;
}
void EnumFiles(HWND hwnd){
	models.clear();
	HANDLE a;WIN32_FIND_DATA b;
	a=FindFirstFile("models\\*.mdl",&b);
	do{
		if(a==INVALID_HANDLE_VALUE)break;
		char str[128];
		wsprintf(str,"models\\%s",b.cFileName);
		Model NewModel=Model(b.cFileName);
		ifstream input(str,ios::in|ios::binary);
		char ch;int nx=0;
		while(input.read(&ch,1)){
			if(ch==','){NewModel.cx=max(NewModel.cx,nx),NewModel.cy++,nx=0;continue;}
			if(ch=='.'){NewModel.cx=max(NewModel.cx,nx),NewModel.cy++;break;}
			NewModel.mp[nx++][NewModel.cy]=ch=='*'?1:0;
		}
		freopen("CON","r",stdin);
		models.push_back(NewModel);
	}while(FindNextFile(a,&b));
	FindClose(a);
}
int GetWait(HWND hwnd){
	char str[128];
	int re=0;
	SendMessage(Edit,WM_GETTEXT,256,(LPARAM)str);
	for(int i=0,len=strlen(str);i<len;i++){
		if(!isdigit(str[i])){
			str[i]='\0',SetWindowText(Edit,str);
			SetActiveWindow(NULL);
			SetActiveWindow(hwnd);
			break;
		}
		re=re*10+str[i]-'0';
	}
	return re*CLOCKS_PER_SEC/1000;
}
