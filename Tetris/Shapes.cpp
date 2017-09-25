#include "stdafx.h"
#include "globals.h"
#include "Grid.h"
#include "Shapes.h"
#include "Board.h"

Shape::Shape() {}

Shape::Shape(HDC hdc, GRID_POS topMidGrid, ORIENTATION orientation)
		: m_hdc(hdc), m_topMidGrid(topMidGrid), m_orientation(orientation)
{}

void Shape::SetHdc(HDC hdc) {
	m_hdc = hdc;
}

void Shape::setOrientation(ORIENTATION orientation) {
	m_orientation = orientation;
}

void Shape::setToMidGrid(GRID_POS topMidGrid) {
	m_topMidGrid = topMidGrid;
}

void Shape::setToMidGrid(int xOffset, int yOffset) {
	m_topMidGrid.m_x += xOffset;
	m_topMidGrid.m_y += yOffset;
}

ORIENTATION Shape::getOrientation() {
	return m_orientation;
}

GRID_POS* Shape::getGridPos() {
	return m_positions;
}

GRID_POS Shape::getTopMidGrid() {
	return m_topMidGrid;
}

COLORREF Shape::getColor() {
	return m_color;
}

void Shape::SetPositions(ORIENTATION orientation) {}

void Shape_Tetris::SetPositions(ORIENTATION orientation) {
	m_positions[0] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y);

	switch(m_orientation) {
	case UP:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
		break;
	case RIGHT:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y - 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y);
		break;
	case DOWN:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y - 1);
		break;
	case LEFT:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y - 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
		break;
	}
}

void Shape_Square::SetPositions(ORIENTATION orientation) {
	m_positions[0] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y);
	m_positions[1] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
	m_positions[2] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y + 1);
	m_positions[3] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
}

void Shape_Line::SetPositions(ORIENTATION orientation) {
	m_positions[0] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y);

	switch(m_orientation) {
	case UP:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 2, m_topMidGrid.m_y);
		break;
	case RIGHT:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y - 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 2);
		break;
	case DOWN:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 2, m_topMidGrid.m_y);
		break;
	case LEFT:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y - 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 2);
		break;
	}
}

void Shape_RightStep::SetPositions(ORIENTATION orientation) {
	m_positions[0] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y);

	switch(m_orientation) {
	case UP:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y + 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
		break;
	case RIGHT:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y - 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y + 1);
		break;
	case DOWN:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y + 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
		break;
	case LEFT:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y - 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y + 1);
		break;
	}
}

void Shape_LeftStep::SetPositions(ORIENTATION orientation) {
	m_positions[0] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y);

	switch(m_orientation) {
	case UP:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y + 1);
		break;
	case RIGHT:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y - 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y + 1);
		break;
	case DOWN:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y + 1);
		break;
	case LEFT:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y - 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y + 1);
		break;
	}
}

void Shape_LeftL::SetPositions(ORIENTATION orientation) {
	m_positions[0] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y);

	switch(m_orientation) {
	case UP:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y - 1);
		break;
	case RIGHT:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y - 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y + 1);
		break;
	case DOWN:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y + 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
		break;
	case LEFT:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y - 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y - 1);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
		break;
	}
}

void Shape_RightL::SetPositions(ORIENTATION orientation) {
	m_positions[0] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y);

	switch(m_orientation) {
	case UP:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y - 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
		break;
	case RIGHT:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y - 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y - 1);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
		break;
	case DOWN:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x + 1, m_topMidGrid.m_y + 1);
		break;
	case LEFT:
		m_positions[1] = GRID_POS(m_topMidGrid.m_x - 1, m_topMidGrid.m_y + 1);
		m_positions[2] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y + 1);
		m_positions[3] = GRID_POS(m_topMidGrid.m_x, m_topMidGrid.m_y - 1);
		break;
	}
}

// Drawing:

void Shape::DrawShape() {
	for(int i = 0; i < 4; i++) {
		DrawGrid( m_positions[i] );
	}
}

void Shape::DeleteShape() {
	for(int i = 0; i < 4; i++) {
		DeleteGrid( m_positions[i] );
	}
}

bool Shape::DrawGrid(GRID_POS gridPos) {
	if(gridPos.m_x <= 0 || gridPos.m_x > NUM_SQUARES_ACROSS) {
		return false;
	}
	HBRUSH hShapeBrush;
	HPEN hLinePen;
	hLinePen = CreatePen(PS_SOLID, GRID_WIDTH, BLACK );
	hShapeBrush = CreateSolidBrush(m_color);
	SelectObject(m_hdc, hShapeBrush);
	SelectObject(m_hdc, hLinePen);

	// Draw the square
	const int numSides = 4;
	tagPOINT point[numSides];
	for(int i = 0; i < numSides; i++) {
		point[i].x = gridPos.m_xPixel + SQ_SIZE*(i == 1 || i == 2);
		point[i].y = gridPos.m_yPixel + SQ_SIZE*(i == 2 || i == 3);
	}
	Polygon(m_hdc, point, numSides);

	DeleteObject(hShapeBrush);
	DeleteObject(hLinePen);

	return true;
}

bool Shape::DeleteGrid(GRID_POS gridPos) {
	if(gridPos.m_x <= 0 || gridPos.m_x > NUM_SQUARES_ACROSS) {
		return false;
	}
	HBRUSH hShapeBrush;
	HPEN hLinePen;
	hLinePen = CreatePen(PS_SOLID, GRID_WIDTH, BLACK );
	hShapeBrush = CreateSolidBrush(WHITE);
	SelectObject(m_hdc, hShapeBrush);
	SelectObject(m_hdc, hLinePen);

	// Draw the square
	const int numSides = 4;
	tagPOINT point[numSides];
	for(int i = 0; i < numSides; i++) {
		point[i].x = gridPos.m_xPixel + SQ_SIZE*(i == 1 || i == 2);
		point[i].y = gridPos.m_yPixel + SQ_SIZE*(i == 2 || i == 3);
	}
	Polygon(m_hdc, point, numSides);

	DeleteObject(hShapeBrush);
	DeleteObject(hLinePen);

	return true;
}

