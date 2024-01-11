#include "Stl.hpp"
using namespace std;

Stl::Stl(){
   ;//DO NOTHING
}

void Stl::load(string filename){
   ifstream ifs(filename);
   if (!ifs){
      cerr << "Error : file open failure" << endl;
      exit(EXIT_FAILURE);
   }

   string tmp;
   getline(ifs,tmp);//solid #####
   rename(tmp.substr(6));//#####
   vector<Point> facets;//ファイル一時読み込み先

   while (getline(ifs,tmp)){// faset normal xx yy zz
      if (tmp==("endsolid "+name)){
         break;
      }
      getline(ifs,tmp);//outer loop
      for (int i=0;i<3;i++){
         getline(ifs,tmp);//vertex xx yy zz
         vector<string> nums=split(tmp," ");
         facets.push_back(Point(stod(nums[1]),stod(nums[2]),stod(nums[3])));
      }
      getline(ifs,tmp);//endloop
      getline(ifs,tmp);//endfaset
   }

   //setに入れることで重複削除
   set<Point> cod;
   for (auto i : facets){
      cod.insert(i);
   }

   //pの構築
   p.resize(cod.size());
   for (int i=0;!cod.empty();i++){
      p[i]=*begin(cod);
      cod.erase(p[i]);
   }

   //relateの構築
   relate.resize(p.size());
   for (int i=0;i<relate.size();i++){
      relate[i]={0,0,0};
      for (int j=0;j<3;j++){
         auto pos=lower_bound(p.begin(),p.end(),facets[3*i+j]);
         relate[i][j]=distance(p.begin(),pos);
      }
   }
}

void Stl::export(string filename){
   ofstream ofs(filename);
   ofs << "solid " << name << endl;
   for (int i=0;i<relate.size();i++){
      ofs << "faset normal " << endl;//ここ追記必須。楽な実装を考える
      ofs << "\touter loop" << endl;
      for (int j=0;j<3;j++){
         ofs << "\t\tvertex " << p[relate[i][j]].x << ' ' << p[relate[i][j]].y << ' ' << p[relate[i][j]].z << endl;
      }
      ofs << "\tendloop" << endl;
      ofs << "endfaset" << endl;
   }
   ofs << "endsolid " << name << endl;
}

// STLファイル内の名前の設定
void Stl::rename(string s){
   name=s;
}
