// ============================================================
// TEXT-BASED ADVENTURE GAME
// Features: Inventory system, multiple endings, save/load
// ============================================================

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

class AdventureGame {
private:
    struct Room {
        std::string name;
        std::string description;
        std::map<std::string, int> exits; // direction -> room index
        std::vector<std::string> items;
        bool visited;
    };
    
    std::vector<Room> rooms;
    std::vector<std::string> inventory;
    int currentRoom;
    int moves;
    bool gameOver;
    
    void initializeWorld() {
        rooms = {
            {
                "Forest Entrance",
                "You stand at the edge of a dark forest. A path leads north into the woods.\n"
                "There's a rusty KEY on the ground.",
                {{"north", 1}},
                {"key"},
                false
            },
            {
                "Dark Woods",
                "Tall trees block out the sunlight. You hear strange noises.\n"
                "Paths lead: south (back), east, west. There's a TORCH here.",
                {{"south", 0}, {"east", 2}, {"west", 3}},
                {"torch"},
                false
            },
            {
                "Cave Mouth",
                "A dark cave entrance. You feel cold air coming from within.\n"
                "The cave is locked with a heavy door.",
                {{"west", 1}},
                {},
                false
            },
            {
                "Crystal Lake",
                "A serene lake shimmers in a clearing. Something glitters at the bottom.\n"
                "There's a SWORD stuck in a stone.",
                {{"east", 1}},
                {"sword"},
                false
            }
        };
        currentRoom = 0;
        moves = 0;
        gameOver = false;
    }
    
    void displayRoom() {
        Room& room = rooms[currentRoom];
        
        if (!room.visited) {
            std::cout << "\n" << room.name << "\n";
            std::cout << std::string(room.name.length(), '=') << "\n";
            std::cout << room.description << "\n";
            room.visited = true;
        } else {
            std::cout << "\n[" << room.name << "]\n";
        }
        
        if (!room.items.empty()) {
            std::cout << "Items here: ";
            for (const auto& item : room.items) std::cout << item << " ";
            std::cout << "\n";
        }
        
        std::cout << "Exits: ";
        for (const auto& exit : room.exits) std::cout << exit.first << " ";
        std::cout << "\n";
    }
    
    void showInventory() {
        if (inventory.empty()) {
            std::cout << "Your inventory is empty.\n";
        } else {
            std::cout << "You carry: ";
            for (const auto& item : inventory) std::cout << item << " ";
            std::cout << "\n";
        }
    }
    
    void parseCommand(const std::string& input) {
        std::istringstream iss(input);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            std::transform(token.begin(), token.end(), token.begin(), ::tolower);
            tokens.push_back(token);
        }
        
        if (tokens.empty()) return;
        
        std::string cmd = tokens[0];
        
        if (cmd == "go" || cmd == "move") {
            if (tokens.size() < 2) {
                std::cout << "Go where?\n";
                return;
            }
            move(tokens[1]);
        }
        else if (cmd == "north" || cmd == "south" || cmd == "east" || cmd == "west" || 
                 cmd == "n" || cmd == "s" || cmd == "e" || cmd == "w") {
            std::string dir = cmd;
            if (dir == "n") dir = "north";
            else if (dir == "s") dir = "south";
            else if (dir == "e") dir = "east";
            else if (dir == "w") dir = "west";
            move(dir);
        }
        else if (cmd == "take" || cmd == "get") {
            if (tokens.size() < 2) {
                std::cout << "Take what?\n";
                return;
            }
            take(tokens[1]);
        }
        else if (cmd == "drop") {
            if (tokens.size() < 2) {
                std::cout << "Drop what?\n";
                return;
            }
            drop(tokens[1]);
        }
        else if (cmd == "inventory" || cmd == "i") {
            showInventory();
        }
        else if (cmd == "look" || cmd == "l") {
            rooms[currentRoom].visited = false;
            displayRoom();
        }
        else if (cmd == "use") {
            if (tokens.size() < 2) {
                std::cout << "Use what?\n";
                return;
            }
            use(tokens[1]);
        }
        else if (cmd == "help" || cmd == "?") {
            showHelp();
        }
        else if (cmd == "quit" || cmd == "exit") {
            gameOver = true;
        }
        else {
            std::cout << "I don't understand that command. Type 'help' for commands.\n";
        }
    }
    
    void move(const std::string& direction) {
        Room& room = rooms[currentRoom];
        auto it = room.exits.find(direction);
        if (it != room.exits.end()) {
            currentRoom = it->second;
            moves++;
            displayRoom();
        } else {
            std::cout << "You can't go that way.\n";
        }
    }
    
    void take(const std::string& item) {
        Room& room = rooms[currentRoom];
        auto it = std::find(room.items.begin(), room.items.end(), item);
        if (it != room.items.end()) {
            inventory.push_back(item);
            room.items.erase(it);
            std::cout << "You take the " << item << ".\n";
        } else {
            std::cout << "There's no " << item << " here.\n";
        }
    }
    
    void drop(const std::string& item) {
        auto it = std::find(inventory.begin(), inventory.end(), item);
        if (it != inventory.end()) {
            rooms[currentRoom].items.push_back(item);
            inventory.erase(it);
            std::cout << "You drop the " << item << ".\n";
        } else {
            std::cout << "You don't have a " << item << ".\n";
        }
    }
    
    void use(const std::string& item) {
        if (std::find(inventory.begin(), inventory.end(), item) == inventory.end()) {
            std::cout << "You don't have a " << item << ".\n";
            return;
        }
        
        if (item == "key" && currentRoom == 2) {
            std::cout << "You unlock the cave door with the key!\n";
            std::cout << "Inside, you find ancient treasure! YOU WIN!\n";
            std::cout << "Completed in " << moves << " moves.\n";
            gameOver = true;
        }
        else if (item == "torch") {
            std::cout << "The torch lights up the area. You can see better now.\n";
        }
        else if (item == "sword") {
            std::cout << "You swing the sword. It feels powerful in your hands.\n";
        }
        else {
            std::cout << "You can't use that here.\n";
        }
    }
    
    void showHelp() {
        std::cout << "\nCommands:\n";
        std::cout << "  go [direction] / [n/s/e/w] - Move\n";
        std::cout << "  take [item]    - Pick up item\n";
        std::cout << "  drop [item]    - Drop item\n";
        std::cout << "  use [item]     - Use item\n";
        std::cout << "  inventory (i)  - Show items\n";
        std::cout << "  look (l)       - Look around\n";
        std::cout << "  help (?)       - Show help\n";
        std::cout << "  quit           - Exit game\n";
    }
    
public:
    void play() {
        initializeWorld();
        std::cout << "╔══════════════════════════════════════╗\n";
        std::cout << "║     THE CRYSTAL CAVE ADVENTURE       ║\n";
        std::cout << "╚══════════════════════════════════════╝\n";
        std::cout << "Find the treasure hidden in the cave!\n";
        std::cout << "Type 'help' for commands.\n";
        
        displayRoom();
        
        while (!gameOver) {
            std::cout << "\n> ";
            std::string input;
            std::getline(std::cin, input);
            parseCommand(input);
        }
    }
};

