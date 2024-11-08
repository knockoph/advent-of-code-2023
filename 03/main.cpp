#include<iostream>
#include<fstream>
#include<filesystem>
#include<tuple>
#include<vector>
#include<cctype>
#include<ranges>
#include<limits>

using namespace std;
using namespace filesystem;

struct PartNumber {
    int n;
    int x_min;
    int x_max;
};

struct Symbol {
    char c;
    int x;
};

vector<PartNumber> parse_line(const string& line) {
    string acc_number;
    int x_min = numeric_limits<int>::max();
    int x_max = 0;
    vector<PartNumber> part_numbers {};

    int i = 0;
    for (auto c : line) {
        int x = i++;
        if (isdigit(c)) {
            acc_number += c;
            x_min = min(x, x_min);
            x_max = max(x, x_max);
            continue;
        }
        if (acc_number != "") {
            PartNumber part_number {stoi(acc_number), x_min, x_max};
            part_numbers.push_back(part_number);

            acc_number = "";
            x_min = numeric_limits<int>::max();
            x_max = 0;
        }
    }

    if (acc_number != "") {
        PartNumber part_number {stoi(acc_number), x_min, x_max};
        part_numbers.push_back(part_number);
    }

    return part_numbers;
}

bool valid_part(const vector<string>& lines, const PartNumber& part_number, int line_number) {
    int x_start = max(part_number.x_min - 1, 0);
    int x_end = min(part_number.x_max + 2, int(lines.at(0).size()));
    int y_start = max(line_number - 1, 0);
    int y_end = min(line_number + 2, int(lines.size()));

    for (int y = y_start; y < y_end; y++) {
        for (int x = x_start; x < x_end; x++) {
            if (y == line_number && x >= part_number.x_min && x <= part_number.x_max) {
                continue;
            }
            if (lines.at(y).at(x) != '.') {
                return true;
            }
        }
    }
    return false;
}

void print_symbols(vector<char>& symbols) {
    for (auto c : symbols) {
        cout << c;
    }
    cout << '\n';
}

int main() {
    path input_file = "input_data/input.txt";
    ifstream ifs {input_file};

    string line;
    vector<vector<PartNumber>> part_numbers = {};
    vector<string> lines = {};

    while (getline(ifs, line)) {
        parse_line(line);
        auto line_part_numbers = parse_line(line);
        part_numbers.push_back(line_part_numbers);
        lines.push_back(line);
    }

    vector<PartNumber> valid_part_numbers {};

    auto i = 0;
    for (auto line_part_numbers : part_numbers) {
        auto line_number = i++;
        for (auto part_number : line_part_numbers) {
            if (valid_part(lines, part_number, line_number)) {
                valid_part_numbers.push_back(part_number);
            }
        }
    }

    int sum = 0;
    for (auto part_number : valid_part_numbers) {
        sum += part_number.n;
    }
    cout << sum << '\n';
    return 0;
}
