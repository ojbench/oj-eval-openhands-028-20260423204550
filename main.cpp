#include <iostream>
#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>

struct Student {
    std::string name;
    char gender;
    int classNum;
    int scores[9];
    int avgScore;
    int rank;
    
    void calculateAvg() {
        int sum = 0;
        for (int i = 0; i < 9; i++) {
            sum += scores[i];
        }
        avgScore = sum / 9;
    }
    
    bool operator<(const Student& other) const {
        if (avgScore != other.avgScore) {
            return avgScore > other.avgScore;
        }
        for (int i = 0; i < 9; i++) {
            if (scores[i] != other.scores[i]) {
                return scores[i] > other.scores[i];
            }
        }
        return name < other.name;
    }
};

struct StudentComparator {
    bool operator()(const Student* a, const Student* b) const {
        if (a->avgScore != b->avgScore) {
            return a->avgScore > b->avgScore;
        }
        for (int i = 0; i < 9; i++) {
            if (a->scores[i] != b->scores[i]) {
                return a->scores[i] > b->scores[i];
            }
        }
        return a->name < b->name;
    }
};

std::unordered_map<std::string, Student*> studentMap;
std::set<Student*, StudentComparator> ranking;
bool started = false;

void addStudent(const std::string& name, char gender, int classNum, const int scores[9]) {
    if (started) {
        std::cout << "[Error]Cannot add student now.\n";
        return;
    }
    
    if (studentMap.find(name) != studentMap.end()) {
        std::cout << "[Error]Add failed.\n";
        return;
    }
    
    Student* student = new Student();
    student->name = name;
    student->gender = gender;
    student->classNum = classNum;
    for (int i = 0; i < 9; i++) {
        student->scores[i] = scores[i];
    }
    student->calculateAvg();
    student->rank = 0;
    
    studentMap[name] = student;
}

void startRanking() {
    started = true;
    for (auto& pair : studentMap) {
        ranking.insert(pair.second);
    }
    
    int rank = 1;
    for (auto student : ranking) {
        student->rank = rank++;
    }
}

void updateScore(const std::string& name, int code, int score) {
    if (studentMap.find(name) == studentMap.end()) {
        std::cout << "[Error]Update failed.\n";
        return;
    }
    
    Student* student = studentMap[name];
    student->scores[code] = score;
    student->calculateAvg();
}

void flushRanking() {
    ranking.clear();
    for (auto& pair : studentMap) {
        ranking.insert(pair.second);
    }
    
    int rank = 1;
    for (auto student : ranking) {
        student->rank = rank++;
    }
}

void printList() {
    for (auto student : ranking) {
        std::cout << student->rank << " " 
                  << student->name << " " 
                  << (student->gender == 'M' ? "male" : "female") << " "
                  << student->classNum << " "
                  << student->avgScore << "\n";
    }
}

void query(const std::string& name) {
    if (studentMap.find(name) == studentMap.end()) {
        std::cout << "[Error]Query failed.\n";
        return;
    }
    
    Student* student = studentMap[name];
    std::cout << "STUDENT " << name << " NOW AT RANKING " << student->rank << "\n";
}

void cleanup() {
    for (auto& pair : studentMap) {
        delete pair.second;
    }
    studentMap.clear();
    ranking.clear();
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::string command;
    while (std::cin >> command) {
        if (command == "ADD") {
            std::string name;
            char gender;
            int classNum;
            int scores[9];
            
            std::cin >> name >> gender >> classNum;
            for (int i = 0; i < 9; i++) {
                std::cin >> scores[i];
            }
            
            addStudent(name, gender, classNum, scores);
        } else if (command == "START") {
            startRanking();
        } else if (command == "UPDATE") {
            std::string name;
            int code, score;
            std::cin >> name >> code >> score;
            updateScore(name, code, score);
        } else if (command == "FLUSH") {
            flushRanking();
        } else if (command == "PRINTLIST") {
            printList();
        } else if (command == "QUERY") {
            std::string name;
            std::cin >> name;
            query(name);
        } else if (command == "END") {
            break;
        }
    }
    
    cleanup();
    return 0;
}
