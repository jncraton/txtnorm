#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#define CR 0x0d
#define LF 0x0a

unsigned char getNextByte() {
    unsigned char byte[2];
    
    if (!fread(byte, 1, 1, stdin)) {
        // This could real error checking
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
        } else if (uChar[0] == CR) {
            uChar[1] = getNextByte();
            
            if (uChar[1] == LF) {
                fwrite(&uChar[1], 1, 1, stdout);
            }
        } else {
            fwrite(uChar, 1, 1, stdout);
        }
        
        uChar[0] = getNextByte();
    }
    
    
    return 0;
}