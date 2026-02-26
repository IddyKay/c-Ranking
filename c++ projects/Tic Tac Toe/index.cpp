
// ============================================================
// PROJECT 3: TIC-TAC-TOE (Object-Oriented)
// Features: AI opponent, score tracking, validation
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

class TicTacToe {
private:
    std::vector<std::vector<char>> board;
    char currentPlayer;
    char playerSymbol;
    char aiSymbol;
    int playerScore;
    int aiScore;
    int draws;
    
    void initializeBoard() {
        board = std::vector<std::vector<char>>(3, std::vector<char>(3, ' '));
    }
    
    void displayBoard() {
        std::cout << "\n    0   1   2\n";
        std::cout << "  ╔═══╦═══╦═══╗\n";
        for (int i = 0; i < 3; ++i) {
            std::cout << i << " ║ ";
            for (int j = 0; j < 3; ++j) {
                std::cout << board[i][j] << " ║ ";
            }
            std::cout << "\n";
            if (i < 2) std::cout << "  ╠═══╬═══╬═══╣\n";
        }
        std::cout << "  ╚═══╩═══╩═══╝\n";
    }
    
    bool isValidMove(int row, int col) {
        return row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ';
    }
    
    bool checkWin(char player) {
        // Check rows, columns, and diagonals
        for (int i = 0; i < 3; ++i) {
            if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return true;
            if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return true;
        }
        if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return true;
        if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return true;
        return false;
    }
    
    bool isBoardFull() {
        for (const auto& row : board)
            for (char cell : row)
                if (cell == ' ') return false;
        return true;
    }
    
    int minimax(int depth, bool isMaximizing, int alpha, int beta) {
        if (checkWin(aiSymbol)) return 10 - depth;
        if (checkWin(playerSymbol)) return depth - 10;
        if (isBoardFull()) return 0;
        
        if (isMaximizing) {
            int maxEval = -1000;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (board[i][j] == ' ') {
                        board[i][j] = aiSymbol;
                        int eval = minimax(depth + 1, false, alpha, beta);
                        board[i][j] = ' ';
                        maxEval = std::max(maxEval, eval);
                        alpha = std::max(alpha, eval);
                        if (beta <= alpha) break;
                    }
                }
            }
            return maxEval;
        } else {
            int minEval = 1000;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (board[i][j] == ' ') {
                        board[i][j] = playerSymbol;
                        int eval = minimax(depth + 1, true, alpha, beta);
                        board[i][j] = ' ';
                        minEval = std::min(minEval, eval);
                        beta = std::min(beta, eval);
                        if (beta <= alpha) break;
                    }
                }
            }
            return minEval;
        }
    }
    
    void aiMove() {
        int bestVal = -1000;
        int bestRow = -1, bestCol = -1;
        
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == ' ') {
                    board[i][j] = aiSymbol;
                    int moveVal = minimax(0, false, -1000, 1000);
                    board[i][j] = ' ';
                    
                    if (moveVal > bestVal) {
                        bestRow = i;
                        bestCol = j;
                        bestVal = moveVal;
                    }
                }
            }
        }
        
        board[bestRow][bestCol] = aiSymbol;
        std::cout << "AI plays at (" << bestRow << ", " << bestCol << ")\n";
    }
    
public:
    TicTacToe() : playerScore(0), aiScore(0), draws(0) {
        initializeBoard();
    }
    
    void play() {
        std::cout << "Choose your symbol (X or O): ";
        std::cin >> playerSymbol;
        playerSymbol = toupper(playerSymbol);
        aiSymbol = (playerSymbol == 'X') ? 'O' : 'X';
        
        char playAgain;
        do {
            initializeBoard();
            currentPlayer = 'X';
            
            while (true) {
                displayBoard();
                
                if (currentPlayer == playerSymbol) {
                    int row, col;
                    std::cout << "Your turn! Enter row (0-2) and column (0-2): ";
                    if (!(std::cin >> row >> col)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input!\n";
                        continue;
                    }
                    
                    if (!isValidMove(row, col)) {
                        std::cout << "Invalid move! Try again.\n";
                        continue;
                    }
                    
                    board[row][col] = playerSymbol;
                } else {
                    std::cout << "AI is thinking...\n";
                    aiMove();
                }
                
                if (checkWin(currentPlayer)) {
                    displayBoard();
                    if (currentPlayer == playerSymbol) {
                        std::cout << "🎉 You win!\n";
                        playerScore++;
                    } else {
                        std::cout << "🤖 AI wins!\n";
                        aiScore++;
                    }
                    break;
                }
                
                if (isBoardFull()) {
                    displayBoard();
                    std::cout << "🤝 It's a draw!\n";
                    draws++;
                    break;
                }
                
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
            
            std::cout << "\nScore - You: " << playerScore << " | AI: " << aiScore << " | Draws: " << draws << "\n";
            std::cout << "Play again? (y/n): ";
            std::cin >> playAgain;
        } while (playAgain == 'y' || playAgain == 'Y');
    }
};

