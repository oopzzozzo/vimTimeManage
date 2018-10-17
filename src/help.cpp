#include <iostream>
#include "options.h"
int displayUsage(char cmd[]){
	std::cerr << "Usage:\t" << cmd << " -[option] <file(s)/dir(s)>\n";
	std::cerr << "\t" << cmd << " -h shows documentation on options\n";
	return 0;
}
int printHelp(int _, char *cmd[]){
	std::cout << "Usage: " << cmd[0] << " -[option] <file(s)/dir(s)>\n";
	std::cout << "\n Manage the data in '.vimtime' record file\n";
	std::cout << "\n Options:\n";
	std::cout << "\t-d \t\t\t display records in days\n";
	std::cout << "\t-m \t\t\t display records in months\n";
	std::cout << "\t-y \t\t\t display records in years\n";
	std::cout << "\t-mv <fn1> <fn2> \t interpret <fn1> as <fn2> in the future\n";
	std::cout << "\t-mvu <fn1> <fn2> \t undo interpreting <fn1> as <fn2>\n";
	std::cout << "\t-mvf <fn1> <fn2> \t rename <fn1> as <fn2>\n";
	std::cout << "\t-rm <fn1> \t\t do not display <fn1> in the future\n";
	std::cout << "\t-rmu <fn1> \t\t undo hiding <fn1>\n";
	std::cout << "\t-rmf <fn1> \t\t erase all <fn1> records\n";
}
