#include <sys/stat.h>
#include <iostream>
#include "options.h"


int checkargv(int argc, char *argv[]){
	if(argc <= 1)
		return -1;
	if(argv[1][0] != '-')
		return listSum(argc, argv);
	for(int i=0; i<sizeof(options); i++){
		if(argv[1] == options[i])
			return functions[i/3](argc, argv);
	}
	return -1;
}

int main(int argc, char *argv[]){
	int re;
	return (checkargv(argc, argv)>=0)? 0: displayUsage(argv[0]);
}
