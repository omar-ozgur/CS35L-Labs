//tr2u.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){
        if(argc != 3){
                fprintf(stderr, "Wrong number of arguments\n");
                exit(1);
        }

        char* from = argv[1];
        char* to = argv[2];
        int fromLen = strlen(from);
        int toLen = strlen(to);

        if(fromLen != toLen){
                fprintf(stderr, "String arguments must be the same size\n");
                exit(1);
        }

        for(int i = 0; i < fromLen; i++){
                for(int j = i + 1; j < fromLen; j++){
                        if(from[i] == from[j]){
                                fprintf(stderr, "The 'from' string cannot have duplicates\n");
                                exit(1);
                        }
                }
        }

        char c[1]; 
	int status = read(0, c, 1);
        while(status > 0){
                int matchFound = 0;
                for(int i = 0; i < fromLen; i++){
                        if(c[0] == from[i]){
				c[0] = to[i];
                                write(1, c, 1);
                                matchFound = 1;
                                break;
                        }
                }
		if(!matchFound){
                        write(1, c, 1);
                }
                status = read(0, c, 1);
        }

        return 0;
}
