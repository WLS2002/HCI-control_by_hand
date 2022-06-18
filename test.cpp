#include<iostream>//取决于你的实际需要
#include<vector>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>//这个头文件是与图像处理有关的必须的
#include<utils.h>
#include<cso.h>

using namespace std;
using namespace cv;

void drawRec(Mat image, double x, double y, double size_x, double size_y){
    double x0[] {x, y};
    double x1[] {x + size_x, y};
    double x2[] {x, y + size_y};
    double x3[] {x + size_x, y + size_y};

    line(image, Point(x0[0], x0[1]), Point(x1[0], x1[1]), Scalar(0,0,255),1, LINE_AA);
    line(image, Point(x0[0], x0[1]), Point(x2[0], x2[1]), Scalar(0,0,255),1, LINE_AA);
    line(image, Point(x1[0], x1[1]), Point(x3[0], x3[1]), Scalar(0,0,255),1, LINE_AA);
    line(image, Point(x2[0], x2[1]), Point(x3[0], x3[1]), Scalar(0,0,255),1, LINE_AA);
}

void drawTri(Mat image, double x0, double y0, double x1, double y1, double x2, double y2){
    double p0[] {x0, y0};
    double p1[] {x1, y1};
    double p2[] {x2, y2};

    line(image, Point(p0[0], p0[1]), Point(p1[0], p1[1]), Scalar(0,0,255),1, LINE_AA);
    line(image, Point(p0[0], p0[1]), Point(p2[0], p2[1]), Scalar(0,0,255),1, LINE_AA);
    line(image, Point(p1[0], p1[1]), Point(p2[0], p2[1]), Scalar(0,0,255),1, LINE_AA);
}

void drawCir(Mat image, double x, double y, double size){
    circle(image, Point(x, y), size, Scalar(0, 255, 120), 0);//画点，其实就是实心圆
}

int main() {

    srand(get_ms());
//    for(int i = 0; i < 100; i++){
//        int* order = random_order(i);
//        for(int j = 0; j < i; j++) cout << order[j] << " ";
//        cout << endl;
//    }

    //Mat image1 = imread("../test.jpg", 1);//路径根据自己的改
    Mat image1(480, 640, CV_8UC3, Scalar(0, 0, 0));


    vector<double *> points;
    for (int i = 0; i <= 30; i++) {
        double *tmp = new double[2];
        tmp[0] = r() * 640;
        tmp[1] = r() * 480;
        points.push_back(tmp);
    }

    for (auto & point : points) {
        circle(image1, Point(point[0], point[1]), 3, Scalar(0, 255, 120), -1);//画点，其实就是实心圆
    }

    CSO_REC cso(points);
    cso.search();
    cout << cso.bestFitness << endl;
    double* weight = cso.globalBest;
    drawRec(image1, weight[0], weight[1], weight[2], weight[3]);
    cout << weight[0] << " " << weight[1] << " " << weight[2] << " " << weight[3] << endl;

    cout << rectangle_error(points, weight[0], weight[1], weight[2], weight[3]) << endl;
    imshow("PointsinImage", image1);
    waitKey(0);//敲键盘关图片，别直接×
    return 0;
}

