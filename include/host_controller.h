#include <windows.h>

class HostController{
public:
    int screen_width;
    int screen_height;
    POINT* cursor_pos;


    HostController(){
        cursor_pos = new POINT;
        init_screensize();
    }

    void init_screensize(){
        HDC hdc = GetDC(nullptr);                           // 得到屏幕DC
        screen_width = GetDeviceCaps(hdc, HORZRES);
        screen_height = GetDeviceCaps(hdc, VERTRES);
        ReleaseDC(nullptr, hdc);
        cout << "[Successful] screen_width: " << screen_width << " screen_height: " << screen_height << endl;
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

};