#include "Stl.hpp"

//デフォルトコンストラクタ
Stl::Stl(){
   ;//DO NOTHING
}

//ASCII形式のSTLデータを読み込む
void Stl::load(string filename){
   ifstream ifs(filename);
   if (!ifs){
      cerr << "Error : file open failure" << endl;
      exit(EXIT_FAILURE);
   }

   string tmp;
   getline(ifs,tmp);//solid #####
   rename(tmp.substr(6));//#####
   vector<Point> fasets;//データ一時読み込み先
   while (getline(ifs,tmp)){// faset normal xx yy zz
      if (tmp.substr(0,8)=="endsolid"){
         break;
      }
      getline(ifs,tmp);//outer loop
      for (int i=0;i<3;i++){
         getline(ifs,tmp);//vertex xx yy zz
         vector<string> nums=split(tmp," ");
         fasets.push_back(Point(stod(nums[1]),stod(nums[2]),stod(nums[3])));
      }
      getline(ifs,tmp);//endloop
      getline(ifs,tmp);//endfaset
   }

   //setに入れることで重複削除
   set<Point> cod;
   for (auto i : fasets){
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
         auto pos=lower_bound(p.begin(),p.end(),fasets[3*i+j]);
         relate[i][j]=distance(p.begin(),pos);
      }
   }
}

//ASCII形式でデータを出力
void Stl::output(string filename){
   ofstream ofs(filename);
   ofs << scientific; // 指数表記で出力
   ofs << "solid " << name << endl;
   for (int i=0;i<relate.size();i++){
      Point unitoutprod = unit(outer_product(p[relate[i][2]]-p[relate[i][0]],p[relate[i][1]]-p[relate[i][0]]));
      ofs << "    faset normal " << unitoutprod.x << ' ' << unitoutprod.y << ' ' << unitoutprod.z << endl;
      ofs << "        outer loop" << endl;
      for (int j=0;j<3;j++){
         ofs << "            vertex " << p[relate[i][j]].x << ' ' << p[relate[i][j]].y << ' ' << p[relate[i][j]].z << endl;
      }
      ofs << "        endloop" << endl;
      ofs << "    endfaset" << endl;
   }
   ofs << "endsolid" << endl;
   ofs << defaultfloat; // 一応戻す
}

// STLファイル内の名前の設定
void Stl::rename(string s){
   name=s;
}
