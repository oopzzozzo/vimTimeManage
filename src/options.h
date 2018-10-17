#include <string>
#include <map>
#include <array>

typedef int(*Function)(int,char*[]);
typedef std::array<int, 6> Accume; // open,edita,edits,norma,norms,cmd   
typedef std::map<std::string, Accume> Record;
int displayUsage(char []);
int printHelp(int, char *[]); //3
int listSum(int, char *[]); //2
int aliasFileName(int cmd, char *[]); //1
int removeFileNames(int cmd, char *[]); //0

const Accume zero_record = {0,0,0,0,0,0};
const std::string options[] = {"-d", "-m", "-y", "-mv", "-mvu", "-mvf", "-rm", "-rmu", "-rmf", "help"};
const Function functions[]={listSum,aliasFileName, removeFileNames, printHelp};
