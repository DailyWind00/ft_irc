#include "main.hpp"

std::vector<std::string> splitCommands(const std::string& str) {
    std::vector<std::string> commands;
    std::istringstream stream(str);
    std::string line;
    
    while (std::getline(stream, line)) {
        // Enlever les retours chariot s'ils existent
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);
        }
        if (!line.empty()) {
            commands.push_back(line);
        }
    }
    
    return commands;
}