#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <cstring>
#include <algorithm>
#include <set>
#include <time.h>
#include <iomanip>
#include "options.h"

int display(Record &map, int deli);
int displayEntity(std::string name, Accume r, int deli);
std::string myTimeFormat(int t);

int appendtimeReduce(std::string dir, std::string &fn, time_t time, int deli){ 
	struct tm *tmd = localtime(&time);
	char tmp[20] = {0};
	switch(deli){
		case 1:
			strftime(tmp, sizeof(tmp), " %F", tmd);
			break;
		case 2:
			strftime(tmp, sizeof(tmp), " %Y-%m", tmd);
			break;
		case 3:
			strftime(tmp, sizeof(tmp), " %Y", tmd);
	}
	fn = dir + "/" + fn + tmp;
}
int readRecordFile(std::string dir_path, int deli, Record &map, int filter){
	std::ifstream fs(dir_path+"/"+".vimtime", std::ifstream::in);
	if(!fs) return 0;
	std::string rline, fn, event, arg;
	int lastopen, laststate, lastspare, lastcmd, state;
	time_t timestamp;
	int line_read = 0;
	for(line_read = 0; getline(fs, rline); line_read++){
		std::istringstream ss(rline);
		ss >> fn >> event >> arg;
		timestamp = stoi(arg, NULL);
		appendtimeReduce(dir_path, fn, timestamp, deli);
		auto it = map.find(fn);
		if(it == map.end()){
			if(filter)
				continue;
			map.emplace(fn, zero_record);
			it = map.find(fn);
		}
		if(event == "open"){
			lastopen = timestamp;
			laststate = timestamp;
			state = 0;
		} else if(event == "clos"){
			it->second[0] += timestamp - lastopen;
			it->second[2] += (timestamp - laststate) * state;
			it->second[4] += (timestamp - laststate) * (state == 0);
		} else if(event == "edit"){
			it->second[4] += (timestamp - laststate) * (state == 0);
			laststate = timestamp;
			state = 1;
		} else if(event == "norm"){
			it->second[2] += (timestamp - laststate) * state;
			laststate = timestamp;
			state = 0;
		} else if(event == "spar"){
			lastspare = timestamp;
		} else if(event == "back"){
			it->second[3-2*state] += (timestamp - lastspare);
		} else if(event == "scmd"){
			lastcmd = timestamp;
		} else if(event == "ecmd"){
			it->second[5] += (timestamp - lastcmd);
		} else if(event == "\\/rmd"){
			map.erase(it);
		} else if(event == "\\/mvd"){
			auto it2 = map.find(arg);
			if(it2 == map.end()){
				auto tmp = *it;
				Accume a = it->second;
				map.emplace(arg, a);
			}
			else{
				for(int i=0; i<sizeof(it->second); i++)
					it2->second[i] += it->second[i];
				map.erase(it);
			}
		} else{
			std::cerr << "Mysterious event '" << event << "' not understood\n";
		}
	}
	fs.close();
	return line_read;
}
	// open,edits,edita,norms,norma,cmd   
int traverse(std::string dir_path, std::set<std::string> &dirs){
	DIR *dir = opendir(dir_path.c_str());
	struct dirent *dp;
	dirs.insert(dir_path);
	while ((dp = readdir(dir)) != NULL) {
		if( dp->d_name == "." || dp->d_name == "..")
			continue;
		if( dp->d_type == DT_DIR)
			traverse(dir_path+"/"+dp->d_name, dirs);
	}
	(void)closedir(dir);
	return 0;
}
int listSum(int argc, char *argv[]){
	int deli = !strcmp(argv[1],"-d")*1 + !strcmp(argv[1],"-m")*2 + !strcmp(argv[1],"-y")*3;
	Record map;
	struct stat st;
	std::set<std::string> dirs, fns, fndirs;
	for(int i=1+!!deli; i<argc; i++){
		if(argv[i][strlen(argv[i])-1] == '/')
			argv[i][strlen(argv[i])-1] = '\0';
		lstat(argv[i], &st);
		if(S_ISDIR(st.st_mode))
			dirs.insert(argv[i]);
		else
			fns.insert(argv[i]);
	}
	for(auto dir : dirs){
		readRecordFile(dir, deli, map, 0);
	}
	for(auto fn : fns){
		if(map.find(fn) == map.end()){
			map.emplace(fn, zero_record);
			size_t r_end = fn.find_last_of("/");
			std::string fr = (r_end == std::string::npos)? ".":fn.substr(0, r_end-1);
			fndirs.insert(fr);
			std::string ft = (r_end == std::string::npos)? fn:fn.substr(r_end+1);
		}
	}
	for(auto dir : fndirs)
		readRecordFile(dir, deli, map, 1);
	return display(map, deli);
}
int display(Record &map, int deli){
	Accume sum = zero_record;
	std::string currfilename = "";
	if(deli){
		currfilename = map.begin()->first;
		currfilename = currfilename.substr(0, currfilename.find(" "));
		std::cout << std::setw(20) << currfilename;
		std::cout << "    open     edit           normal          cmd   \n"; 
	}
	else{
		std::cout << "filename                open     edit           normal          cmd   \n"; 
		std::cout << " ---------------------------------------------------------------------\n";
	}
	for(auto en : map){
		std::string fn = en.first.substr(0, en.first.find(" "));
		if(fn != currfilename && deli){
			displayEntity("SUM", sum, 0);
			sum = zero_record;
			std::cout << std::endl;
			currfilename = fn;
			std::cout << std::setw(20) << currfilename;
			std::cout << "    open     edit           normal          cmd   \n"; 
		}
		displayEntity(en.first, en.second, deli);
		for(int i=0; i<6; i++)
			sum[i] += en.second[i];
	}
	if(!deli)
		std::cout << " ---------------------------------------------------------------------\n";
	displayEntity("SUM", sum, 0);
	return 0; 
}
int displayEntity(std::string name, Accume r, int deli){
	const std::string timepre[] = {"   ", "   ", "/", "   ", "/", "   "};
	if(deli)
		name = name.substr(name.find(" ")+1);
	name = name.substr((name.size() <= 17)? 0:name.size()-17);
	std::cout << std::setw(20) << name;
	r[1] = r[2]-r[1]; r[3] = r[4]-r[3];
	for(int i=0; i<6; i++)
		std::cout << timepre[i] <<  myTimeFormat(r[i]);
	std::cout << std::endl;
}

std::string myTimeFormat(int t){
	const char *unit[] = {"\"", "'", "°", "d"};
	const int base[] = {60, 60, 24, 999999};
	int i = 0;
	while(t>=base[i]*base[i+1])
		t /= base[i++];
	std::ostringstream re;
	re << std::setw(2) << std::setfill('0') << t/base[i] << unit[i+1];
	re << std::setw(2) << std::setfill('0') << t%base[i] << unit[i];
	return re.str();
}
