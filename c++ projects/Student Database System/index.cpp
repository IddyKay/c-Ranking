// ============================================================
// STUDENT DATABASE SYSTEM
// Features: CRUD operations, file persistence, sorting, search
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <limits>

struct Student {
    int rollNumber;
    std::string name;
    std::string department;
    float gpa;
    std::vector<float> grades;
    
    float calculateGPA() {
        if (grades.empty()) return 0.0f;
        float sum = 0;
        for (float g : grades) sum += g;
        return sum / grades.size();
    }
    
    void updateGPA() {
        gpa = calculateGPA();
    }
};

class StudentDatabase {
private:
    std::vector<Student> students;
    const std::string filename = "students.dat";
    int nextRollNumber;
    
    void saveToFile() {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            std::cout << "Error saving data!\n";
            return;
        }
        
        size_t count = students.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(count));
        
        for (const auto& s : students) {
            file.write(reinterpret_cast<const char*>(&s.rollNumber), sizeof(s.rollNumber));
            
            size_t nameLen = s.name.length();
            file.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
            file.write(s.name.c_str(), nameLen);
            
            size_t deptLen = s.department.length();
            file.write(reinterpret_cast<const char*>(&deptLen), sizeof(deptLen));
            file.write(s.department.c_str(), deptLen);
            
            file.write(reinterpret_cast<const char*>(&s.gpa), sizeof(s.gpa));
            
            size_t gradeCount = s.grades.size();
            file.write(reinterpret_cast<const char*>(&gradeCount), sizeof(gradeCount));
            for (float g : s.grades) {
                file.write(reinterpret_cast<const char*>(&g), sizeof(g));
            }
        }
        file.close();
    }
    
    void loadFromFile() {
        std::ifstream file(filename, std::ios::binary);
        if (!file) return;
        
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        for (size_t i = 0; i < count; ++i) {
            Student s;
            file.read(reinterpret_cast<char*>(&s.rollNumber), sizeof(s.rollNumber));
            
            size_t nameLen;
            file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
            s.name.resize(nameLen);
            file.read(&s.name[0], nameLen);
            
            size_t deptLen;
            file.read(reinterpret_cast<char*>(&deptLen), sizeof(deptLen));
            s.department.resize(deptLen);
            file.read(&s.department[0], deptLen);
            
            file.read(reinterpret_cast<char*>(&s.gpa), sizeof(s.gpa));
            
            size_t gradeCount;
            file.read(reinterpret_cast<char*>(&gradeCount), sizeof(gradeCount));
            for (size_t j = 0; j < gradeCount; ++j) {
                float g;
                file.read(reinterpret_cast<char*>(&g), sizeof(g));
                s.grades.push_back(g);
            }
            
            students.push_back(s);
            if (s.rollNumber >= nextRollNumber) nextRollNumber = s.rollNumber + 1;
        }
        file.close();
    }
    
    Student* findStudent(int roll) {
        for (auto& s : students) {
            if (s.rollNumber == roll) return &s;
        }
        return nullptr;
    }
    
    void displayStudent(const Student& s) {
        std::cout << "┌─────────────────────────────────────┐\n";
        std::cout << "│ Roll: " << std::setw(33) << std::left << s.rollNumber << "│\n";
        std::cout << "│ Name: " << std::setw(33) << s.name << "│\n";
        std::cout << "│ Dept: " << std::setw(33) << s.department << "│\n";
        std::cout << "│ GPA:  " << std::setw(33) << std::fixed << std::setprecision(2) << s.gpa << "│\n";
        std::cout << "│ Grades: ";
        for (float g : s.grades) std::cout << g << " ";
        std::cout << std::string(33 - s.grades.size() * 4, ' ') << "│\n";
        std::cout << "└─────────────────────────────────────┘\n";
    }
    
public:
    StudentDatabase() : nextRollNumber(1001) {
        loadFromFile();
    }
    
    ~StudentDatabase() {
        saveToFile();
    }
    
    void addStudent() {
        Student s;
        s.rollNumber = nextRollNumber++;
        
        std::cout << "Enter name: ";
        std::cin.ignore();
        std::getline(std::cin, s.name);
        
        std::cout << "Enter department: ";
        std::getline(std::cin, s.department);
        
        int numGrades;
        std::cout << "Number of grades to enter: ";
        std::cin >> numGrades;
        
        for (int i = 0; i < numGrades; ++i) {
            float grade;
            std::cout << "Grade " << (i + 1) << ": ";
            std::cin >> grade;
            s.grades.push_back(grade);
        }
        
        s.updateGPA();
        students.push_back(s);
        std::cout << "Student added with Roll Number: " << s.rollNumber << "\n";
    }
    
    void viewAll() {
        if (students.empty()) {
            std::cout << "No students in database.\n";
            return;
        }
        
        std::cout << "\n=== ALL STUDENTS ===\n";
        for (const auto& s : students) {
            displayStudent(s);
            std::cout << "\n";
        }
    }
    
    void searchStudent() {
        std::cout << "Search by:\n1. Roll Number\n2. Name\nChoice: ";
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            int roll;
            std::cout << "Enter roll number: ";
            std::cin >> roll;
            
            Student* s = findStudent(roll);
            if (s) displayStudent(*s);
            else std::cout << "Student not found.\n";
        }
        else if (choice == 2) {
            std::string name;
            std::cout << "Enter name (partial match): ";
            std::cin.ignore();
            std::getline(std::cin, name);
            
            bool found = false;
            for (const auto& s : students) {
                if (s.name.find(name) != std::string::npos) {
                    displayStudent(s);
                    found = true;
                }
            }
            if (!found) std::cout << "No students found.\n";
        }
    }
    
    void updateStudent() {
        int roll;
        std::cout << "Enter roll number to update: ";
        std::cin >> roll;
        
        Student* s = findStudent(roll);
        if (!s) {
            std::cout << "Student not found.\n";
            return;
        }
        
        std::cout << "Current info:\n";
        displayStudent(*s);
        
        std::cout << "\nWhat to update?\n";
        std::cout << "1. Name\n2. Department\n3. Add Grade\nChoice: ";
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            std::cout << "Enter new name: ";
            std::cin.ignore();
            std::getline(std::cin, s->name);
        }
        else if (choice == 2) {
            std::cout << "Enter new department: ";
            std::cin.ignore();
            std::getline(std::cin, s->department);
        }
        else if (choice == 3) {
            float grade;
            std::cout << "Enter new grade: ";
            std::cin >> grade;
            s->grades.push_back(grade);
            s->updateGPA();
        }
        
        std::cout << "Updated successfully!\n";
        saveToFile();
    }
    
    void deleteStudent() {
        int roll;
        std::cout << "Enter roll number to delete: ";
        std::cin >> roll;
        
        auto it = std::remove_if(students.begin(), students.end(),
            [roll](const Student& s) { return s.rollNumber == roll; });
        
        if (it != students.end()) {
            students.erase(it, students.end());
            std::cout << "Student deleted.\n";
            saveToFile();
        } else {
            std::cout << "Student not found.\n";
        }
    }
    
    void sortStudents() {
        std::cout << "Sort by:\n1. Roll Number\n2. Name\n3. GPA\nChoice: ";
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            std::sort(students.begin(), students.end(),
                [](const Student& a, const Student& b) { return a.rollNumber < b.rollNumber; });
        }
        else if (choice == 2) {
            std::sort(students.begin(), students.end(),
                [](const Student& a, const Student& b) { return a.name < b.name; });
        }
        else if (choice == 3) {
            std::sort(students.begin(), students.end(),
                [](const Student& a, const Student& b) { return a.gpa > b.gpa; });
        }
        
        std::cout << "Sorted!\n";
        viewAll();
    }
    
    void generateReport() {
        if (students.empty()) return;
        
        float totalGPA = 0;
        float maxGPA = 0;
        float minGPA = 4.0;
        std::string topStudent;
        
        for (const auto& s : students) {
            totalGPA += s.gpa;
            if (s.gpa > maxGPA) {
                maxGPA = s.gpa;
                topStudent = s.name;
            }
            if (s.gpa < minGPA) minGPA = s.gpa;
        }
        
        std::cout << "\n╔════════════════════════════════════╗\n";
        std::cout << "║         CLASS STATISTICS           ║\n";
        std::cout << "╠════════════════════════════════════╣\n";
        std::cout << "║ Total Students: " << std::setw(19) << students.size() << " ║\n";
        std::cout << "║ Average GPA:    " << std::setw(19) << std::fixed << std::setprecision(2) << (totalGPA/students.size()) << " ║\n";
        std::cout << "║ Highest GPA:    " << std::setw(19) << maxGPA << " ║\n";
        std::cout << "║ Lowest GPA:     " << std::setw(19) << minGPA << " ║\n";
        std::cout << "║ Top Student:    " << std::setw(19) << topStudent << " ║\n";
        std::cout << "╚════════════════════════════════════╝\n";
    }
    
    void run() {
        int choice;
        do {
            std::cout << "\n╔════════════════════════════════════╗\n";
            std::cout << "║      STUDENT DATABASE SYSTEM       ║\n";
            std::cout << "╠════════════════════════════════════╣\n";
            std::cout << "║ 1. Add Student                     ║\n";
            std::cout << "║ 2. View All Students               ║\n";
            std::cout << "║ 3. Search Student                  ║\n";
            std::cout << "║ 4. Update Student                  ║\n";
            std::cout << "║ 5. Delete Student                  ║\n";
            std::cout << "║ 6. Sort Students                   ║\n";
            std::cout << "║ 7. Generate Report                 ║\n";
            std::cout << "║ 0. Exit                            ║\n";
            std::cout << "╚════════════════════════════════════╝\n";
            std::cout << "Choice: ";
            std::cin >> choice;
            
            switch(choice) {
                case 1: addStudent(); break;
                case 2: viewAll(); break;
                case 3: searchStudent(); break;
                case 4: updateStudent(); break;
                case 5: deleteStudent(); break;
                case 6: sortStudents(); break;
                case 7: generateReport(); break;
                case 0: std::cout << "Saving data...\n"; break;
                default: std::cout << "Invalid choice!\n";
            }
        } while (choice != 0);
    }
};

// ============================================================
// MAIN MENU TO SELECT PROJECT
// ============================================================

int main() {
    int choice;
    
    do {
        std::cout << "\n╔════════════════════════════════════╗\n";
        std::cout << "║      C++ PROJECT COLLECTION        ║\n";
        std::cout << "╠════════════════════════════════════╣\n";
        std::cout << "║ 1. Advanced Calculator             ║\n";
        std::cout << "║ 2. Number Guessing Game            ║\n";
        std::cout << "║ 3. Tic-Tac-Toe (vs AI)             ║\n";
        std::cout << "║ 4. Text Adventure Game             ║\n";
        std::cout << "║ 5. Student Database System         ║\n";
        std::cout << "║ 0. Exit                            ║\n";
        std::cout << "╚════════════════════════════════════╝\n";
        std::cout << "Select project: ";
        std::cin >> choice;
        
        switch(choice) {
            case 1: {
                Calculator calc;
                calc.run();
                break;
            }
            case 2: {
                NumberGuessingGame game;
                game.play();
                break;
            }
            case 3: {
                TicTacToe ttt;
                ttt.play();
                break;
            }
            case 4: {
                AdventureGame adv;
                adv.play();
                break;
            }
            case 5: {
                StudentDatabase db;
                db.run();
                break;
            }
            case 0:
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice!\n";
        }
    } while (choice != 0);
    
    return 0;
}
