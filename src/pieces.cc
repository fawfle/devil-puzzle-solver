#include <iostream>

#include "pieces.hh"

// board
// 10 <- 5, 1, 4

// 50
// 50
// 50
// 10
// 8

// 178 pieces total

// fills the remaining spaces in the array with 1s. Organizational
PieceData::PieceData(int arr[PIECE_SIZE][PIECE_SIZE], int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			this->arr[i][j] = arr[i][j];
		}
	}
}

// TODO: Sanity check piece arrays

int piece1Array[PIECE_SIZE][PIECE_SIZE] = {
	{ 1, 0, 1, 2, 1 },
	{ 2, 0, 2, 0, 2 },
	{ 1, 2, 1, 0, 0 },
};
PieceData piece1Data = PieceData(piece1Array, 3, 5);

int piece2Array[PIECE_SIZE][PIECE_SIZE] = {
	{ 1, 2, 1, 0, 0 },
	{ 0, 0, 2, 0, 0 },
	{ 1, 2, 1, 2, 1 },
};
PieceData piece2Data = PieceData(piece2Array, 3, 5);

int piece3Array[PIECE_SIZE][PIECE_SIZE] = {
	{ 0, 0, 0, 2, 0, 0, 0 },
	{ 2, 1, 2, 1, 2, 1, 0 },
	{ 0, 0, 0, 2, 0, 0, 0 },
	{ 0, 0, 0, 1, 2, 1, 2 }
};
PieceData piece3Data = PieceData(piece3Array, 4, 7);

int piece4Array[PIECE_SIZE][PIECE_SIZE] = {
	{ 1, 2, 1, 0, 0 },
	{ 2, 0, 2, 0, 0 },
	{ 1, 0, 1, 2, 1 },
};
PieceData piece4Data = PieceData(piece4Array, 3, 5);

int piece5Array[PIECE_SIZE][PIECE_SIZE] = {
	{ 2, 1, 2, 1, 2, 1, 0, 0 },
	{ 0, 0, 0, 0, 0, 2, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 0, 1 },
	{ 0, 0, 0, 0, 0, 2, 0, 2 },
	{ 0, 0, 0, 0, 0, 1, 2, 1 },
};
PieceData piece5Data = PieceData(piece5Array, 5, 8);

int piece6Array[PIECE_SIZE][PIECE_SIZE] = {
	{ 1, 2, 1 },
	{ 2, 0, 2 },
	{ 1, 0, 1 },
	{ 2, 0, 2 },
	{ 1, 0, 1 },
};
PieceData piece6Data = PieceData(piece6Array, 5, 3);

int piece7Array[PIECE_SIZE][PIECE_SIZE] = {
	{ 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 2, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 2, 0, 0, 0 },
	{ 1, 2, 1, 2, 1, 2 },
};
PieceData piece7Data = PieceData(piece7Array, 5, 6);

 PieceData pieceDataList[PIECE_COUNT] = { piece1Data, piece2Data, piece3Data, piece4Data, piece5Data, piece6Data, piece7Data };

bool pieceSanityCheck(PieceData p) {
	// parity is whatever number is in the odd space
	int parity = -1;
	int oddParity = -1;
	for (int i = 0; i < PIECE_SIZE; i++) {
		for (int j = 0; j < PIECE_SIZE; j++) {
			bool isOddSquare = (i+ j % 2) % 2;
			if (parity == -1 && p.arr[i][j] != 0) {
				parity = ((p.arr[i][j] == 2 && !isOddSquare) || (p.arr[i][j] == 1 && isOddSquare)) ? 1 : 2;
				oddParity = parity == 1 ? 2 : 1;
				continue;
			}

			if ((!isOddSquare && p.arr[i][j] == parity) || (isOddSquare && p.arr[i][j] == oddParity)) {
				std::cout << "FAILED PIECE SANITY CHECK at i: " << i << " j: " << j << " with " << p.arr[i][j] << std::endl;
				return false;
			}
		}
	}

	return true;
}

bool gridSanityCheck(int grid[GRID_SIZE][GRID_SIZE]) {
	// grid[0][0] is 2
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			bool isOddSquare = (i + j % 2) % 2;
			// even square that's 1
			// odd square that's 2
			if ((!isOddSquare && grid[i][j] == 1) || (isOddSquare && grid[i][j] == 2)) {
				std::cout << "FAILED GRID SANITY CHECK at i: " << i << " j: " << j << " with " << grid[i][j] << std::endl;
				return false;
			}
		}
	}

	return true;
}
