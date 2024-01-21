/*
STLデータクラス
実際のSTLの保持の仕方と少し違うデータ保持をする
・STLの内側と外側に関する情報を持たない(法線ベクトルの情報を持たない)
・頂点リスト、頂点のつなぎ方のみによってデータ保持を行う
・実際の物体で表現が可能なデータしか来ないと仮定する(全ての頂点で三角形の面がなされている、覆い切れる)
STLデータ出力時にその整合性全部何とかする(投げやり)

name : モデルの名前
p : 頂点の座標
relate : 頂点の繋がり方。頂点の添え字ベースで繋ぎ方を指定
*/
#ifndef STL
#define STL
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include "Point.hpp"
using ll=long long;
using namespace std;

class Stl{
private:
   string name;//STLファイル内の名前
   vector<Point> p;//頂点リスト
   vector<vector<ll>> relate;//三角形の面の作り方

public:
   Stl();
   void load(string filename);
   void output(string filename);
   void rename(string s);
};

#endif
