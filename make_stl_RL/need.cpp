#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

//nCr
long long comb(int n, int r) {
  vector<vector<long long>> v(n + 1,vector<long long>(n + 1, 0));
  for (int i = 0; i < v.size(); i++) {
    v[i][0] = 1;
    v[i][i] = 1;
  }
  for (int j = 1; j < v.size(); j++) {
    for (int k = 1; k < j; k++) {
      v[j][k] = (v[j - 1][k - 1] + v[j - 1][k]);
    }
  }
  return v[n][r];
}

//求められているモノの中で使うもの。ラムダ式にして求められているモノの中で宣言するとキレイだね
void innergetAllPattern(int pn,vector<vector<int>> &allpattern,vector<int> nowstate,int n,int left){
    if (nowstate.size()==n){
        allpattern.push_back(nowstate);
        return;
    }
    for (int i=left;i<pn-(n-nowstate.size()-1);i++){
        nowstate.push_back(i);
        innergetAllPattern(pn,allpattern,nowstate,n,i+1);
        nowstate.pop_back();
    }
}

//求められているモノ
vector<vector<tuple<int,int,int>>> getAllPattern(vector<tuple<int,int,int>> ppattern,int n){
    //いったん添え字ベースで考える
    vector<vector<int>> allpattern_index;
    allpattern_index.reserve(comb(ppattern.size(),n));
    vector<int> emp={};
    emp.reserve(n);
    innergetAllPattern(ppattern.size(),allpattern_index,emp,n,0);
    
    vector<vector<tuple<int,int,int>>> allpattern(allpattern_index.size(),vector<tuple<int,int,int>>(n));
    for (int i=0;i<allpattern.size();i++){
        for (int j=0;j<n;j++){
            allpattern[i][j]=ppattern[allpattern_index[i][j]];
        }
    }
    return allpattern;
}

//main内は検証用コード
int main(void){
    // Your code here!
    vector<tuple<int,int,int>> ga;
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            for (int k=0;k<3;k++){
                ga.push_back({i,j,k});
            }
        }
    }
    
    auto gap = getAllPattern(ga,8);
    cout << gap.size() << endl;
    
    // for (auto p : gap){
    //     for (auto q : p){
    //         cout << get<0>(q) << ' ' << get<1>(q) << ' ' << get<2>(q) << endl;
    //     }
    //     cout << endl;
    // }
    vector<tuple<int, int, int>> tmp = gap[0];
    for (tuple<int, int, int> tmpp: tmp) {
        cout << get<0>(tmpp) << ' ' << get<1>(tmpp) << ' ' << get<2>(tmpp) << endl;
    }

    
}