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

};