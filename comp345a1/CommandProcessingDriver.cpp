//
// Created by Matteo on 11/5/2023.
//
#include "CommandProcessing.h"

void testCommandProcessor(const std::string& mode, const std::string& filename = "") {
    if (mode == "-console") {
        // Console mode: process a sequence of commands from the console
        std::cout << "Console Command Processing:" << std::endl;
        CommandProcessor cp("start");
        while (true) {
            Command cmd = cp.getCommand();
            std::cout << cmd << std::endl;
            if (cmd.getCommand() == "quit") {
                break;
            }
        }
    } else if (mode == "-file" && !filename.empty()) {
        // File mode: process commands sequentially from a file
        std::cout << "File Command Processing:" << std::endl;
        FileCommandProcessorAdapter fcpa("start", filename);
        while (true) {
            Command cmd = fcpa.getCommand();
            std::cout << cmd << std::endl;
            if (cmd.getCommand() == "quit" || fcpa.isEndOfFile()) {
                break;
            }
        }
    } else {
        std::cerr << "Invalid mode. Use '-console' for console input or '-file' with a filename for file input." << std::endl;
    }
}



