// Tetris.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Tetris.h"
#include "globals.h"
#include "Board.h"
#include "Shapes.h"
#include <thread>
#include <time.h>
#include <mutex>


#define MAX_LOADSTRING 100

// Global Variables 
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The main window class name
bool endGame = false;
bool resetGame = false;
std::thread dropPieceThread;					// Thread that drops the current piece
std::mutex m;									// Used to lock sections of code shared by the main thread and dropPieceThread

// Forward declarations of functions included in this code module:

	// Windows Functions:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

	// User Helper Functions:
bool				lockInPieceAndSpawn(HWND&, Board&);
bool				dropPiece(HWND&, Board&);
void				dropPieceLoop(HWND*, Board*, int);

// Main Function
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// Window management and painting declarations
	MSG msg;
	HACCEL hAccelTable;
	PAINTSTRUCT ps;
	HDC hdc;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TETRIS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRIS));

	// Get the first message
	GetMessage(&msg, NULL, 0, 0);

	// Enable drawing
	hdc = BeginPaint(msg.hwnd, &ps);

	// Create the board and first shape and draw them
	Board board(hdc, 0);
	board.DisplayUserControlsLayout();
	board.spawnShape();
	board.getShape()->SetHdc(hdc);
	board.DrawMainGrid();
	board.getShape()->DrawShape();

	//Begin dropPieceThread at 1200ms delay between drops
	dropPieceThread = std::thread(dropPieceLoop, &msg.hwnd, &board, 1200);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{	
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			int result = DispatchMessage(&msg);

			// Handle the different possible user initiated events
			switch (result) {
			case PAINT:
				{
					// Redraws everything and marks rect as valid
					board.DrawMainGrid();
					board.DisplayUserControlsLayout();
					board.DisplayCurrentLevel();
					board.getShape()->DrawShape();
					ValidateRect(msg.hwnd, 0); // Signify that window is set so WM_PAINT messages stop getting sent
					break;
				}
			case SPIN:
				{
					m.lock();
					board.getShape()->DeleteShape();
					board.spinShape(); 
					board.getShape()->DrawShape();		
					m.unlock();
					break;
				}
			case LEFT_MOVE:
				{
					m.lock();
					board.getShape()->DeleteShape();
					board.MoveShape(LEFT_MOVE);
					board.getShape()->DrawShape();
					m.unlock();
					break;
				}
			case RIGHT_MOVE:
				{
					m.lock();
					board.getShape()->DeleteShape();
					board.MoveShape(RIGHT_MOVE);
					board.getShape()->DrawShape();
					m.unlock();
					break;
				}
			case DOWN_MOVE:
				{
					m.lock();
					bool b_setPiece = true;
					board.getShape()->DeleteShape();
					b_setPiece = !( board.MoveShape(DOWN_MOVE) );
					board.getShape()->DrawShape();
					m.unlock();

					if( b_setPiece ) {
						lockInPieceAndSpawn(msg.hwnd, board);
					}
					
					break;
				}
			case SET_PIECE:
				{
					// User has performed a "hard drop" so drop the piece to the bottom of its path and spawn a new piece
					lockInPieceAndSpawn(msg.hwnd, board);
					break;
				}
			default:
				{
					
				}
			}
		}
	}
	dropPieceThread.join();
	EndPaint(msg.hwnd, &ps);

	return (int) msg.wParam;
}

/* BEGIN USER HELPER FUNCTIONS*/

bool lockInPieceAndSpawn(HWND &hWnd, Board &board) {
	m.lock();

	board.quickDrop(); // Immediately move piece to first blocking grid in path

	board.clearLines(); // If there are lines to clear, clear them and move all squares above down

	// Spawn the next shape and determine whether the game is now over
	if (!board.spawnShape()) {
		int iMessage = ::MessageBox(hWnd, _T("Play again?"), _T("Game Over"), MB_YESNO | MB_ICONQUESTION);
		switch (iMessage) {
		case IDYES:
		{
			resetGame = true;
			break;
		}
		case IDNO:
		{
			endGame = true;
			break;
		}
		}
		m.unlock();
		return false;
	}
	board.getShape()->DrawShape();

	m.unlock();
	return true;
}

bool dropPiece(HWND &hWnd, Board &board) {
	bool b_setPiece = true;

	m.lock();
	board.getShape()->DeleteShape();
	b_setPiece = !(board.MoveShape(DOWN_MOVE));
	board.getShape()->DrawShape();
	m.unlock();

	// Add the current piece to the board and spawn the next piece now 
	// that it has reached a locked in section.
	// Don't do this if the game has been reset or ended by the main thread
	if (b_setPiece && !resetGame && !endGame) {
		lockInPieceAndSpawn(hWnd, board);
		return false;
	}
	return true;
}

void dropPieceLoop(HWND *hWnd, Board *board, int startingDropDelayMiliseconds) {
	int level = 1;
	int linesClearedThreshold = 4;
	int dropDelayMiliseconds = startingDropDelayMiliseconds;
	board->setCurrentLevel(level); // Initialize the current level text display
	board->DisplayCurrentLevel();  // ^

	while (!endGame) {
		bool b_dropping = true;

		do {
			// Before dropping, check that if the game has been ended or reset
			if (endGame || resetGame) {
				break;
			}
			// Increase the level when we pass linesClearedThreshold, until dropDelayMiliseconds hits 50ms
			if (board->getLinesCleared() >= linesClearedThreshold && dropDelayMiliseconds != 50) {

				// Graded system of decreasing dropDelayMiliseconds and increasing linesClearedThreshold
				if (dropDelayMiliseconds <= 500) {
					if (dropDelayMiliseconds <= 200) {
						dropDelayMiliseconds -= 25;
						linesClearedThreshold += 8;
					}
					else {
						dropDelayMiliseconds -= 50;
						linesClearedThreshold += 6;
					}
				}
				else {
					dropDelayMiliseconds -= 100;
					linesClearedThreshold += 4;
				}
				level++;
				board->setCurrentLevel(level); // Update the current level text display
				board->DisplayCurrentLevel();  // ^
			}
			Sleep(dropDelayMiliseconds); //Wait to drop the piece based on the current level
			b_dropping = dropPiece(*hWnd, *board);
		} while (b_dropping);

		if (resetGame) {
			board->resetGame(); // Begin a new game
			resetGame = false;

			//Reset Local Variables
			level = 1;
			dropDelayMiliseconds = startingDropDelayMiliseconds;
			linesClearedThreshold = 4;

			board->setCurrentLevel(level); // Reset the current level text display
			board->DisplayCurrentLevel();  // ^
		}
	}
}

/* END USER HELPER FUNCTIONS*/

/* BEGIN WINDOWS FUNCTIONS*/

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TETRIS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}



//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 700, 1000, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}




//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
		
	case WM_KEYDOWN:
		{
			// Determine what user action has occurred
			switch (wParam)
			{
			case VK_SPACE:
				return SPIN;
			case VK_LEFT:
				return LEFT_MOVE;
			case 'A':
				return LEFT_MOVE;
			case VK_RIGHT:
				return RIGHT_MOVE;
			case 'D':
				return RIGHT_MOVE;
			case VK_DOWN:
				return DOWN_MOVE;
			case 'S':
				return DOWN_MOVE;
			case VK_UP:
				return SET_PIECE;
			case 'W':
				return SET_PIECE;

			default:
				break;
			}

			// Repaint the windo after the update
			InvalidateRect(hWnd, 0, TRUE);
			break;
		}

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			::MessageBox(hWnd, _T("Tetris!"), _T("Game Description"), MB_ICONQUESTION);
			break;
		case IDM_NEW_GAME:
			resetGame = true;
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		return PAINT;
		break;
	case WM_DESTROY:
		endGame = true;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

/* END WINDOWS FUNCTIONS*/
