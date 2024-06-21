#include "NetTerminal.h"

#include <string.h>

NetTerminal::NetTerminal(const char *ip, unsigned short port) {
    if (port > 0) {
        printf("Start NetTerminal client...\n\n");
        _net_client = new NetSocketClient(ip, port);
        printf("NetTerminal Connected to server\n\n");
    }
    else {
        _net_server = new NetSocketServer(8080);
    }
}

NetTerminal::~NetTerminal() {
    PtrSafeRelease(&_net_server);
    PtrSafeRelease(&_net_client);
}

void NetTerminal::WaitingCommands() {
    char command_buffer[1024] = { 0 };
    char *data_buffer = nullptr;
    int data_size = 0;
    
    while (true) {
        /* Input command & check conditions */
        Input(command_buffer, sizeof(command_buffer));
        if (memcmp(command_buffer, "exit", 4) == 0) {
            break;
        }
        else if (strlen(command_buffer) == 0) {
            continue;
        }
        else if (memcmp(command_buffer, "remote exit", 11) == 0) {
            /* Send exit command */
            SmartSend(_net_client, command_buffer, sizeof(command_buffer));
            break;
        }
        
        /* Send command */
        SmartSend(_net_client, command_buffer, sizeof(command_buffer));
        /* Recieve Data */
        if ((data_size = SmartRecieve(_net_client, &data_buffer)) <= 0) {
            continue;
        }
        
        /* Print output */
        printf("%s\n", data_buffer);
        
        /* Clean up */
        ArrSafeRelease(&data_buffer, data_size);
    }
    _net_client->ShutdownConnection();
}

void NetTerminal::RunCommands() {
    char *command_buffer = nullptr;
    int command_size = 0;
    char *data_buffer = nullptr;
    unsigned long long data_size = 0ull;
    
    printf("Starting NetTerminal server...\n");
    
    /* Accept connection */
    while (_net_server->AcceptConnection()) {
        printf("Connection has been accepted!\n");
        /* Recieve command */
        while ((command_size = SmartRecieve(_net_server, &command_buffer)) > 0) {
            printf("%s\n", command_buffer);/* Log */
            
            /* Check condition */
            if (command_size >= 11) {
                if (memcmp(command_buffer, "remote exit", 11) == 0) {
                    printf("Close server!\n");
                    ArrSafeRelease(&command_buffer, command_size);
                    return;
                }
            }
            
            /* Execute command */
            system(command_buffer);
            /* Clean command buffer */
            ArrSafeRelease(&command_buffer, command_size);
            
            /* Get command output */
            if ((data_size = GetFileData(&data_buffer)) <= 0) {
                printf("* => Couldn't get command's output!\n");
                SmartSend(_net_server, (char *)"* => Couldn't get command's output!\n", 36);
                continue;
            }
            
            printf("%s\n", data_buffer);/* Log */
            
            /* Send result */
            SmartSend(_net_server, data_buffer, data_size);
            
            /* Clean output buffer */
            ArrSafeRelease(&data_buffer, data_size);
        }
        printf("NetTerminal client has disconnected!\n");
    }
}
