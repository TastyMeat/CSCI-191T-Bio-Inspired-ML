#include <array>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

struct Vector2
{
public:
    float x() const { return _x; }
    float y() const { return _y; }
    // float sqrMagnitude() const;
    // float magnitude() const;
    // Vector2 normalized() const;

    void Set(const float x, const float y)
    {
        _x = x;
        _y = y;
    }

    // static float Dot(const Vector2&, const Vector2&);
    // static float Distance(const Vector2&, const Vector2&);

    // static Vector2 zero();
    static Vector2 one() { return Vector2(1, 1); }
    // static Vector2 up();
    // static Vector2 down();
    // static Vector2 left();
    // static Vector2 right();

    Vector2(const float x, const float y) : _x(x), _y(y) {}
    Vector2(const Vector2 &obj) : _x(obj.x()), _y(obj.y()) {}

    Vector2 operator+(const Vector2 &obj) const { return Vector2(x() + obj.x(), y() + obj.y()); }
    Vector2 operator-(const Vector2 &obj) const { return Vector2(x() - obj.x(), y() - obj.y()); }
    // Vector2 operator*(const float) const;
    // Vector2 operator/(const float) const;

    Vector2 &operator+=(const Vector2 &obj) { return *this = this->operator+(obj); }
    Vector2 &operator-=(const Vector2 &obj) { return *this = this->operator-(obj); }
    // Vector2& operator*=(const float);
    // Vector2& operator/=(const float);

    // bool operator==(const Vector2&) const;
    // bool operator!=(const Vector2&) const;
private:
    float _x, _y;
};
class Chessboard
{
public:
    enum TileState
    {
        Unoccupied,
        Attacked,
        Occupied,
    };
    Chessboard(int width, int height) : width(width), height(height) {
        if(this->width < this->height) swap(this->width, this->height);
    }
    int width = 0;
    int height = 0;
    static const string dot;
    static const string queen;
    const array<Vector2, 8> knightsMoves = {Vector2(1, 2), Vector2(2, 1), Vector2(2, -1), Vector2(1, -2), Vector2(-1, -2), Vector2(-2, -1), Vector2(-2, 1), Vector2(-1, 2)};

    vector<Vector2> solutions = vector<Vector2>();

    void NQueens()
    {
        static int queenCount = min(width, height);
        for(int i = 0; i < queenCount; i++)
            KnightsMove(Vector2(0, i), vector<vector<TileState> >(width, vector<TileState>(height, Unoccupied)), solutions);
    }

    static void DisplaySolution(const int width, const int height, vector<Vector2> solution)
    { // array or vector of Vector2
        vector<vector<bool> > table(width, vector<bool>(height, false));
        for(int i = 0; i < solution.size(); i++)
        table[solution[i].x()][solution[i].y()] = true;

        for (int y = height - 1; y >= 0; y--)
        {
            for (int x = 0; x < width; x++)
            {
                cout<< (table[x][y] ? queen : dot);
            }
            cout << endl;
        }
    }
//take solution, assign solution into table, print table with solution
private:
    void KnightsMove(const Vector2 position, vector<vector<TileState> > chessboard, vector<Vector2> &solutions)
    {
        UpdateBoardState(position, chessboard);
        for (int i = 0; i < knightsMoves.size(); i++)
        {
            Vector2 nextPosition = position + knightsMoves[i];
            if (chessboard[nextPosition.x()][nextPosition.y()] == Unoccupied)
                KnightsMove(nextPosition, chessboard, solutions);
        }
    }
    void UpdateBoardState(const Vector2 &position, vector<vector<TileState> > &chessboard)
    {
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
    void UpdateDiagonalTiles(const Vector2 &position, const Vector2 &direction, vector<vector<TileState> > &chessboard)
    {
        Vector2 diagonalPosition = position + direction;
        while (0 <= diagonalPosition.x() < width && 0 <= diagonalPosition.y() < height)
        {
            if (chessboard[diagonalPosition.x()][diagonalPosition.y()] == Unoccupied)
                chessboard[diagonalPosition.x()][diagonalPosition.y()] = Attacked;
            diagonalPosition += direction;
        }
    }
};
const string Chessboard::dot = "\u00B7";
const string Chessboard::queen = "\u2655";

int main()
{
    Chessboard::DisplaySolution(5, 5, vector<Vector2>() = {Vector2(1, 2), Vector2(0,1)});
    cout << "hello "
         << "" << endl;
    return 0;
}
/*
- Recursively call knight's move until queens placed = grid width
- On each queen placement, update all affected tiles on chess board
- When a solution is found, add solution to solution vector, and add any other rotations of the solution
- At the end of computation, user is able to request solution index as input to display solution on terminal as chessboard
*/