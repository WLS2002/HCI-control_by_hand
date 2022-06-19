#include <windows.h>
#include <tuple>
#include <iostream>
#include <json.hpp>
#include <utils.h>
#include <cso.h>

class HostController {
public:
    int screen_width;
    int screen_height;

    double low = 0.20, high = 0.80;
    POINT *cursor_pos;

    bool left_down, right_down, drawing_mode;

    std::vector<double *> drawingPoints;

    HandTracker *handTracker;

    long long last_time = 0;

    double* history_weight;
    int history_type = -1;

    HostController(HandTracker *ht) {
        this->handTracker = ht;
        cursor_pos = new POINT;
        left_down = false;
        right_down = false;
        drawing_mode = false;
        init_screensize();
    }

    void init_screensize() {
        HDC hdc = GetDC(nullptr);                           // 得到屏幕DC
        screen_width = GetDeviceCaps(hdc, HORZRES);
        screen_height = GetDeviceCaps(hdc, VERTRES);
        ReleaseDC(nullptr, hdc);
        std::cout << "[Successful] screen_width: " << screen_width << " screen_height: " << screen_height << std::endl;
    }

    static void set_cursor_pos(int x, int y) {
        SetCursorPos(x, y);
    }

    static void set_cursor_pos(double x, double y) {
        SetCursorPos((int) x, (int) y);
    }

    void get_cursor_pos() {
        GetCursorPos(cursor_pos);
    }

    void drawRec(Mat image, double x, double y, double size_x, double size_y){
        double x0[] {x, y};
        double x1[] {x + size_x, y};
        double x2[] {x, y + size_y};
        double x3[] {x + size_x, y + size_y};

        line(image, Point(x0[0], x0[1]), Point(x1[0], x1[1]), Scalar(255, 0, 0),3, LINE_AA);
        line(image, Point(x0[0], x0[1]), Point(x2[0], x2[1]), Scalar(255, 0, 0),3, LINE_AA);
        line(image, Point(x1[0], x1[1]), Point(x3[0], x3[1]), Scalar(255, 0, 0),3, LINE_AA);
        line(image, Point(x2[0], x2[1]), Point(x3[0], x3[1]), Scalar(255, 0, 0),3, LINE_AA);
    }

    void drawTri(Mat image, double x0, double y0, double x1, double y1, double x2, double y2){
        double p0[] {x0, y0};
        double p1[] {x1, y1};
        double p2[] {x2, y2};

        line(image, Point(p0[0], p0[1]), Point(p1[0], p1[1]), Scalar(255, 0, 0),3, LINE_AA);
        line(image, Point(p0[0], p0[1]), Point(p2[0], p2[1]), Scalar(255, 0, 0),3, LINE_AA);
        line(image, Point(p1[0], p1[1]), Point(p2[0], p2[1]), Scalar(255, 0, 0),3, LINE_AA);
    }

    void drawCir(Mat image, double x, double y, double size){
        circle(image, Point(x, y), size, Scalar(255, 0, 0), 3);
    }

    void trigger_drawing(Mat& img){

        cout << "trigger!" << endl;
        CSO_REC csoRec(drawingPoints);
        csoRec.search();
        double recBest = csoRec.bestFitness;
        cout << "rec: " << csoRec.bestFitness << endl;

        CSO_TRI csoTri(drawingPoints);
        csoTri.search();
        double triBest = csoTri.bestFitness;
        cout << "tri: " << csoTri.bestFitness << endl;

        CSO_CIR csoCir(drawingPoints);
        csoCir.search();
        double cirBest = csoCir.bestFitness;
        cout << "cir: " << csoCir.bestFitness << endl;

        if(triBest > recBest && triBest > cirBest){
            history_weight = csoTri.globalBest;
            history_type = 0;

        } else if (recBest > triBest && recBest > cirBest){
            history_weight = csoRec.globalBest;
            history_type = 1;

        } else{
            history_weight = csoCir.globalBest;
            history_type = 2;

        }
    }

    void step(Mat& img, nlohmann::json &result) {

        long long current_time = get_ms();
        double fps = 1000.0 / (current_time - last_time);
        last_time = current_time;

        get_cursor_pos();

        if (result[0].is_null()) { // no hand
            if (left_down) {
                mouse_event(MOUSEEVENTF_LEFTUP, cursor_pos->x, cursor_pos->y, 0, 0);
                left_down = false;
            }

            if (right_down) {
                mouse_event(MOUSEEVENTF_RIGHTUP, cursor_pos->x, cursor_pos->y, 0, 0);
                right_down = false;
            }

            if(drawing_mode) {
                trigger_drawing(img);
                drawing_mode = false;
            }
        }
        else {
            int left_click_count = get_left_click_count(result), right_click_count = get_right_click_count(
                    result), drawing_count = check_drawing_mode(result);

            if (drawing_count >= 2) {
                if (!drawing_mode) {
                    drawing_mode = true;
                    drawingPoints.clear();
                    history_type = -1;
                }
            }
            else if (1 <= drawing_count && drawing_count <= 1) {

            }
            else {
                if (drawing_mode) {
                    trigger_drawing(img);
                    drawing_mode = false;
                }
            }

            if(!drawing_mode){
                if (left_click_count >= 8) { // must be clicking
                    if (!left_down) {
                        mouse_event(MOUSEEVENTF_LEFTDOWN, cursor_pos->x, cursor_pos->y, 0, 0);
                        left_down = true;
                    }
                }
                else if (4 <= left_click_count && left_click_count <= 7) {
                    // transition state, doing nothing at all
                }
                else {
                    if (left_down) {
                        mouse_event(MOUSEEVENTF_LEFTUP, cursor_pos->x, cursor_pos->y, 0, 0);
                        left_down = false;
                    }
                }

                if (right_click_count >= 11) { // must be clicking
                    if (!right_down) {
                        mouse_event(MOUSEEVENTF_RIGHTDOWN, cursor_pos->x, cursor_pos->y, 0, 0);
                        right_down = true;
                    }
                }
                else if (4 <= right_click_count && right_click_count <= 10) {
                    // transition state, doing nothing at all
                }
                else {
                    if (right_down) {
                        mouse_event(MOUSEEVENTF_RIGHTUP, cursor_pos->x, cursor_pos->y, 0, 0);
                        right_down = false;
                    }
                }
            }
            else {
                if (left_down) {
                    mouse_event(MOUSEEVENTF_LEFTUP, cursor_pos->x, cursor_pos->y, 0, 0);
                    left_down = false;
                }

                if (right_down) {
                    mouse_event(MOUSEEVENTF_RIGHTUP, cursor_pos->x, cursor_pos->y, 0, 0);
                    right_down = false;
                }
            }


            std::tuple<double, double> handPosition = get_center(result);
            double x = (std::get<0>(handPosition) - low) / (high - low);
            double y = (std::get<1>(handPosition) - low) / (high - low);
            if (x < 0) x = 0;
            if (x > 1) x = 1;
            if (y < 0) y = 0;
            if (y > 1) y = 1;

            if (drawing_mode) {
                auto *tmp = new double[2];
                tmp[0] = x * 640;
                tmp[1] = y * 480;
                drawingPoints.push_back(tmp);
            }

            int x_pos = x * screen_width;
            int y_pos = y * screen_height;
            set_cursor_pos(x_pos, y_pos);

        }

        if (history_type == -1) {

        } else if (history_type == 0) {
            drawTri(img, history_weight[0], history_weight[1], history_weight[2], history_weight[3], history_weight[4], history_weight[5]);
            putText(img, "Triangle!", Point(0, 100), 2, 1, cv::FONT_HERSHEY_COMPLEX, 2, 8, false);
        } else if (history_type == 1) {
            drawRec(img, history_weight[0], history_weight[1], history_weight[2], history_weight[3]);
            putText(img, "Rectangle!", Point(0, 100), 2, 1, cv::FONT_HERSHEY_COMPLEX, 2, 8, false);
        } else {
            drawCir(img, history_weight[0], history_weight[1], history_weight[2]);
            putText(img, "Circle!", Point(0, 100), 2, 1, cv::FONT_HERSHEY_COMPLEX, 2, 8, false);
        }
        handTracker->show_window(img, result, drawingPoints, fps);
    }


};
