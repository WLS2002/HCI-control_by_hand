#include <host_controller.h>
#include <iostream>

[[noreturn]] void change_cursor_pos(void* args){
    HostController* hc = (HostController*) args;

    std::cout << "[successful] start controlling cursor" << std::endl;

    while(true){

        //std::cout << ht->v_x << " " << ht->v_y << std::endl;
        // ht->mtx.lock();
        hc->get_cursor_pos();
        double cur_x = hc->cursor_pos->x, cur_y = hc->cursor_pos->y;
        double new_x = cur_x + hc->v_x, new_y = cur_y + hc->v_y;

        //ht->limit_screen(new_x, new_y);

//      std::cout << new_x << " " << new_y << std::endl;
        HostController::set_cursor_pos(new_x, new_y);
        // ht->mtx.unlock();
        Sleep(1);
    }
}
