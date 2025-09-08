#define GRID_SIZE 77
#define PIECE_SIZE 14
#define PIECE_COUNT 200


struct PieceData {
	int arr[PIECE_SIZE][PIECE_SIZE] = {0};
	PieceData(int arr[PIECE_SIZE][PIECE_SIZE], int rows, int cols);
};


extern PieceData pieceDataList[];

bool pieceSanityCheck(PieceData p);
bool gridSanityCheck(int grid[GRID_SIZE][GRID_SIZE]);
bool pieceCellCountSanityCheck(int grid[GRID_SIZE][GRID_SIZE], PieceData pieces[PIECE_COUNT]);
