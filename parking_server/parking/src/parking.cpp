#include "parking.hpp"

int Parking::RunParking() {
    if (!client_parking.GetHasInfo()) {
        std::cout << "Need to Train" << std::endl;
        manager.MakeDataset();
        manager.FitParams();
    }

    // while (true) {
    manager.UpdateSpace();

    // }
    std::cout << "End of RunParking" << std::endl; 
    return 0;
}

int Parking::PushView(std::string ip) {
    return 0;
}

int Parking::PopView(std::string ip) {
    return 0;
}

int ViewsManager::MakeDataset() {
    for (size_t i = 0; i < parking_list.size(); ++i) {
        parking_list.at(i).UpdateViewDataset();
    }

    return 0;
}

int ViewsManager::FitParams() {
    for (size_t i = 0; i < parking_list.size(); ++i) {
        parking_list.at(i).SetViewParams();
    }

    return 0;
}

int ViewsManager::SetParams() {
    return 0;
}

int ViewsManager::UpdateSpace() {
    for (size_t i = 0; i < parking_list.size(); ++i) {
        parking_list.at(i).UpdateViewSpace();
    }

    return 0;
}

int ClientParking::Send() {
    std::cout << "Send from ClientParking" << std::endl;
    return 0;
}

int ParkingView::UpdateViewDataset() {
    cv::Mat img = camera.GetImage();
    std::cout << "Params-SetDataset: " << params.SetDataset(img) << std::endl;

    return 0;
}
int ParkingView::UpdateViewSpace() {
    // camera.GetImage();
    cv::Mat curr = cv::imread("../parking_server/parking/dataset/img9.jpg");
    cv::Mat last = cv::imread("../parking_server/parking/dataset/img10.jpg");

    cv::Mat dif = curr.clone();

    params.threshold = 20;
    // params.Difference(curr, last);
    

    return 0;
}
int ParkingView::SetViewParams() {
    params.FitParams();

    return 0;
}

SpaceInfo ParkingView::GetSpaceInfo() {
    SpaceInfo space_info;
    return space_info;
}

void Params::PrintIMG() {
    cv::Mat res = dataset.front().clone();

    // for (int i = 0; i < HIGHT; i++) {
    //     for (int j = 0; j < WIDTH; j++) {
    //         if (counter_matrix[i][j] >= 4) {
    //             // res.at<unsigned char>(i, j) = counter_matrix[i][j] * 10;
    //             res.at<unsigned char>(i, j) = 255;
    //         }
    //     }
    // }
    
    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            res.at<unsigned char>(i, j) = counter_matrix[i][j] * 30;
        }
    }

    bool charCheckForEscKey = true;
    while (charCheckForEscKey /*&& capWebcam.isOpened()*/) {
        cv::imshow("result", res);
        charCheckForEscKey = cv::waitKey(1);   // delay (in ms) and get key press, if any
    }
}

void Params::AssignWeight(int x, int y) {
    counter_matrix[x][y] += 5;
    for (short i = 1; i < 6; ++i) {
        counter_matrix[x + i][y] += (5 - i);
        counter_matrix[x - i][y] += (5 - i);
        counter_matrix[x][y + i] += (5 - i);
        counter_matrix[x][y - i] += (5 - i);
    }
}

void Params::Fill(int (&mas)[HIGHT][WIDTH], int x, int y, int new_val = 254, int old_val = 255) {
    if(x >= 0 && x < HIGHT && y >= 0 && y < WIDTH && mas[x][y] == old_val && mas[x][y] != new_val) {
        if (x > max_pos_x) max_pos_x = x;
        if (y > max_pos_y) max_pos_y = y;
        if (x < min_pos_x) min_pos_x = x;
        if (y < min_pos_y) min_pos_y = y;

        mas[x][y] = new_val;
        square++;
        Fill(mas, x + 1, y, new_val, old_val);
        Fill(mas, x - 1, y, new_val, old_val);
        Fill(mas, x, y + 1, new_val, old_val);
        Fill(mas, x, y - 1, new_val, old_val);
    }
}

int Params::Difference(cv::Mat bg_frame, cv::Mat cam_frame) {
    if (dataset.size() == 0) {
        return -1;
    }

    int mas[HIGHT][WIDTH];
    cv::Mat diff_frame;

    resize(cam_frame, cam_frame, bg_frame.size());

    absdiff(bg_frame, cam_frame, diff_frame);   // разница между матрицами bg и cam

    cv::threshold(diff_frame, diff_frame, threshold, 255, cv::THRESH_BINARY);

    for (int i = 0; i < diff_frame.rows; i++) {
        for (int j = 0; j < diff_frame.cols; j++) {
            mas[i][j] = (diff_frame.at<unsigned char>(i, j) == 255 ? 255 : 0);
        }
    }

    short cnt_cars = 0;

    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (mas[i][j] == 255) {
                
                Fill(mas, i, j, 254, 255);
                min_pos_x = 2000, min_pos_y = 2000, max_pos_x = 0, max_pos_y = 0;

                if (square > min_square) {
                    cnt_cars++;
                    Fill(mas, i, j, 253, 254);
                    int dif_x = floor((max_pos_x - min_pos_x) / 2);
                    int dif_y = floor((max_pos_y - min_pos_y) / 2);
                    AssignWeight(min_pos_x + dif_x, min_pos_y + dif_y);
                    min_pos_x = 2000, min_pos_y = 2000, max_pos_x = 0, max_pos_y = 0;
                }
                square = 0;
            }
        }
    }

    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (mas[i][j] != 253) {
                mas[i][j] = 0;
                diff_frame.at<unsigned char>(i, j) = 0;
            } else {
                counter_matrix[i][j]++;
                // location_matrix[i][j] == true ? location_matrix[i][j] = false : location_matrix[i][j] = true;
            }
        }
    }
    return 0;
    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            std::cout << counter_matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << threshold << " - CARS: " << cnt_cars  << std::endl;

    return cnt_cars;
}

int Params::FitParams() {
    min_square = 1500;

    for (size_t index = 0; index < dataset.size() - 1; ++index) {
        // short cnt_cars[20];
        // for (short i = 0; i < 20; ++i) cnt_cars[i] = 0;
        
        cv::Mat bg_frame = dataset.at(index);
        cv::Mat cam_frame = dataset.at(index + 1);

        threshold = 20;
        Difference(bg_frame, cam_frame);
    }

    PrintIMG();

        // for (short threshold_idx = 15; threshold_idx < 40; threshold_idx++) {
        //     threshold = threshold_idx;
        //     cnt_cars[CarsCount(bg_frame, cam_frame)]++;
        //     // break;
        // }

        // short mx = 0, ind = 0; 
        // for (short i = 0; i < 20; ++i) {
        //     if (cnt_cars[i] > mx) {
        //         mx = cnt_cars[i];
        //         ind = i;
        //     }
        // }

        // std::cout << index << ") CNT_Cars = " << ind << std::endl;
}

int Params::SetDataset(cv::Mat img) {
    // dataset.push_back(img);

    std::string path("../parking_server/parking/dataset/"); 

    for (int i = 4; i < 11; ++i) {
        cv::Mat new_img1 = cv::imread(path + "img" + std::to_string(i) + ".jpg", 0);
        if (new_img1.empty()) {
            std::cout << "File 'img" << std::to_string(i) << ".jpg is not found" << std::endl;
        } else {
            dataset.push_back(new_img1);
        }
    }
        
    return 0;
}

cv::Mat Camera::GetImage() {
    // будет брать изображение с камеры

    cv::Mat img;
    return img;
}