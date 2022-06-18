#include "utils.h"
#include <iostream>
const int FINGER_INFO[5][4]{{1,  2,  3,  4},
                            {5,  6,  7,  8},
                            {9,  10, 11, 12},
                            {13, 14, 15, 16},
                            {17, 18, 19, 20}};

int64_t get_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}

double r(){
    return (double)rand() / RAND_MAX;
}

int* random_order(int size){
    int* order = new int[size];
    for(int i = 0; i < size; i++) order[i] = i;
    for(int i = size; i >= 1; i--){
        int r = rand() % size;
        std::swap(order[i - 1], order[r]);
    }
    return order;
}

void start_python(const char *interpreter) {
    std::string command;
    command.append("\"").append(interpreter).append("\" ../python/main.py");
    system(command.c_str());
}

double get_angle(double p0[], double p1[], double p2[], double p3[]) {
    double v0[2] = {p1[0] - p0[0], p1[1] - p0[1]};
    double v1[2] = {p3[0] - p2[0], p3[1] - p2[1]};

    double cos = (v0[0] * v1[0] + v0[1] * v1[1]) / (sqrt(v0[0] * v0[0] + v0[1] * v0[1])) /
                 (sqrt(v1[0] * v1[0] + v1[1] * v1[1]));
    double res = acos(cos) / std::numbers::pi * 180;
    return res;
}

/**
 * checks one finger is straight or not
 * @param result
 * @param index
 * @return true means straight, false means curve
 */
bool check_single_finger(nlohmann::json &result, int index) {
    double pos[4][2] = {{result[0][FINGER_INFO[index][0]][0], result[0][FINGER_INFO[index][0]][1]},
                        {result[0][FINGER_INFO[index][1]][0], result[0][FINGER_INFO[index][1]][1]},
                        {result[0][FINGER_INFO[index][2]][0], result[0][FINGER_INFO[index][2]][1]},
                        {result[0][FINGER_INFO[index][3]][0], result[0][FINGER_INFO[index][3]][1]}};
    if (get_angle(pos[0], pos[1], pos[1], pos[2]) > 90) return false;
    if (get_angle(pos[1], pos[2], pos[2], pos[3]) > 90) return false;
    return true;
}

int get_finger_state(nlohmann::json &result) {
    int res = 0;
    if (check_single_finger(result, 0)) res |= THUMB_STRAIGHT;
    if (check_single_finger(result, 1)) res |= INDEX_FINGER_STRAIGHT;
    if (check_single_finger(result, 2)) res |= MIDDLE_FINGER_STRAIGHT;
    if (check_single_finger(result, 3)) res |= RING_FINGER_STRAIGHT;
    if (check_single_finger(result, 4)) res |= LITTLE_FINGER_STRAIGHT;
    return res;
}

double get_distance(const double p0[], const double p1[]) {
    return sqrt((p1[0] - p0[0]) * (p1[0] - p0[0]) + (p1[1] - p0[1]) * (p1[1] - p0[1]));
}

int get_left_click_count(nlohmann::json& result){
    if(result[0].is_null()) return 0;
    double ps[21][2];
    for (int i = 0; i <= 20; i++) {
        ps[i][0] = result[0][i][0];
        ps[i][1] = result[0][i][1];
    }
    double d48 = get_distance(ps[4], ps[8]);

    int sum = 0;
    for (int i = 1; i < 20; i++){
        if (i == 4 || i == 8 || i == 12 || i == 16)
            continue;
        if (d48 < get_distance(ps[i], ps[i + 1]))
            sum++;
    }
    return sum;
}

int get_right_click_count(nlohmann::json& result){
    if(result[0].is_null()) return 0;
    double ps[21][2];
    for (int i = 0; i <= 20; i++) {
        ps[i][0] = result[0][i][0];
        ps[i][1] = result[0][i][1];
    }
    double d812 = get_distance(ps[8], ps[12]);

    int sum = 0;
    for (int i = 1; i < 20; i++){
        if (i == 4 || i == 8 || i == 12 || i == 16)
            continue;
        if (d812 < get_distance(ps[i], ps[i + 1]))
            sum++;
    }
    return sum;
}

int check_drawing_mode(nlohmann::json& result){
    if(result[0].is_null()) return 0;
    double ps[21][2];
    for (int i = 0; i <= 20; i++) {
        ps[i][0] = result[0][i][0];
        ps[i][1] = result[0][i][1];
    }
    double d420 = get_distance(ps[4], ps[20]);
    double d416 = get_distance(ps[4], ps[16]);
    int sum = 0;
    for (int i = 1; i < 20; i++){
        if (i == 4 || i == 8 || i == 12 || i == 16)
            continue;
        if (d420 < get_distance(ps[i], ps[i + 1]))
            sum++;
        if (d416 < get_distance(ps[i], ps[i + 1]))
            sum++;
    }
    return sum;
}

std::tuple<double, double> get_center(nlohmann::json &result) {
    double sum_x = 0, sum_y = 0;
    for (int i = 0; i <= 20; i++) {
        if (5 <= i && i <= 12)
            continue;
        sum_x += (double) result[0][i][0];
        sum_y += (double) result[0][i][1];
    }

    return std::tuple(sum_x / (21 - 8), sum_y / (21 - 8));
}

double get_point_line_distance(const double* point, const double* line_start, const double* line_end){
    double x1 = line_start[0], y1 = line_start[1];
    double x2 = line_end[0], y2 = line_end[1];
    double x = point[0], y = point[1];

    double cross = (x2 - x1) * (x - x1) + (y2 - y1) * (y - y1);
    if (cross <= 0) return sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));

    double d2 = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
    if(cross >= d2) return sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));

    double r = cross / d2;
    double px = x1 + (x2 - x1) * r;
    double py = y1 + (y2 - y1) * r;
    return sqrt((x - px) * (x - px) + (y - py) * (y - py));

}

double triangle_error(std::vector<double*>& points, double x0, double y0, double x1, double y1, double x2, double y2){
    double p0[] {x0, y0};
    double p1[] {x1, y1};
    double p2[] {x2, y2};

    double error = 0, min_distance, new_distance;
    for(double* point: points){
        min_distance = get_point_line_distance(point, p0, p1);

        new_distance = get_point_line_distance(point, p0, p2);
        min_distance = new_distance < min_distance ? new_distance : min_distance;

        new_distance = get_point_line_distance(point, p1, p2);
        min_distance = new_distance < min_distance ? new_distance : min_distance;

        error += min_distance;
    }
    return -error;
}

double rectangle_error(std::vector<double*>& points, double x, double y, double size_x, double size_y){
    double x0[] {x, y};
    double x1[] {x + size_x, y};
    double x2[] {x, y + size_y};
    double x3[] {x + size_x, y + size_y};

    double error = 0, min_distance, new_distance;
    for(double* point: points){
        min_distance = get_point_line_distance(point, x0, x1);

        new_distance = get_point_line_distance(point, x0, x2);
        min_distance = new_distance < min_distance ? new_distance : min_distance;

        new_distance = get_point_line_distance(point, x1, x3);
        min_distance = new_distance < min_distance ? new_distance : min_distance;

        new_distance = get_point_line_distance(point, x2, x3);
        min_distance = new_distance < min_distance ? new_distance : min_distance;

        error += min_distance;
        //std::cout << point[0] << " " << point[1] << " " << min_distance << std::endl;
    }
    return -error;
}

double circle_error(std::vector<double*>& points, double x, double y, double size){
    double error = 0, distance;
    for(double* point: points){
        distance = sqrt((x - point[0]) * (x - point[0]) + (y - point[1]) * (y - point[1]));
        error += abs(distance - size);
    }
    return -error;
}