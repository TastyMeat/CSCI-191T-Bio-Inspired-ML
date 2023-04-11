#include <array>
#include <iostream>
#include <cmath>

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

    //static float Dot(const Vector2&, const Vector2&);
    //static float Distance(const Vector2&, const Vector2&);

    //static Vector2 zero();
    //static Vector2 one();
    //static Vector2 up();
    //static Vector2 down();
    //static Vector2 left();
    //static Vector2 right();

    Vector2(const float x, const float y):_x(x), _y(y) {}
    Vector2(const Vector2& obj): _x(obj.x()), _y(obj.y()) {}

    Vector2 operator+(const Vector2& obj) const { return Vector2(x() + obj.x(), y() + obj.y()); }
    Vector2 operator-(const Vector2& obj) const { return Vector2(x() - obj.x(), y() - obj.y()); }
    //Vector2 operator*(const float) const;
    //Vector2 operator/(const float) const;

    Vector2& operator+=(const Vector2& obj) { return *this = this->operator+(obj); }
    Vector2& operator-=(const Vector2& obj) { return *this = this->operator-(obj); }
    //Vector2& operator*=(const float);
    //Vector2& operator/=(const float);

    //bool operator==(const Vector2&) const;
    //bool operator!=(const Vector2&) const;

private:
    float _x, _y;
};

enum TileState { Unoccupied, Attacked, Occupied, };
array<Vector2, 8> knightsMove{Vector2(1, 2), Vector2(2, 1), Vector2(2, -1), Vector2(1, -2), Vector2(-1, -2), Vector2(-2, -1), Vector2(-2, 1), Vector2(-1, 2)};

class Chessboard {
public:
    Chessboard(int width, int height): width(width), height(height) {}
    int width = 0;
    int height = 0;

    void NQueens() {
        static int queenCount = min(width, height);

        
    }



    static void DisplaySolution(int width, int height){//array or vector of Vector2
        for (int y = 0; y < height; y++) {
            cout << "\u00B7";
            for (int x = 0; x < width; x++)
                cout << "\u2655"<<endl;

        }
    }
};


int main() {
    Chessboard::DisplaySolution(3,3);
    cout << "hello " << "" << endl;
    return 0;
}
/*
- Recursively call knight's move until queens placed = grid width
- On each queen placement, update all affected tiles on chess board
- When a solution is found, add solution to solution vector, and add any other rotations of the solution
- At the end of computation, user is able to request solution index as input to display solution on terminal as chessboard
*/