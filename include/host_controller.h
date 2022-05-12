#include <windows.h>
#include <tuple>
#include <iostream>
#include <json.hpp>


class HostController{
public:

    const double SHACK_CEO = 0.008;
    const double SPEED = 7.5;

    int screen_width;
    int screen_height;

    POINT* cursor_pos;

    double volatile v_x = 0;
    double volatile v_y = 0;
    // std::mutex mtx = std::mutex();


    std::tuple<double, double, long> last_location;

    HostController(){
        cursor_pos = new POINT;
        init_screensize();
        last_location = std::tuple(-1, -1, -1);
    }

    void init_screensize(){
        HDC hdc = GetDC(nullptr);                           // 得到屏幕DC
        screen_width = GetDeviceCaps(hdc, HORZRES);
        screen_height = GetDeviceCaps(hdc, VERTRES);
        ReleaseDC(nullptr, hdc);
        std::cout << "[Successful] screen_width: " << screen_width << " screen_height: " << screen_height << std::endl;
    }

    static void set_cursor_pos(int x, int y){
        SetCursorPos(x, y);
    }

    static void set_cursor_pos(double x, double y){
        SetCursorPos((int) x, (int) y);
    }

    void get_cursor_pos(){
        GetCursorPos(cursor_pos);
    }

    void step(nlohmann::json result, long time){
        if(result[0].is_null()){  // no hands
            clear_velocity();
            return;
        }
        auto pos = get_hand_location(result);

        if(get<0>(last_location) == -1){  // first time
            last_location = std::tuple(get<0>(pos), get<1>(pos), time);
            return;
        }

        double n_x = get<0>(pos), n_y = get<1>(pos);
        double o_x = get<0>(last_location), o_y = get<1>(last_location);

        if(check_shake(n_x, n_y, o_x, o_y)){
            clear_velocity();
            return;
        }

        //std::cout << n_x << " " << n_y << " " << o_x << " " << o_y << " " << time << " " << get<2>(last_location) << std::endl;

        update_velocity(n_x, n_y, o_x, o_y, time, get<2>(last_location));

        last_location = std::tuple(n_x, n_y, time);
    }

    std::tuple<double, double> get_hand_location(nlohmann::json result){
        return std::tuple(result[0][0][0], result[0][0][1]);
    }

    bool check_shake(double n_x, double n_y, double o_x, double o_y){
        return abs(n_x - o_x) < SHACK_CEO && abs(n_y - o_y) < SHACK_CEO;
    }

    void update_velocity(double n_x, double n_y, double o_x, double o_y, long new_time, long last_time){
        // mtx.lock();
        v_x = (n_x - o_x) * screen_width / (new_time - last_time) * SPEED;
        v_y = (n_y - o_y) * screen_height / (new_time - last_time) * SPEED;
        // mtx.unlock();
        //std::cout << "[update_vel]";
        //std::cout << v_x << " " << v_y << std::endl;
    }

    void limit_screen(double& new_x, double& new_y){
        if(new_x < 0) new_x = 0;
        if(new_x > screen_width) new_x = screen_width;
        if(new_y < 0) new_y = 0;
        if(new_y > screen_height) new_y = screen_height;
    }

    void clear_velocity(){
        // mtx.lock();
        v_x = 0;
        v_y = 0;
        // mtx.unlock();
    }
};

[[noreturn]] void change_cursor_pos(void*);
