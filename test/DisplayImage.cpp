#define WIDTH 1280
#define HIGHT 720

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

#include <iostream>
#include <typeinfo>
#include <vector>
#include <string>

using namespace cv;


struct CarPoint {
    int i, j;

    CarPoint(int i_, int j_): i(i_), j(j_) {}
    CarPoint() {}
};


// Класс вычисления всех парковочных мест.
// Каждая камера - это объект данного класса.
// Подбираются параметры для конкретного объекта.

class ParkPlaces {
public:
    ParkPlaces(): min_square(0), threshold_value(30) {}
    ~ParkPlaces() {}

    void Fit(int cars_counter, std::string img1 = "../img1.jpg", std::string img2 = "../img2.jpg") {

        bg_frame = imread(img1, 0);
        cam_frame = imread(img2, 0);

        resize(cam_frame, cam_frame, bg_frame.size());

        absdiff(bg_frame, cam_frame, diff_frame);   // разница между матрицами bg и cam

        threshold(diff_frame, diff_frame, threshold_value, 255, THRESH_BINARY);

        for (int i = 0; i < diff_frame.rows; i++) {
            for (int j = 0; j < diff_frame.cols; j++) {
                mas[i][j] = (diff_frame.at<unsigned char>(i, j) == 255 ? 255 : 0);
            }
        }

        erode(diff_frame, diff_frame, getStructuringElement(MORPH_RECT, Size(1, 1)));
        // erode(diff_frame, diff_frame, getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1)));
        // dilate(diff_frame, diff_frame, Mat(), Point(-1, -1));
        // Canny(diff_frame, diff_frame, 30, 70);

        imshow("dif_1", diff_frame);

        short cnt_cars = 0;

        for (int i = 0; i < HIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (mas[i][j] == 255) {
                    filling(mas, i, j, 254, 255);

                    if (min_square > 500) {
                        cnt_cars++;
                        filling(mas, i, j, 253, 254);
                    }
                    min_square = 0;
                }
            }
        }
    }

    void show_img() {

        for (int i = 0; i < HIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (mas[i][j] != 253) {
                    mas[i][j] = 0;
                    diff_frame.at<unsigned char>(i, j) = 0;
                }
            }
        }

        // show difference image
        while (charCheckForEscKey != 27 /*&& capWebcam.isOpened()*/) {

            imshow("result", diff_frame);

            charCheckForEscKey = cv::waitKey(1);   // delay (in ms) and get key press, if any
        }
    }


private:
    // рассчет площадей пятен интенсивности
    void filling(int (&mas)[HIGHT][WIDTH], int x, int y, int new_val = 254, int old_val = 255) {

        if(x >= 0 && x < HIGHT && y >= 0 && y < WIDTH && mas[x][y] == old_val && mas[x][y] != new_val) {
            // mas[x][y] = old_val;
            // if (new_val == 1) {
            mas[x][y] = new_val;
            // }
            min_square++;
            filling(mas, x + 1, y, new_val, old_val);
            filling(mas, x - 1, y, new_val, old_val);
            filling(mas, x, y + 1, new_val, old_val);
            filling(mas, x, y - 1, new_val, old_val);
        }
    }


    int mas[HIGHT][WIDTH];

    Mat bg_frame;
    Mat cam_frame;
    Mat diff_frame;
    char charCheckForEscKey = 0;
    int min_square;
    int threshold_value;
};

int main() {

    ParkPlaces test_cam;
    test_cam.Fit(3);
    test_cam.show_img();

    return 0;
}
