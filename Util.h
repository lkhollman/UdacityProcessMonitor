#include <string>
#include <fstream>


// Classic helper class with classic helper functions.
//This class is used as a storage place for general functions.
class Util {
    public:
        static std::string convertToTime ( long int input_seconds );
        static std::string getProgressBar(std::string percent);
        static std::string getStream(std::string path, std::ifstream& stream); //need getstream to open up a stream to a file for
//processing.  We aren't writing a getStream each time we want to read a file because we want error handling in 1 place
//which is below.   //wrapper for creating streams
};

std::string Util::convertToTime (long int input_seconds){  //unix is going to return a qty of seconds, so you have to 
    long minutes = input_seconds / 60;                        //divide it out and store into variables to put the time in a 
    long hours = minutes / 60;                               //useful format.
    long seconds = int(input_seconds%60);
    minutes = int(minutes%60);
    std::string result = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    return result;
}
// constructing string for given percentage
// 50 bars is uniformly streched 0 - 100 %
// meaning: every 2% is one bar(|)
std::string Util::getProgressBar(std::string percent){
    std::string result = "0%% ";
    int _size= 50;
    int  boundaries;
    try {
        boundaries = (stof(percent)/100)*_size;
    } catch (...){
    boundaries = 0;
    }

    for(int i=0;i<_size;i++){
        if(i<=boundaries){
        result +="|";
        }
        else{
        result +=" ";
        }
    }

    result +=" " + percent.substr(0,5) + " /100%%";
    return result;
}

// wrapper for creating streams
std::string Util::getStream(std::string path, std::ifstream& stream){   //returns a Stream object of type ifstream
    stream.open (path, std::ifstream::in);  //attempt to open stream
    if (!stream && !stream.is_open()){ //if stream is false (can't open)
    stream.close();  //close the stream
    }
    try {
        throw std::runtime_error("Non - existing PID");   //throw a runtime error that states "Non - existing PID"
    }
    catch (...){
    }
    return path;
}
