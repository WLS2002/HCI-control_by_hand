#include <cv.h>
#include <host_controller.h>

std::pair<double, double> handle_points(char*);

void* process_image(void*);

void* process_image(void* args0, void* args1){
    auto* ht = (hand_tracker*) args0;
    auto* hc = (HostController*) args1;

    ht->com->receive();
    long last_time = get_ms();

    cout << "[successful] start sending image" << endl;
    while(1){
        string text = "";
        Mat img;
        *ht->cam->cap >> img;
        long delay_dic = get_ms();
        flip(img, img, 1);

        ht->com->send_img(img, ht->cam->length);
        ht->com->receive();

        std::pair<double, double> p = handle_points(ht->com->receive_buffer);
        double x = p.first, y = p.second;

        hc->set_cursor_pos(x * hc->screen_width, y * hc->screen_height);

        text.append("DELAY: ").append(std::to_string(get_ms() - delay_dic)).append(" ");

        long new_time = get_ms();

        text.append("FPS: ").append(std::to_string(1000 /(new_time - last_time)));
        last_time = new_time;


        circle( img,Point ((int)(x * ht->cam->width), (int)(y * ht->cam->height)),5,Scalar( 0, 0, 255 ),FILLED,LINE_8 );
        putText(img, text, Point(0,50), 2, 1, cv::FONT_HERSHEY_COMPLEX, 2, 8, 0);
        imshow("", img);
        char c = waitKey(1);
        if(c == 27){
            break;
        }
    }
}

std::pair<double, double> handle_points(char* res){
    json j = json::parse(res);

    double x, y;

    if(!j[0][0].is_null()){
        x = j[0][0][0];
        y = j[0][0][1];
    } else {
        x = -1;
        y = -1;
    }
    return std::pair<double, double>(x, y);

}

int main(){
    cout << "try to open camera..." << endl;
    auto* cam = new camera(0);
    cout << "[successful] connect to camera" << endl;
    cout << "[successful] width: " << cam->width << ", height: " << cam->height << ", total length: " << cam->length << endl;

    cout << "waiting python client connect..." << endl;
    auto* com = new communication();
    cout << "[successful] connect to python client" << endl;
    auto* ht = new hand_tracker(cam, com);

    cout << "try to initialize host controller..." << endl;
    auto* hc = new HostController();
    cout << "[successful] host controller has been initialized" << endl;

    process_image(ht, hc);
}
