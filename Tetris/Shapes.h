#pragma once

class Grid;

class Board;

class Shape {

public:
	//constructor:
	Shape();
	Shape(HDC hdc, GRID_POS topMidGrid, ORIENTATION orientation);

	// setters and getters:
	virtual void SetPositions(ORIENTATION orientation);
	void SetHdc(HDC hdc);
	void setOrientation(ORIENTATION orientation);
	void setToMidGrid(GRID_POS topMidGrid);
	void setToMidGrid(int xOffset, int yOffset);

	ORIENTATION getOrientation();
	GRID_POS getTopMidGrid();
	GRID_POS* getGridPos();
	COLORREF getColor();
	

	// Movement functions:
	

	// Drawing functions
	void DrawShape();
	void DeleteShape();

	bool DrawGrid(GRID_POS gridPos);
	bool DeleteGrid(GRID_POS gridPos);
protected:
	// Properties:
	HDC m_hdc;
	COLORREF m_color;
	ORIENTATION m_orientation;
	GRID_POS m_topMidGrid;
	GRID_POS m_positions[4];

};

class Shape_Tetris : public Shape {
public:
	// constructor
	Shape_Tetris(HDC hdc, GRID_POS topMidGrid, ORIENTATION orientation) : Shape(hdc, topMidGrid, orientation) {
		m_color = PURPLE;
		SetPositions(UP);
	}

	// setters and getters:
	void SetPositions(ORIENTATION orientation);

	// Properties:

};

class Shape_Square : public Shape {
public:
	// constructor
	Shape_Square(HDC hdc, GRID_POS topMidGrid, ORIENTATION orientation) : Shape(hdc, topMidGrid, orientation) {
		m_color = YELLOW;
		SetPositions(UP);
	}

	// setters and getters:
	void SetPositions(ORIENTATION orientation);

	// Properties:

};

class Shape_Line : public Shape {
public:
	// constructor
	Shape_Line(HDC hdc, GRID_POS topMidGrid, ORIENTATION orientation) : Shape(hdc, topMidGrid, orientation) {
		m_color = LIGHT_BLUE;
		SetPositions(UP);
	}

	// setters and getters:
	void SetPositions(ORIENTATION orientation);

	// Properties:

};

class Shape_RightStep : public Shape {
public:
	// constructor
	Shape_RightStep(HDC hdc, GRID_POS topMidGrid, ORIENTATION orientation) : Shape(hdc, topMidGrid, orientation) {
		m_color = RED;
		SetPositions(UP);
	}

	// setters and getters:
	void SetPositions(ORIENTATION orientation);

	// Properties:

};

class Shape_LeftStep : public Shape {
public:
	// constructor
	Shape_LeftStep(HDC hdc, GRID_POS topMidGrid, ORIENTATION orientation) : Shape(hdc, topMidGrid, orientation) {
		m_color = GREEN;
		SetPositions(UP);
	}

	// setters and getters:
	void SetPositions(ORIENTATION orientation);

	// Properties:

};

class Shape_LeftL : public Shape {
public:
	// constructor
	Shape_LeftL(HDC hdc, GRID_POS topMidGrid, ORIENTATION orientation) : Shape(hdc, topMidGrid, orientation) {
		m_color = ORANGE;
		SetPositions(UP);
	}

	// setters and getters:
	void SetPositions(ORIENTATION orientation);

	// Properties:

};

class Shape_RightL : public Shape {
public:
	// constructor
	Shape_RightL(HDC hdc, GRID_POS topMidGrid, ORIENTATION orientation) : Shape(hdc, topMidGrid, orientation) {
		m_color = BLUE;
		SetPositions(UP);
	}

	// setters and getters:
	void SetPositions(ORIENTATION orientation);

	// Properties:

};