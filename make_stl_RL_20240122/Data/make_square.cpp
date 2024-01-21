/*
1辺EDGEの立方体を覆う点群を生成する。
面一つ当たりN個の点で覆う。
*/
#include <iostream>
#include <fstream>
#include <random>
using namespace std;

#define N (100) //面1つ当たりの点の数
#define EDGE (10) //立方体の1辺の長さ

int main(){
   random_device rd;
   mt19937 mt(rd());
   uniform_real_distribution<double> di(0,EDGE);

   string filename="squere.csv";
   ofstream ofs(filename);

   //x軸、y軸、z軸いずれに垂直か(0:x,1:y,2:z)
   for (int xyz=0;xyz<3;xyz++){
      //垂直な軸に対し=0かどうか
      for (int i=0;i<=1;i++){
         for (int j=0;j<N;j++){
            double point[3]={di(mt),di(mt),di(mt)};
            point[xyz]=EDGE*i; //0 or EDGE
            ofs << point[0] << ',' << point[1] << ',' << point[2] << endl;
         }
      }
   }
   ofs.close();
}
