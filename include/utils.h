#include <cstdio>
#include <windows.h>
#include <string>
#include <thread>
#include<cmath>
#include <json.hpp>
#include <iostream>

#define int_to_char(ch, a) sprintf(ch,"%d",a);

int64_t get_ms();

int* random_order(int size);

void start_python(const char* interpreter);

#define THUMB_STRAIGHT 1<<0
#define INDEX_FINGER_STRAIGHT 1<<1
#define MIDDLE_FINGER_STRAIGHT 1<<2
#define RING_FINGER_STRAIGHT 1<<3
#define LITTLE_FINGER_STRAIGHT 1<<4

double r();

double get_angle(double p0[], double p1[], double p2[], double p3[]);

bool check_single_finger(nlohmann::json& result, int index);

int get_finger_state(nlohmann::json& result);

double get_distance(const double p0[], const double p1[]);

std::tuple<double, double> get_center(nlohmann::json& result);

int get_left_click_count(nlohmann::json& result);

int get_right_click_count(nlohmann::json& result);

double get_point_line_distance(const double* point, const double* line_start, const double* line_end);

double triangle_error(std::vector<double*>& points, double x0, double y0, double x1, double y1, double x2, double y2);

double rectangle_error(std::vector<double*>& points, double x, double y, double size_x, double size_y);

double circle_error(std::vector<double*>& points, double x, double y, double size);

int check_drawing_mode(nlohmann::json& result);