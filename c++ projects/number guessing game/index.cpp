// ============================================================
// NUMBER GUESSING GAME
// Features: Multiple difficulty levels, hints, score tracking
// ============================================================

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

class NumberGuessingGame {
private:
    int secretNumber;
    int attempts;
    int maxAttempts;
    int range;
    int score;
    
    void setDifficulty() {
        std::cout << "\nSelect Difficulty:\n";
        std::cout << "1. Easy (1-50, 10 attempts)\n";
        std::cout << "2. Medium (1-100, 7 attempts)\n";
        std::cout << "3. Hard (1-200, 5 attempts)\n";
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        
        switch(choice) {
            case 1: range = 50; maxAttempts = 10; break;
            case 2: range = 100; maxAttempts = 7; break;
            case 3: range = 200; maxAttempts = 5; break;
            default: range = 100; maxAttempts = 7;
        }
    }
    
public:
    NumberGuessingGame() : score(0) {
        srand(static_cast<unsigned>(time(0)));
    }
    
    void play() {
        char playAgain;
        do {
            setDifficulty();
            secretNumber = rand() % range + 1;
            attempts = 0;
            bool won = false;
            
            std::cout << "\nI'm thinking of a number between 1 and " << range << "...\n";
            
            while (attempts < maxAttempts && !won) {
                std::cout << "\nAttempt " << (attempts + 1) << "/" << maxAttempts << ": ";
                int guess;
                
                if (!(std::cin >> guess)) {
                    std::cout << "Invalid input! Please enter a number.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                
                attempts++;
                
                if (guess == secretNumber) {
                    std::cout << "🎉 Correct! You guessed it in " << attempts << " attempts!\n";
                    int roundScore = (maxAttempts - attempts + 1) * 100;
                    score += roundScore;
                    std::cout << "Round score: " << roundScore << " | Total: " << score << "\n";
                    won = true;
                }
                else if (guess < secretNumber) {
                    std::cout << "Too low! ";
                    if (abs(guess - secretNumber) <= 5) std::cout << "(Very close!)";
                    std::cout << "\n";
                }
                else {
                    std::cout << "Too high! ";
                    if (abs(guess - secretNumber) <= 5) std::cout << "(Very close!)";
                    std::cout << "\n";
                }
                
                // Temperature hint
                if (attempts > 1 && !won) {
                    int diff = abs(guess - secretNumber);
                    if (diff <= 5) std::cout << "🔥 Burning hot!\n";
                    else if (diff <= 15) std::cout << "🌡️ Warm\n";
                    else if (diff <= 30) std::cout << "❄️ Cold\n";
                    else std::cout << "🧊 Freezing\n";
                }
            }
            
            if (!won) {
                std::cout << "\nGame Over! The number was: " << secretNumber << "\n";
            }
            
            std::cout << "\nPlay again? (y/n): ";
            std::cin >> playAgain;
        } while (playAgain == 'y' || playAgain == 'Y');
        
        std::cout << "Final Score: " << score << "\n";
    }
};

