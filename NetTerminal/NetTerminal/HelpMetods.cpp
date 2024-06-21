#include "NetTerminal.h"

#include <string.h>
#include <stdio.h>

void NetTerminal::Input(char *input, int size) {
    if (input) {
        memset(input, 0, size);
    }
    printf("@NetTerminal ~$ ");
    fgets(input, (int)size, stdin);
    input[strlen(input) - 1] = 0;/* Delete \n in the end of string */
    //std::cin.getline(input, size);
/*
 #ifdef _WIN32
    fgetc(stdin);
    //std::cin.ignore(256, '\n');
#endif
 */
    
    while (input[0] == ' ' || input[0] == '\t') {
        snprintf(input, size, "%s", &input[1]);
    }
    
    snprintf(input, size, "%s > buffer_file.txt", input);
}

/* Send data size & data */
void NetTerminal::SmartSend(NetSocket *socket, char *buffer, const size_t &buffer_size) {
    char tmp = 0;
    char size_buffer[32] = { 0 };
    
    snprintf(size_buffer, sizeof(size_buffer), "%llu", (unsigned long long)buffer_size);
    socket->Send(size_buffer, sizeof(size_buffer));
    if (socket->Recieve(&tmp, 1) <= 0) {
        return;
    }
    socket->Send(buffer, buffer_size);
    socket->Recieve(&tmp, 1);
}

/* Need free memory after use */
int NetTerminal::SmartRecieve(NetSocket *socket, char **buffer) {
    int result = 0;
    char size_buffer[32] = { 0 };
    unsigned long long data_size = 0ull;
    
    if ((result = socket->Recieve(size_buffer, sizeof(size_buffer))) <= 0) {
        return result;
    }
    socket->Send((char *)"\0", 1);
    
    data_size = (unsigned long long)atoll(size_buffer);
    *buffer = new char[data_size]{ 0 };
    
    if ((result = socket->Recieve(*buffer, data_size)) <= 0) {
        ArrSafeRelease(buffer, data_size);
        return result;
    }
    socket->Send((char *)"\0", 1);
    
    return result;
}

/* Need free memory after use */
unsigned long long NetTerminal::GetFileData(char **buffer) {
    unsigned long long file_size = 0ull;
    
    FILE *file = fopen("buffer_file.txt", "r");
    if (file) {
        /* Get file size */
        fseek(file, 0, SEEK_END);
        file_size = (unsigned long long)ftell(file);
        rewind(file);/* Move pos to the beggining in a file */
        
        if (file_size) {
            *buffer = new char[file_size]{ 0 };
            fread(*buffer, sizeof(char), file_size, file);
        }
        
        if (ferror(file)){
            throw std::string("File reading error!");
        }
        /*else if (fgetc(file) == EOF) {
            printf("log: EOF - file!\n");
        }*/
        
        fclose(file);
    }
    
    return file_size;
}
