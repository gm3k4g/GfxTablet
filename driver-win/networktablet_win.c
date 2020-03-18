// Windows Implementation of GfxTablet, MinGW


// Defines
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define MAX_LOADSTRING 100
#define WM_USER_SHELLICON(WM_USER+1)

#include <SDKDDKVer.h> //#include "targetver.h"

//#include "stdafx.h" everything below
// Windows Header Files:
#include <windows.h>
#include <shellapi.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Useful libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <wininet.h>

//some types
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef char int8_t;

// From linux
#include "protocol_win.h"

//#include "networktablet_win.h"
#include <WinSock2.h>

//Global variables
HINSTANCE hInst;					//Current instance
TCHAR szTitle[MAX_LOADSTRING];		//The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];//Main window class name

// Forward declarations of functions included in this code module
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int 	RunDriver();

int APIENTRY _tWinMain(HINSTANCE hInstance,
					  HINSTANCE hPrevInstance,
					  LPTSTR	lpCmdLine,
					  int 		nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place Code Here?
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRIVERWIN32, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	//Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) {
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRIVERWIN32));

	//
	PostMessage(NULL, WM_USER+1,0,0);

	//Main message loop
	while(GetMessage(&msg, NULL, 0, 0)) {
		if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRIVERWIN32));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRIVERWIN32);
	wcex.lpszClassName = szWindowClass;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

NOTIFYICONDATA nidApp;

void CreateTrayIcon(HWND hWnd) {
	nidApp.cbSize = sizeof(NOTIFYCONDATA);
	nidApp.hWnd = (HWND) hWnd;
	nidApp.uID = IDI_DRIVERWIN32;
	nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nidApp.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DRIVERWIN32));
	nidApp.uCallbackMessage = WM_USER_SHELLICON;
	_tcscpy(nidApp.szTip, _T("GfxTablet Windows Driver"));

	Shell_NotifyIcon(NIM_ADD, &nidApp);
}

void RemoveTrayIcon(HWND hWnd) {
	Shell_NotifyIcon(NIM_DELETE, &nidApp);
}

DWORD dwDriverThreadId = 0;
DWORD WINAPI DriverThreadStart(LPVOID lpThreadParameter) {
	return RunDriver();
}

HWND hWnd = NULL;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance // Store instance handle in global variable
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USERDEFAULT, 0, CW_USERDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if(!hWnd) {
		return FALSE;
	}

	CreateTrayIcon(hWnd);
	CreateThread(NULL, 500000, DriverThreadStart, NULL, 0, &dwDriverThreadId);

	return TRUE;
}

void ShowTrayIconContextMenu(HWND hWnd) {
	POINT lpClickPoint;
	UINT uFlag = MF_BYPOSITION|MF_STRING;
	GetCursrPos(&lpClickPoint);
	HMENU hPopMenu = CreatePopupMenu();
	InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION|MF_STRING, IDM_EXIT, _T("Exit"));
	TrackPopupMenu(hPopMenu,TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_BOTTOMALIGN,
		lpClickPoint.x, lpClickPoint.y,0,hWnd,NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch(message) {
		case WM_USER_SHELLICON:
			switch(LOWORD(lParam)) {
				case WM_RBUTTONDOWN:
					ShowTrayIconContextMenu(hWnd);
			} break;

		case WM_COMMAND:
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			//Parse menu selections:
			switch(wmId) {
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParamn, lParam);
			} break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			RemoveTrayIcon(hWnd);
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;	
}

int udp_socket;

void die(TCHAR* fmt, ...) {
	va_list args;
	va_start(args,fmt);
	TCHAR msgbuf[4096];
	_vsntprintf_s(msgbuf, _countof(msgbuf), fmt, args);
	MessageBox(NULL, msgbuf, _T("GfxTablet Win32 driver error"), MB_ICONERROR|MB_OK);

	RemoveTrayIcon(hWnd);
	ExitProcess(EXIT_FAILURE);
}

int prepare_socket() {
	int s;
	struct sockaddr_in addr;

	WSADATA wsadata;
	WSAStartup(WINSOCK_VERSION, &wsadata);

	if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		die(_T("Error: prepare_socket()"));
	}

	ZeroMemory(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(GFXTABLET_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		DWORD dwErr = WSAGetLastError();
		if (dwErr = WSAEADDRINUSE) {
			die(_T("Error: prepare_socket(). Port already in use."));
		} else {
			die(_T("Error: prepare_socket(). Error = 0x%08x"), dwErr);
		}
	}
	return s;
}

void quit(int signal) {
	closesocket(udp_socket);
}

int RunDriver(void) {
	int device;
	struct event_packet ev_pkt;

	udp_socket = prepare_socket();

	printf("GfxTablet driver (protocol version %u) is ready and listening on 0.0.0.0:%u (UDP)\n"
		"Hint: Make sure that this port is not blocked by your firewall.\n", PROTOCOL_VERSION, GFXTABLET_PORT);

	signal(SIGINT, quit);
	signal(SIGTERM, quit);

	BOOL isdown = false;

	while (recv(udp_socket, (char*)&ev_pkt, sizeof(ev_pkt), 0) >= 9) { //Every packet has at least 9 bytes
		printf("."); fflush(0);

		if(memcmp(ev_pkt.signature, "GfxTablet", 9) != 0) {
			fprintf(stderr, "\nGot unknown packet on port %i, ignoring.\n", GFXTABLET_PORT);
			continue;
		}
		ev_pkt.version = ntohs(ev_pkt.version);

		if(ev_pkt.version != PROTOCOL_VERISON) {
			fprintf(stderr, "\nGfxTablet app speaks protocl version %i, but driver speaks version %i. Please update. \n",
				ev_pkt.version, PROTOCOL_VERSION);
			break;
		}

		ev_pkt.x = ntohs(ev_pkt.x);
		ev_pkt.y = ntohs(ev_pkt.y);
		ev_pkt.pressure = ntohs(ev_pkt.pressure);
		printf("x: %hi, y: %hi, pressure: %hi\n", ev_pkt.x, ev_pkt.y, ev_pkt.pressure);

		//Windows uses 0-65535, GfxTablet uses 0-32767
		int absoluteX = ((int)ev_pkt.x)*2;
		int absoluteY = ((int)ev_pkt.y)*2;

		INPUT inputs[1];
		ZeroMemory(&inputs[0],sizeof(inputs));
		inputs[0].type = INPUT_MOUSE;
		inputs[0].mi.dx = absoluteX;
		inputs[0].mi.dy = absoluteY;
		inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE

		/* ?!no support for pressure on windows?!
		This is only possible with an HID miniport driver.
		GIMP uses DirectInput, probably the HID miniport driver would be visible through HID
		If not.. a DirectInput driver would be necessary too
		*/

		#if SIMULATE_HOVER_THROUGH_LIGHT_TOUCH

		/*
		hack: ignore EVENT_TYPE_BUTTON (android touch events), but generates them
		when pressure is over a specific limit, so it is possible to move the finger around the tablet
		without drawing.
		*/
		if(ev_pkt.type == EVENT_TYPE_BUTTON && ev_pkt.down) {
			ev_pkt.type = EVENT_TYPE_MOTION;
		} else if (ev_pkt.type == EVENT_TYPE_MOTION && ev_pkt.pressure>18000) {
			if(!isDown) {
				ev_pkt.type = EVENT_TYPE_BUTTON;
			}
		}
		#endif

		switch(ev_pkt.type) {
			case EVENT_TYPE_MOTION:
				inputs[0].mi.dwFlags |= MOUSEEVENTF_MOVE;
				SendInput(1, &inputs[0], sizeof(inputs[0]));
				break;
			case EVENT_TYPE_BUTTON;
				if(ev_pkt.button == 0) {
					inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE;
					SendInput(1, &inputs[0], sizeof(inputs[0]));
				}

				if(ev_pkt.down) {
					inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN;
					isDown = true;
				} else {
					input[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP;
					isDown = false;
				}
				SendInput(1, &inputs[0], sizeof(inputs[0]));
				break;
		}
	}
	closesocket(udp_socket);

	printf("GfxTablet Driver shut down gracefully\n");
	return 0;
}


//Linux fragments
/*
#define die(str, args...) { \
	perror(str); \
	exit(EXIT_FAILURE); \
}

int udp_socket;

void quit(int signal) {
	close(udp_socket);
}


int main(void)
{
	int device;
	struct event_packet ev_pkt;

	if ((device = open("/dev/input/directx", O_WRONLY)) < 0)
		die("error: open"); //open doesn't exist on windows

	//init_device(device);
	//udp_socket = prepare_socket();

	printf("GfxTablet driver (protocol version %u) is ready and listening on 0.0.0.0:%u (UDP)\n"
		"Hint: Make sure that this port is not blocked by your firewall.\n", PROTOCOL_VERSION, GFXTABLET_PORT);

	signal(SIGINT, quit);
	signal(SIGTERM, quit);

	printf("Removing network tablet from device list\n");
	//ioctl(device, UI_DEV_DESTROY);
	close(device);

	printf("GfxTablet driver shut down gracefully\n");
	return 0;
}
*/