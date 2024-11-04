#include<iostream>
#include<fstream>
#include<filesystem>
#include<cassert>
#include<string>
#include<cctype>
#include<algorithm>
#include<map>
#include<regex>
#include<vector>
#include<format>
#include<limits>

using namespace std;
using namespace filesystem;


struct Match {
    char c;
    int pos;

    friend ostream& operator<<(ostream& os, const Match& obj) {
        os << "Match(c: " << obj.c << ", pos: " << obj.pos << ")";
        return os;
    }
};

const map<string, char> digit_map {
    {"zero", '0'},
    {"one", '1'},
    {"two", '2'},
    {"three", '3'},
    {"four", '4'},
    {"five", '5'},
    {"six", '6'},
    {"seven", '7'},
    {"eight", '8'},
    {"nine", '9'},
};

Match find_by_char(string line) {
    int pos = 0;
    for(auto c : line) {
        if (isdigit(c)) {
            return Match(c, pos);
        }
        pos++;
    }
    return Match('e', numeric_limits<int>::max());
}

Match find_by_word(string line, regex pattern, bool reversed = false) {
    smatch matches;
    if (regex_search(line, matches, pattern)) {
        auto key = matches.str(0);
        auto pos = matches.position(0);
        if (reversed) {
            reverse(key.begin(), key.end());
        }
        auto c = digit_map.at(key);
        return Match(c, pos);
    }
    return Match('e', numeric_limits<int>::max());
}

void print_vec(const vector<string>& vec) {
    for (auto s : vec) {
        cout << s << ' ';
    }
    cout << '\n';
}

string join_vec(const vector<string>& vec, string sep = "|") {
    if (vec.empty()) {
        return "";
    }

    ostringstream oss;
    auto iter = vec.begin();
    oss << *iter++;
    for(; iter != vec.end(); ++iter) {
        oss << sep << *iter;
    }
    return oss.str();
}

pair<regex, regex> construct_patterns() {
    vector<string> forward_strings;
    vector<string> reverse_strings;

    for (auto pair : digit_map) {
        auto key = pair.first;
        forward_strings.push_back(format("({})", key));
        reverse(key.begin(), key.end());
        reverse_strings.push_back(format("({})", key));
    }

    auto forward_string = join_vec(forward_strings);
    auto reverse_string = join_vec(reverse_strings);
    return {regex(forward_string), regex(reverse_string)};
}

int main(){
    path f = "input_data/input.txt";
    assert(exists(f));
    assert(is_regular_file(f));
    ifstream ifs {f};
    assert(ifs);

    auto [forward_pattern, reverse_pattern] = construct_patterns();

    string line;
    auto sum = 0;
    while (getline(ifs, line)) {
        auto first_by_word = find_by_word(line, forward_pattern);
        auto first_by_char = find_by_char(line);

        // cout << line << '\n';
        // cout << "first word: " << first_by_word << '\n';
        // cout << "first char: " << first_by_char << '\n';

        reverse(line.begin(), line.end());
        auto last_by_word = find_by_word(line, reverse_pattern, true);
        auto last_by_char = find_by_char(line);

        // cout << " last word: " << last_by_word << '\n';
        // cout << " last char: " << last_by_char << "\n\n";

        Match first = first_by_word;
        if (first_by_char.pos < first_by_word.pos) {
            first = first_by_char;
        }
        assert(first.c != 'e');

        Match last = last_by_word;
        if (last_by_char.pos < last_by_word.pos) {
            last = last_by_char;
        }
        assert(last.c != 'e');

        string s;
        s += first.c;
        s += last.c;
        auto calibration_value = stoi(s);
        sum += calibration_value;
    }

    cout << sum << "\n";

    return 0;
}
