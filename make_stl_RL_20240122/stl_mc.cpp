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
#include <iomanip> //出力桁数

#include "Point.hpp"

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




//generate random
random_device rd;
// long long seed = rd();//seedを数値で指定するかrd()で実行毎に変えるか
/*
seedによってプログラムがどこかで無限ループに入ることがある．
たとえば
2712030358
*/
long long seed = 0;
mt19937 gen(seed);

//mx以上mn未満の乱数を一様分布で返す(整数)
double rand_double(double mn, double mx) {
    uniform_real_distribution<> dist(mn, mx);//一様分布
    return dist(gen);
}

//mx以上mn未満の乱数を一様分布で返す(浮動小数点数)
int rand_int(int mn, int mx) {
    uniform_int_distribution<> dist_int(mn, mx);//一様分布
    return dist_int(gen);
}

//改行区切りでファイル名sで出力
void outputTextFile(vd &v, string s) {
    int n = v.size();
    if (s.substr(s.size()-4, 4) != ".txt") s += ".txt";
    ofstream outputFile (s);
    for (int i=0; i<n; ++i) {
        outputFile << v[i] << endl;
    }
}

//改行/スペース区切りでファイル名sで出力
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

//後で確認する(おい)
vd greedy_probs(map<pair<int, int>, map<int, double>> Q, pair<int, int> state, double epsilon=0, int action_size=4) {
    vd qs(action_size);
    for (int i=0; i<action_size; ++i) qs[i] = Q[state][i];

    int max_action;
    for (int i=0; i<action_size; ++i)
        if (qs[i] == *max_element(qs.begin(), qs.end()))
            max_action = i;

    double base_prob = epsilon / action_size;
    vd action_probs(4, 0);
    action_probs[max_action] += 1-epsilon;
    return action_probs;
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
    map<Point,int> environment;//環境
    map<vector<Point>, double> reward_map;//座標状態列に対応してその価値
    int x_ = 3, y_ = 3, z_ = 3;//方向最大値？

    vector<Point> goal_state, start_state, agent_state;//座標状態
public:
    GridWorld();
    // int height(void);
    // int width(void);
    // pair<int, int> shape();
    vvi actions();
    map<Point,int> states();
    pair<int, int> next_state(pair<int, int>, int);
    double reward(Point next_state);
    vector<Point> reset(void);
    bool isin(Point);
    tuple<pair<int, int>, double, bool> step(int);
};

GridWorld::GridWorld() {
    this->vertices = 8;
    this->direction = {0, 1, 2, 3, 4, 5};
    this->direction_mean = {{0, "UP"}, {1, "DOWN"}, {2, "RIGHT"}, {3, "LEFT"}, {4, "FRONT"}, {5, "BACK"}};
    this->action_space.reserve(vertices);
    for (int i=0; i<vertices; ++i) this->action_space.push_back(direction);

    this->environment.assign(x_, vvi(y_, vi(z_, -1)));//←これの役割下のgenerate_statesに統合してよくない？
    this->environment = generate_states();
    //reward_map

    for (auto state: this->environment) {
        reward_map[state.first] = 0;
    }
    goal_state = {{0,0,0}, {0,0,2}, {0,2,0}, {0,2,2}, {2,0,0}, {2,0,2}, {2,2,0}, {2,2,2}};
    start_state = {{0,0,0}, {0,0,1}, {0,1,0}, {0,1,1}, {1,0,0}, {1,0,1}, {1,1,0}, {1,1,1}};
    agent_state = start_state;
    reward_map[goal_state] = 10;
}
// int GridWorld::height(void) {
//     return this->reward_map.size();
// }
// int GridWorld::width(void) {
//     return this->reward_map[0].size();
// }
// pair<int, int> GridWorld::shape(void) {
//     return {this->height(), this->width()};
// }
vector<vector<int>> GridWorld::actions(void) {
    return this->action_space;
}

/*
{
    {{0, 0, 0}, {0, 0, 1}, {0, 0, 2}, {1, 0, 0}, {1, 0, 1}, {1, 0, 2}, {1, 1, 0}, {1, 1, 1}},
    ...
    {{2, 0, 1}, {2, 0, 2}, {2, 1, 0}, {2, 1, 1}, {2, 1, 2}, {2, 2, 0}, {2, 2, 1}, {2, 2, 2}}
}
*/
map<Point,int> generate_states() {
    map<Point,int> tmp = {};
    return tmp;
}

map<Point,int> GridWorld::states(void) {
    map<Point,int> vec = generate_states();
    // for (int i=0; i<this->vertices; ++i) {

    // }

    return vec;
}

vector<Point> GridWorld::next_state(vector<Point> state, int vertex, int action) {
    //vertex 動かす頂点番号
    //actionの0~5がそれぞれ，上下左右前後の何と対応しているかは確認済み
    vector<Point> action_move_map = {{0, 1, 0}, {0, -1, 0}, {1, 0, 0}, {-1, 0, 0}, {0, 0, 1}, {0, 0, -1}};
    Point move = action_move_map[action];
    vector<Point> next_state = state;

    //指定された頂点を指定された方向actionに動かす．
    next_state[vertex] += move;

    //next_stateが条件を満たしていない場合動かさないものとする．関数moveableは後で合わせる
    if (!isin(next_state) || !moveable(state,vertex,action)) {
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
double GridWorld::reward(vector<Point> next_state) {
    return this->reward_map[next_state];
}

vector<Point> GridWorld::reset(void) {
    this->agent_state = this->start_state;
    return this->agent_state;
}

bool GridWorld::isin(Point p){
   int nx = next_state[vertex].x;
   int ny = next_state[vertex].y;
   int nz = next_state[vertex].z;
   if (nx < 0 || nx >= this->x_ || ny < 0 || ny >= this->y_ || nz < 0 || nz >= this->z_){
      return false;
   }
   else{
      return true;
   }
}

tuple<pair<int, int>, double, bool> GridWorld::step(int action) {
    pair<int, int> state = this->agent_state;
    pair<int, int> next_state = this->next_state(state, action);
    double reward = this->reward(state, action, next_state);
    bool done = (next_state == this->goal_state);
    this->agent_state = next_state;
    return make_tuple(next_state, reward, done);
    // return {next_state, reward, done};
}
//---------------------------------------------------------------------

//---------------------------------------------------------------------
class McAgent {
public:
    double gamma;
    double epsilon;
    double alpha;
    int action_size;
    map<int, double> random_actions;
    map<pair<int, int>, map<int, double>> pi;//各座標（マス）に対して各行動をする確率
    map<pair<int, int>, map<int, double>> Q;//行動価値関数
    map<pair<int, int>, map<int, double>> cnts;//型が変わった？
    vector<tuple<pair<int, int>, int, double>> memory;

public:
    McAgent();
    int get_action(pair<int, int>);
    void add(pair<int, int>, int, double);
    void reset(void);
    void update(void);
};

McAgent::McAgent() {
    this->gamma = 0.9;
    this->epsilon = 0.1;
    this->alpha = 0.1;
    this->action_size = 4;

    this->random_actions = {{0, 0.25}, {1, 0.25}, {2, 0.25}, {3, 0.25}};
    GridWorld tmp;
    int h = tmp.height();
    int w = tmp.width();
    for (int i=0; i<h; ++i) {
        for (int j=0; j<w; ++j) {
            this->pi[{i, j}] = random_actions;
        }
    }
}
int McAgent::get_action(pair<int, int> state) {
    map<int, double> action_probs = this->pi[state];
    vector<int> index;
    vector<double> probs;
    for (auto itr=action_probs.begin(); itr != action_probs.end(); ++itr) {
        index.push_back(itr->first);
        if (itr == action_probs.begin()) probs.push_back(itr->second);
        else probs.push_back(*(--probs.end()) + itr->second);
    }
    double tmpP = rand_double(0.0, 1.0);
    for (int i=0; i<index.size(); ++i) {
        if (probs[i] >= tmpP) return index[i];
    }
    return 0;
}
void McAgent::add(pair<int, int> state, int action, double reward) {
    tuple<pair<int, int>, int, double> data = {state, action, reward};
    this->memory.push_back(data);
}
void McAgent::reset(void) {
    this->memory.resize(0);
}
void McAgent::update(void) {
    double G = 0;
    vector<tuple<pair<int, int>, int, double>> mem = this->memory;
    reverse(mem.begin(), mem.end());
    for (auto data: mem) {
        pair<int, int> state = get<0>(data);
        int action = get<1>(data);
        double reward = get<2>(data);
        G = this->gamma * G + reward;
        // this->cnts[state][action] += 1;
        this->Q[state][action] += (G - this->Q[state][action]) * this->alpha;

        vector<double> action_prob = greedy_probs(this->Q, state, this->epsilon);
        for (int i=0; i<(int)action_prob.size(); ++i) {
            this->pi[state][i] = action_prob[i];
        }
    }
}




int main() {
    cout << "random seed = " << seed << endl;
    GridWorld env;
    McAgent agent;
    int episodes = 10000;
    for (int episode=0; episode<episodes; ++episode) {
        pair<int, int> state = env.reset();
        agent.reset();
        while (true) {
            int action = agent.get_action(state);
            tuple<pair<int, int>, double, bool> step = env.step(action);
            pair<int, int> next_state = get<0>(step);
            double reward = get<1>(step);
            bool done = get<2>(step);

            agent.add(state, action, reward);
            if (done) {
                agent.update();
                // cout << "episode " << episode << " done" << endl;
                // cout << agent.memory.size() << endl;
                break;
            }
            state = next_state;
        }
    }
    cout << fixed << setprecision(2);
    for (int i=0; i<env.height(); ++i) {
        for (int j=0; j<env.width(); ++j) {
            // cout << "i, j = " << i << ',' << j << endl;
            if (env.wall_state == make_pair(i, j)) {
                cout << "W";
                continue;
            } else if (env.goal_state == make_pair(i, j)) {
                cout << "G";
                continue;
            }
            double mx = -10;
            for (int k=0; k<agent.action_size; ++k) {
                // cout << env.action_meaning[k] << " : ";
                // cout << agent.Q[{i, j}][k] << ' ';
                mx = max(mx, agent.Q[{i, j}][k]);
            }
            for (int k=0; k<agent.action_size; ++k) {
                if (agent.Q[{i, j}][k] == mx) {
                    if (env.action_meaning[k] == "LEFT") cout << "<";
                    else if (env.action_meaning[k] == "RIGHT") cout << ">";
                    else if (env.action_meaning[k] == "UP") cout << "^";
                    else cout << "v";
                }
            }
            // cout << endl;
        }
        cout << endl;
    }
}


/*

aaaxxxxxaaa
xxxxxaxxxxx
aaaxxxxxaaa


*/
