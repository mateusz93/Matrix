#include <iostream>
#include <fstream>
#include <iomanip>
#include "Matrix.h"

using namespace std;

int Matrix::counter = 0; // musi byc zdefiniowana poza definicja klasy

Matrix::Matrix(int x, int y)
{
    this->x = x;
    this->y = y;
    try
    {
        tab = new double* [x];
        for(int i = 0; i < x; ++i)
            tab[i] = new double [y];
    }
    catch(bad_alloc&)
    {
        cout<<"Blad przy alokacji pamieci!"<<endl;
    }
    ++counter;
}

Matrix::Matrix(char const* fileName)
{
    ifstream file;
    file.open(fileName);
    if(!file.good()) throw FileOpen();
    file>>x>>y;
    try
    {
        tab = new double* [x];
        for(int i = 0; i < x; ++i)
            tab[i] = new double [y];
    }
    catch(bad_alloc&)
    {
        cout<<"Blad przy alokacji pamieci!"<<endl;
    }
    for(int i = 0; i < x; ++i)
        for(int j = 0; j < y; ++j)
            file>>tab[i][j];
    file.close();
    ++counter;
}

void freeMemory(double** tab, int x, int y)
{
    for(int i = 0; i < x; ++i)
        delete [] tab[i];
    delete [] tab;
}

Matrix::~Matrix()
{
    freeMemory(tab, x, y);
    --counter;
}

Matrix::Matrix(const Matrix& macierz)
{
    x = macierz.x;
    y = macierz.y;
    try
    {
        tab = new double* [x];
        for(int i = 0; i < x; ++i)
            tab[i] = new double [y];
    }
    catch(bad_alloc&)
    {
        cout<<"Blad przy alokacji pamieci!"<<endl;
    }
    for(int i = 0; i < x; ++i)
        for(int j = 0; j < y; ++j)
            tab[i][j] = macierz.tab[i][j];
}

Matrix Matrix::operator+(const Matrix& macierz) const
{
    if(x != macierz.x || y != macierz.y) throw WrongSize();
    Matrix temp = *this;
    for(int i = 0; i < x; ++i)
        for(int j = 0; j < y; ++j)
            temp.tab[i][j] += macierz.tab[i][j];
    return temp;
}

Matrix Matrix::operator-(const Matrix& macierz) const
{
    if(x != macierz.x || y != macierz.y) throw WrongSize();
    Matrix temp = *this;
    for(int i = 0; i < x; ++i)
        for(int j = 0; j < y; ++j)
            temp.tab[i][j] -= macierz.tab[i][j];
    return temp;
}

Matrix Matrix::operator*(const Matrix& macierz) const
{
    if(y != macierz.x) throw WrongSize();
    Matrix C(x, macierz.y);
    double s;
    for(int i = 0; i < x; ++i)
    {
        for(int j = 0; j < macierz.y; ++j)
        {
            for(int k = s = 0; k < y; ++k)
                s += tab[i][k] * macierz.tab[k][j];
            C.tab[i][j] = s;
        }
    }
    return C;
}

Matrix& Matrix::operator=(const Matrix& macierz)
{
    if(this == &macierz) return *this;
    if(x != macierz.x || y != macierz.y)
    {
        freeMemory(tab, x, y);
        try
        {
            tab = new double* [macierz.x];
            for(int i = 0; i < macierz.x; ++i)
                tab[i] = new double [macierz.y];
        }
        catch(bad_alloc&)
        {
            cout<<"Blad przy alokacji pamieci!"<<endl;
        }
    }
    x = macierz.x;
    y = macierz.y;
    for(int i = 0; i < x; ++i)
        for(int j = 0; j < y; ++j)
            tab[i][j] = macierz.tab[i][j];
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& macierz)
{
    if(x != macierz.x || y != macierz.y) throw WrongSize();
    for(int i = 0; i < x; ++i)
        for(int j = 0; j < y; ++j)
            tab[i][j] += macierz.tab[i][j];
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& macierz)
{
    if(x != macierz.x || y != macierz.y) throw WrongSize();
    for(int i = 0; i < x; ++i)
        for(int j = 0; j < y; ++j)
            tab[i][j] -= macierz.tab[i][j];
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& macierz)
{
    if(y != macierz.x) throw WrongSize();
    Matrix C(x, macierz.y);
    double s;
    for(int i = 0; i < x; ++i)
    {
        for(int j = 0; j < macierz.y; ++j)
        {
            for(int k = s = 0; k < y; ++k)
                s += tab[i][k] * macierz.tab[k][j];
            C.tab[i][j] = s;
        }
    }
    *this = C;
    return *this;
}

double& Matrix::operator()(const int x, const int y)
{
    return tab[x][y];
}

double Matrix::operator()(const int x, const int y) const
{
    return tab[x][y];
}

bool Matrix::operator==(const Matrix& macierz) const
{
    if(x != macierz.x || y != macierz.y) return false;
    for(int i = 0; i < x; ++i)
        for(int j = 0; j < y; ++j)
            if(tab[i][j] != macierz.tab[i][j]) return false;
    return true;
}

bool Matrix::readFromFile(char const* fileName)
{
    ifstream file;
    file.open(fileName);
    if(!file.good()) throw FileOpen();
    freeMemory(tab, x, y);
    file>>x>>y;
    try
    {
        tab = new double* [x];
        for(int i = 0; i < x; ++i)
            tab[i] = new double [y];
    }
    catch(bad_alloc&)
    {
        cout<<"Blad przy alokacji pamieci!"<<endl;
    }
    for(int i = 0; i < x; ++i)
        for(int j = 0; j < y; ++j)
            file>>tab[i][j];
    file.close();
    return true;
}

ostream& operator<<(ostream& out, const Matrix& macierz)
{
    for(int i = 0; i < macierz.x; ++i)
    {
        for(int j = 0; j < macierz.y; ++j)
            out<<setw(6)<<macierz.tab[i][j];
        out<<endl;
    }
    return out;
}
