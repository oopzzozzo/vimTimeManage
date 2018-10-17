#include "options.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <algorithm>
using namespace std;
inline auto getCurrTime(){
	return chrono::system_clock::now();
}
void displayTimeInt(auto start_time){
	double time_int = (getCurrTime()-start_time).count();
	cout << "\t" << time_int << "s spent\n"; 
}
void respond789(int, string, string);

int mvPreprocess(char *argv[], string &fr, string &fn1, string &fn2){
	string fp1 = argv[2], fp2 = argv[3];
	size_t r_end1 = fp1.find_last_of("/"), r_end2 = fp2.find_last_of("/");
	string fr1 = (r_end1 == string::npos)? ".":fp1.substr(0, r_end1);
	string fr2 = (r_end2 == string::npos)? ".":fp2.substr(0, r_end2);
	if(fr1 != fr2){
		cerr << "\tAlias could only work under the same directory\n";
		return 1;
	}
	fr = fr1;
	fn1 = (r_end1 == string::npos)? fp1:fp1.substr(r_end1+1);
	fn2 = (r_end2 == string::npos)? fp2:fp2.substr(r_end2+1);
	return 0;
}

int aliasFileName(int type, char *argv[]){
	auto t = getCurrTime();
	string fr, fn1, fn2;
	int pre = mvPreprocess(argv, fr, fn1, fn2);
	if(pre)
		return min(pre, 0);
	string postit, syscmd, succ, fail;
	switch(type){
		case 0:
			postit = "fn1+" "+/mvd "+fn2;
			syscmd = "echo '"+postit+"' >> "+fr+"/.vimtime";
			succ = fn1+" -> "+fn2;
			fail = "error occured, nothing done";
			break;
		case 1:
			postit = fn1+" \\/mvd "+fn2+"$";
			syscmd = "sed -i -e '/"+fn1+"/!{q789}; {s/"+postit+"//g} "+fr+"/.vimtime";
			succ = fn1+" -> "+fn2+" undone";
			fail = fn1+" -> "+fn2+" not aliased";
			break;
		case 2:
			postit = "^"+fn1+"/"+fn2;
			syscmd = "sed -i -e '/^"+fn1+"/!{q789}; {s/"+postit+"/g} ";
			succ = fn1+" is "+fn2+" from now on.";
			fail = "No record of "+fn1+" found.";
	}
	respond789(system(syscmd.c_str()), succ, fail);
	displayTimeInt(t);
	return 0;
}

int removeFileNames(int type, char *argv[]){};

void respond789(int sys, string succ, string strange){
	switch(sys){
		case 789:
			cerr << "\t" << strange << endl;
		break;
		case 0:
			cout << "\t" << succ << endl;
			break;
		default:
			cout << "\terror occured, nothing done\n";
	}
}
