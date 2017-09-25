#pragma once

extern COLORREF WHITE;
extern COLORREF BLACK;
extern COLORREF RED;
extern COLORREF GREEN;
extern COLORREF BLUE;
extern COLORREF YELLOW;
extern COLORREF PURPLE;
extern COLORREF LIGHT_BLUE;
extern COLORREF ORANGE;


extern const int NUM_SQUARES_ACROSS;
extern const int NUM_SQAURES_DOWN;
extern const int MAIN_GRID_WIDTH;
extern const int MAIN_GRID_HEIGHT;
extern const int SQ_SIZE;
extern const int BORDER_WIDTH;
extern const int GRID_WIDTH;
extern const int OFFSET;

struct GRID_POS {
	int m_xPixel;
	int m_yPixel;
	int m_x;
	int m_y;

	GRID_POS() {}	
	GRID_POS(int x, int y) : m_x(x), m_y(y)
	{
		m_xPixel = OFFSET + (x - 1)*SQ_SIZE;
		m_yPixel = OFFSET + (y - 1)*SQ_SIZE;
	}
};

enum ORIENTATION {UP, RIGHT, DOWN, LEFT};

enum MOVEMENT {DEFAULT, DEFAULT1, DEFAULT2, DEFAULT3, PAINT, SPIN, LEFT_MOVE, RIGHT_MOVE, DOWN_MOVE, SET_PIECE};

enum ShapeType {LINE, TETRIS, SQUARE, RIGHT_STEP, LEFT_STEP, LEFT_L, RIGHT_L, NUM_SHAPES};