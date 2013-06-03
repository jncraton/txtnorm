#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

unsigned char getNextByte() {
    unsigned char byte[2];
    
    if (!fread(byte, 1, 1, stdin)) {
        // This could use real error checking
        // It's probably end of input
        exit(0);
    }
    
    return byte[0];
}

int printReplacement(unsigned char* uChar) {
    if (uChar[0] == 0xe2 && 
        uChar[1] == 0x80 && 
        uChar[3] == 0x00) {
        if (uChar[2] == 0x98 || uChar[2] == 0x99) {
            printf("'");
            return 0;
        }
        if (uChar[2] == 0x9c || uChar[2] == 0x9d) {
            printf("\"");
            return 0;
        }
        if (uChar[2] == 0x93 || uChar[2] == 0x94 || uChar[2] == 0x90) {
            printf("-");
            return 0;
        }
    }
    
    if (uChar[0] == 0xe2 && 
        uChar[1] == 0x88 && 
        uChar[3] == 0x00) {
        if (uChar[2] == 0x92) {
            printf("-");
            return 0;
        }
    }
    
    return -1;
}

int main (int argc, char **argv) {
    int linePos = 0;
    unsigned char uChar[4];
    
    uChar[0] = getNextByte();
    
    #ifdef _WIN32
    // Work around newline replacement on Windows
    extern int fileno(FILE*);
    setmode(fileno(stdout), O_BINARY);
    #endif
    
    while (1) {
        for (int i = 1; i  < 4; i++) {
            uChar[i] = 0x00;
        }
        
        if ((uChar[0] &  0b11100000) == 0b11100000) {
            uChar[1] = getNextByte();
            uChar[2] = getNextByte();
            if (printReplacement(uChar)) {
                fwrite(uChar, 3, 1, stdout);
            }
        } else if (uChar[0] == '\r') {
            uChar[1] = getNextByte();
            
            if (uChar[1] == '\n') {
                fwrite(&uChar[1], 1, 1, stdout);
            }
            
            linePos = 0;
        } else if (uChar[0] == '\n') {
            
            uChar[0] = getNextByte();
            
            if (linePos > 60 && uChar[0] != ' ' && uChar[0] != '\t' && uChar[0] != '\n') {
                fwrite(" ", 1, 1, stdout);
                fwrite(uChar, 1, 1, stdout);
            } else {
                fwrite("\n", 1, 1, stdout);
                fwrite(uChar, 1, 1, stdout);
            }
                
            linePos = 0;
        } else {
            fwrite(uChar, 1, 1, stdout);
        }
        
        linePos +=1;
        
        uChar[0] = getNextByte();
    }
    
    
    return 0;
}