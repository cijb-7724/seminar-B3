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

using namespace std;

#define yes "Yes"
#define no "No"
#define yesno(bool) if(bool){cout<<"Yes"<<endl;}else{cout<<"No"<<endl;}

#define tp() cout << "here~~" << endl


//型エイリアス vector<set<pair<tuple : bool<char<string<int<ll<ull
using ll = long long;
using ull = unsigned long long;
using vb = vector<bool>;
using vc = vector<char>;
using vs = vector<string>;
using vi = vector<int>;
using vll = vector<long long>;
using vd = vector<double>;
using si = set<int>;
using sll = set<ll>;
using msi = multiset<int>;
using msll = multiset<ll>;
using mss = multiset<string>;
using pii = pair<int, int>;
using pill = pair<int, ll>;
using plli = pair<ll, int>;
using pllll = pair<long long, long long>;
using vvb = vector<vector<bool>>;
using vvc = vector<vector<char>>;
using vvs = vector<vector<string>>;
using vvi = vector<vector<int>>;
using vvll = vector<vector<ll>>;
using vvd = vector<vector<double>>;
using vsi = vector<set<int>>;
using vpii = vector<pair<int, int>>;
using vpllll = vector<pair<long long, long long>>;
using spii = set<pair<int, int>>;

//generate random 
random_device rd;
int seed = 0;//seedを数値で指定するかrd()で実行毎に変えるか
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

//grid world class p.97~
//---------------------------------------------------------------------
class GridWorld {
public:
    vector<int> action_space;
    map<int, string> action_meaning;
    vector<vector<double>> reward_map;
    pair<int, int> goal_state, wall_state, start_state, agent_state;
public:
    GridWorld();
    int height(void);
    int width(void);
    pair<int, int> shape();
    vector<int> actions();
    vector<pair<int, int>> states();
    pair<int, int> next_state(pair<int, int>, int);
    double reward(pair<int, int>, int, pair<int, int>);
    pair<int, int> reset(void);
    tuple<pair<int, int>, double, bool> step(int);
};

GridWorld::GridWorld() {
    this->action_space = {0, 1, 2, 3};
    this->action_meaning = {{0, "UP"}, {1, "DOWN"}, {2, "LEFT"}, {3, "RIGHT"}};
    reward_map = {
        {0, 0, 0, 1.0},
        {0, 0, 0, -1.0},
        {0, 0, 0, 0}
    };
    goal_state = {0, 3};
    wall_state = {1, 1};
    start_state = {2, 0};
    agent_state = start_state;
}
int GridWorld::height(void) {
    return this->reward_map.size();
}
int GridWorld::width(void) {
    return this->reward_map[0].size();
}
pair<int, int> GridWorld::shape(void) {
    return {this->height(), this->width()};
}
vector<int> GridWorld::actions(void) {
    return this->action_space;
}
vector<pair<int, int>> GridWorld::states(void) {
    vector<pair<int, int>> vec;
    for (int h=0; h<this->height(); ++h) {
        for (int w=0; w<this->width(); ++w) {
            vec.push_back({h, w});
        }
    }
    return vec;
}
pair<int, int> GridWorld::next_state(pair<int, int> state, int action) {
    vector<pair<int, int>> action_move_map = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    pair<int, int> move = action_move_map[action];
    pair<int, int> next_state = {state.first + move.first, state.second + move.second};
    int ny = next_state.first, nx = next_state.second;

    if (nx < 0 || nx >= this->width() || ny < 0 || ny >= this->height()) {
        next_state = state;
    } else if (next_state == this->wall_state) {
        next_state = state;
    }
    return next_state;
}
double GridWorld::reward(pair<int, int> state, int, pair<int, int> next_state) {
    return this->reward_map[next_state.first][next_state.second];
}
pair<int, int> GridWorld::reset(void) {
    this->agent_state = this->start_state;
    return this->agent_state;
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
class RandomAgent {
public:
    double gamma;
    int action_size;
    map<int, double> random_actions;
    map<pair<int, int>, map<int, double>> pi;//各座標（マス）に対して各行動をする確率
    map<pair<int, int>, double> V;//各座標（マス）の価値
    map<pair<int, int>, double> cnts;
    vector<tuple<pair<int, int>, int, double>> memory;

public:
    RandomAgent();
    int get_action(pair<int, int>);
    void add(pair<int, int>, int, double);
    void reset(void);
    void eval(void);
};

RandomAgent::RandomAgent() {
    this->gamma = 0.9;
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
int RandomAgent::get_action(pair<int, int> state) {
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
void RandomAgent::add(pair<int, int> state, int action, double reward) {
    tuple<pair<int, int>, int, double> data = {state, action, reward};
    this->memory.push_back(data);
}
void RandomAgent::reset(void) {
    this->memory.resize(0);
}
void RandomAgent::eval(void) {
    double G = 0;
    vector<tuple<pair<int, int>, int, double>> mem = this->memory;
    reverse(mem.begin(), mem.end());
    for (auto data: mem) {
        pair<int, int> state = get<0>(data);
        int action = get<1>(data);
        double reward = get<2>(data);
        G = this->gamma * G + reward;
        this->cnts[state] += 1;
        this->V[state] += (G - this->V[state]) / this->cnts[state];
    }
}
//------------------------------------------------------------------------

int main() {
    GridWorld env;
    RandomAgent agent;
    int episodes = 1000;//1000
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
                // cout << "lenght" << episode << ' ' << agent.memory.size() << endl;
                agent.eval();
                break;
            }
            state = next_state;
        }
    }
    cout << fixed << setprecision(3);
    for (int i=0; i<env.height(); ++i) {
        for (int j=0; j<env.width(); ++j) {
            if (agent.V[{i, j}] >= 0) cout << ' ';
            cout << agent.V[{i, j}] << ' ';
        }
        cout << endl;
    }
}


/*

*/


 