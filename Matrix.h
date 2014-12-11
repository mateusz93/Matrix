#include <iostream>

using namespace std;

class Matrix
{
private:
    int x, y;
    double **tab;
public:
    static int counter;
    /* exception classess */
    class WrongSize {};
    class FileOpen {};

    Matrix(int x = 1, int y = 1);
    Matrix(const Matrix&);
    Matrix(char const*);
    Matrix& operator=(const Matrix&);
    Matrix operator+(const Matrix&) const;
    Matrix operator-(const Matrix&) const;
    Matrix operator*(const Matrix&) const;
    Matrix& operator+=(const Matrix&);
    Matrix& operator-=(const Matrix&);
    Matrix& operator*=(const Matrix&);
    double& operator()(const int, const int);
    double operator()(const int, const int) const;
    bool readFromFile(char const*);
    bool operator==(const Matrix&) const;
    friend ostream& operator<<(ostream&, const Matrix&);
    friend void freeMemory(double**, int, int);
    ~Matrix();
};
