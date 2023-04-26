#define WIDTH 1280
#define HIGHT 720

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

#include <iostream>
#include <typeinfo>
#include <vector>

using namespace std;
using namespace cv;

Mat bg_frame;
Mat cam_frame;
Mat diff_frame;
char charCheckForEscKey = 0;


int square = 0;

void filling(int (&mas)[HIGHT][WIDTH], int x, int y, int new_val = 254, int old_val = 255) {

    if(x >= 0 && x < 1280 && y >= 0 && y < 720 && mas[x][y] == old_val && mas[x][y] != new_val) {
        mas[x][y] = new_val;
        square++;
        filling(mas, x + 1, y);
        filling(mas, x - 1, y);
        filling(mas, x, y + 1);
        filling(mas, x, y - 1);
    }
}

int main() {

    while (charCheckForEscKey != 27 /*&& capWebcam.isOpened()*/) {

        bg_frame = imread("../img11.jpg", 0);
        cam_frame = imread("../img12.jpg", 0);

        //  720x1280

        resize(cam_frame, cam_frame, bg_frame.size());

        // imshow("test1", bg_frame);
        // imshow("test2", cam_frame);

        absdiff(bg_frame, cam_frame, diff_frame);   // разница между матрицами bg и cam

        threshold(diff_frame, diff_frame, 30, 255, THRESH_BINARY);
        int hight = diff_frame.rows;
        int width = diff_frame.cols;

        int mas[HIGHT][WIDTH];

        for (int i = 0; i < diff_frame.rows; i++) {
            for (int j = 0; j < diff_frame.cols; j++) {
                mas[i][j] = (diff_frame.at<unsigned char>(i, j) == 255 ? 255 : 0);
            }
        }

        erode(diff_frame, diff_frame, getStructuringElement(MORPH_RECT, Size(1, 1)));
        // erode(diff_frame, diff_frame, getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1)));

        dilate(diff_frame, diff_frame, Mat(), Point(-1, -1));

        // Canny(diff_frame, diff_frame, 30, 70);

        imshow("test3", diff_frame);

        charCheckForEscKey = cv::waitKey(1);            // delay (in ms) and get key press, if any

        // int test[][12] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        //                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        //                   {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        //                   {0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0},
        //                   {0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0},
        //                   {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0},
        //                   {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0},
        //                   {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        //                   {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        //                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        //                   {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        //                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

        short cnt_cars = 0;

        for (int i = 0; i < HIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (mas[i][j] == 255) {
                    filling(mas, i, j);
                    
                    for (int a = 0; a < HIGHT; a++) {
                        for (int b = 0; b < WIDTH; b++) {
                            cout << mas[i][j] << " ";
                        }
                        cout << endl;
                    } 
                    return 0;

                    if (square > 500) {
                        cnt_cars++;
                    }
                    square = 0;
                }
            }
        }

        cout << "CARS: " << cnt_cars << endl;

        break;

    }

    return 0;

}
