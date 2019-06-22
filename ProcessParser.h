#pragma once  //I added
#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include "constants.h"
#include "Process.h"  //I added
#include "Util.h"   //I added


using namespace std;


//ProcessParser class interacts with the file system
class ProcessParser{
private:
    std::ifstream stream;
//I moved public down and brought out the margin
public:
    static string getCmd(string pid);  //return type string and parameter is pid.  get the command to run a process, 
                                      //and pass the pid to this function.  based on the pid it needs to return the
                                      //command as a string.
    static vector<string> getPidList();   //get a list of all the pid('s) and return in a vector of type string
    static string getVmSize(string pid);  //get virtual memory size based on the process (pid) passed to it and return
                                               //it as a string
    static std::string getCpuPercent(string pid);
    static int getSysUpTime();  //I took out long, so not long int anymore
    static std::string getProcUpTime(string pid);
    static string getProcUser(string pid); 
    static vector<string> getSysCpuPercent(string coreNumber = "");
    static float getSysRamPercent();
    static string getSysKernelVersion();
    static int getTotalThreads();
    static int getTotalNumberOfProcesses();
    static int getNumberOfRunningProcesses();
    static string getOSName();
    static std::string printCpuStats(std::vector<std::string> values1, std::vector<std::string>values2);

    static bool isPidExisting(string pid);

    static int getNumberOfCores(); // I added.  function defined below, but not declared here.
};
// TODO: Define all of the above functions below:


std::string getVmSize(std::string pid){  //I entered this method, but was supplied
    std::string line;
    //Declaring search attribute for file
    std::string name = "VmData";  //the line in the file that we need to get the data from starts with VmData
           //The entire line is structured as --- VmData:    1131692 kB -----
    std::string value;
    float result;
    //opening stream for specific file
    std::ifstream stream; //declare a stream

    //fill stream with a valid stream
    Util::getStream(Path::basePath() + pid + Path::statusPath(),stream);  //  /proc/4431/status is
                             //what this line is looking at (if 4431 was the current pid)
    while(std::getline(stream, line)){
        //searching line by line
        if(line.compare(0, name.size(), name) == 0){ //starting at the 0th position of the line go through the string 
             //when you reach the end is it actually the same size as name.
            //slicing string line on ws for values using sstream.
            //once we find VmData then we want to extract what is after it in that same line.
            std::istringstream buf(line);  //read a line into input stream buf
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values (beg, end);
            //conversion of kB to GB
            result = (stof(values[1])/float(1024));  //The entire line is structured as --- VmData:    1131692 kB -----
                     //The data we want is after VmData.  VmData is element 0, so we want to extract element 1 - which is
                     //specified in the values[1] return command. stof converts string to float, and then divide by the 
                     //float 1024 to get GB
            break;
        }
    };
    return to_string(result);
}


std::string getCpuPercent(std::string pid){
    //acquiring relevant times for calculation of active occupation of CPU for selected process

    //information is parsed from /proc/stat

    //read this information into 1 line.  The output from stat is all 1 line. Put the line into a vector and then parse 
    //through to extract positions 14, 15, 16, and 21.  After extraction convert each to floats and place into variables. 
    //Then make some calculations on these variables to determine CPU percent usage.

    //Path::basePath()+ pid + "/" + Path::statPath()

    std::string line;
    std::string value;
    float result;

    //opening stream for specific file
    std::ifstream stream; //declare stream

    //fill stream with a valid stream
    Util::getStream(Path::basePath() + pid + Path::statusPath(), stream);  

    std::getline(stream, line);
    string str = line;
    std::istringstream buf(str);  //There is only 1 line to read in total, so read it into the string str
    std::istream_iterator<string> beg(buf), end;
    std::vector<string> values (beg, end);  //done reading in the string
    
    //aquiring relevant times for calculation of active occupation of CPU for selected process
    float utime = stof(ProcessParser::getProcUpTime(pid));

    float stime = stof(values[14]); //index position 14
    float cutime = stof(values[15]); //index position 15
    float cstime = stof(values[16]); //index position 16
    float starttime = stof(values[21]); //index position 21

    float uptime = ProcessParser::getSysUpTime();

    float freq = sysconf(_SC_CLK_TCK);

    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime/freq);
    result = 100.0*((total_time/freq)/seconds);

    return to_string(result); //convert result to a string and return it
}


//retrieve position 13 from the file.
std::string ProcessParser::getProcUpTime(string pid){  //get process up time...(S/B GetProcUpTime   ??)
    std::string line;
    std::string value;
    float result;

    std::ifstream stream;  //declare a stream

    //fill stream with a valid stream
    Util::getStream(Path::basePath() + pid + Path::statPath(), stream);

    std::getline(stream, line);
    string str = line;
    std::istringstream buf(str);
    std::istream_iterator<string> beg(buf), end;
    std::vector<string> values(beg, end); // done!
    // Using sysconf to get clock ticks of the host machine
    return to_string(float(stof(values[13])/sysconf(_SC_CLK_TCK)));   //puts the return and calculations into 1 line.
        // "/sysconf(_SC_CLK_TCK) "gets how many clock ticks
        //from the host machine.  
        //  (float(stof(values[13]) gets the value at position 13 and converts to a float.  Then the code is dividing
        //this number by # of clock ticks.
        //this is then returned as a string. 

}

int ProcessParser::getSysUpTime() {  //declared wrong at top or here?  GetSysUpTime
    std::string line;

    std::ifstream stream;  //declare a new stream

    //Fill stream with a valid stream
    Util::getStream(Path::basePath() + Path::upTimePath(), stream);

    getline(stream,line);
    std::istringstream buf(line);
    std::istream_iterator<string> beg(buf), end;
    std::vector<string> values(beg, end);
    return stoi(values[0]); // extracting position 0, convert string to int and return.  Even though the number has a 
            //decimal it will be dropped with no problem at conversion to int from long.
}


std::string ProcessParser::getProcUser(string pid){  //get process user
    std::string line;
    std::string name = "Uid:";
    std::string result ="";

    std::ifstream stream;  //delclare a stream
    
    //Fill stream with a valid stream
    Util::getStream(Path::basePath() + pid + Path::statusPath(), stream);

    // Getting UID for user
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(),name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result =  values[1];
            break;
        }
    }


    //COMMENTED OUT BECAUSE A REDECLARATION std::ifstream stream;  //delcare a stream

    Util::getStream("/etc/passwd", stream);
    name =("x:" + result);
    // Searching for name of the user with selected UID  (from etc/passwd)
    while (std::getline(stream, line)) {
        if (line.find(name) != std::string::npos) {
            result = line.substr(0, line.find(":"));
            return result;
        }
    }
    return "";
}

//this was a straight copy.  Should work as is
std::vector<string> ProcessParser::getPidList(){  //return a Pid List from /proc directory (process ID directories) in
                                                   // a vector of strings (directory names)
    DIR* dir;  //a pointer of type DIR for directory
    // Basically, we are scanning /proc dir for all directories with numbers as their names
    // If we get valid check we store dir names in vector as list of machine pids
    std::vector<string> container;  //declare vector to hold file name (numbers) to return.  container is the return value
    if(!(dir = opendir("/proc")))  //try to open directory /proc.  
        throw std::runtime_error(std::strerror(errno));//  if there is an error display an error, otherwise continue

    while (dirent* dirp = readdir(dir)) {
        // check to see if this is this a directory
        if(dirp->d_type != DT_DIR)
            continue;
        // Is every character of the name a digit?
        if (all_of(dirp->d_name, dirp->d_name + std::strlen(dirp->d_name), [](char c){ return std::isdigit(c); })) {
            //         1st                                         last
            //all_of function takes a range of 1st to last.  specify a test.  this is a lamda function.
            //make sure what we are accessing is a directory and the name of the directory is all digits.
            container.push_back(dirp->d_name);  
            // lamda function that checks to make sure every char in the specified range is a number
            //want to make sure every character of the directory name is a digit
        }
    }
    //Validating process of directory closing
    if(closedir(dir))
        throw std::runtime_error(std::strerror(errno));
    return container;  //return the vector of strings

}

std::string ProcessParser::getCmd(string pid){  //retrieve the command that executed the process
    string line;

    std::ifstream stream;  //declare a new stream

    //Fill stream with a valid stream
    //from above for example//Util::getStream(Path::basePath() + Path::upTimePath(), stream);


    Util::getStream(Path::basePath() + pid + Path::cmdPath(), stream);

    std::getline(stream, line);
    return line;
}

int ProcessParser::getNumberOfCores(){   //removed std::
    // Get the number of host cpu cores
    string line;
    string name = "cpu cores";

    ifstream stream;  //declare a new stream
    
    //fill stream with a valid stream
    Util::getStream((Path::basePath() + "cpuinfo"), stream);  //added 2nd argument
    
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(),name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            return stoi(values[3]); //return the 3rd element of the vector which contains the number of CPU cores on the
        }                           //system
    }
    return 0;
}


std::vector<string> ProcessParser::getSysCpuPercent(string coreNumber){  //get the CPU % usage
    // It is possible to use this method for selection of data for overall cpu or every core.
    // when nothing is passed "cpu" line is read
    // when, for example "0" is passed  -> "cpu0" -> data for first core is read
    string line;
    string name = "cpu" + coreNumber;  //example: returns cpu0, cpu1, cpu2, etc
    string value;
    int result;

    ifstream stream;  //declare a new stream

    //fill new stream with a valid stream
    Util::getStream(Path::basePath() + Path::statPath(), stream);  //added 2nd argument "stream"
    
    while (std::getline(stream, line)) {  
        if (line.compare(0, name.size(),name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            // set of cpu data active and idle times;
            return values;
        }
    }
    return (vector<string>());
}

float get_sys_active_cpu_time(vector<string> values){  //get system active time ...removed std::
    return (stof(values[S_USER]) +
            stof(values[S_NICE]) +
            stof(values[S_SYSTEM]) +
            stof(values[S_IRQ]) +
            stof(values[S_SOFTIRQ]) +
            stof(values[S_STEAL]) +
            stof(values[S_GUEST]) +
            stof(values[S_GUEST_NICE]));
}

float get_sys_idle_cpu_time(vector<string>values){  //get system idle time  //removed std::
    return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}

std::string ProcessParser::printCpuStats(vector<string> values1, vector<string> values2){
/*
Because CPU stats can be calculated only if you take measures in two different time,
this function has two parameters: two vectors of relevant values.
We use a formula to calculate overall activity of processor.
*/
    float activeTime = get_sys_active_cpu_time(values2) - get_sys_active_cpu_time(values1);  //calculate active time
    float idleTime = get_sys_idle_cpu_time(values2) - get_sys_idle_cpu_time(values1);  //calculate idle time
    float totalTime = activeTime + idleTime;  //calculate total time up
    float result = 100.0*(activeTime / totalTime);  //convert to percentage
    return to_string(result);  //return the result
}



//opening a stream on proc/memInfo where we can read three characteristic values: MemAvailable,buffers and memFree. 
//From these values we are calculating RAM usage in percentage.
float ProcessParser::getSysRamPercent(){   //removed std::
    string line;
    string name1 = "MemAvailable:";  //looking for field with this name and storing in name1
    string name2 = "MemFree:";  //looking for field with this name and storing in name2
    string name3 = "Buffers:";  //looking for field with this name and storing in name3

    string value;
    int result;
    ifstream stream; //declare a new stream

    //fill new stream with a valid stream
    Util::getStream(Path::basePath() + Path::memInfoPath(), stream);  //added 2nd argument "stream"

    float total_mem = 0;   //name1 values [1], 2nd position
    float free_mem = 0;   //name2 values [1]
    float buffers = 0;     //name3 values[1]
    while (std::getline(stream, line)) {
        if (total_mem != 0 && free_mem != 0)     //total_mem and free_mem were mispelled. fixed.
            break;
        if (line.compare(0, name1.size(), name1) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            total_mem = stof(values[1]);
        }
        if (line.compare(0, name2.size(), name2) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            free_mem = stof(values[1]);
        }
        if (line.compare(0, name3.size(), name3) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            buffers = stof(values[1]);
        }
    }
    //calculating usage:
    return float(100.0*(1-(free_mem/(total_mem-buffers))));
}



//Open a stream on /proc/version. Getting data about the kernel version.
std::string ProcessParser::getSysKernelVersion(){
    string line;
    string name = "Linux version ";

    ifstream stream;  //declare a new stream
    
    //fill new stream with a valid stream
    Util::getStream(Path::basePath() + Path::versionPath(), stream); //added 2nd argument "stream"
    
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(),name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            return values[2];
        }
    }
    return "";
}



//Read /etc/os-release. We expect a string with extra characters, which we delete based on specifications in 
//documentation. The result is the name of the operating system.
std::string ProcessParser::getOSName(){
    string line;
    string name = "PRETTY_NAME=";

    ifstream stream;  //declare a new stream
    
    //fill new stream with a valid stream
    Util::getStream(("/etc/os-release"), stream);  //added 2nd argument "stream"

    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
              std::size_t found = line.find("=");
              found++;
              string result = line.substr(found);
              result.erase(std::remove(result.begin(), result.end(), '"'), result.end());
              return result;
        }
    }
    return "";

}


//The total thread count is calculated, rather than read from a specific file.
//open every PID folder and read its thread count. After that, we sum the thread counts to calculate the total number of 
//threads on the host machine.
int ProcessParser::getTotalThreads(){  //removed std::
    string line;
    int result = 0;
    string name = "Threads:";
    vector<string>_list = ProcessParser::getPidList();
    for (int i=0 ; i<_list.size();i++) {
    string pid = _list[i];
    //getting every process and reading their number of their threads

    ifstream stream;  //delcare a new stream
    
    //fill new stream with a valid stream
    Util::getStream(Path::basePath() + pid + Path::statusPath(), stream);  //added 2nd argument "stream"
    
        //indented this while loop to look proper after discovering missing bracket below.
        while (std::getline(stream, line)) {
            if (line.compare(0, name.size(), name) == 0) {
                istringstream buf(line);
                istream_iterator<string> beg(buf), end;
                vector<string> values(beg, end);
                result += stoi(values[1]);
                break;
            }
        }
    return result;
    }
}  //this bracket was missing, so inserted




//Retrieve this info by reading /proc/stat. Search for the “processes” line.
int ProcessParser::getTotalNumberOfProcesses(){  //removed std::
    string line;
    int result = 0;
    string name = "processes";

    ifstream stream;  //declare a new stream
    
    //fill new stream with a valid stream
    Util::getStream(Path::basePath() + Path::statPath(), stream);  //added 2nd argument "stream"

    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result += stoi(values[1]);
            break;
        }
    }
    return result;
}



//This function uses the same logic as previous functions.
int ProcessParser::getNumberOfRunningProcesses(){    //removed std::
    string line;
    int result = 0;
    string name = "procs_running";

    ifstream stream;  //declare a new stream
    
    //fill new stream with a valid stream
    Util::getStream((Path::basePath() + Path::statPath()), stream);  //added 2nd argument "stream"

    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result += stoi(values[1]);
            break;
        }
    }
    return result;
}


