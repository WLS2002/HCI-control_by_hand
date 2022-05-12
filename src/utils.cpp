#include "utils.h"

int64_t get_ms(){
    return std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count();
}

void start_python(const char* interpreter){
    std::string command;
    command.append("\"").append(interpreter).append("\" ../python/main.py");
    system(command.c_str());
}