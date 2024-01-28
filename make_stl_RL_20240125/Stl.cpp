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

//vector<Point>形式で、データを渡す
vector<Point> Stl::point_data(){
   return p;
}

//Ball-Pivoting Algo
void Stl::construct_stl(vector<Point> point){
   sort(point.begin(),point.end());
   this->p=point;

   vector<double> length=nearest_length(point);
   double radius=0;
   for (auto l : length){
      radius+=l;
   }
   radius/=length.size();
   radius*=2;
   cout << radius << endl;

   set<pair<int,int>> active_edge;
   Point center;
   Point circent;

   {//変数残したくなかったので…
      vector<int> tmp=get_near_point(p,p[0],1);
      int near_point1=tmp[0];
      tmp=get_near_point(p,(p[near_point1]+p[0])/2.0,2);
      int near_point2=tmp[1];
      active_edge.insert({0,near_point1});
      active_edge.insert({0,near_point2});
      active_edge.insert({min(near_point1,near_point2),max(near_point1,near_point2)});
      Point out_manifold = {-1,-1,-1};//明らかに外部な点

      Point vec1 = p[near_point1]-p[0];
      Point vec2 = p[near_point2]-p[0];
      Point vec3 = unit(outer_product(vec1,vec2)*(outer_product(vec1,vec2)*out_manifold)/(outer_product(vec1,vec2)*outer_product(vec1,vec2)));

      circent = circumcenter(p[0],p[near_point1],p[near_point2]);

      double d = distance(circent,p[0]);
      double h = sqrt(radius*radius - d*d);
      center = circent+vec3*h;

      if (unit(outer_product(vec1,vec2))==vec3){
         vector<ll> relate_tmp = {0,near_point1,near_point2};
         relate.push_back(relate_tmp);
      }
      else{
         vector<ll> relate_tmp = {0,near_point2,near_point1};
         relate.push_back(relate_tmp);
      }
   }

   while(!active_edge.empty()){
      auto edge = *begin(active_edge);
      active_edge.erase(edge);
      int pi1=edge.first,pi2=edge.second;
      Point new_center,new_circent;
      int new_vertex_index;

      vector<int> tmp = get_near_point(p,(p[pi1]+p[pi2])/2.0);
      int i=1;
      while (true){
         i++;
         new_circent = circumcenter(p[pi1],p[pi2],p[new_vertex_index]);
         if (new_circent == circent){
            continue;
         }
         new_vertex_index=tmp[i];
         Point vec1 = p[pi2]-p[pi1];
         Point vec2 = p[new_vertex_index]-p[pi1];
         Point vec3 = unit(outer_product(vec1,vec2));
         double d = distance(circent,p[pi1]);
         double h = sqrt(radius*radius - d*d);
         Point new_center1 = new_circent+vec3*h;
         Point new_center2 = new_circent-vec3*h;

         if (parallel(center,new_center1)){
            new_center=new_center1;
         }
         else{
            new_center=new_center2;
         }
         if (unit(new_center-new_circent)==unit(outer_product(vec1,vec2))){
            relate.push_back({pi1,pi2,new_vertex_index});
         }
         else{
            relate.push_back({pi1,new_vertex_index,pi2});
         }
         break;
      }
      auto new_edge1=make_pair(min(pi1,new_vertex_index),max(pi1,new_vertex_index));
      auto new_edge2=make_pair(min(pi2,new_vertex_index),max(pi2,new_vertex_index));
      if (0<active_edge.count(new_edge1)){
         active_edge.erase(new_edge1);
      }
      else{
         active_edge.insert(new_edge1);
      }
      if (0<active_edge.count(new_edge2)){
         active_edge.erase(new_edge2);
      }
      else{
         active_edge.insert(new_edge2);
      }
      center=new_center;
      circent=new_circent;

   }
}
