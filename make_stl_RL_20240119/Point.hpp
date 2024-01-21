/*
三次元空間座標/ベクトルクラス
*/
#ifndef POINT
#define POINT
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
using namespace std;

vector<string> split(string s,string delimiter);

class Point{
public:
   double x,y,z;

   Point();
   Point(const double x,const double y,const double z);
   Point operator + (const Point p)const;
   Point operator - (const Point p)const;
   template <typename T>
   Point operator * (const T n)const;
   double operator * (const Point p)const;
   template <typename T>
   Point operator / (const T n)const;
   Point operator = (const Point p);
   bool operator < (const Point &p)const;
   bool operator > (const Point p)const;
   Point operator += (const Point p);
   Point operator -= (const Point p);
   bool operator <= (const Point p)const;
   bool operator >= (const Point p)const;
   template <typename T>
   Point operator *= (const T n);
   template <typename T>
   Point operator /= (const T n);
   bool operator == (const Point p)const;
   bool operator != (const Point p)const;
   void unit();
};

Point unit(const Point p);
double distance(const Point p1,const Point p2);
Point outer_product(const Point p1,const Point p2);
vector<Point> load(const string filename);
#endif
