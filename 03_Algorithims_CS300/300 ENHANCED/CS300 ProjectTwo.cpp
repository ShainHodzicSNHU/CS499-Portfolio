//Shain Hodzic//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Global data structure to hold courses
map<string, Course> courseMap;

// Function to split a line by delimiter
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Load data from file into courseMap
void loadDataStructure(const string& fileName) {
    courseMap.clear();
    ifstream file(fileName);
    string line;

    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }

    while (getline(file, line)) {
        vector<string> tokens = split(line, ',');
        if (tokens.size() >= 2) {
            Course course;
            course.courseNumber = tokens[0];
            course.courseTitle = tokens[1];
            for (size_t i = 2; i < tokens.size(); ++i) {
                course.prerequisites.push_back(tokens[i]);
            }
            courseMap[course.courseNumber] = course;
        }
    }

    file.close();
    cout << "Courses loaded successfully." << endl;
}

// Print all courses in alphanumeric order
void printCourseList() {
    vector<string> sortedKeys;
    for (const auto& pair : courseMap) {
        sortedKeys.push_back(pair.first);
    }
    sort(sortedKeys.begin(), sortedKeys.end());

    cout << "\nHere is a sample schedule:\n" << endl;
    for (const auto& key : sortedKeys) {
        cout << courseMap[key].courseNumber << ", " << courseMap[key].courseTitle << endl;
    }
}

// Print a specific course's details
void printCourseInfo(const string& courseNumber) {
    auto it = courseMap.find(courseNumber);
    if (it != courseMap.end()) {
        const Course& course = it->second;
        cout << course.courseNumber << ", " << course.courseTitle << endl;

        if (!course.prerequisites.empty()) {
            cout << "Prerequisites: ";
            for (size_t i = 0; i < course.prerequisites.size(); ++i) {
                cout << course.prerequisites[i];
                if (i < course.prerequisites.size() - 1) cout << ", ";
            }
            cout << endl;
        } else {
            cout << "Prerequisites: None" << endl;
        }
    } else {
        cout << "Course not found." << endl;
    }
}

int main() {
    int choice = 0;
    string fileName;
    string inputCourse;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << "\n1. Load Data Structure.\n";
        cout << "2. Print Course List.\n";
        cout << "3. Print Course.\n";
        cout << "9. Exit\n";
        cout << "\nWhat would you like to do? ";

        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter the file name to load: ";
                cin >> fileName;
                loadDataStructure(fileName);
                break;
            case 2:
                printCourseList();
                break;
            case 3:
                cout << "What course do you want to know about? ";
                cin >> inputCourse;
                // Convert to uppercase for case-insensitive match
                transform(inputCourse.begin(), inputCourse.end(), inputCourse.begin(), ::toupper);
                printCourseInfo(inputCourse);
                break;
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;
            default:
                cout << "\n" << choice << " is not a valid option." << endl;
                break;
        }
    }

    return 0;
}
