#define WIDTH 1280
#define HIGHT 720

#include <iostream>
#include<opencv2/core.hpp>
// #include<opencv2/highgui.hpp>
// #include<opencv2/imgproc.hpp>

using namespace cv;


struct Params {
    int threshold;
    int min_square;
    bool location_matrix[HIGHT][WIDTH];
    int counter_matrix[HIGHT][WIDTH];

    Params(): threshold(255), min_square(0) {
        for (int i = 0; i < HIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                location_matrix[i][j] = true;
                counter_matrix[i][j] = 0;
            }
        }
    }
};

struct SpaceInfo {
    cv::Mat image;
    int spaces;
    int free_spaces;

    SpaceInfo(): spaces(0), free_spaces(0) {}
};


class Settings {
public:
    Settings() {}
    ~Settings() {}

    Params Fit() {}

private:
    void Fill() {}
    void SetData() {}
};


class Camera {
public:
    Camera() {}
    ~Camera() {}

    cv::Mat GetImage() {}

private:
    std::string ip;
};


class ParkingView {
public:
    SpaceInfo space_info;

    ParkingView() {}
    ~ParkingView() {}

    void SetParams() {}

    void DetectCars() {}

    void FindSpace() {}

private:

    Camera camera;
    Params params;

    // int mas[HIGHT][WIDTH];
    // Mat bg_frame;
    // Mat cam_frame;
    // Mat diff_frame;
    // char charCheckForEscKey = 0;
    // int min_square;
    // int threshold_value;
};


class Parking {
public:
    Parking() {}
    ~Parking() {}

    // SpaceInfo TakeSpace() {}        ???

private:
    void Send() {}

    std::vector<ParkingView> parking_list;
};


class Bot {
public:
    Bot() {}
    ~Bot() {}
    
    void SetEvents() {}

    void RunBot() {}

private:
    void Get() {}
    
    // SpaceInfo Space() {}  ??? зачем
    
    std::string StartMessage() {}
    std::string HelpMessage() {}
    std::string AnyMessage() {}
};




int main() {

}
