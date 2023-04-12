#include <array>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

struct Vector2 {
public:
    float x() const { return _x; }
    float y() const { return _y; }
    // float sqrMagnitude() const;
    // float magnitude() const;
    // Vector2 normalized() const;

    void Set(const float x, const float y) {
        _x = x;
        _y = y;
    }

    // static float Dot(const Vector2&, const Vector2&);
    // static float Distance(const Vector2&, const Vector2&);

    Vector2(const float x, const float y): _x(x), _y(y) {}
    Vector2(const Vector2& obj): _x(obj.x()), _y(obj.y()) {}

    Vector2 operator+(const Vector2& obj) const { return Vector2(x() + obj.x(), y() + obj.y()); }
    Vector2 operator-(const Vector2& obj) const { return Vector2(x() - obj.x(), y() - obj.y()); }
    // Vector2 operator*(const float) const;
    // Vector2 operator/(const float) const;

    Vector2& operator+=(const Vector2& obj) { return *this = this->operator+(obj); }
    Vector2& operator-=(const Vector2& obj) { return *this = this->operator-(obj); }
    // Vector2& operator*=(const float);
    // Vector2& operator/=(const float);

    // bool operator==(const Vector2&) const;
    // bool operator!=(const Vector2&) const;
private:
    float _x, _y;
};
class Chessboard {
public:
    enum TileState {
        Unoccupied,
        Attacked,
        Occupied,
    };
    Chessboard(int width, int height): width(width), height(height) {
        //if (this->width < this->height) swap(this->width, this->height);
    }
    int width = 0;
    int height = 0;
    static const string dot;
    static const string queen;
    const array<Vector2, 8> knightsMoves = { Vector2(1, 2), Vector2(2, 1), Vector2(2, -1), Vector2(1, -2), Vector2(-1, -2), Vector2(-2, -1), Vector2(-2, 1), Vector2(-1, 2) };

    vector<vector<Vector2>> solutions;

    void NQueens() {
        if (this->width < this->height) swap(this->width, this->height);

        for (int i = 0; i < height; i++) {
            cout << "begin: " << i << "(" << width << "," << height << ")" << endl;
            PlaceQueen(Vector2(0, i), vector<vector<TileState> >(width, vector<TileState>(height, Unoccupied)), vector<Vector2>{});
        }
    }

    void NQueens_KnightsMove() {
        if (this->width < this->height) swap(this->width, this->height);

        for (int i = 0; i < height; i++)
            KnightsMove(Vector2(0, i), vector<vector<TileState> >(width, vector<TileState>(height, Unoccupied)), vector<Vector2>{});
    }

    static void DisplaySolution(const Chessboard& board, const vector<Vector2>& solution) { // array or vector of Vector2
        vector<vector<bool> > table(board.width, vector<bool>(board.height, false));
        for (int i = 0; i < (int)solution.size(); i++)
            table[solution[i].x()][solution[i].y()] = true;

        cout << endl;
        for (int y = board.height - 1; y >= 0; y--) {
            cout << (y + 1) << " ";
            for (int x = 0; x < board.width; x++) {
                cout << (table[x][y] ? queen : dot) << " ";
            }
            cout << endl;
        }
        cout << "  ";
        for (int x = 1; x <= board.width; x++) {
            cout << (x) << " ";
        }
        cout << endl;
    }
    //take solution, assign solution into table, print table with solution
private:
    void PlaceQueen(const Vector2 position, vector<vector<TileState> > chessboard, vector<Vector2> solution) {
        //cout << position.x() << "," << position.y();
        solution.push_back(position);
        UpdateBoardState(position, chessboard);

        if ((int)solution.size() == height) {
            solutions.push_back(solution);
            //cout << " solved" << endl;
            return;
        }
        //cout << endl;

        for (int y = 0; y < height; y++) {
            if (y == position.y())continue;
            for (int x = 0; x < width; x++) {
                if (chessboard[x][y] == Unoccupied)
                    PlaceQueen(Vector2(x, y), chessboard, solution);
            }
        }
    }

    void KnightsMove(const Vector2 position, vector<vector<TileState> > chessboard, vector<Vector2> solution) {
        solution.push_back(position);
        UpdateBoardState(position, chessboard);

        if ((int)solution.size() == height) {
            //todo? Rotate solution
            solutions.push_back(solution);
            return;
        }

        for (int i = 0; i < (int)knightsMoves.size(); i++) {
            Vector2 nextPosition = position + knightsMoves[i];
            //cout << nextPosition.x() << "," <<nextPosition.y()<<endl;
            if (0 <= nextPosition.x() && nextPosition.x() < width && 0 <= nextPosition.y() && nextPosition.y() < height &&
                chessboard[nextPosition.x()][nextPosition.y()] == Unoccupied)
                return KnightsMove(nextPosition, chessboard, solution);
        }

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
                if (chessboard[x][y] == Unoccupied)
                    KnightsMove(Vector2(x, y), chessboard, solution);
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
const string Chessboard::dot = "·";
const string Chessboard::queen = "♕";

int main() {
    Chessboard board(8, 8);
    board.NQueens();
    cout << "Solutions: " << board.solutions.size() << endl;
    for (int i = 0; i < min(6, (int)board.solutions.size()); i++)
        Chessboard::DisplaySolution(board, board.solutions[i]);

    while (true) {
        int id = 0;
        cin >> id;
        Chessboard::DisplaySolution(board, board.solutions[id]);
    }
    return 0;
}
/*
- Recursively call knight's move until queens placed = grid width
- On each queen placement, update all affected tiles on chess board
- When a solution is found, add solution to solution vector, and add any other rotations of the solution
- At the end of computation, user is able to request solution index as input to display solution on terminal as chessboard
*/