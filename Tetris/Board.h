#pragma once
#include <vector>

class Grid;

class Shape; // Must forward declare a class used in this header file's class

class Board {
public:
	// Constructor:
	Board(HDC hdc, int linesCleared);

	// reset
	void resetGame();

	// Setters and Getters
	void setHdc(HDC hdc);
	void setLinesCleared(int linesCleared);
	void setCurrentLevel(int currentLevel);
	Shape* getShape();
	int getLinesCleared();
	std::vector< std::vector<Grid*> > getBoardArray();

	// Shape handling functions:
	bool spawnShape();
	bool placePiece();
	void spinShape();
	void revertSpinShape();
	bool MoveShape(MOVEMENT direction);
	

	// Boounds Checking functions:
	bool isPositionInBounds();
	bool isPositionInBounds( GRID_POS gridPos[4] , MOVEMENT direction);
	void adjustForX();
	void adjustForY();
	bool canMoveShape(MOVEMENT direction);
	bool canMoveShape();
	bool canPlaceShape(GRID_POS gridPos[4]);

	// Gameplay functions
	void quickDrop();
	void clearLines();

	// Draw functions
	int borderOffset(int xPos, int yPos);
	void DrawMainGrid();
	void DisplayCurrentLevel();
	void Board::DisplayUserControlsLayout();
private:
	// Properties:
	HDC m_hdc;
	std::vector< std::vector<Grid*> > m_boardArray;
	Shape* m_currentShape;
	int m_linesCleared;
	int m_currentLevel;
};