#include<iostream>
#include<fstream>
#include<filesystem>
#include<cassert>
#include<string>
#include<cctype>
#include<algorithm>

using namespace std;
using namespace filesystem;

char find_first_digit(string line) {
    for(auto c : line) {
        if (isdigit(c)) {
            return c;
        }
    }
    return 'e';
}

char find_last_digit(string line) {
    reverse(line.begin(), line.end());
    for(auto c : line) {
        if (isdigit(c)) {
            return c;
        }
    }
    return 'e';
}

int main(){
    path f = "input_data/input.txt";
    assert(exists(f));
    assert(is_regular_file(f));
    ifstream ifs {f};
    assert(ifs);

    string line;
    auto sum = 0;
    while (getline(ifs, line)) {
        auto first_digit = find_first_digit(line);
        assert(first_digit != 'e');
        auto last_digit = find_last_digit(line);
        string s;
        s += first_digit;
        s += last_digit;
        auto calibration_value = stoi(s);
        sum += calibration_value;
    }

    cout << sum << "\n";

    return 0;
}
