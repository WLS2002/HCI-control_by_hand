#include <cv.h>
#include <utils.h>

Camera::Camera(int index) {
    cap = VideoCapture(index);
    cap.set(CV_CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(CV_CAP_PROP_FPS, 30);

    cap >> img;
    width = img.cols;
    height = img.rows;
    length = 3 * width * height;
}


HandTracker::HandTracker(Camera *cam, Communication *com) {
    this->cam = cam;
    this->com = com;
    com->send_int(cam->width);
    com->send_int(cam->height);
}

void HandTracker::show_window(Mat& img, nlohmann::json& result, vector<double*>& drawingPoints, long long fps) {
    int left_click_count = get_left_click_count(result), right_click_count = get_right_click_count(result), drawing_count = check_drawing_mode(result);
    string text = format("fps: %lld c: %d %d %d", fps, left_click_count, right_click_count, drawing_count);

    putText(img, text, Point(0, 50), 2, 1, cv::FONT_HERSHEY_COMPLEX, 2, 8, 0);

    if (!result[0].is_null()) {
        drawHandPoint(img, result);
    }
    drawDrawingPoints(img, drawingPoints);
    imshow("", img);
}

void HandTracker::drawHandPoint(Mat& img, nlohmann::json& result) const {
    for (int i = 0; i <= 20; i++) {
        circle(img, Point((int) (double(result[0][i][0]) * cam->width), (int) (double(result[0][i][1]) * cam->height)),
               2, Scalar(0, 0, 255), FILLED, LINE_8);
    }
    auto center = get_center(result);
    circle(img, Point((int) (std::get<0>(center) * cam->width), (int) (std::get<1>(center) * cam->height)), 2,
           Scalar(255, 0, 0), FILLED, LINE_8);
}

void HandTracker::drawDrawingPoints(Mat& img, vector<double*>& drawingPoints){
    for (auto & point : drawingPoints) {
        circle(img, Point(point[0], point[1]), 3, Scalar(0, 255, 120), -1);
    }
}