#include <iostream>
#include <vector>
#include <cmath>
#include <random>
using namespace std;

#define EPS1 (0.001)
#define EPS2 (0.1)
#define G (0.9)
#define ALPHA (0.8)

const pair<int,int> action[4] = {{-1,0},{0,1},{1,0},{0,-1}};

class Policy;
class StateValue;
class ActionValue;

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
   void improveDP(StateValue s,vector<vector<double>> rw,vector<vector<bool>> wall);
   void reflectActionValue(ActionValue av);
   void show(const vector<vector<bool>> wall,const pair<int,int> goal);
};

class StateValue{
private:
   int height,width;
   double gamma;
   vector<vector<double>> svf;

   bool isin(int h,int w);
public:
   StateValue(int h,int w,double g);
   double getgamma();
   void calc(const vector<vector<double>> rw,const vector<vector<bool>> wall,Policy p,const pair<int,int> goal);
   void improveVI(const vector<vector<double>> rw,const vector<vector<bool>> wall,const pair<int,int> goal);
   void improveTD(const vector<vector<double>> rw,const vector<vector<bool>> wall,const pair<int,int> goal,double alpha,int episodes);
   double getsvf(int i,int j);
   vector<vector<double>> getsvf();
   void show();
};

class ActionValue{
private:
   int height,width;
   double gamma;
   vector<vector<vector<double>>> av;

   bool isin(int h,int w);
   int argmax(vector<double> arr);
public:
   ActionValue(int h,int w,double gamma);
   double getgamma();
   vector<double> getav(int i,int j);
   void show();
   void sarsa(const vector<vector<double>> rw,const vector<vector<bool>> wall,const pair<int,int> goal,double alpha,double eps,int episodes);
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

void Policy::improveDP(StateValue s,vector<vector<double>> rw,vector<vector<bool>> wall){
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

void Policy::reflectActionValue(ActionValue av){
   for (int i=0;i<height;i++){
      for (int j=0;j<width;j++){
         p[i][j]={0,0,0,0};
         int act=argmax(av.getav(i,j));
         p[i][j][act]=1;
      }
   }
}

void Policy::show(const vector<vector<bool>> wall,const pair<int,int> goal){
   for (int i=0;i<height;i++){
      for (int j=0;j<width;j++){
         int dir=argmax(p[i][j]);
         char c;
         if (dir==0) c='^';
         else if (dir==1) c='>';
         else if (dir==2) c='v';
         else c='<';
         if (wall[i][j]) c='.';
         if (goal==make_pair(i,j)) c='G';
         cout << c << ' ';
      }
      cout << endl;
   }
}

bool StateValue::isin(int h,int w){
   if (0<=h && h<height && 0<=w && w<width){
      return true;
   }
   else{
      return false;
   }
}

StateValue::StateValue(int h,int w,double g) : svf(h,vector<double>(w)){
   this->height=h;
   this->width=w;
   this->gamma=g;
}

double StateValue::getgamma(){
   return gamma;
}

void StateValue::calc(const vector<vector<double>> rw,const vector<vector<bool>> wall,Policy p,const pair<int,int> goal){
   double delta=1;
   while (delta>EPS1){
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

void StateValue::improveVI(const vector<vector<double>> rw,const vector<vector<bool>> wall,const pair<int,int> goal){
   double delta=1;
   while (delta>EPS1){
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

void StateValue::improveTD(const vector<vector<double>> rw,const vector<vector<bool>> wall,const pair<int,int> goal,double alpha,int episodes){
   random_device rd;
   mt19937 mt(rd());
   uniform_int_distribution<int> dist(0,3);

   for (int i=0;i<episodes;i++){
      pair<int,int> loc=make_pair(0,0);
      while (loc!=goal){
         int a=dist(mt);
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

vector<vector<double>> StateValue::getsvf(){
   return svf;
}

double StateValue::getsvf(int i,int j){
   return svf[i][j];
}

void StateValue::show(){
   for (int i=0;i<height;i++){
      for (int j=0;j<width;j++){
         printf("%8.5lf ",svf[i][j]);
      }
      printf("\n");
   }
}

bool ActionValue::isin(int h,int w){
   if (0<=h && h<height && 0<=w && w<width){
      return true;
   }
   else{
      return false;
   }
}

int ActionValue::argmax(vector<double> arr){
   int ans=0;
   for (int i=0;i<arr.size();i++){
      if (arr[ans]<arr[i]){
         ans=i;
      }
   }
   return ans;
}

ActionValue::ActionValue(int h,int w,double g) : av(h,vector<vector<double>>(w,vector<double>(4,0))){
   this->height=h;
   this->width=w;
   this->gamma=g;
}

double ActionValue::getgamma(){
   return gamma;
}

vector<double> ActionValue::getav(int i,int j){
   return av[i][j];
}

void ActionValue::show(){
   for (int i=0;i<height*3;i++){
      for (int j=0;j<width*3;j++){
         if (i%3==0 && j%3==1){
            printf("%5.2lf ",av[i/3][j/3][0]);
         }
         else if (i%3==1 && j%3==2){
            printf("%5.2lf ",av[i/3][j/3][1]);
         }
         else if (i%3==2 && j%3==1){
            printf("%5.2lf ",av[i/3][j/3][2]);
         }
         else if (i%3==1 && j%3==0){
            printf("%5.2lf ",av[i/3][j/3][3]);
         }
         else {
            printf("      ");
         }
      }
      printf("\n\n");
   }
}

void ActionValue::sarsa(const vector<vector<double>> rw,const vector<vector<bool>> wall,const pair<int,int> goal,double alpha,double eps,int episodes){
   random_device rd;
   mt19937 mt(rd());
   uniform_int_distribution<int> dist03(0,3);
   uniform_real_distribution<double> dist01(0.0,1.0);

   for (int ep=0;ep<episodes;ep++){
      pair<int,int> loc=make_pair(0,0);
      int act=argmax(av[loc.first][loc.second]);
      while (loc!=goal){
         pair<int,int> nextloc = make_pair(loc.first+action[act].first,loc.second+action[act].second);
         if (!isin(nextloc.first,nextloc.second) || wall[nextloc.first][nextloc.second]){
            nextloc=loc;
         }
         int r=rw[nextloc.first][nextloc.second];
         int nextact=argmax(av[nextloc.first][nextloc.second]);
         if (dist01(rd)<eps){
            nextact=dist03(rd);
         }

         av[loc.first][loc.second][act]+=alpha*(r+gamma*av[nextloc.first][nextloc.second][nextact]-av[loc.first][loc.second][act]);

         loc=nextloc;
         act=argmax(av[nextloc.first][nextloc.second]);
         if (dist01(rd)<eps){
            act=dist03(rd);
         }
      }
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

   ActionValue av(h,w,G);
   int episodes=10000;
   av.sarsa(reward,wall,goal,ALPHA,EPS2,episodes);
   av.show();

   Policy p(h,w);
   p.reflectActionValue(av);
   p.show(wall,goal);
}
