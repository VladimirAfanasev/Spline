//  main.cpp
//  Spline
//
//  Created by Владимир Афанасьев on 13.03.2020.
//  Copyright © 2020 Владимир Афанасьев. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

ifstream inp;
ofstream out("out.txt");
ofstream func("func.txt");
ofstream dots("dots.txt");
double eps = 0.001;
double A;
double B;
double* g;
double a;
double b;
double* X;//узлы
int N;//количество узлов
double* value;// значение в узлах
double f(double x)//функция
{
    if(x>-15)
    {
        return 100 + 3 * cos(x / 3);
    }
    else
    {
        return 93;
    }
}
void Init()
{/*
  if (inp.is_open())
  {
  inp >> N >> A >> B;
  value = new double[N];
  X = new double[N];
  
  
  for (int i = 0; i < N; i++)
  {
  inp >> X[i];
  }
  
  for (int i = 0; i < N; i++)
  {
  value[i] = f(X[i]);
  }
  }
  else
  cout << "ERROR OPEN FILE";*/
    N = 9;
    X = new double[N];
    value = new double[N];
    a = -30;
    b = -10;
    double step = (b - a) / (N-1);
    for (int i = 0; i < N; i++)
    {
        X[i] = a + i * step;
        value[i] = f(X[i]);
    }
    
    A = 15;
    B = -7;
}

void tridiag(double* a, double* b, double* d)// метод прогонки
{
    double* xi;
    double* eta;
    double denom;
    xi = new double[N];
    eta = new double[N];
    //прямой ход
    xi[1] = 0;
    eta[1] = A;
    for (int i = 1; i < N - 1; i++)
    {
        denom = a[i] * xi[i] + b[i];
        xi[i + 1] = -a[i + 1] / denom;
        eta[i + 1] = (d[i] - a[i] * eta[i]) / denom;
    }
    g[N - 1] = B;
    for (int i = N - 2; i > -1; i--)
    {
        g[i] = xi[i + 1] * g[i + 1] + eta[i + 1];
    }
    return;
}
void makeSpline() {
    Init();
    g = new double[N];
    double* h;
    double* d;
    d = new double[N];
    h = new double[N];
    for (int i = 1; i < N; i++)
    {
        h[i] = X[i] - X[i - 1];
    }
    for (int i = 1; i < N - 1; i++) {
        d[i] = (value[i + 1] - value[i]) / h[i + 1] - (value[i] - value[i - 1]) / h[i];
    }
    d[0] = A;
    d[N - 1] = B;
    double* FirstCoef;
    double* SecondCoef;
    FirstCoef = new double[N];
    SecondCoef = new double[N];
    
    
    for (int i = 0; i < N; i++) {
        FirstCoef[i] = h[i] / 6;
    }
    for (int i = 1; i < N - 1; i++) {
        SecondCoef[i] = 2 * (FirstCoef[i] + FirstCoef[i + 1]);
    }
    SecondCoef[0] = 1;
    SecondCoef[N - 1] = 1;
    tridiag(FirstCoef, SecondCoef, d);
    
}
double valSpline(double x) {
    int i = 0;
    if (x < X[0])
    {
        
    }
    else if (x > X[N - 2])
    {
        i = N - 2;
    }
    else
    {
        while (x >= X[i])
        {
            i++;
        }
        i--;
        
    }
    double h = X[i + 1] - X[i];
    double FirstT = value[i] * (X[i + 1] - x);
    double SecondT = value[i + 1] * (x - X[i]);
    double ThirdT = (g[i] * ((X[i + 1] - x) * (X[i + 1] - x) * (X[i + 1] - x) - h * h * (X[i + 1] - x))) / 6;
    double FourthT = (g[i + 1] * ((x - X[i]) * (x - X[i]) * (x - X[i]) - h * h * (x - X[i]))) / 6;
    double ans = (FirstT + SecondT + ThirdT + FourthT) / h;
    return ans;
}
void output()
{
    double x = X[0];
    if (out.is_open() && func.is_open()) {
        while (x < X[N - 1])
        {
            func << x << " " << f(x) << endl;
            out << x << " " << valSpline(x) << endl;
            x += 0.01;
            
        }
        x = X[N - 1];
        func << x << " " << f(x) << endl;
        out << x << " " << valSpline(x) << endl;
        for (int i = 0; i < N; i++)
        {
            dots << X[i] << " " << value[i] << endl;
        }
    }
    
    else
        cout << "error" << endl;
}
double d2S(double x) {
    return(valSpline(x) - 2 * valSpline(x + eps) + valSpline(x + 2 * eps)) / (eps * eps);
}
int main() {
    
    makeSpline();
    unsigned int count = 0;
    while (count<4)
    {
        
        cout << "eps = " << eps << ":" << endl << "second derivative in left end = " << (valSpline(a) - 2 * valSpline(a + eps) + valSpline(a + 2 * eps)) / (eps * eps) << endl;
        cout << "second derivative in right end = " << (valSpline(b) - 2 * valSpline(b - eps) + valSpline(b - 2 * eps)) / (eps * eps )<< endl;
        eps = eps * 0.1;
        count++;
    }
    output();
    
    delete[] g;
    delete[] X;
    delete[] value;
    return 0;
    
}
