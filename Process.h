#pragma once //I added
#include <string>
#include "ProcessParser.h"  //I added


using namespace std;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
private:
    string pid;  
    string user;
    string cmd;
    string cpu;
    string mem;
    string upTime;

public:
    Process(string pid){  //the public Process constructor.  public needs to initialize all the variables in private
        this -> pid = pid;
        this -> user = ProcessParser::getProcUser(pid);

        //TODOs: I defined these
        this -> mem = ProcessParser::getVmSize(pid);  //return pid for VMSize
        this -> cmd = ProcessParser::getCmd(pid);
        this -> upTime = ProcessParser::getProcUpTime(pid);
        this -> cpu = ProcessParser::getCpuPercent(pid);
    }
    void setPid(int pid);
    string getPid()const;
    string getUser()const;
    string getCmd()const;
    int getCpu()const;
    int getMem()const;
    string getUpTime()const;
    string getProcess();
};
void Process::setPid(int pid){
    this->pid = pid;
}
string Process::getPid()const {
    return this->pid;
}
string Process::getProcess(){
    if(!ProcessParser::isPidExisting(this->pid)){   //should it be getPidList??? and remove the definition from ProcessParser for isPidExisting???
        return "";
    }  //added brackets to this if statement

    this->mem = ProcessParser::getVmSize(this->pid);
    this->upTime = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);


    //concatenate the values and return in a string
    //TODO: finish the string! this->user + "   "+ mem...cpu...upTime...;
    return (this->pid + "   " 
            + this->user 
            + "   " 
            + this->mem.substr(0,5)    //substring from 0 to 5
            + "   " 
            + this->cpu.substr(0,5) 
            + "   " 
            + this->upTime.substr(0,5) //fixed from up_time
            + "   "
            + this->cmd.substr(0,30)
            + "..."); //I completed this.  
                                                                         
    
}
