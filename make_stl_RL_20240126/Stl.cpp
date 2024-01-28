#include "Stl.hpp"

//デフォルトコンストラクタ
Stl::Stl(){
   rename("default");
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

   //各点の最近傍の点までの距離を得る。
   vector<double> length=nearest_length(point);
   //Ballの半径を最近傍点間の距離の2倍とする。
   double radius=0;
   for (auto l : length){
      radius+=l;
   }
   radius/=length.size();
   radius*=2;

   set<pair<int,int>> active_edge;//一度使った辺
   set<vector<int>> triangle;//既に存在する三角形
   Point center;//現在のBallの点の中心
   Point circent;//現在のBallをなす3点を通る円の中心

   {//変数残したくなかったので…
      vector<int> tmp=get_near_point(p,p[0],1);
      int near_point1=tmp[0];
      tmp=get_near_point(p,(p[near_point1]+p[0])/2.0,2);
      int near_point2=tmp[1];
      active_edge.insert({0,near_point1});
      active_edge.insert({0,near_point2});
      active_edge.insert({min(near_point1,near_point2),max(near_point1,near_point2)});
      vector<int> triangle_edges = {0,min(near_point1,near_point2),max(near_point1,near_point2)};
      triangle.insert(triangle_edges);
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
      //軸辺
      auto edge = *begin(active_edge);
      active_edge.erase(edge);

      cout << "\nedge : " << edge.first << ' ' << edge.second << endl;

      int pi1=edge.first,pi2=edge.second;
      Point new_center,new_circent;
      int new_vertex_index;

      //辺の中点の近傍点リスト
      vector<int> tmp = get_near_point(p,(p[pi1]+p[pi2])/2.0);
      //次の三角形をなす点を探索
      for (int i=2;true;i++){
         new_vertex_index=tmp[i];
         new_circent = circumcenter(p[pi1],p[pi2],p[new_vertex_index]);
         cout << "checking dot : " << new_vertex_index << endl;
         cout << "checking circent : " << new_circent.get_string() << endl;
         vector<int> triangle_edges = {pi1,pi2,new_vertex_index};
         sort(triangle_edges.begin(),triangle_edges.end());
         if (triangle.count(triangle_edges)>0){
            continue;
         }
         {//既に存在する辺と交差するような辺は作れない
            bool flag=false;
            for (auto k : active_edge){
               cout << k.first << ' ' << k.second << endl;
               if (is_cross(p[pi1],p[new_vertex_index],p[k.first],p[k.second]) || is_cross(p[pi2],p[new_vertex_index],p[k.first],p[k.second])){
                  flag=true;
                  break;
               }
            }
            if (flag){
               continue;
            }
         }
         Point vec1 = p[pi2]-p[pi1];
         Point vec2 = p[new_vertex_index]-p[pi1];
         Point vec3 = unit(outer_product(vec1,vec2));
         double d = distance(new_circent,p[pi1]);
         double h = sqrt(radius*radius - d*d);
         Point new_center1 = new_circent+vec3*h;
         Point new_center2 = new_circent-vec3*h;
         cout << "sasasasa" << endl;
         if (center==new_center1 || parallel(center-new_center1,circent-new_circent)){
            new_center=new_center1;
         }
         else{
            new_center=new_center2;
         }
         if (unit(new_center-new_circent)==unit(outer_product(vec1,vec2))){
            relate.push_back({pi1,pi2,new_vertex_index});
            cout << "triangle : " << pi1 << ' ' << pi2 << ' ' << new_vertex_index << endl;
         }
         else{
            relate.push_back({pi1,new_vertex_index,pi2});
            cout << "triangle : " << pi1 << ' ' << new_vertex_index << ' ' << pi2 << endl;
         }
         triangle.insert(triangle_edges);

         cout << "circumcenter : " << new_circent.get_string() << endl;
         cout << "normal : " << unit(new_center-new_circent).get_string() << endl;

         break;
      }
      auto new_edge1=make_pair(min(pi1,new_vertex_index),max(pi1,new_vertex_index));
      auto new_edge2=make_pair(min(pi2,new_vertex_index),max(pi2,new_vertex_index));

      cout << "newedge \n" << new_edge1.first << ' ' << new_edge1.second << endl;
      cout << new_edge2.first << ' ' << new_edge2.second << endl;

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

      cout << "triangleList" << endl;
      for (auto k : triangle){
         for (auto q : k){
            cout << q << ' ';
         }
         cout << endl;
      }

   }
}
