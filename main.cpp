#include <cv.h>
#include <host_controller.h>

using namespace nlohmann;
std::pair<double, double> handle_points(char*);

void process_image(HostController& hc){

    hc.handTracker->com->receive();  // wait for python process connect
    cout << "[successful] start sending image" << endl;

    while(true){

        Mat img;
        hc.handTracker->cam->cap >> img;


        flip(img, img, 1);

        hc.handTracker->com->send_img(img, hc.handTracker->cam->length);  //send img to python
        hc.handTracker->com->receive();   // receive hand location information

        json j = json::parse(hc.handTracker->com->receive_buffer);

        hc.step(img, j);

        //hc.handTracker->show_window(img, j, delay_start, delay_end, last_time);

        int c = waitKey(1);
        if(c == 27){ //esc
            break;
        }
    }
}

int main(){

    const char* interpreter = R"(C:\Users\cvgcv\anaconda3\envs\mediapipe\python.exe)";

    cout << "try to open camera..." << endl;
    Camera cam = Camera(0);
    cout << "[successful] connect to camera" << endl;
    cout << "[successful] width: " << cam.width << ", height: " << cam.height << ", total length: " << cam.length << endl;

    cout << "try to start python process..." << endl;
    thread python = thread(start_python, interpreter);

    cout << "waiting python client connect..." << endl;
    Communication com = Communication();

    cout << "[successful] connect to python client" << endl;
    HandTracker ht = HandTracker(&cam, &com);

    cout << "try to initialize host controller..." << endl;
    HostController hc = HostController(&ht);
    cout << "[successful] host controller has been initialized" << endl;

    process_image(hc);
}
