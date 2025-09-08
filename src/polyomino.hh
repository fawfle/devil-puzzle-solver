#include <vector>

#define GRID_SIZE 77
#define PIECE_SIZE 23


#define MATRIX_WIDTH 456421
#define MATRIX_HEIGHT 6129
#define MATRIX_SIZE 2797404309
// Width: 456421
// Height: 6129
// Size: 2797404309


struct Piece {
	private:
		int startI = 0;
		int endI = PIECE_SIZE - 1;
		int startJ = 0;
		int endJ = PIECE_SIZE - 1;

		void updateBoundingBox();
	public:
		Piece(int arr[PIECE_SIZE][PIECE_SIZE]);

		int arr[PIECE_SIZE][PIECE_SIZE];

		void rotate();

		bool checkZeroRow(int row) const;
		bool checkZeroCol(int col) const;

		void printPiece() const;
		void printBoundingBox() const;

		int effectiveHeight() const { return 1 + endI - startI; };
		int effectiveWidth() const { return 1 + endJ - startJ; };

		// return piece cells relative to bounding box
		int getCellRelative(int i, int j) const { return arr[i + startI][j + startJ]; };

		friend bool operator == (const Piece p1, const Piece p2);
		friend bool operator != (const Piece p1, const Piece p2);
};

long cellToColumnIndex(long i, long j);
void columnToCellIndex(long columnIndex, long &i, long &j);

// get number of valid grid cells, for constructing columns
// int getValidGridCells(int arr[GRID_SIZE][GRID_SIZE]);
bool checkValidPlacement(int grid[GRID_SIZE][GRID_SIZE], Piece piece, int startI, int startJ);

std::vector<Piece> getUniqueOrientations(Piece piece);


void addMoveRow(int grid[GRID_SIZE][GRID_SIZE], std::vector<short> &moves, int pieceCount, int pieceIndex, Piece piece, int startI, int startJ);
void addMoveRows(int grid[GRID_SIZE][GRID_SIZE], std::vector<short> &moves, int pieceCount, int pieceIndex, Piece piece);

// 1d vector with rows of length (getGridSize + pieceCount);
void addAllMoves(int grid[GRID_SIZE][GRID_SIZE], std::vector<short> &moves, int pieceCount, int pieceIndex, Piece piece);

void createPolyominoMatrix(int grid[GRID_SIZE][GRID_SIZE], std::vector<short> &moves, std::vector<Piece> pieces, long &matrixHeight, long &matrixWidth);
// void createPolyominoMatrix(int grid[GRID_SI:w
// ZE][GRID_SIZE], int (&moves)[MATRIX_SIZE], std::vector<Piece> pieces, int &matrixHeight, int &matrixWidth);
