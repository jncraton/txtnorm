#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

unsigned char buf[256];
unsigned char buf_start = 0;

void fillBuf() {
    fread(buf, 1, 256, stdin);
}

unsigned char getNextByte() {
    unsigned char byte[2];
    
    unsigned char ret = buf[buf_start];
    
    if (!ret) {
        exit(0);
    }

    if (!fread(byte, 1, 1, stdin)) {
        // This could use real error checking
        // It's probably end of input
        buf[buf_start] = 0x00;
    } else {
        buf[buf_start] = byte[0];
    }
    
    buf_start++;
    return ret;
}

unsigned char bufferContainsPair(unsigned char a, unsigned char b) {
    for (unsigned char i = buf_start; i != buf_start - 1; i++) {
        if (buf[i] == a && buf[i+1] == b) {
            return 1;
        }
    }
    
    return 0;
}

unsigned char nextLineIndented() {
    if (bufferContainsPair('\n', ' ') || 
         bufferContainsPair('\n', '\t')) {
         return 1;
     }
     
     return 0;
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
    int precedingNewlines = 0;
    unsigned char uChar[4];
    
    fillBuf();
    
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
            precedingNewlines = 0;
        } else if (uChar[0] == '\r') {
            uChar[1] = getNextByte();
            
            if (uChar[1] == '\n') {
                fwrite(&uChar[1], 1, 1, stdout);
            }
            
            precedingNewlines++;
            linePos = 0;
        } else {
            if (uChar[0] == '\n') {
                precedingNewlines++;
                /* Handle text wrapping */
                uChar[0] = getNextByte();
                
                if (linePos > 45 && uChar[0] != ' ' && uChar[0] != '\t' && uChar[0] != '\n') {
                    fwrite(" ", 1, 1, stdout);
                } else {
                    fwrite("\n", 1, 1, stdout);
                }
                    
                linePos = 0;
            }
            
            if (uChar[0] == 0x91 || uChar[0] == 0x92) {
                fwrite("'", 1, 1, stdout);
                precedingNewlines = 0;
            } else if (uChar[0] == 0x93 || uChar[0] == 0x94) {
                fwrite("\"", 1, 1, stdout);
                precedingNewlines = 0;
            } else if ( (uChar[0] != ' ' && uChar[0] != '\t') || precedingNewlines < 2 || nextLineIndented()) {
                fwrite(uChar, 1, 1, stdout);
                
                if (uChar[0] != '\n') {
                    precedingNewlines = 0;
                } else {
                    precedingNewlines++;
                }
            }
        }
        
        linePos +=1;
        
        uChar[0] = getNextByte();
    }
    
    return 0;
}