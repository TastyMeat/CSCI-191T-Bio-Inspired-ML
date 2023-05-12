#include <array>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

struct Vector2 {
public:
    float x() const { return _x; }
    float y() const { return _y; }

    void Set(const float x, const float y) {
        _x = x;
        _y = y;
    }

    Vector2(const float x, const float y): _x(x), _y(y) {}
    Vector2(const Vector2& obj): _x(obj.x()), _y(obj.y()) {}

    Vector2 operator+(const Vector2& obj) const { return Vector2(x() + obj.x(), y() + obj.y()); }
    Vector2 operator-(const Vector2& obj) const { return Vector2(x() - obj.x(), y() - obj.y()); }

    Vector2& operator+=(const Vector2& obj) { return *this = this->operator+(obj); }
    Vector2& operator-=(const Vector2& obj) { return *this = this->operator-(obj); }

    bool operator==(const Vector2& obj) const { return this->x() == obj.x() && this->y() == obj.y(); }
    bool operator!=(const Vector2& obj) const { return !this->operator==(obj); }
private:
    float _x, _y;
};

class Chessboard {
public:
    enum TileState { Unoccupied, Attacked, Occupied, };
    Chessboard(int width, int height): width(width), height(height) {
        //if (this->width < this->height) swap(this->width, this->height);
    }
    int width = 0;
    int height = 0;
    static const array<Vector2, 8> knightsMoves;

    vector<vector<Vector2>> NQueens() {
        if (this->width < this->height) swap(this->width, this->height);

        vector<vector<Vector2>> solutions;

        for (int i = 0; i < height; i++)
            PlaceQueen(Vector2(0, i), vector<vector<TileState> >(width, vector<TileState>(height, Unoccupied)), vector<Vector2>{}, solutions);

        return solutions;
    }

    vector<vector<Vector2>> NQueens_KnightsMove() {
        if (this->width < this->height) swap(this->width, this->height);

        vector<vector<Vector2>> solutions;

        for (int i = 0; i < height; i++)
            KnightsMove(Vector2(0, i), vector<vector<TileState> >(width, vector<TileState>(height, Unoccupied)), vector<Vector2>{}, solutions);

        return solutions;
    }

    static void DisplaySolution(const Chessboard& board, const vector<Vector2>& solution) {
        //take solution, assign solution into table, print table with solution
        vector<vector<bool>> table(board.width, vector<bool>(board.height, false));
        for (int i = 0; i < (int)solution.size(); i++)
            table[solution[i].x()][solution[i].y()] = true;

        cout << endl;
        for (int y = board.height - 1; y >= 0; y--) {
            cout << (y + 1) << " ";
            for (int x = 0; x < board.width; x++) cout << (table[x][y] ? "♕" : "·") << " ";
            cout << endl;
        }
        cout << "  ";
        for (int x = 0; x < board.width; x++) cout << (char)('A' + x) << " ";
        cout << endl;
    }

private:
    void PlaceQueen(const Vector2 position, vector<vector<TileState> > chessboard, vector<Vector2> currentSolution, vector<vector<Vector2>>& solutions) {
        currentSolution.push_back(position);
        UpdateBoardState(position, chessboard);

        if ((int)currentSolution.size() == height) {
            solutions.push_back(currentSolution);
            return;
        }

        for (int x = position.x() + 1; x < width; x++) {
            for (int y = 0; y < height; y++) {
                if (chessboard[x][y] == Unoccupied) PlaceQueen(Vector2(x, y), chessboard, currentSolution, solutions);
            }
        }
    }

    void KnightsMove(const Vector2 position, vector<vector<TileState> > chessboard, vector<Vector2> currentSolution, vector<vector<Vector2>>& solutions) {
        currentSolution.push_back(position);
        UpdateBoardState(position, chessboard);

        if ((int)currentSolution.size() == height) {
            //todo? Rotate solution
            solutions.push_back(currentSolution);
            return;
        }

        for (int i = 0; i < (int)knightsMoves.size(); i++) {
            Vector2 nextPosition = position + knightsMoves[i];
            if (0 <= nextPosition.x() && nextPosition.x() < width && 0 <= nextPosition.y() && nextPosition.y() < height
                && chessboard[nextPosition.x()][nextPosition.y()] == Unoccupied)
                KnightsMove(nextPosition, chessboard, currentSolution, solutions);
        }

        for (int x = position.x() - 1; x >= 0; x -= 2) {
            for (int y = 0; x >= 0 && y < height; y++)
                if (chessboard[x][y] == Unoccupied) KnightsMove(Vector2(x, y), chessboard, currentSolution, solutions);
        }
        //for (int x = position.x() + 1, y = 0; x < width && y < height; y++)
            //if (chessboard[x][y] == Unoccupied) KnightsMove(Vector2(x, y), chessboard, currentSolution, solutions);
    }
    void UpdateBoardState(const Vector2& position, vector<vector<TileState> >& chessboard) {
        chessboard[position.x()][position.y()] = Occupied;
        for (int x = 0; x < width; x++)
            if (chessboard[x][position.y()] == Unoccupied)
                chessboard[x][position.y()] = Attacked;

        for (int y = 0; y < height; y++)
            if (chessboard[position.x()][y] == Unoccupied)
                chessboard[position.x()][y] = Attacked;

        UpdateDiagonalTiles(position, Vector2(1, 1), chessboard);
        UpdateDiagonalTiles(position, Vector2(1, -1), chessboard);
        UpdateDiagonalTiles(position, Vector2(-1, -1), chessboard);
        UpdateDiagonalTiles(position, Vector2(-1, 1), chessboard);
    }
    void UpdateDiagonalTiles(const Vector2& position, const Vector2& direction, vector<vector<TileState> >& chessboard) {
        Vector2 diagonalPosition = position + direction;
        while (0 <= diagonalPosition.x() && diagonalPosition.x() < width && 0 <= diagonalPosition.y() && diagonalPosition.y() < height) {
            if (chessboard[diagonalPosition.x()][diagonalPosition.y()] == Unoccupied)
                chessboard[diagonalPosition.x()][diagonalPosition.y()] = Attacked;
            diagonalPosition += direction;
        }
    }
};
const array<Vector2, 8> Chessboard::knightsMoves = { Vector2(1, 2), Vector2(2, 1), Vector2(2, -1), Vector2(1, -2), Vector2(-1, -2), Vector2(-2, -1), Vector2(-2, 1), Vector2(-1, 2) };

int main() {
    // To display unicode
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);

    Chessboard board(8, 8);
    auto solution = board.NQueens_KnightsMove();

    while (true) {
        static int id = 0;
        cout << endl;
        cout << "Board: " << board.width << " x " << board.height << endl;
        cout << "Solutions Found: " << solution.size() << endl;
        cout << "Showing solution: " << id << endl;
        Chessboard::DisplaySolution(board, solution[id]);

        cout << "Enter next solution: ";
        cin >> id;

        if (id >= solution.size()) break;
    }
    return 0;
}
/*
- Recursively call knight's move until queens placed = grid width
- On each queen placement, update all affected tiles on chess board
- When a solution is found, add solution to solution vector, and add any other rotations of the solution
- At the end of computation, user is able to request solution index as input to display solution on terminal as chessboard
*/