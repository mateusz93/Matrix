#include <iostream>
#include <fstream>
#include <iomanip>
#include "Matrix.h"

using namespace std;

/* function for disallocation memory */
void freeMemory(double** tab, int x)
{
    for(int i = 0; i < x; ++i)
        delete [] tab[i];
    delete [] tab;
}

Matrix::Matrix(int x, int y)
{
    data = new DataMatrix(x, y);
}

Matrix::Matrix(char const* fileName)
{
    data = new DataMatrix(fileName);
}

Matrix::Matrix(const Matrix& A)
{
    A.data->howManyObjects++;
    data = A.data;
}

Matrix::~Matrix()
{
    if(--data->howManyObjects <= 0)
        freeMemory(data->tab, data->x);
}

Matrix Matrix::operator+(const Matrix& A) const
{
    if(data->x != A.data->x || data->y != A.data->y) throw WrongSize();
    Matrix temp(data->x, data->y);
    for(int i = 0; i < data->x; ++i)
        for(int j = 0; j < data->y; ++j)
            temp.data->tab[i][j] += A.data->tab[i][j];
    return temp;
}

Matrix Matrix::operator-(const Matrix& A) const
{
    if(data->x != A.data->x || data->y != A.data->y) throw WrongSize();
    Matrix temp(data->x, data->y);
    for(int i = 0; i < data->x; ++i)
        for(int j = 0; j < data->y; ++j)
            temp.data->tab[i][j] -= A.data->tab[i][j];
    return temp;
}

Matrix Matrix::operator*(const Matrix& A) const
{
    if(data->y != A.data->x) throw WrongSize();
    Matrix C(data->x, A.data->y);
    double s;
    for(int i = 0; i < data->x; ++i)
    {
        for(int j = 0; j < A.data->y; ++j)
        {
            for(int k = s = 0; k < data->y; ++k)
                s += data->tab[i][k] * A.data->tab[k][j];
            C.data->tab[i][j] = s;
        }
    }
    return C;
}

Matrix& Matrix::operator=(const Matrix& A)
{
    if(this == &A) return *this;
    A.data->howManyObjects++;
    data = A.data;
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& A)
{
    if(data->x != A.data->x || data->y != A.data->y) throw WrongSize();
    if(data->howManyObjects == 1)
    {
        for(int i = 0; i < data->x; ++i)
            for(int j = 0; j < data->y; ++j)
                data->tab[i][j] += A.data->tab[i][j];
    }
    else
    {
        --data->howManyObjects;
        data->detach();           // odlaczenie wskaznika, zaalokowanie pamieci i przepisanie starej zawartosci
        for(int i = 0; i < data->x; ++i)
            for(int j = 0; j < data->y; ++j)
                data->tab[i][j] += A.data->tab[i][j];
    }
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& A)
{
    if(data->x != A.data->x || data->y != A.data->y) throw WrongSize();
    if(data->howManyObjects == 1)
    {
        for(int i = 0; i < data->x; ++i)
            for(int j = 0; j < data->y; ++j)
                data->tab[i][j] -= A.data->tab[i][j];
    }
    else
    {
        --data->howManyObjects;
        data->detach();           // odlaczenie wskaznika, zaalokowanie pamieci i przepisanie starej zawartosci
        for(int i = 0; i < data->x; ++i)
            for(int j = 0; j < data->y; ++j)
                data->tab[i][j] -= A.data->tab[i][j];
    }
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& A)
{
    if(data->y != A.data->x) throw WrongSize();
    Matrix C(data->x, A.data->y);
    double s;
    for(int i = 0; i < data->x; ++i)
    {
        for(int j = 0; j < A.data->y; ++j)
        {
            for(int k = s = 0; k < data->y; ++k)
                s += data->tab[i][k] * A.data->tab[k][j];
            C.data->tab[i][j] = s;
        }
    }
    --data->howManyObjects;
    data->detach();       // odlaczenie wskaznika, zaalokowanie pamieci i przepisanie starej zawartosci
    *this = C;
    return *this;
}

double Matrix::operator()(const int x, const int y) const
{
    check(x, y);
    return data->tab[x][y];
}

Matrix::MatrixReadWrite Matrix::operator()(const int x, const int y)
{
    check(x, y);
    return MatrixReadWrite(*this, x, y);
}

void Matrix::check(int i, int j) const
{
    if(data->x != i || data->y != j)
        throw WrongSize();
}

double Matrix::read(int x, int y) const
{
    return data->tab[x][y];
}

void Matrix::write(int x, int y, double value)
{
    data = data->detach();
    data->tab[x][y] = value;
}

bool Matrix::operator==(const Matrix& A) const
{
    if(data->x != A.data->x || data->y != A.data->y) return false;
    for(int i = 0; i < data->x; ++i)
        for(int j = 0; j < data->y; ++j)
            if(data->tab[i][j] != A.data->tab[i][j]) return false;
    return true;
}

bool Matrix::readFromFile(char const* fileName)
{
    ifstream file;
    file.open(fileName);
    if(!file.good()) throw FileOpen();
    data->detach();
    file>>data->x>>data->y;
    for(int i = 0; i < data->x; ++i)
        for(int j = 0; j < data->y; ++j)
            file>>data->tab[i][j];
    file.close();
    return true;
}

ostream& operator<<(ostream& out, const Matrix& A)
{
    for(int i = 0; i < A.data->x; ++i)
    {
        for(int j = 0; j < A.data->y; ++j)
            out<<setw(6)<<A.data->tab[i][j];
        out<<endl;
    }
    return out;
}
