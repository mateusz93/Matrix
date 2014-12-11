#include <iostream>

using namespace std;

class Matrix
{
    /* class for store data */
    class DataMatrix;
    DataMatrix* data;
public:
    /* class for write and read */
    class MatrixReadWrite;
    /* exception classes */
    class WrongSize {};
    class FileOpen {};

    Matrix(int = 0, int = 0);
    Matrix(const Matrix&);
    Matrix(char const*);
    Matrix& operator=(const Matrix&);
    Matrix operator+(const Matrix&) const;
    Matrix operator-(const Matrix&) const;
    Matrix operator*(const Matrix&) const;
    Matrix& operator+=(const Matrix&);
    Matrix& operator-=(const Matrix&);
    Matrix& operator*=(const Matrix&);
    void check(int, int) const;
    double read(int, int) const;
    void write(int, int, double);
    bool readFromFile(char const*);
    bool operator==(const Matrix&) const;
    double operator()(int, int) const;
    MatrixReadWrite operator()(int, int);
    friend ostream& operator<<(ostream&, const Matrix&);
    friend void freeMemory(double**, int);
    ~Matrix();
};

void freeMemory(double**, int);
void rewrite(double**, double**, int, int);

class Matrix::DataMatrix
{
//    DataMatrix(const DataMatrix&);
//    DataMatrix& operator=(const DataMatrix&);
    friend class Matrix;
public:
    int howManyObjects, x, y;
    double** tab;

    DataMatrix(int x, int y)
    {
        howManyObjects = 1;
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
            cout<<"Blad przy alokowaniu pamieci"<<endl;
        }
    };
    DataMatrix(char const* fileName)
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
    }
    ~DataMatrix()
    {
        freeMemory(tab, x);
    };
    DataMatrix* detach()
    {
        if(howManyObjects == 1)
            return this;
        --howManyObjects;
        DataMatrix* t = new DataMatrix(x, y);
        for(int i = 0; i < x; ++i)
            for(int j = 0; j < y; ++j)
                t->tab[i][j] = tab[i][j];
        return t;
    };
};

class Matrix::MatrixReadWrite
{
    friend class Matrix;
    Matrix& tmp;
    int x, y;
    MatrixReadWrite(Matrix& a, int xx, int yy): tmp(a), x(xx), y(yy) {};
public:
    operator double() const
    {
        // cout << "operator char() const"<<endl;
        return tmp.read(x, y);
    }
    Matrix::MatrixReadWrite& operator=(double value)
    {
        // cout << "void operator = (char c)"<<endl;
        tmp.write(x, y, value);
        return *this;
    }
    Matrix::MatrixReadWrite& operator=(const MatrixReadWrite& ref)
    {
        return operator=((double)ref);
    }
};
