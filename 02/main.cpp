#include<iostream>
#include<fstream>
#include<filesystem>
#include<cassert>
#include<vector>
#include<algorithm>
#include<cctype>
#include<map>
#include<ranges>

using namespace std;
using namespace filesystem;


struct Cube {
    int count;
    string color;

    friend ostream& operator<<(ostream& os, const Cube& obj) {
        os << "(" << obj.count << ", " << obj.color << ")";
        return os;
    }
};

struct Draw {
    vector<Cube> cubes;

    friend ostream& operator<<(ostream& os, const Draw& obj) {
        os << "[ ";
        for (auto cube : obj.cubes) {
            os << cube << ' ';
        }
        os << "]";
        return os;
    }
};

struct Game {
    int id;
    vector<Draw> draws;

    friend ostream& operator<<(ostream& os, const Game& obj) {
        os << "Game(" << obj.id << ", [ ";
        for (auto draw : obj.draws) {
            os << draw << ' ';
        }
        os << "])";
        return os;
    }
};

vector<string> split(const string& s, char delimiter) {
    vector<string> result {};
    ostringstream oss {};
    for (auto c : s) {
        if (c == delimiter) {
            result.push_back(oss.str());
            oss = ostringstream {};
            continue;
        }
        oss << c;
    }
    result.push_back(oss.str());
    return result;
}

string strip(const string& s) {
    auto f = [](char c){return isspace(c);};
    auto start = find_if_not(s.begin(), s.end(), f);
    auto end = find_if_not(s.rbegin(), s.rend(), f).base();
    return(string(start, end));
}

Game parse_game(const string& line) {
    auto line_parts = split(line, ':');
    auto game_id_parts = split(line_parts.at(0), ' ');
    auto draw_parts = split(line_parts.at(1), ';');

    int game_id = stoi(game_id_parts.at(1));

    vector<Draw> draws {};
    for (auto draw_part : draw_parts) {
        auto cube_parts = split(draw_part, ',');
        vector<Cube> cubes {};
        for (auto cube_part : cube_parts) {
            cube_part = strip(cube_part);
            auto data_parts = split(cube_part, ' ');
            cubes.emplace_back(stoi(data_parts.at(0)), data_parts.at(1));
        }
        draws.emplace_back(cubes);
    }
    return {game_id, draws};
}

bool check_game_possible(Game& game, vector<Cube>& constraints) {
    for (auto constraint : constraints) {
        for (auto draw: game.draws) {
            for (auto cube: draw.cubes) {
                if (cube.color != constraint.color) continue;
                if (cube.count > constraint.count) return false;
            }
        }
    }
    return true;
}

int calculate_game_power(Game& game) {
    map<string, int> high {
        {"red", 0},
        {"green", 0},
        {"blue", 0}
    };
    for (auto draw: game.draws) {
        for (auto cube: draw.cubes) {
            if (cube.count > high[cube.color]) high[cube.color] = cube.count;
        }
    }
    return high["red"] * high["green"] * high["blue"];
}

int main() {
    path input_file = "input_data/input.txt";
    ifstream ifs {input_file};
    assert(ifs);

    vector<Game> games {};
    string line;
    while (getline(ifs, line)) {
        games.push_back(parse_game(line));
    }

    vector<Cube> constraints = {
        {12, "red"},
        {13, "green"},
        {14, "blue"}
    };

    auto possible_games = games | std::views::filter([&constraints](Game& game) {
        return check_game_possible(game, constraints);
    });

    int sum1 = 0;
    for (auto possible_game : possible_games) {
        sum1 = sum1 + possible_game.id;
        cout << possible_game << '\n';
    };
    cout << sum1 << '\n';

    int sum2 = 0;
    for (auto game : games) {
        sum2 += calculate_game_power(game);
    }
    cout << sum2 << '\n';

    return 0;
}
