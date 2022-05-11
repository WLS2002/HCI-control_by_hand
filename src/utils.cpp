#include "utils.h"

long get_ms(){
    return std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count();
}

void start_python(const char* interpreter){
    std::string command;
    command.append("\"").append(interpreter).append("\" ../python/main.py");
    system(command.c_str());
}

std::thread start_python_thread(const char* interpreter){
    std::thread t(start_python, interpreter);
    return t;
}