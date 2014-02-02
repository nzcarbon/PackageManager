#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
char *io_popen(char *args);
char *expandString(char *input);

int main (char *argc[], char *argv[]) { 
	//printf(argv[1]);
	io_popen(argv[1]);
	return 0;
}

char *io_popen(char *dir) {
   	DIR *dp;
    if((dp = opendir(dir)) == NULL) {
        printf("cannot open directory: %s\n", dir);
        return;
    }
    struct dirent *entry;
    while((entry = readdir(dp)) != NULL){
    	printf("%s ",entry->d_name);
    }
    return "asda";
}