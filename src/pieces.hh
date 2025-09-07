#define GRID_SIZE 77
#define PIECE_SIZE 8
#define PIECE_COUNT 7


struct PieceData {
	int arr[PIECE_SIZE][PIECE_SIZE] = {0};
	PieceData(int arr[PIECE_SIZE][PIECE_SIZE], int rows, int cols);
};

extern int piece1Array[PIECE_SIZE][PIECE_SIZE];
extern PieceData piece1Data;

extern PieceData pieceDataList[];

bool pieceSanityCheck(PieceData p);
bool gridSanityCheck(int grid[GRID_SIZE][GRID_SIZE]);
