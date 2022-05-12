#include <cv.h>
#include <host_controller.h>

std::pair<double, double> handle_points(char*);

void process_image(void*, void*);

void process_image(void* arg0, void* arg1){

    auto* ht = (hand_tracker*) arg0;
    auto* hc = (HostController*) arg1;

    ht->com->receive();  // wait for python process connect
    long last_time=-1;
    long delay_start, delay_end;

    cout << "[successful] start sending image" << endl;

    thread controller = thread(change_cursor_pos, hc);

    while(true){

        Mat img;
        *ht->cam->cap >> img;

        delay_start = get_ms();

        flip(img, img, 1);

        ht->com->send_img(img, ht->cam->length);  //send img to python
        ht->com->receive();   // receive hand location information

        auto j = json::parse(ht->com->receive_buffer);

        hc->step(j, get_ms());

        delay_end = get_ms();

        ht->show_window(img, j, delay_start, delay_end, last_time);

        last_time = delay_end;

        char c = waitKey(1);
        if(c == 27){ //esc
            break;
        }
    }
}

//std::pair<double, double> handle_points(char* res){
//    json j = json::parse(res);
//
//    double x, y;
//
//    if(!j[0][9].is_null()){
//        x = j[0][9][0];
//        y = j[0][9][1];
//    } else {
//        x = -1;
//        y = -1;
//    }
//    return std::pair<double, double>(x, y);
//
//}

int main(){

    const char* interpreter = R"(C:\Users\cvgcv\anaconda3\envs\mediapipe\python.exe)";

    cout << "try to open camera..." << endl;
    auto* cam = new camera(0);
    cout << "[successful] connect to camera" << endl;
    cout << "[successful] width: " << cam->width << ", height: " << cam->height << ", total length: " << cam->length << endl;

    cout << "try to start python process..." << endl;
    thread python = thread(start_python, interpreter);

    cout << "waiting python client connect..." << endl;
    auto* com = new communication();

    cout << "[successful] connect to python client" << endl;
    auto ht = new hand_tracker(cam, com);

    cout << "try to initialize host controller..." << endl;
    auto hc = new HostController();
    cout << "[successful] host controller has been initialized" << endl;

    thread capture_and_process = thread(process_image, ht, hc);
    capture_and_process.join();
}
