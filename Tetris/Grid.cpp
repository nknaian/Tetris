#include "stdafx.h"
#include "globals.h"
#include "Grid.h"

// Setters and Getters:
void Grid::setIsPlaced(bool isPlaced) {
	m_isPlaced = isPlaced;
}

void Grid::setGridPos(GRID_POS gridPos) {
	m_gridPos = gridPos;
}

void Grid::setGridColor(COLORREF gridColor) {
	m_gridColor = gridColor;
}

bool Grid::getIsPlaced() {
	return m_isPlaced;
}

// Drawing Functions:

bool Grid::DrawGrid(GRID_POS gridPos) {
	if(gridPos.m_x <= 0 || gridPos.m_x > NUM_SQUARES_ACROSS) {
		return false;
	}
	HBRUSH hShapeBrush;
	HPEN hLinePen;
	hLinePen = CreatePen(PS_SOLID, GRID_WIDTH, BLACK );
	hShapeBrush = CreateSolidBrush(m_gridColor);
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

bool Grid::DeleteGrid(GRID_POS gridPos) {
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