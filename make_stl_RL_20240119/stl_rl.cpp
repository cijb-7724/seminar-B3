/*
強化学習を用いて、点群のデータからSTLデータを整形する
*/
#include "Point.hpp"
#include "Stl.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;


int main(){
   vector<Point> dots=load("squere.csv");

   //点群の各軸方向の範囲
   vector<pair<double,double>> range(3);
   range[0].first=range[0].second=dots[0].x;
   range[1].first=range[1].second=dots[0].y;
   range[2].first=range[2].second=dots[0].z;
   for (auto d : dots){
      range[0].first=min(range[0].first,d.x);
      range[0].second=max(range[0].second,d.x);
      range[1].first=min(range[1].first,d.y);
      range[1].second=max(range[1].second,d.y);
      range[2].first=min(range[2].first,d.z);
      range[2].second=max(range[2].second,d.z);
   }
   /*
   cout << "x軸方向の最小 : " << range[0].first << endl;
   cout << "x軸方向の最大 : " << range[0].second << endl;
   cout << "y軸方向の最小 : " << range[1].first << endl;
   cout << "y軸方向の最大 : " << range[1].second << endl;
   cout << "z軸方向の最小 : " << range[2].first << endl;
   cout << "z軸方向の最大 : " << range[2].second << endl;
   */

   
}
