#include <iostream>
#include <vector>
#include <cmath>
#include <random>
using namespace std;

#define EPS (0.001)
#define G (0.9)
#define ALPHA (0.01)

const pair<int,int> action[4] = {{0,1},{1,0},{0,-1},{-1,0}};

class Policy;
class StateValueFunction;

class Policy{
private:
   int height,width;
   vector<vector<vector<double>>> p;

   int argmax(vector<double> arr);
   bool isin(int h,int w);
public:
   Policy(int h,int w);
   void set(int h,int w,vector<double> pp);
   vector<vector<vector<double>>> getpolicy();
   vector<double> getpolicy(int h,int w);
   void improveDP(StateValueFunction s,vector<vector<double>> rw,vector<vector<bool>> wall);
   void show(const vector<vector<bool>> wall,const pair<int,int> goal);
};

class StateValueFunction{
private:
   int height,width;
   double gamma;
   vector<vector<double>> svf;

   bool isin(int h,int w);
public:
   StateValueFunction(int h,int w,double g);
   double getgamma();
   void calc(const vector<vector<double>> rw,const vector<vector<bool>> wall,Policy p,const pair<int,int> goal);
   void improveVI(const vector<vector<double>> rw,const vector<vector<bool>> wall,const pair<int,int> goal);
   void improveTD(const vector<vector<double>> rw,const vector<vector<bool>> wall,const pair<int,int> goal,double alpha,int episodes);
   double getsvf(int i,int j);
   vector<vector<double>> getsvf();
   void show();
};

int Policy::argmax(vector<double> arr){
   int m=0;
   for (int i=0;i<arr.size();i++){
      if (arr[m]<arr[i]){
         m=i;
      }
   }
   return m;
}

bool Policy::isin(int h,int w){
   if (0<=h && h<height && 0<=w && w<width){
      return true;
   }
   else{
      return false;
   }
}

Policy::Policy(int h,int w) : p(h,vector<vector<double>>(w,vector<double>(4,0.25))){
   this->height=h;
   this->width=w;
}

void Policy::set(int h,int w,vector<double> pp){
   p[h][w]=pp;
}

vector<vector<vector<double>>> Policy::getpolicy(){
   return p;
}

vector<double> Policy::getpolicy(int h,int w){
   return p[h][w];
}

void Policy::improveDP(StateValueFunction s,vector<vector<double>> rw,vector<vector<bool>> wall){
   for (int i=0;i<height;i++){
      for (int j=0;j<width;j++){
         vector<double> arr(4);
         for (int a=0;a<4;a++){
            int movedi=i+action[a].first;
            int movedj=j+action[a].second;
            if (!isin(movedi,movedj) || wall[movedi][movedj]){
               movedi=i;
               movedj=j;
            }
            arr[a]=rw[movedi][movedj]+s.getgamma()*s.getsvf(movedi,movedj);
         }
         p[i][j]={0,0,0,0};
         p[i][j][argmax(arr)]=1;
      }
   }
}

void Policy::show(const vector<vector<bool>> wall,const pair<int,int> goal){
   for (int i=0;i<height;i++){
      for (int j=0;j<width;j++){
         int dir=argmax(p[i][j]);
         char c;
         if (dir==0) c='>';
         else if (dir==1) c='v';
         else if (dir==2) c='<';
         else c='^';
         if (wall[i][j] || goal==make_pair(i,j)) c='.';
         cout << c << ' ';
      }
      cout << endl;
   }
}

bool StateValueFunction::isin(int h,int w){
   if (0<=h && h<height && 0<=w && w<width){
      return true;
   }
   else{
      return false;
   }
}

StateValueFunction::StateValueFunction(int h,int w,double g) : svf(h,vector<double>(w)){
   this->height=h;
   this->width=w;
   this->gamma=g;
}

double StateValueFunction::getgamma(){
   return gamma;
}

void StateValueFunction::calc(const vector<vector<double>> rw,const vector<vector<bool>> wall,Policy p,const pair<int,int> goal){
   double delta=1;
   while (delta>EPS){
      delta=0;
      vector<vector<double>> newsvf(height,vector<double>(width,0));
      for (int i=0;i<height;i++){
         for (int j=0;j<width;j++){
            if (wall[i][j] || goal==make_pair(i,j)){
               continue;
            }
            for (int a=0;a<4;a++){
               int movedi=i+action[a].first;
               int movedj=j+action[a].second;
               if (!isin(movedi,movedj) || wall[movedi][movedj]){
                  movedi=i;
                  movedj=j;
               }
               newsvf[i][j]+=p.getpolicy(i,j)[a]*(rw[movedi][movedj]+gamma*svf[movedi][movedj]);
            }
            delta=max(delta,fabs(svf[i][j]-newsvf[i][j]));
         }
      }
      svf=newsvf;
   }
}

void StateValueFunction::improveVI(const vector<vector<double>> rw,const vector<vector<bool>> wall,const pair<int,int> goal){
   double delta=1;
   while (delta>EPS){
      delta=0;
      vector<vector<double>> newsvf(height,vector<double>(width,0));
      for (int i=0;i<height;i++){
         for (int j=0;j<width;j++){
            if (wall[i][j] || goal==make_pair(i,j)){
               continue;
            }

            for (int a=0;a<4;a++){
               int movedi=i+action[a].first;
               int movedj=j+action[a].second;
               if (!isin(movedi,movedj) || wall[movedi][movedj]){
                  movedi=i;
                  movedj=j;
               }
               newsvf[i][j]=max(newsvf[i][j],rw[movedi][movedj]+gamma*svf[movedi][movedj]);
            }
            delta=max(delta,fabs(svf[i][j]-newsvf[i][j]));
         }
      }
      svf=newsvf;
   }
}

void StateValueFunction::improveTD(const vector<vector<double>> rw,const vector<vector<bool>> wall,const pair<int,int> goal,double alpha,int episodes){
   random_device rd;
   mt19937 mt(rd());
   uniform_int_distribution<int> dist(1,4);

   for (int i=0;i<episodes;i++){
      pair<int,int> loc=make_pair(0,0);
      while (loc!=goal){
         int a=dist(mt)-1;
         pair<int,int> nextloc=loc;
         nextloc.first+=action[a].first;
         nextloc.second+=action[a].second;
         if (!isin(nextloc.first,nextloc.second) || wall[nextloc.first][nextloc.second]){
            nextloc=loc;
         }
         svf[loc.first][loc.second]+=alpha*(rw[nextloc.first][nextloc.second]+gamma*svf[nextloc.first][nextloc.second]-svf[loc.first][loc.second]);
         loc=nextloc;
      }
   }
}

vector<vector<double>> StateValueFunction::getsvf(){
   return svf;
}

double StateValueFunction::getsvf(int i,int j){
   return svf[i][j];
}

void StateValueFunction::show(){
   for (int i=0;i<height;i++){
      for (int j=0;j<width;j++){
         printf("%8.5lf ",svf[i][j]);
      }
      printf("\n");
   }
}

int main(){
   int h=3;
   int w=4;

   vector<vector<bool>> wall(3,vector<bool>(4,false));
   wall[1][1]=true;

   vector<vector<double>> reward(3,vector<double>(4,0));
   reward[2][3]=1;
   reward[1][3]=-1;
   pair<int,int> goal=make_pair(2,3);

   StateValueFunction s(h,w,G);
   int episodes=1000;
   s.improveTD(reward,wall,goal,ALPHA,episodes);
   s.show();
}
