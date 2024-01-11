#include "Point.hpp"

//sをdelimiter区切りでvector<string>にする
vector<string> split(string s,string delimiter){
   s+=delimiter;
   vector<string> ans;
   int pos=0;
   while (pos<s.size()){
      int nextpos=s.find(delimiter,pos);
      ans.push_back(s.substr(pos,nextpos-pos));
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
Point::Point(double x,double y,double z){
   this->x=x;
   this->y=y;
   this->z=z;
}

//ベクトル加算
Point Point::operator + (Point p){
   return Point(this->x+p.x,this->y+p.y,this->z+p.z);
}

//ベクトル減算
Point Point::operator - (Point p){
   return Point(this->x-p.x,this->y-p.y,this->z-p.z);
}

//ベクトルスカラー乗算
template <typename T>
Point Point::operator * (T n){
   return Point(this->x*n,this->y*n,this->z*n);
}

//内積
double Point::operator * (Point p){
   return this->x*p.x+this->y*p.y+this->z*p.z;
}

//ベクトルスカラー除算
template <typename T>
Point Point::operator / (T n){
   return Point(this->x/n,this->y/n,this->z/n);
}

//代入
Point Point::operator = (Point p){
   this->x=p.x;
   this->y=p.y;
   this->z=p.z;
   return *this;
}

//比較(sortのために一応用意)
bool Point::operator < (Point p){
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
bool Point::operator > (Point p){
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
Point Point::operator += (Point p){
   (*this)=(*this)+p;
   return *this;
}

//減算代入
Point Point::operator -= (Point p){
   (*this)=(*this)-p;
   return *this;
}

//比較
bool Point::operator <= (Point p){
   return (*this)<p || (*this)==p;
}

//比較
bool Point::operator >= (Point p){
   return (*this)>p || (*this)==p;
}

//乗算代入
template <typename T>
Point Point::operator *= (T n){
   (*this)=(*this)*n;
   return *this;
}

//除算代入
template <typename T>
Point Point::operator /= (T n){
   (*this)=(*this)/n;
   return *this;
}

//等価比較
bool Point::operator == (Point p){
   if (this->x==p.x && this->y==p.y && this->z==p.z){
      return true;
   }
   else{
      return false;
   }
}

//非等価比較
bool Point::operator != (Point p){
   return !((*this)==p);
}

//(ユークリッド)距離
double distance(Point p1,Point p2=Point(0,0,0)){
   return sqrt(p1*p2);
}

//頂点リストのcsvの読み込み
vector<Point> load(string filename){
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
