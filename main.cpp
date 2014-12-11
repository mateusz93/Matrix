#include <iostream>
#include <fstream>
#include <iomanip>
#include "Matrix.h"

using namespace std;

/* zaimplementowano referecne counting */

int main()
{

    try
    {
        double G;
        Matrix A(3, 3);
        Matrix B(2, 2);
        Matrix C("exampleMatrix.txt");
        if(B.readFromFile("exampleMatrix2.txt") && A.readFromFile("exampleMatrix3.txt"))
        {
            Matrix D;
            cout<<"Macierz A:\n"<<A<<endl;
            cout<<"Macierz B:\n"<<B<<endl;
            D = A * B;
            cout<<"Macierz D = A x B:"<<endl;
            cout<<D<<endl;
            cout<<"Obecnie sa stworzone "<<Matrix::counter<<" obiekty typu Matrix"<<endl;
            A *= B;
            cout<<"Macierz A *= B\n"<<A<<endl;
        }
        else
            throw Matrix::FileOpen();
        A(1, 1) = 3.4;
        A(1, 2) = 31.2;
        A(0, 1) = 22.4;
        cout<<"A(1, 1) = 3.4;\nA(1, 2) = 31.2;\nA(0, 1) = 22.4;"<<endl;
        cout<<endl<<"Zmieniona macierz A:\n"<<A<<endl;
        cout<<"Macierz A + B:"<<endl;
        G = A(0, 1);
        cout<<endl<<"G = A(0, 1); \nG = "<<G<<endl;
        cout<<"Obecnie sa stworzone "<<Matrix::counter<<" obiekty typu Matrix"<<endl;
    }
    catch(Matrix::WrongSize)
    {
        cout<<"Nieprawidlowe rozmiary tablic"<<endl;
    }
    catch(Matrix::FileOpen)
    {
        cout<<"Blad przy otwarciu pliku"<<endl;
    }
    return 0;
}
