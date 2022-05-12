#include <cstdio>
#include <chrono>
#include <windows.h>
#include <string>
#include <thread>

#define int_to_char(ch, a) sprintf(ch,"%d",a);

int64_t get_ms();

void start_python(const char* interpreter);
