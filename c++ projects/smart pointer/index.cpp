// ============================================================
// PROJECT 1: ADVANCED CALCULATOR (Console Application)
// Features: Basic arithmetic, scientific functions, history
// ============================================================

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

class Calculator {
private:
    std::vector<std::string> history;
    
    void addToHistory(const std::string& operation, double result) {
        history.push_back(operation + " = " + std::to_string(result));
        if (history.size() > 10) history.erase(history.begin());
    }
    
public:
    void displayMenu() {
        std::cout << "\n╔════════════════════════════════════╗\n";
        std::cout << "║         ADVANCED CALCULATOR        ║\n";
        std::cout << "╠════════════════════════════════════╣\n";
        std::cout << "║  1. Addition (+)                   ║\n";
        std::cout << "║  2. Subtraction (-)                ║\n";
        std::cout << "║  3. Multiplication (*)             ║\n";
        std::cout << "║  4. Division (/)                   ║\n";
        std::cout << "║  5. Power (x^y)                    ║\n";
        std::cout << "║  6. Square Root (√x)               ║\n";
        std::cout << "║  7. Logarithm (ln x)               ║\n";
        std::cout << "║  8. Sine (sin x)                   ║\n";
        std::cout << "║  9. Cosine (cos x)                 ║\n";
        std::cout << "║ 10. Tangent (tan x)                ║\n";
        std::cout << "║ 11. Factorial (x!)                 ║\n";
        std::cout << "║ 12. View History                   ║\n";
        std::cout << "║  0. Exit                           ║\n";
        std::cout << "╚════════════════════════════════════╝\n";
        std::cout << "Choice: ";
    }
    
    double getNumber(const std::string& prompt) {
        double num;
        while (true) {
            std::cout << prompt;
            if (std::cin >> num) break;
            std::cout << "Invalid input! Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return num;
    }
    
    long long factorial(int n) {
        if (n < 0) return -1;
        if (n == 0 || n == 1) return 1;
        long long result = 1;
        for (int i = 2; i <= n; ++i) result *= i;
        return result;
    }
    
    void run() {
        int choice;
        do {
            displayMenu();
            std::cin >> choice;
            
            if (choice >= 1 && choice <= 5) {
                double a = getNumber("Enter first number: ");
                double b = getNumber("Enter second number: ");
                double result = 0;
                std::string op;
                
                switch(choice) {
                    case 1: result = a + b; op = std::to_string(a) + " + " + std::to_string(b); break;
                    case 2: result = a - b; op = std::to_string(a) + " - " + std::to_string(b); break;
                    case 3: result = a * b; op = std::to_string(a) + " * " + std::to_string(b); break;
                    case 4: 
                        if (b == 0) { std::cout << "Error: Division by zero!\n"; continue; }
                        result = a / b; 
                        op = std::to_string(a) + " / " + std::to_string(b); 
                        break;
                    case 5: result = pow(a, b); op = std::to_string(a) + " ^ " + std::to_string(b); break;
                }
                std::cout << "Result: " << std::fixed << std::setprecision(6) << result << "\n";
                addToHistory(op, result);
            }
            else if (choice >= 6 && choice <= 11) {
                double a = getNumber("Enter number: ");
                double result = 0;
                std::string op;
                
                switch(choice) {
                    case 6: 
                        if (a < 0) { std::cout << "Error: Negative input!\n"; continue; }
                        result = sqrt(a); op = "√" + std::to_string(a); 
                        break;
                    case 7: 
                        if (a <= 0) { std::cout << "Error: Invalid input for log!\n"; continue; }
                        result = log(a); op = "ln(" + std::to_string(a) + ")"; 
                        break;
                    case 8: result = sin(a); op = "sin(" + std::to_string(a) + ")"; break;
                    case 9: result = cos(a); op = "cos(" + std::to_string(a) + ")"; break;
                    case 10: result = tan(a); op = "tan(" + std::to_string(a) + ")"; break;
                    case 11: 
                        if (a < 0 || a > 20) { std::cout << "Error: Input too large or negative!\n"; continue; }
                        result = factorial(static_cast<int>(a)); 
                        op = std::to_string(static_cast<int>(a)) + "!"; 
                        break;
                }
                std::cout << "Result: " << std::fixed << std::setprecision(6) << result << "\n";
                addToHistory(op, result);
            }
            else if (choice == 12) {
                std::cout << "\n--- Calculation History ---\n";
                for (const auto& h : history) std::cout << h << "\n";
            }
            else if (choice != 0) {
                std::cout << "Invalid choice!\n";
            }
        } while (choice != 0);
        std::cout << "Goodbye!\n";
    }
};

