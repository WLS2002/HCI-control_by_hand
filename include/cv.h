#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <communication.h>
#include <json.hpp>
#include <utils.h>

using namespace cv;
using namespace std;
using namespace nlohmann;


struct camera{
    VideoCapture* cap;
    int width;
    int height;
    int length;
    Mat img;

    camera(int index){
        cap = new VideoCapture(index);
        cap->set(CV_CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
        cap->set(CV_CAP_PROP_FRAME_WIDTH, 640);
        cap->set(CV_CAP_PROP_FRAME_HEIGHT, 480);
        cap->set(CV_CAP_PROP_FPS, 30);

        *cap >> img;
        width = img.cols;
        height = img.rows;
        length = 3 * width * height;
    }


};


struct hand_tracker{
    camera* cam;
    communication* com;
    pthread_t* pid;
    void* status;

    hand_tracker(camera* cam, communication* com){
        this->cam = cam;
        this->com = com;
        initialize();

        pid = new pthread_t;
        status = new int;
    }

    void initialize(){
        com->send_int(cam->width);
        com->send_int(cam->height);
    }

    void show_window(Mat img, json result, long delay_start, long delay_end, long last_time){
        string text = format("delay: %ld, fps: %ld", delay_end - delay_start, 1000 / (delay_end - last_time));
        putText(img, text, Point(0,50), 2, 1, cv::FONT_HERSHEY_COMPLEX, 2, 8, 0);

        if(!result[0].is_null()){
            circle( img,Point ((int)(double(result[0][0][0]) * cam->width), (int)(double(result[0][0][1]) * cam->height)),2,Scalar( 0, 0, 255 ),FILLED,LINE_8 );
        }
        imshow("", img);
    }

};