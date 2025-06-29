#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <conio.h>
#include <windows.h>

using namespace std;

const int WIDTH = 10;
const int HEIGHT = 20;
const int INITIAL_SPEED = 500; // Initial speed in milliseconds

vector<vector<char>> board(HEIGHT, vector<char>(WIDTH, ' '));
vector<vector<vector<char>>> tetrominos = {
    { // "T" shape
        {' ', '#', ' '},
        {'#', '#', '#'},
        {' ', ' ', ' '},
    },
    { // "L" shape
        {' ', ' ', '#'},
        {'#', '#', '#'},
        {' ', ' ', ' '},
    },
    { // "S" shape
        {' ', '#', '#'},
        {'#', '#', ' '},
        {' ', ' ', ' '},
    }
};

int currentShape;
int currentX, currentY;
int score = 0;
int currentSpeed = INITIAL_SPEED; // Initial speed

void drawBoard() {
    system("cls"); // Clear the screen on Windows
    // Print top border
    cout << "+";
    for (int i = 0; i < WIDTH; i++)
        cout << "-";
    cout << "+" << endl;
    // Print board content with side borders
    for (int i = 0; i < HEIGHT; i++) {
        cout << "|";
        for (int j = 0; j < WIDTH; j++) {
            if (i >= currentY && i < currentY + 3 && j >= currentX && j < currentX + 3) {
                cout << tetrominos[currentShape][i - currentY][j - currentX];
            }
            else {
                cout << board[i][j];
            }
        }
        cout << "|" << endl;
    }
    // Print bottom border
    cout << "+";
    for (int i = 0; i < WIDTH; i++)
        cout << "-";
    cout << "+" << endl;
    // Print score
    cout << "Score: " << score << endl;
}

bool isValidMove(int x, int y, int shape) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tetrominos[shape][i][j] != ' ' && (x + j < 0 || x + j >= WIDTH || y + i >= HEIGHT || board[y + i][x + j] != ' '))
                return false;
        }
    }
    return true;
}

void placeShape() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tetrominos[currentShape][i][j] != ' ')
                board[currentY + i][currentX + j] = tetrominos[currentShape][i][j];
        }
    }
}

void clearLine(int line) {
    for (int i = line; i > 0; i--) {
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = board[i - 1][j];
        }
    }
    for (int j = 0; j < WIDTH; j++) {
        board[0][j] = ' ';
    }
}

int checkLines() {
    int linesCleared = 0;
    for (int i = HEIGHT - 1; i >= 0; i--) {
        bool isLineFull = true;
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == ' ') {
                isLineFull = false;
                break;
            }
        }
        if (isLineFull) {
            clearLine(i);
            linesCleared++;
        }
    }
    return linesCleared;
}

bool spawnNewShape() {
    currentShape = rand() % tetrominos.size();
    currentX = WIDTH / 2 - 1;
    currentY = 0;
    // Check if the new shape can be placed
    if (!isValidMove(currentX, currentY, currentShape)) {
        return false; // Game over condition
    }
    return true;
}

void moveShapeDown() {
    if (isValidMove(currentX, currentY + 1, currentShape)) {
        currentY++;
    }
    else {
        placeShape();
        score += checkLines() * 100; // Update score based on lines cleared
        if (!spawnNewShape()) {
            cout << "Game Over!" << endl;
            cout << "Final Score: " << score << endl;
            exit(0); // Exit the game
        }
        currentSpeed = INITIAL_SPEED; // Reset speed when a new shape spawns
    }
}

void moveShapeLeft() {
    if (isValidMove(currentX - 1, currentY, currentShape)) {
        currentX--;
    }
}

void moveShapeRight() {
    if (isValidMove(currentX + 1, currentY, currentShape)) {
        currentX++;
    }
}

void rotateShape() {
    vector<vector<char>> rotated(3, vector<char>(3));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            rotated[j][2 - i] = tetrominos[currentShape][i][j];
        }
    }
    if (isValidMove(currentX, currentY, currentShape)) {
        tetrominos[currentShape] = rotated;
    }
}

// Function to adjust speed over time
void adjustSpeedOverTime() {
    // Decrease speed gradually over time
    if (currentSpeed > 100) {
        currentSpeed -= 5; // Adjust this value for desired speed increase rate
    }
}

void play() {
    char input;
    while (true) {
        drawBoard();
        moveShapeDown();
        Sleep(currentSpeed); // Sleep for current speed milliseconds
        adjustSpeedOverTime(); // Adjust speed over time
        if (_kbhit()) {
            input = _getch();
            if (input == 'a' || input == 'A') {
                moveShapeLeft();
            }
            else if (input == 'd' || input == 'D') {
                moveShapeRight();
            }
            else if (input == 's' || input == 'S') {
                moveShapeDown();
            }
            else if (input == 'w' || input == 'W') {
                rotateShape();
            }
        }
    }
}

int main() {
    cout << "____________________Welcome_______________________\n";
    cout << "____Tetris____\n";
    cout << "Objective:\nThe objective of Tetris is to score points by clearing lines of blocks, which are called tetrominoes, that fall from the top of the playing field to the bottom.The game ends when the stack of tetrominoes reaches the top of the playing field.\n";
    cout << "INSTRUCTIONS:\n1. Use the W,A,S,D to rotate the piece, move left, move down and move right respectively\n";
    cout << "Press any key to start the game...\n";
    _getch(); // Wait for a key press
    srand(time(0)); // Seed random number generator
    if (!spawnNewShape()) {
        cout << "Game Over!" << endl;
        cout << "Final Score: " << score << endl;
        return 0; // Immediate game over if first shape cannot be placed
    }
    play();
    return 0;
}
