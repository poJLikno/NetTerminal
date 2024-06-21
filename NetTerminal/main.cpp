#include <iostream>

#ifdef _WIN32
#include "NetTerminal/NetTerminal.h"
#else
#include "NetTerminal.h"
#endif

int main(int argc, const char **argv) {
    try {
        if (argc >= 3) {
            NetTerminal terminal(argv[1], atoi(argv[2]));
            terminal.WaitingCommands();
        }
        else {
            NetTerminal terminal;
            terminal.RunCommands();
        }
    }
    catch (std::string &error) {
        perror(error.c_str());
    }
    
    return 0;
}
