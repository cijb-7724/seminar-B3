#include "Point.hpp"

//sをdelimiter区切りでvector<string>にする
vector<string> split(string s,string delimiter){
   s+=delimiter;
   vector<string> ans;
   int pos=0;
   while (pos<s.size()){
      int nextpos=s.find(delimiter,pos);
      if (pos!=nextpos){
         ans.push_back(s.substr(pos,nextpos-pos));
      }
      pos=nextpos+1;
   }
   return ans;
}

//デフォルトコンストラクタ
Point::Point(){
   this->x=0;
   this->y=0;
   this->z=0;
}

//コンストラクタ
Point::Point(const double x,const double y,const double z){
   this->x=x;
   this->y=y;
   this->z=z;
}

//ベクトル加算
Point Point::operator + (const Point p)const{
   return Point(this->x+p.x,this->y+p.y,this->z+p.z);
}

//ベクトル減算
Point Point::operator - (const Point p)const{
   return Point(this->x-p.x,this->y-p.y,this->z-p.z);
}

//ベクトルスカラー乗算
template <typename T>
Point Point::operator * (const T n)const{
   return Point(this->x*n,this->y*n,this->z*n);
}

//内積
double Point::operator * (const Point p)const{
   return this->x*p.x+this->y*p.y+this->z*p.z;
}

//ベクトルスカラー除算
template <typename T>
Point Point::operator / (const T n)const{
   return Point(this->x/n,this->y/n,this->z/n);
}

//代入
Point Point::operator = (const Point p){
   this->x=p.x;
   this->y=p.y;
   this->z=p.z;
   return *this;
}

template <typename T>
Point Point::operator = (const vector<T> p){
   this->x=p[0];
   this->y=p[1];
   this->z=p[2];
   return *this;
}

//比較(sortのために一応用意)
bool Point::operator < (const Point &p) const {
   if (this->x==p.x){
      if (this->y==p.y){
         return this->z<p.z;
      }
      else{
         return this->y<p.y;
      }
   }
   else{
      return this->x<p.x;
   }
}

//比較(sortのために一応用意)
bool Point::operator > (const Point &p)const{
   if (this->x==p.x){
      if (this->y==p.y){
         return this->z>p.z;
      }
      else{
         return this->y>p.y;
      }
   }
   else{
      return this->x>p.x;
   }
}

//加算代入
Point Point::operator += (const Point p){
   (*this)=(*this)+p;
   return *this;
}

//減算代入
Point Point::operator -= (const Point p){
   (*this)=(*this)-p;
   return *this;
}

//比較
bool Point::operator <= (const Point p)const{
   return (*this)<p || (*this)==p;
}

//比較
bool Point::operator >= (const Point p)const{
   return (*this)>p || (*this)==p;
}

//乗算代入
template <typename T>
Point Point::operator *= (const T n){
   (*this)=(*this)*n;
   return *this;
}

//除算代入
template <typename T>
Point Point::operator /= (const T n){
   (*this)=(*this)/n;
   return *this;
}

//等価比較
bool Point::operator == (const Point p)const{
   if (fabs(this->x-p.x)<EPS && fabs(this->y-p.y)<EPS && fabs(this->z-p.z)<EPS){
      return true;
   }
   else{
      return false;
   }
}

//非等価比較
bool Point::operator != (const Point p)const{
   return !((*this)==p);
}

//自身を単位ベクトルにする
void Point::unit(){
  double length=distance(*this,Point(0,0,0));
  if (length==0){
    cerr << "Error : devided by zero" << endl;
    exit(EXIT_FAILURE);
  }
   (*this)/=length;
}

double Point::norm()const{
   return distance(*this,Point(0,0,0));
}

string Point::get_string()const{
   return '('+to_string(x)+", "+to_string(y)+", "+to_string(z)+')';
}

//単位ベクトル化
Point unit(const Point p){
  double length=distance(p,Point(0,0,0));
  if (length==0){
    cerr << "Error : devided by zero" << endl;
    exit(EXIT_FAILURE);
  }
  return p/length;
}

//(ユークリッド)距離
double distance(const Point p1,const Point p2){
   return sqrt((p1-p2)*(p1-p2));
}

//外積
Point outer_product(const Point p1,const Point p2){
   return Point(p1.y*p2.z-p1.z*p2.y,p1.z*p2.x-p1.x*p2.z,p1.x*p2.y-p1.y*p2.x);
}

//頂点リストのcsvの読み込み
vector<Point> load(const string filename){
   ifstream ifs(filename);
   if (!ifs){
      cerr << "Error : file open failure" << endl;
      exit(EXIT_FAILURE);
   }
   vector<Point> d;
   string tmp;
   while (getline(ifs,tmp)){
      vector<string> nums=split(tmp,",");
      d.push_back(Point(stod(nums[0]),stod(nums[1]),stod(nums[2])));
   }
   return d;
}

//各点の最近傍点の距離のリスト
vector<double> nearest_length(const vector<Point> p){
   vector<double>  length(p.size(),-1);
   for (int i=0;i<p.size();i++){
      int t=(!i);
      for (int j=1;j<p.size();j++){
         if (i!=j && distance(p[i],p[j])<distance(p[i],p[t])){
            t=j;
         }
      }
      length[i]=distance(p[i],p[t]);
   }
   return length;
}

//point_listの中で、pからの距離が近い点n個のIndexのリスト
vector<int> get_near_point(const vector<Point> point_list, Point p, int n){
   if (n==-1){
      n=point_list.size();
   }
   vector<pair<double,int>> np(point_list.size());
   for (int i=0;i<point_list.size();i++){
      np[i]=make_pair(distance(p,point_list[i]),i);
   }
   sort(np.begin(),np.end());
   vector<int> ans(min((int)(point_list.size()-1),n));
   for (int j=0;j<ans.size();j++){
      ans[j]=np[j+1].second;
   }
   return ans;
}

//この3点の重心
Point circumcenter(const Point p1,const Point p2,const Point p3){
   Point v1=p2-p1;
   Point v2=p3-p1;
   double x = (v1*v1)*(v2*v2)-(v2*v2)*(v1*v2)/((v1*v1)*(v2*v2)-(v1*v2)*(v1*v2))/2;
   double y = (v1*v1)*(v2*v2)-(v1*v1)*(v1*v2)/((v1*v1)*(v2*v2)-(v1*v2)*(v1*v2))/2;
   return p1+v1*y+v2*y;
}

//平行であるかどうか
bool parallel(const Point p1,const Point p2){
   if (unit(p1)==unit(p2) || unit(p1)==unit(p2)*(-1)){
      return true;
   }
   else{
      return false;
   }
}
