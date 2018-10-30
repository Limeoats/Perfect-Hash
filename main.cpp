#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>
#include <algorithm>

std::vector<std::string> presidentList;
int a, b, c, best;

std::vector<std::string> split(std::string str, char c) {
    std::vector<std::string> list;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, c)) {
        list.push_back(item);
    }
    return list;
}

void loadPresidentList() {
    std::ifstream in("../presidents.txt", std::ios::binary | std::ios::in);
    std::string line;
    while (std::getline(in, line)) {
        presidentList.push_back(line);
    }
    in.close();
}

void loadData() {
    std::ifstream in("../results.txt", std::ios::binary | std::ios::in);
    std::string line;
    while (std::getline(in, line)) {
        auto pair = split(line, '=');
        if (pair[0] == "a") a = atoi(pair[1].c_str());
        else if (pair[0] == "b") b = atoi(pair[1].c_str());
        else if (pair[0] == "c") c = atoi(pair[1].c_str());
        else if (pair[0] == "best") best = atoi(pair[1].c_str());
    }
    in.close();
}

void saveData() {
    std::ofstream out("../results.txt");
    out << "a=" << a << std::endl;
    out << "b=" << b << std::endl;
    out << "c=" << c << std::endl;
    out << "best=" << best << std::endl;
    out.flush();
    out.close();
}

int hash(std::string str) {
    auto sum = 0;
    for (char ch : str) {
        sum += (int)ch;
    }
    sum = (((sum + a) ^ b * c) % presidentList.size()) - 1;
    return sum;
}

int main() {
    srand (time(NULL));
    loadPresidentList();
    loadData();
    int totalIterations = 0;
    while (true) {
        ++totalIterations;
        a = rand() % 10000000 + 1;
        b = rand() % 10000000 + 1;
        c = rand() % 10000000 + 1;
        std::map<int, int> uniques;
        for (auto &s : presidentList) {
            int index = hash(s);
            uniques[index] += 1;
        }
        int totalUniques = std::count_if(uniques.begin(), uniques.end(), [](const std::pair<int, int> &p) {
            return p.second == 1;
        });
        if (totalUniques > best) {
            std::cout << "New best found: " << totalUniques << std::endl;
            std::cout << "Total iterations so far: " << totalIterations << std::endl << "---" << std::endl;
            best = totalUniques;
            saveData();
        }
    }

}