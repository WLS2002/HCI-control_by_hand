#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <communication.h>
#include <json.hpp>
#include <utils.h>

using namespace cv;
using namespace std;

class Camera{
public:
    VideoCapture cap;
    int width;
    int height;
    int length;
    Mat img;

    Camera(int index);

    Camera();
};



class HandTracker{
public:
    Camera* cam;
    Communication* com;

    HandTracker(Camera* cam, Communication* com);

    void show_window(Mat& img, nlohmann::json& result, vector<double*>& drawingPoints, long long fps);

    void drawHandPoint(Mat& img, nlohmann::json& result) const;

    void drawDrawingPoints(Mat& img, vector<double*>& drawingPoints);
};