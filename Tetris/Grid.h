#pragma once

class Grid {
public:
	Grid(HDC hdc, COLORREF gridColor, GRID_POS gridPos, bool isPlaced) :
		m_hdc(hdc), m_gridColor(gridColor), m_gridPos(gridPos), m_isPlaced(isPlaced) {}

	// Setters and Getters:
	void setIsPlaced(bool isPlaced);
	void setGridPos(GRID_POS gridPos);
	void setGridColor(COLORREF gridColor);

	bool getIsPlaced();

	// Drawing functions:
	bool DrawGrid(GRID_POS gridPos);
	bool DeleteGrid(GRID_POS gridPos);
	
private:
	HDC m_hdc;
	bool m_isPlaced;
	COLORREF m_gridColor;
	GRID_POS m_gridPos;
};