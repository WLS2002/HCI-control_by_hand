#include <cstdio>
#include <chrono>
#include <windows.h>
#include <string>
#include <thread>

#define int_to_char(ch, a) sprintf(ch,"%d",a);

long get_ms();

std::thread start_python_thread(const char* interpreter);
