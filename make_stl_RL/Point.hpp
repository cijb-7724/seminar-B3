/*
三次元空間座標クラス
*/
#ifndef POINT
#define POINT
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

vector<string> split(string s,string delimiter);

class Point{
public:
   double x,y,z;

   Point();
   Point(double x,double y,double z);
   Point operator + (Point p);
   Point operator - (Point p);
   template <typename T>
   Point operator * (T n);
   double operator * (Point p);
   template <typename T>
   Point operator / (T n);
   Point operator = (Point p);
   bool operator < (Point p);
   bool operator > (Point p);
   Point operator += (Point p);
   Point operator -= (Point p);
   bool operator <= (Point p);
   bool operator >= (Point p);
   template <typename T>
   Point operator *= (T n);
   template <typename T>
   Point operator /= (T n);
   bool operator == (Point p);
   bool operator != (Point p);
};

double distance(Point p1,Point p2);
vector<Point> load(string filename);
#endif
