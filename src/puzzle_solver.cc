#include <chrono>
#include <iostream>

#include "dlx.hh"
#include "polyomino.hh"
#include "pieces.hh"
#include "grid.hh"

#include "../glad/include/glad/gl.h"
#include <GLFW/glfw3.h>

using namespace std;

const GLuint WIDTH = 1400, HEIGHT = 1400;

// "workaround" to avoid using dynamically sized arrays
#define GL_GRID_SIZE 77

struct Vec3 {
	float x = 0;
	float y = 0;
	float z = 0;
	Vec3() {};
	Vec3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};


// GLAD gl loading library https://github.com/Dav1dde/glad/tree/glad2
// drawing tutorial https://open.gl/drawing
// drawing tutorial https://antongerdelan.net/opengl/hellotriangle.html

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

const char* vertexSource = R"glsl(
	#version 150 core

	in vec2 position;
	in vec3 color;

	out vec3 Color;

	void main() {
		Color = color;
		gl_Position = vec4(position, 0.0, 1.0);
	}
)glsl";

const char* fragmentSource = R"glsl(
	#version 150 core
	
	in vec3 Color;

	out vec4 outColor;

	void main() {
	outColor = vec4(Color, 1.0);
	}
)glsl";

Vec3 colorArray[GL_GRID_SIZE][GL_GRID_SIZE];

void addQuad(float* vertices, GLuint* elements, int quadIndex, float x, float y, float size) {
	// 4 5 element vertices (to form a square)
	int verticesStart = quadIndex * 5 * 4;
	// 2 3 vertex elements (triangles)
	int elementsStart = quadIndex * 3 * 2;
	// 4 vertices per quad
	int relativeElementsStart = quadIndex * 4;
	for (int i = 0; i < 4; i++) {
		int xi = i % 2;
		int yi = i / 2;
		bool oddSquare = quadIndex % 2 == 0;
		vertices[verticesStart + i * 5 + 0] = x * size + (oddSquare ? 0.1 : 0) * size + xi * size * (oddSquare ? 0.8 : 1);
		vertices[verticesStart + i * 5 + 1] = y * size + (oddSquare ? 0.1 : 0) * size + yi * size * (oddSquare ? 0.8 : 1);
		// get color from color matrix
		vertices[verticesStart + i * 5 + 2] = colorArray[quadIndex % GL_GRID_SIZE][quadIndex / GL_GRID_SIZE].x;
		vertices[verticesStart + i * 5 + 3] = colorArray[quadIndex % GL_GRID_SIZE][quadIndex / GL_GRID_SIZE].y;
		vertices[verticesStart + i * 5 + 4] = colorArray[quadIndex % GL_GRID_SIZE][quadIndex / GL_GRID_SIZE].z;
	}

	GLuint relativeElements[] = {
		0, 1, 2,
		1, 2, 3
	};

	
	for (int i = 0; i < 6; i++) {
		elements[elementsStart + i] = relativeElementsStart + relativeElements[i];
	}
}

void generateGridVertices(float *vertices, GLuint *elements, int gridSize) {
	float size = 2.0 / (gridSize);

	for (uint i = 0; i < gridSize * gridSize; i++) {
		int x = i % gridSize;
		int y = gridSize - 1 - (i / gridSize); // flip y so (0,0) is top left. Matches matrixes
		addQuad(vertices, elements, i, x - gridSize / 2.0, y - gridSize / 2.0, size);
	}
}

// create a checkerboard for base grid
void generateCheckeredBoard(Vec3 (&colors)[GL_GRID_SIZE][GL_GRID_SIZE]) {
	for (int x = 0; x < GL_GRID_SIZE; x++) {
		for (int y = 0; y < GL_GRID_SIZE; y++) {
			int oddSquare = (abs(x) + (abs(y) % 2)) % 2;
			float color = 0.6 + 0.1 * oddSquare;
			colors[x][y] = Vec3(color, color, color);
		}
	}
}

// set 0s in grid to black
void cutoutGrid(Vec3 (&colors)[GL_GRID_SIZE][GL_GRID_SIZE], int grid[GRID_SIZE][GRID_SIZE]) {
	for (int x = 0; x < GL_GRID_SIZE; x++) {
		for (int y = 0; y < GL_GRID_SIZE; y++) {
			// x and y are flipped in 2d grid array
			if (grid[y][x] == 0) colors[x][y] = Vec3(0, 0, 0);
		}
	}
}

double randd() {
  return (double)rand() / (RAND_MAX + 1.0);
}

int main() {
	// Convert grid from cutout to usable grid
	if (!gridSanityCheck(gridArray)) {
		cout << "grid cutout check failed" << endl;
		return -1;
	}
	convertGrid(gridArray);

	Vec3 colors[200];

	for (int i = 0; i < 200; i++) {
		colors[i] = Vec3(randd(), randd(), randd());
	}

	generateCheckeredBoard(colorArray);

	if (!gridSanityCheck(gridArray)) {
		return -1;
	}
	
	cutoutGrid(colorArray, gridArray);

	vector<int> moves;

	int matrixHeight;
	int matrixWidth;

	vector<Piece> pieces;
	
	
	for (int i = 0; i < PIECE_COUNT; i++) {
		/*
		cout << "Piece: " << i + 1 << endl;

		for (int r = 0; r < PIECE_SIZE; r++) {
			for (int c = 0; c < PIECE_SIZE; c++) {
				cout << pieceDataList[i].arr[r][c] << " ";
			}
			cout << endl;
		}
		*/
		if (!pieceSanityCheck(pieceDataList[i])) {
			cout << "failed at piece index: " << i << endl;
			return -1;
		}

		pieces.push_back(Piece(pieceDataList[i].arr));
	}
	

	createPolyominoMatrix(gridArray, moves, pieces, matrixHeight, matrixWidth);

	/*
	for (int i = 0; i < matrixHeight; i++) {
		for (int j = 0; j < matrixWidth; j++) {
			std::cout << moves.at(i * matrixWidth + j) << " ";
		}
		std::cout << std::endl;
	}
	*/

	int *matrix = new int[moves.size()];

	// https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
	matrix = moves.data();
	//std::copy(moves.begin(), moves.end(), matrix);

	cout << "Height: " << matrixHeight << endl;
	cout << "Width: " << matrixWidth << endl;

	ExactCoverSolver solver = ExactCoverSolver(matrixHeight, matrixWidth, matrix, true);
	// cover blocked grid squares
	solver.coverZeroColumns();

	chrono::steady_clock::time_point start = chrono::steady_clock::now();
	solver.search();
	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	//solver.printNumberOfSolutions();
	std::cout << "Solution: ";
	solver.printSolution();

	auto solution = solver.getSolution();
	if (solution.size() != 0 && solution.size() != PIECE_COUNT) cout << "SANITY CHECK FAILED: SOLUTIONS NOT SAME SIZE AS PIECE COUNT (solution size " << solution.size() << " != piece count " << PIECE_COUNT << ")\n";

	cout << "Elapsed = " << chrono::duration_cast<chrono::microseconds>(end - start).count() << "[µs]" << std::endl;
	
	// return 0;
	
	// set color array to match solution
	for (uint i = 0; i < solution.size(); i++) {
		int startIndex = solution.at(i)->row * matrixWidth;
		int pieceIndex = 0;

		for (int j = 0; j < PIECE_COUNT; j++) {
			cout << solver.getMatrix()[startIndex + j];
			if (solver.getMatrix()[startIndex + j] == 1) {
				pieceIndex = j;
				//break;
			}
		}


		cout << "|";

		for (int j = 0; j < matrixWidth - PIECE_COUNT; j++) {
			cout << solver.getMatrix()[startIndex + pieces.size() + j] << "";
			if (solver.getMatrix()[startIndex + PIECE_COUNT + j] == 1) {
				int x, y;
				columnToCellIndex(j, y, x);
				colorArray[x][y] = colors[pieceIndex];
			}
		}

		cout << endl;
	}


	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);


	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Puzzle Solver", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	int version = gladLoadGL(glfwGetProcAddress);
	printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);



	/*
	float vertices[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f
	};
	*/

	/*
	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};
	*/
	
	

	float vertices[GL_GRID_SIZE * GL_GRID_SIZE * 4 * 5];
	GLuint elements[GL_GRID_SIZE * GL_GRID_SIZE * 6];

	generateGridVertices(vertices, elements, GL_GRID_SIZE);

	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo = 0;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

			// Create Shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		cerr << "Error compiling vertex shader.\n";
		cerr << buffer;
		return -1;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		cerr << "Error compiling fragment shader. \n";
		cerr << buffer;
		return -1;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(colAttrib);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor((206.0 / 255.0), (187.0 / 255.0), (167.0 / 255.0), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, GL_GRID_SIZE * GL_GRID_SIZE * 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	//delete[] vertices;
	//delete[] elements;

	return 0;
}
