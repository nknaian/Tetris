#include "stdafx.h"
#include "globals.h"
#include "Board.h"
#include "Shapes.h"
#include "Grid.h"
#include <time.h>
#include <random>

Board::Board(HDC hdc, int linesCleared) : m_hdc(hdc), m_linesCleared(linesCleared)
{
	std::vector<Grid*> row;
	for(int i = 1; i <= NUM_SQAURES_DOWN + 1; i++) { 
		for(int j = 1; j <= NUM_SQUARES_ACROSS + 1; j++) {
			Grid* grid = new Grid(m_hdc, WHITE, GRID_POS(i, j), false);
			row.push_back(grid);
		}
		m_boardArray.push_back(row);
		row.clear();
		
	}

	// Also initialize seed at construction
	srand( time(NULL) );
}

void Board::resetGame() {
	// Clear all locked-in grids
	for(int j = 1; j <= NUM_SQAURES_DOWN; j++) {
		for(int i = 1; i <= NUM_SQUARES_ACROSS; i++) {
			delete m_boardArray[j][i];
			m_boardArray[j][i] = new Grid(m_hdc, WHITE, GRID_POS(i, j), false);
			m_boardArray[j][i]->DrawGrid( GRID_POS(i, j) );
		}
	}

	spawnShape(); // spawn a new shape
	m_linesCleared = 0; // put lines cleared back to 0
}

void Board::setHdc(HDC hdc) {
	m_hdc = hdc;
}

void Board::setLinesCleared(int linesCleared) {
	m_linesCleared = linesCleared;
}

void Board::setCurrentLevel(int currentLevel) {
	m_currentLevel = currentLevel;
}

Shape* Board::getShape() {
	return m_currentShape;
}

int Board::getLinesCleared() {
	return m_linesCleared;
}

std::vector< std::vector<Grid*> > Board::getBoardArray() {
	return m_boardArray;
}



// Bounds Checking functions
bool Board::isPositionInBounds(){
	for(int i = 0; i < 4; i++) {
		if( m_currentShape->getGridPos()[i].m_x <= 0 ) {
			return false;
		}
		if( m_currentShape->getGridPos()[i].m_x > NUM_SQUARES_ACROSS) {
			return false;
		}
		if( m_currentShape->getGridPos()[i].m_y <= 0 ) {
			return false;
		}
		if( m_currentShape->getGridPos()[i].m_y > NUM_SQAURES_DOWN) {
			return false;
		}
	}
	return true;
}

bool Board::isPositionInBounds( GRID_POS gridPos[4] , MOVEMENT direction){
	int xOffset = 0;
	int yOffset = 0;
	if(direction == LEFT_MOVE) {
		xOffset = -1;
	}
	else if(direction == RIGHT_MOVE) {
		xOffset = 1;
	}
	else {
		yOffset = 1;
	}
	for(int i = 0; i < 4; i++) {
		if( gridPos[i].m_x + xOffset <= 0 ) {
			return false;
		}
		if( gridPos[i].m_x + xOffset > NUM_SQUARES_ACROSS) {
			return false;
		}
		if( gridPos[i].m_y + yOffset > NUM_SQAURES_DOWN) {
			return false;
		}
	}
	return true;
}

void Board::adjustForX() {
	int maxX = 0;
	int minX = NUM_SQUARES_ACROSS;
	int curX;
	int offset = 0;
	for(int i = 0; i < 4; i++) {
		curX = m_currentShape->getGridPos()[i].m_x;
		if( curX > maxX ) {
			maxX = curX;
		}
		if( curX < minX) {
			minX = curX;
		}
	}
	if(maxX > NUM_SQUARES_ACROSS) {
		offset = NUM_SQUARES_ACROSS - maxX;
		m_currentShape->setToMidGrid(offset, 0);
	}
	else if( minX < 1 ) {
		offset = 1 - minX;
		m_currentShape->setToMidGrid(offset, 0);
	}
}

void Board::adjustForY() {
	int maxY = 0;
	int minY = NUM_SQAURES_DOWN;
	int curY;
	int offset = 0;
	for(int i = 0; i < 4; i++) {
		curY = m_currentShape->getGridPos()[i].m_y;
		if( curY > maxY ) {
			maxY = curY;
		}
		if( curY < minY ) {
			minY = curY;
		}
	}
	if( maxY > NUM_SQAURES_DOWN ) {
		offset = NUM_SQAURES_DOWN - maxY;
		m_currentShape->setToMidGrid(0, offset);
	}
	else if( minY < 1 ) {
		offset = 1 - minY;
		m_currentShape->setToMidGrid(0, offset);
	}
}

bool Board::canMoveShape() {
	int curX;
	int curY;
	for(int i = 0; i < 4; i++) {
		curX = m_currentShape->getGridPos()[i].m_x;
		curY = m_currentShape->getGridPos()[i].m_y;
		if( m_boardArray[curY][curX]->getIsPlaced() == true ) {
			return false;								// if this space is equal to true, then piece already owns it
		}
	}
	return true;
}

bool Board::canMoveShape(MOVEMENT direction) {
	int curX;
	int curY;
	for(int i = 0; i < 4; i++) {
		switch(direction) {
		case RIGHT_MOVE:
			curX = m_currentShape->getGridPos()[i].m_x + 1;
			curY = m_currentShape->getGridPos()[i].m_y;
			break;
		case LEFT_MOVE:
			curX = m_currentShape->getGridPos()[i].m_x - 1;
			curY = m_currentShape->getGridPos()[i].m_y;
			break;
		case DOWN_MOVE:
			curX = m_currentShape->getGridPos()[i].m_x;
			curY = m_currentShape->getGridPos()[i].m_y + 1;
			break;
		case SPIN:
			curX = m_currentShape->getGridPos()[i].m_x;
			curY = m_currentShape->getGridPos()[i].m_y;
			break;
		}
		if( m_boardArray[curY][curX]->getIsPlaced() == true ) {
			return false;								// if this space is equal to true, then piece already owns it
		}
	}
	return true;
}

bool Board::canPlaceShape(GRID_POS gridPos[4]) {
	int curX;
	int curY;
	for(int i = 0; i < 4; i++) {
		curX = gridPos[i].m_x;
		curY = gridPos[i].m_y;
		if( curY == NUM_SQAURES_DOWN ) {
			return true;
		}
		else if( m_boardArray[curY + 1][curX]->getIsPlaced() == true ) {
			return true;
		}

	}
	return false;
}

// Shape handling functions:
bool Board::spawnShape() {
	int shapeType = rand() % NUM_SHAPES;

	if (shapeType == TETRIS) {
		m_currentShape = new Shape_Tetris( m_hdc, GRID_POS(4, 1), UP );
	}
	else if (shapeType == SQUARE) {
		m_currentShape = new Shape_Square( m_hdc, GRID_POS(4, 1), UP );
	}
	else if(shapeType == LINE) {
		m_currentShape = new Shape_Line( m_hdc, GRID_POS(4, 1), UP );
	}
	else if(shapeType == RIGHT_STEP) {
		m_currentShape = new Shape_RightStep( m_hdc, GRID_POS(4, 1), UP );
	}
	else if(shapeType == LEFT_STEP) {
		m_currentShape = new Shape_LeftStep( m_hdc, GRID_POS(4, 1), UP );
	}
	else if(shapeType == LEFT_L) {
		m_currentShape = new Shape_LeftL( m_hdc, GRID_POS(4, 2), UP );
	}
	else if(shapeType == RIGHT_L) {
		m_currentShape = new Shape_RightL( m_hdc, GRID_POS(4, 2), UP );
	}

	if( !canMoveShape() ) {
		return false;
	}
	else {
		return true;
	}

}


bool Board::placePiece() {
	if( !canPlaceShape(m_currentShape->getGridPos() ) ) {
		return false;
	}
	int curX;
	int curY;
	for(int i = 0; i < 4; i++) {
		curX = m_currentShape->getGridPos()[i].m_x;
		curY = m_currentShape->getGridPos()[i].m_y;
		m_boardArray[curY][curX]->setIsPlaced(true);
		m_boardArray[curY][curX]->setGridColor(m_currentShape->getColor() );

		m_currentShape->DeleteGrid( m_currentShape->getGridPos()[i] ); // Take grids out of the "shape" realm...
		m_boardArray[curY][curX]->DrawGrid(m_currentShape->getGridPos()[i] ); // Move to the "board" realm
	}

	return true;
}

// Shape Movement:

void Board::spinShape() {
	switch(m_currentShape->getOrientation() ) {
	case UP:
		m_currentShape->setOrientation(RIGHT);
		break;
	case RIGHT:
		m_currentShape->setOrientation(DOWN);
		break;
	case DOWN:
		m_currentShape->setOrientation(LEFT);
			break;
	case LEFT:
		m_currentShape->setOrientation(UP);
		break;
	default:
		break;
	}

	m_currentShape->SetPositions( m_currentShape->getOrientation() );

	if( !isPositionInBounds() ) {
		adjustForX();
		adjustForY();
		m_currentShape->SetPositions( m_currentShape->getOrientation() );
	}
	if( !canMoveShape(SPIN) ) {
		revertSpinShape();
		m_currentShape->SetPositions( m_currentShape->getOrientation() );
	}
	
}

void Board::revertSpinShape() {
	switch(m_currentShape->getOrientation() ) {
	case UP:
		m_currentShape->setOrientation(LEFT);
		break;
	case LEFT:
		m_currentShape->setOrientation(DOWN);
		break;
	case DOWN:
		m_currentShape->setOrientation(RIGHT);
			break;
	case RIGHT:
		m_currentShape->setOrientation(UP);
		break;
	default:
		break;
	}
}

bool Board::MoveShape(MOVEMENT direction) {
	switch(direction) {
	case RIGHT_MOVE:
		if( !isPositionInBounds(m_currentShape->getGridPos(), RIGHT_MOVE) || !canMoveShape( RIGHT_MOVE) ) {
			return false;
		}
		m_currentShape->setToMidGrid(1, 0);
		break;
	case LEFT_MOVE:
		if( !isPositionInBounds(m_currentShape->getGridPos(), LEFT_MOVE) || !canMoveShape( LEFT_MOVE) ) {
			return false;
		}
		m_currentShape->setToMidGrid(-1, 0);
		break;
	case DOWN_MOVE:
		if( !isPositionInBounds(m_currentShape->getGridPos(), DOWN_MOVE) || !canMoveShape(DOWN_MOVE) ) {
			return false;
		}
		m_currentShape->setToMidGrid(0, 1);
		break;
	}

	m_currentShape->SetPositions( m_currentShape->getOrientation() );

	return true;
}

// Gameplay functions:

void Board::quickDrop() {
	m_currentShape->DeleteShape();

	// Move piece down until it is touching a locked in grid.
	// When it is, lock the piece in by setting the piece's 
	// current grids in the board array
	while( !placePiece() ) { 
		MoveShape(DOWN_MOVE);
	}
}

void Board::clearLines() {
	bool isRowFilled = false;
	std::vector<int> rowsFilledIndex;

	// Iterate through board. If a row is filled, add the row
	// to the rowsFilledIndex and set isRowFilled to true to
	// signify that at least one line needs to be cleared
	for(int j = 1; j <= NUM_SQAURES_DOWN; j++) {
		for(int i = 1; i <= NUM_SQUARES_ACROSS; i++) {
			if(m_boardArray[j][i]->getIsPlaced() == false) { 
				break;										
			}
			if( i == NUM_SQUARES_ACROSS ) {
				isRowFilled = true;
				rowsFilledIndex.push_back(j);
			}
		}
		
	} 

	// Return now if no row needs to be cleared
	if (isRowFilled == false) {
		return;
	}

	int rowFilled = rowsFilledIndex.size() - 1;
	int addNewRowsIndex = rowsFilledIndex.size();
	int shiftRow = 0;

	// Traverse the rows from the bottom up. Delete the 
	// grids in rows that are filled and shift down
	// above rows the correct amount.
	for(int j = NUM_SQAURES_DOWN; j >= 1; j--) {
		if( j == rowsFilledIndex[rowFilled] ) {
			for(int i = 1; i <= NUM_SQUARES_ACROSS; i++) {
				delete m_boardArray[j][i];
			}

			shiftRow++; // Increase the amount of rows subsequent non-filled lines need to be shifted by

			if(rowFilled != 0) {
				rowFilled--; // Move on to the next line to delete
			}
		}
		else if( shiftRow ) { // Shift the row by "shiftRow" if it is not filled
			for(int i = 1; i <= NUM_SQUARES_ACROSS; i++) {
				m_boardArray[j + shiftRow][i] = m_boardArray[j][i];
				m_boardArray[j][i] = NULL;
				m_boardArray[j + shiftRow][i]->DrawGrid(GRID_POS(i, j + shiftRow) );
			}
		}

	}

	// Add in new lines to the top of the board to replace shifted rows
	for(int j = 1; j <= addNewRowsIndex; j++) {
		for(int i = 1; i <= NUM_SQUARES_ACROSS; i++) {
			Grid* grid = new Grid(m_hdc, WHITE, GRID_POS(i, j), false);
			m_boardArray[j][i] = grid;
			m_boardArray[j][i]->DrawGrid( GRID_POS(i, j) );
		}
	}

	// Increment the amount of lines cleared
	m_linesCleared += rowsFilledIndex.size();
}

// Drawing functions:
int Board::borderOffset(int xPos, int yPos) {
	if(xPos == OFFSET || yPos == OFFSET) {
		return -1*(BORDER_WIDTH/2);
	}
	else if( yPos == MAIN_GRID_HEIGHT + OFFSET || xPos == MAIN_GRID_WIDTH + OFFSET) {
		return BORDER_WIDTH/2;
	}
	else{
		return 0;
	}
}
void Board::DrawMainGrid() {

	//Draw vertical lines
	for(int horizGridPos = OFFSET; horizGridPos <= MAIN_GRID_WIDTH + OFFSET; horizGridPos += SQ_SIZE) {
		HPEN variablePen;
		if(horizGridPos == OFFSET || horizGridPos == MAIN_GRID_WIDTH + OFFSET) {
			variablePen = CreatePen(PS_SOLID, BORDER_WIDTH, BLUE);
		} else {
			variablePen = CreatePen(PS_DASHDOTDOT, GRID_WIDTH, BLACK);
		}
		SelectObject(m_hdc, variablePen);


		MoveToEx(m_hdc, horizGridPos + borderOffset(horizGridPos, 0), OFFSET, NULL);
		LineTo(m_hdc, horizGridPos + borderOffset(horizGridPos, 0), MAIN_GRID_HEIGHT + OFFSET);
		
		DeleteObject(variablePen);
	}
	//Draw horizontal lines
	bool isGridLine = false;;
	for(int vertGridPos = OFFSET; vertGridPos <= MAIN_GRID_HEIGHT + OFFSET; vertGridPos += SQ_SIZE) {
		HPEN variablePen;
		if(vertGridPos == OFFSET || vertGridPos == MAIN_GRID_HEIGHT + OFFSET) {
			variablePen = CreatePen(PS_SOLID, BORDER_WIDTH, BLUE);
		} else {
			variablePen = CreatePen(PS_DASHDOTDOT, GRID_WIDTH, BLACK);
			isGridLine = true;
		}
		SelectObject(m_hdc, variablePen);

		MoveToEx(m_hdc, OFFSET , vertGridPos + borderOffset(0, vertGridPos) , NULL);
		LineTo(m_hdc, MAIN_GRID_WIDTH + OFFSET , vertGridPos + borderOffset(0, vertGridPos) );
		
		DeleteObject(variablePen);
		isGridLine = false;
	}

	for(int j = 1; j <= NUM_SQAURES_DOWN; j++) {
		for(int i = 1; i <= NUM_SQUARES_ACROSS; i++) {
			m_boardArray[j][i]->DrawGrid( GRID_POS(i, j) );
		}
	}

}

void Board::DisplayCurrentLevel() {
	TCHAR buff[16];
	if (m_currentLevel < 10) {
		TextOut(m_hdc, MAIN_GRID_WIDTH + 50, MAIN_GRID_HEIGHT / 2, buff, wsprintf(buff, TEXT("Level 0%d"), m_currentLevel));
	}
	else {
		TextOut(m_hdc, MAIN_GRID_WIDTH + 50, MAIN_GRID_HEIGHT / 2, buff, wsprintf(buff, TEXT("Level %d"), m_currentLevel));
	}
}

void Board::DisplayUserControlsLayout() {
	TCHAR buff0[64], buff1[64], buff2[64], buff3[64], buff4[64], buff5[64];
	int controlsLayoutStartingHeight = MAIN_GRID_HEIGHT / 8;
	TextOut(m_hdc, MAIN_GRID_WIDTH + 50, controlsLayoutStartingHeight, buff0, wsprintf(buff0, TEXT("Controls Layout:")));
	TextOut(m_hdc, MAIN_GRID_WIDTH + 50, controlsLayoutStartingHeight + 30, buff1, wsprintf(buff1, TEXT("Spin Piece = Space Bar")));
	TextOut(m_hdc, MAIN_GRID_WIDTH + 50, controlsLayoutStartingHeight + 45, buff2, wsprintf(buff2, TEXT("Move Left = Left Arrow or 'A' Key")));
	TextOut(m_hdc, MAIN_GRID_WIDTH + 50, controlsLayoutStartingHeight + 60, buff3, wsprintf(buff3, TEXT("Move Right = Right Arrow or 'D' Key")));
	TextOut(m_hdc, MAIN_GRID_WIDTH + 50, controlsLayoutStartingHeight + 75, buff4, wsprintf(buff4, TEXT("Hard Drop = Up Arrow or 'W' Key")));
	TextOut(m_hdc, MAIN_GRID_WIDTH + 50, controlsLayoutStartingHeight + 90, buff5, wsprintf(buff5, TEXT("Soft Drop = Down Arrow or 'S' Key")));
}