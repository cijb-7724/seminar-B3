#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <math.h>
#include <string>
#include <random>
#include <fstream>
#include <chrono> // 時間を計測

#include "Point.hpp"
#include "Point.cpp"

using namespace std;

#define yes "Yes"
#define no "No"
#define yesno(bool) if(bool){cout<<"Yes"<<endl;}else{cout<<"No"<<endl;}

#define tp() cout << "here~~" << endl

using vi = vector<int>;
using vd = vector<double>;
using vvi = vector<vector<int>>;
using vvd = vector<vector<double>>;
using vvvi = vector<vector<vector<int>>>;
using vvvd = vector<vector<vector<double>>>;


long long comb(int n, int r);
void innergetAllPattern(int pn,vector<vector<int>> &allpattern,vector<int> nowstate,int n,int left);
vector<vector<Point>> getAllPattern(vector<Point> ppattern,int n);
vector<vector<Point>> generate_states(void);

//generate random 
random_device rd;
// long long seed = rd();//seedを数値で指定するかrd()で実行毎に変えるか

long long seed = 0;
mt19937 gen(seed);
double rand_double(double mn, double mx) {
    uniform_real_distribution<> dist(mn, mx);//一様分布
    return dist(gen);
}
int rand_int(int mn, int mx) {
    uniform_int_distribution<> dist_int(mn, mx);//一様分布
    return dist_int(gen);
}

void outputTextFile(vd &v, string s) {
    int n = v.size();
    if (s.substr(s.size()-4, 4) != ".txt") s += ".txt";
    ofstream outputFile (s);
    for (int i=0; i<n; ++i) {
        outputFile << v[i] << endl;
    }
}
void outputTextFile2d(vvd &v, string s) {
    int n = v.size(), m = v[0].size();
    if (s.substr(s.size()-4, 4) != ".txt") s += ".txt";
    ofstream outputFile (s);
    for (int i=0; i<n; ++i) {
        for (int j=0; j<m; ++j) {
            outputFile << v[i][j];
            if (j != m-1) outputFile << " ";
        }
        outputFile << endl;
    }
}
// map<pair<int, int>, double> greedy_probs(
//     map<vector<Point>, map<pair<int, int>, double>> &Q,
//     vector<Point> &state,
//     double epsilon=0,
//     int action_size_vertex=8,
//     int action_size_direction=6
//     ) {
//     vvd qs(action_size_vertex, vd(action_size_direction));
//     for (int i=0; i<action_size_vertex; ++i) {
//         for (int j=0; j<action_size_direction; ++j) {
//             qs[i][j] = Q[state][{i, j}];
//         }
//     }

//     double max_elm = -1e6;
//     for (int i=0; i<action_size_vertex; ++i) {
//         max_elm = max(max_elm, *max_element(qs[i].begin(), qs[i].end()));
//     }
    
//     pair<int, int> max_action;
//     for (int i=0; i<action_size_vertex; ++i) {
//         for (int j=0; j<action_size_direction; ++j) {
//             if (qs[i][j] == max_elm) {
//                 max_action = {i, j};
//             }
//         }
//     }
    
//     double base_prob = epsilon / (action_size_vertex * action_size_direction);
//     map<pair<int, int>, double> action_probs;
//     for (int i=0; i<action_size_vertex; ++i) {
//         for (int j=0; j<action_size_direction; ++j) {
//             action_probs[{i, j}] = base_prob;
//         }
//     }
//     action_probs[max_action] += 1-epsilon;
//     return action_probs;
// }

map<pair<int, int>, double> greedy_probs(
    const map<vector<Point>, map<pair<int, int>, double>> &Q,
    const vector<Point> &state,
    double epsilon = 0,
    int action_size_vertex = 8,
    int action_size_direction = 6
    ) {
    // 初期化の一部を変更
    vector<vector<double>> qs(action_size_vertex, vector<double>(action_size_direction, 0.0));

    // ループ内の不要なマップアクセスを削減
    auto stateQ = Q.at(state);
    for (int i = 0; i < action_size_vertex; ++i) {
        for (int j = 0; j < action_size_direction; ++j) {
            qs[i][j] = stateQ[{i, j}];
        }
    }

    // 最大値を計算するときにループ内の関数呼び出しを減らす
    double max_elm = -1e6;
    for (const auto& row : qs) {
        max_elm = max(max_elm, *max_element(row.begin(), row.end()));
    }

    pair<int, int> max_action;

    // max_actionを見つけるときにループ内の条件判定を変更
    for (int i = 0; i < action_size_vertex; ++i) {
        auto max_j = max_element(qs[i].begin(), qs[i].end());
        if (*max_j == max_elm) {
            max_action = {i, static_cast<int>(distance(qs[i].begin(), max_j))};
        }
    }

    // ループ内の不要な初期化を削減
    double base_prob = epsilon / (action_size_vertex * action_size_direction);
    map<pair<int, int>, double> action_probs;
    for (int i = 0; i < action_size_vertex; ++i) {
        for (int j = 0; j < action_size_direction; ++j) {
            action_probs[{i, j}] = base_prob;
        }
    }
    action_probs[max_action] += 1 - epsilon;
    return action_probs;
}


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
vector<vector<Point>> getAllPattern(vector<Point> ppattern,int n){
    //いったん添え字ベースで考える
    vector<vector<int>> allpattern_index;
    allpattern_index.reserve(comb(ppattern.size(),n));
    vector<int> emp={};
    emp.reserve(n);
    innergetAllPattern(ppattern.size(),allpattern_index,emp,n,0);
    
    vector<vector<Point>> allpattern(allpattern_index.size(),vector<Point>(n));
    for (int i=0;i<allpattern.size();i++){
        for (int j=0;j<n;j++){
            allpattern[i][j]=ppattern[allpattern_index[i][j]];
        }
    }
    return allpattern;
}
vector<vector<Point>> generate_states(void) {
    vector<Point> ga;
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            for (int k=0;k<3;k++){
                ga.push_back({i,j,k});
            }
        }
    }
    vector<vector<Point>> gap = getAllPattern(ga, 8);
    return gap;
}

//grid world class p.97~
//---------------------------------------------------------------------
/*
agent : i番目の頂点がPoint型
grid world :
Point型に対して，そこにagentの頂点があったら頂点番号，なかったら-1
*/
class GridWorld {
public:
    int vertices;//頂点数
    vector<int> direction;//移動方向パターン
    map<int, string> direction_mean;//各点の移動の意味
    // bokuzinコメント : コレ↑mapだけど、vector<string>でよくね？
    vector<vector<int>> action_space;//各点ごとの移動パターン　必要？
    vector<vector<Point>> environment;//環境
    map<vector<Point>, double> reward_map;//座標状態列に対応してその価値
    int x_ = 3, y_ = 3, z_ = 3;//方向最大値？
    int state_size;

    vector<Point> goal_state, start_state, agent_state;//座標状態
public:
    GridWorld();
    vvi actions();
    vector<vector<Point>> states(void);
    vector<Point> next_state(vector<Point> &, int, int);
    double reward(vector<Point> &);
    vector<Point> reset(void);
    bool isin(Point &);
    bool moveable(vector<Point> &, int, int);
    tuple<vector<Point>, double, bool> step(pair<int, int> &);
};
GridWorld::GridWorld() {
    this->vertices = 8;
    this->direction = {0, 1, 2, 3, 4, 5};
    this->direction_mean = {{0, "UP"}, {1, "DOWN"}, {2, "LEFT"}, {3, "RIGHT"}, {4, "FRONT"}, {5, "BACK"}};
    for (int i=0; i<vertices; ++i) this->action_space.push_back(direction);
    this->environment = generate_states();
    //reward_map
    for (auto state: this->environment) {
        reward_map[state] = 0;
    }
    goal_state = {{0,0,0}, {0,0,2}, {0,2,0}, {0,2,2}, {2,0,0}, {2,0,2}, {2,2,0}, {2,2,2}};
    start_state = {{0,0,0}, {0,0,1}, {0,1,0}, {0,1,1}, {1,0,0}, {1,0,1}, {1,1,0}, {1,1,1}};
    agent_state = start_state;
    reward_map[goal_state] = 10;
    state_size = environment.size();
    cout << "state_size = " << state_size << endl;
}

vector<vector<int>> GridWorld::actions(void) {
    return this->action_space;
}

vector<vector<Point>> GridWorld::states(void) {
    vector<vector<Point>> vec = generate_states();
    return vec;
}
vector<Point> GridWorld::next_state(vector<Point> &state, int vertex, int direction) {
    //vertex 動かす頂点番号
    //directionの0~5がそれぞれ，上下左右前後の何と対応しているかは確認済み
    vector<Point> action_move_map = {{0, 1, 0}, {0, -1, 0}, {1, 0, 0}, {-1, 0, 0}, {0, 0, 1}, {0, 0, -1}};
    Point move = action_move_map[direction];
    vector<Point> next_state = state;

    //指定された頂点を指定された方向directionに動かす．
    next_state[vertex] += move;

    //next_stateが条件を満たしていない場合動かさないものとする．関数moveableは後で合わせる
    if (!isin(next_state[vertex]) || !moveable(next_state,vertex,direction)) {
        next_state = state;
    }
    else{
      //agentの頂点を昇順に振り付けなおす
      sort(next_state.begin(), next_state.end());//単にこれでいいのか？
    }
    return next_state;
}


// double GridWorld::reward(pair<int, int> state, int, pair<int, int> next_state) {
//     return this->reward_map[next_state.first][next_state.second];
// }
double GridWorld::reward(vector<Point> &next_state) {
    return this->reward_map[next_state];
}
vector<Point> GridWorld::reset(void) {
    this->agent_state = this->start_state;
    return this->agent_state;
}
//範囲内かの判定
bool GridWorld::isin(Point &p){
   int nx = p.x;
   int ny = p.y;
   int nz = p.z;
   if (nx < 0 || nx >= this->x_ || ny < 0 || ny >= this->y_ || nz < 0 || nz >= this->z_){
      return false;
   }
   else{
      return true;
   }
}
//動けるかどうかの判定
bool GridWorld::moveable(vector<Point> &state, int vertex, int direction) {
    vector<Point> state_tmp = state;
    sort(state_tmp.begin(), state_tmp.end());
    for (int i=0; i<state_tmp.size()-1; ++i) {
        if (state_tmp[i] == state_tmp[i+1]) return false;
    }
    return true;
}

tuple<vector<Point>, double, bool> GridWorld::step(pair<int, int> &action) {
    vector<Point> state = this->agent_state;
    vector<Point> next_state = this->next_state(state, action.first, action.second);
    double reward = this->reward(next_state);
    bool done = (next_state == this->goal_state);
    this->agent_state = next_state;
    return make_tuple(next_state, reward, done);
}

//---------------------------------------------------------------------

//---------------------------------------------------------------------
class McAgent {
public:
    double gamma;
    double epsilon;
    double alpha;
    int action_size_vertex;//頂点の選び方通り数
    int action_size_direction;//方向の選び方の通り数
    map<pair<int, int>, double> random_actions;//どの頂点，方向に対していくらの確率か
    map<vector<Point>, map<pair<int, int>, double>> pi;//各状態に対して各行動をする確率

    //行動価値関数=ある状態において，ある行動をした時の価値double
    map<vector<Point>, map<pair<int, int>, double>> Q;

    vector<tuple<vector<Point>, pair<int, int>, double>> memory;//{state, action, reward}のtupleのvector

public:
    McAgent();
    pair<int, int> get_action(vector<Point> &);
    void add(vector<Point> &, pair<int, int> &, double &);
    void reset(void);
    void update(void);
};

McAgent::McAgent() {
    this->gamma = 0.9;
    this->epsilon = 0.1;
    this->alpha = 0.1;
    this->action_size_vertex = 8;
    this->action_size_direction = 6;

    for (int i=0; i<this->action_size_vertex; ++i) {
        for (int j=0; j<this->action_size_direction; ++j) {
            this->random_actions[{i, j}] = (double)1 / (this->action_size_vertex * this->action_size_direction);
        }
    }
    GridWorld tmp;
    auto env = tmp.environment;
    for (vector<Point> state: env) {
        this->pi[state] = this->random_actions;
    }
}

pair<int, int> McAgent::get_action(vector<Point> &state) {
    map<pair<int, int>, double> action_probs = this->pi[state];//<vertex, direction>, prob
    vector<pair<int, int>> index;
    vector<double> probs;
    for (auto itr = action_probs.begin(); itr != action_probs.end(); ++itr) {
        index.push_back(itr->first);
        if (itr == action_probs.begin()) probs.push_back(itr->second);
        else probs.push_back(*(--probs.end()) + itr->second);
    }
    double tmpP = rand_double(0.0, 1.0);
    for (int i=0; i<index.size(); ++i) {
        if (probs[i] >= tmpP) return index[i];
    }

}
void McAgent::add(vector<Point> &state, pair<int, int> &action, double &reward) {
    tuple<vector<Point>, pair<int, int>, double> data = {state, action, reward};
    this->memory.push_back(data);
}
void McAgent::reset(void) {
    this->memory.resize(0);
}
void McAgent::update(void) {
    double G = 0;
    vector<tuple<vector<Point>, pair<int, int>, double>> mem = this->memory;
    reverse(mem.begin(), mem.end());
    cout << "reverse" << endl;
    long long cnt = 0;
    for (auto data: mem) {
        ++cnt;
        if (cnt % 1000000 == 0) cout << cnt << endl;
        auto start_time = std::chrono::high_resolution_clock::now();
        vector<Point> &state = get<0>(data);
        const pair<int, int> &action = get<1>(data);
        const double& reward = get<2>(data);
        G = this->gamma * G + reward;
        this->Q[state][action] += (G - this->Q[state][action]) * this->alpha;
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        // cout << "G caluculate time: " << duration.count() << " microseconds" << endl;

        // start_time = std::chrono::high_resolution_clock::now();
        map<pair<int, int>, double> action_prob = greedy_probs(this->Q, state, this->epsilon);
        // end_time = std::chrono::high_resolution_clock::now();
        // duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        // cout << "greedy_probs caluculate time: " << duration.count() << " microseconds" << endl;

        // start_time = std::chrono::high_resolution_clock::now();
        // for (int i=0; i<action_size_vertex; ++i) {
        //     for (int j=0; j<action_size_direction; ++j) {
        //         this->pi[state][{i, j}] = action_prob[{i, j}];
        //     }
        // }
        pi[state] = action_prob;
        // end_time = std::chrono::high_resolution_clock::now();
        // duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        // cout << "pi update caluculate time: " << duration.count() << " microseconds" << endl;
        
    }
}

void show_state(vector<Point> &state) {
    for (int i=0; i<state.size(); ++i) {
        cout << "(" << state[i].x << "," << state[i].y << "," << state[i].z << ") ";
    }
    cout << endl;
}

int main() {
    GridWorld env;
    McAgent agent;
    int episodes = 10000;
    for (int episode=0; episode<episodes; ++episode) {
        cout << "epsode = " << episode << endl;
        vector<Point> state = env.reset();
        show_state(state);
        agent.reset();
        long long cnt = 0;
        while (true) {
            ++cnt;
            if (cnt % 1000000 == 0) cout << cnt << endl;
            pair<int, int> action = agent.get_action(state);
            // cout << "get action" << endl;
            tuple<vector<Point>, double, bool>  step = env.step(action);
            // cout << "step" << endl;
            vector<Point> next_state = get<0>(step);
            double reward = get<1>(step);
            bool done = get<2>(step);
            if (state != next_state) agent.add(next_state, action, reward);//動いたときはmemoryに記録
            // cout << "add" << endl;
            if (done) {
                cout << cnt << endl;
                cout << agent.memory.size() << endl;
                agent.update();
                cout << "episode " << episode << " done" << endl;
                break;
            }
            state = next_state;
            // show_state(state);
        }
    }
    cout << "learn finished" << endl;
    // cout << fixed << setprecision(2);
    // for (int i=0; i<env.height(); ++i) {
    //     for (int j=0; j<env.width(); ++j) {
    //         // cout << "i, j = " << i << ',' << j << endl;
    //         if (env.wall_state == make_pair(i, j)) {
    //             cout << "W";
    //             continue;
    //         } else if (env.goal_state == make_pair(i, j)) {
    //             cout << "G";
    //             continue;
    //         }
    //         double mx = -10;
    //         for (int k=0; k<agent.action_size; ++k) {
    //             // cout << env.action_meaning[k] << " : ";
    //             // cout << agent.Q[{i, j}][k] << ' ';
    //             mx = max(mx, agent.Q[{i, j}][k]);
    //         }
    //         for (int k=0; k<agent.action_size; ++k) {
    //             if (agent.Q[{i, j}][k] == mx) {
    //                 if (env.action_meaning[k] == "LEFT") cout << "<";
    //                 else if (env.action_meaning[k] == "RIGHT") cout << ">";
    //                 else if (env.action_meaning[k] == "UP") cout << "^";
    //                 else cout << "v";
    //             }
    //         }
    //         // cout << endl;
    //     }
    //     cout << endl;
    // }
}


/*



*/


 