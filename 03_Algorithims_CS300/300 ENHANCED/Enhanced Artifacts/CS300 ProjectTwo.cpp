// Shain Hodzic
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Global data structure to hold courses (std::map keeps keys sorted)
map<string, Course> courseMap;

// Trim whitespace from both ends
static inline string trim(const string& s) {
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) start++;

    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1]))) end--;

    return s.substr(start, end - start);
}

// Convert a string to uppercase
static inline void toUpperInPlace(string& s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return static_cast<char>(toupper(c)); });
}

// Split a line by delimiter (CSV-style)
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

// Load data from file into courseMap with validation and duplicate detection
void loadDataStructure(const string& fileName) {
    courseMap.clear();

    ifstream file(fileName);
    string line;

    if (!file.is_open()) {
        cout << "Error opening file: " << fileName << endl;
        return;
    }

    int loadedCount = 0;
    int skippedCount = 0;
    int duplicateCount = 0;

    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) {
            continue; // ignore blank lines
        }

        vector<string> tokens = split(line, ',');
        if (tokens.size() < 2 || tokens[0].empty() || tokens[1].empty()) {
            skippedCount++;
            continue;
        }

        Course course;
        course.courseNumber = tokens[0];
        course.courseTitle  = tokens[1];

        // Normalize course number to uppercase for consistent storage/search
        toUpperInPlace(course.courseNumber);

        // Collect prerequisites (normalized and non-empty)
        for (size_t i = 2; i < tokens.size(); ++i) {
            if (!tokens[i].empty()) {
                string prereq = tokens[i];
                toUpperInPlace(prereq);
                course.prerequisites.push_back(prereq);
            }
        }

        // Duplicate detection (prevents silent overwrite)
        if (courseMap.find(course.courseNumber) != courseMap.end()) {
            duplicateCount++;
            continue;
        }

        courseMap[course.courseNumber] = course;
        loadedCount++;
    }

    file.close();

    int missingPrereqCount = 0;
    for (const auto& pair : courseMap) {
        for (const auto& prereq : pair.second.prerequisites) {
            if (courseMap.find(prereq) == courseMap.end()) {
                missingPrereqCount++;
            }
        }
    }

    cout << "Courses loaded successfully.\n";
    cout << "Loaded: " << loadedCount
         << " | Skipped invalid: " << skippedCount
         << " | Duplicates ignored: " << duplicateCount << "\n";

    if (missingPrereqCount > 0) {
        cout << "Warning: " << missingPrereqCount
             << " prerequisite reference(s) were not found in the dataset.\n";
    }
}

// Print all courses in alphanumeric order
// Improvement: std::map is already sorted by key, so we can iterate directly (no extra sort).
void printCourseList() {
    if (courseMap.empty()) {
        cout << "No courses loaded. Please load data first.\n";
        return;
    }

    cout << "\nHere is a sample schedule:\n\n";
    for (const auto& pair : courseMap) {
        cout << pair.second.courseNumber << ", " << pair.second.courseTitle << "\n";
    }
}

// Print a specific course's details
void printCourseInfo(const string& courseNumber) {
    if (courseMap.empty()) {
        cout << "No courses loaded. Please load data first.\n";
        return;
    }

    auto it = courseMap.find(courseNumber);
    if (it == courseMap.end()) {
        cout << "Course not found.\n";
        return;
    }

    const Course& course = it->second;
    cout << course.courseNumber << ", " << course.courseTitle << "\n";

    cout << "Prerequisites: ";
    if (course.prerequisites.empty()) {
        cout << "None\n";
        return;
    }

    for (size_t i = 0; i < course.prerequisites.size(); ++i) {
        cout << course.prerequisites[i];
        if (i < course.prerequisites.size() - 1) cout << ", ";
    }
    cout << "\n";
}

int main() {
    int choice = 0;
    string fileName;
    string inputCourse;

    cout << "Welcome to the course planner.\n";

    while (choice != 9) {
        cout << "\n1. Load Data Structure.\n";
        cout << "2. Print Course List.\n";
        cout << "3. Print Course.\n";
        cout << "9. Exit\n";
        cout << "\nWhat would you like to do? ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

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
                inputCourse = trim(inputCourse);
                toUpperInPlace(inputCourse);
                printCourseInfo(inputCourse);
                break;

            case 9:
                cout << "Thank you for using the course planner!\n";
                break;

            default:
                cout << choice << " is not a valid option.\n";
                break;
        }
    }

    return 0;
}
