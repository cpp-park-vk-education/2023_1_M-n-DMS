#include "parking.hpp"

std::string imageToString(const cv::Mat& image) {
    std::vector<uchar> buffer;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(20);  // JPEG quality (0-100)
    cv::imencode(".jpg", image, buffer, params);
    std::string encodedImage(buffer.begin(), buffer.end());
    encodedImage = base64::to_base64(encodedImage);
    return encodedImage;
}

int Parking::RunParking() {
    if (!client_parking.GetHasInfo()) {
        std::cout << "Need to Train" << std::endl;
        manager.MakeDataset();
        manager.FitParams();
        std::cout << "\nThe parameters are configured\n" << std::endl;
    }

    int cnt = 8;
    while (cnt > 2) {           // с задержкой в 20 секунд

        manager.UpdateSpace(cnt);

        std::vector<SpaceInfo> result = manager.GetSpaceInfo();
        short free_spaces = 0;
        short all_spaces = 0;
        for (size_t i = 0; i < result.size(); ++i) {
            free_spaces += result.at(i).free_space;
            all_spaces += result.at(i).space;
        }

        // Здесь загрузить нужное изображение
        // cv::Mat image = cv::imread("../parking_server/parking/dataset/img1.jpg");
        // std::string imageString = imageToString(image);

        std::string request =
                              "{\"method\": \"POST\","
                              "\"body\": {"
                              "\"id\": \"" + std::to_string(1) + "\","
                              "\"value\": \"" + std::to_string(free_spaces) + "\","
                              "\"photo\": \"" + "../parking_server/parking/dataset/img1.jpg" + "\"}}";

        std::string request_size = std::to_string(request.size());

        request = request_size + " " + request;

        client_parking.Send(request);

        client_parking.Run();

        std::cout << "\nPLACES = " << free_spaces << "/" << all_spaces << std::endl;
        cnt--;
        sleep(5);
    }

    std::cout << "\nEnd of RunParking" << std::endl;
    return 0;
}

// --------------------------------------------------
std::vector<SpaceInfo> ViewsManager::GetSpaceInfo() {
    std::vector<SpaceInfo> result;
    for (size_t i = 0; i < parking_list.size(); ++i) {
        result.push_back(parking_list.at(i).GetSpaceInfo());
    }

    return result;
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
        
        // ЗАДЕРЖКА В 5 МИНУТ //
    }

    return 0;
}

int ViewsManager::FitParams() {
    for (size_t i = 0; i < parking_list.size(); ++i) {
        if (parking_list.at(i).SetViewParams() == -1) {
            std::cerr << "ParkingView " << i << " can't set the params" << std::endl;
        }
    }

    /* exception */

    return 0;
}

// ---------------------------------
int ViewsManager::SetParams() {
    return 0;
}

int ViewsManager::UpdateSpace(int index) {
    for (size_t i = 0; i < parking_list.size(); ++i) {
        if (parking_list.at(i).UpdateViewSpace(index) == -1) {
            std::cerr << "ParkingView " << i << " is not responding";
        }
    }

    return 0;
}

int ParkingView::UpdateViewDataset() {
    // cv::Mat img = camera.GetImage();
    cv::Mat img;
    params.SetDataset(img);

    return 0;
}

// ---------------------------------------------------
int Params::CountPlaces(std::string format) {
    int cnt_places = 0;

    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (format == "all") {
                if (counter_matrix[i][j] > 0) {
                    cnt_places++;
                }
            } else if (format == "free") {
                if (counter_matrix[i][j] > 0 && !location_matrix[i][j]) {
                    cnt_places++;
                }
            } else {
                std::cerr << "ERROR: incorrect format";
                return -1;
            }
        }
    }

    return cnt_places;
}

void Params::UpdateLocationMatrix(cv::Mat dif) {



    int mas1[HIGHT][WIDTH]; // dif
    // int mas2[HIGHT][WIDTH]; // location

    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            mas1[i][j] = (dif.at<unsigned char>(i, j) == 255 ? 255 : 0);
            // mas2[i][j] = (location_matrix[i][j] == true ? 255 : 0);
        }
    }

    cnt_tops = 0;
    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (mas1[i][j] == 255) { // если True
                
                Fill1(mas1, i, j, 254, 255);
            }
        }
    }







    // cnt_tops = 0;
    // for (int i = 0; i < HIGHT; i++) {
    //     for (int j = 0; j < WIDTH; j++) {
    //         if (mas1[i][j] == 255) { // если True
                
    //             Fill(mas1, i, j, 254, 255);
    //             std::cout << cnt_tops << std::endl;
    //             // min_pos_x = 2000, min_pos_y = 2000, max_pos_x = 0, max_pos_y = 0;
    //             if ((cnt_tops > 0) && location_matrix[point.x_ind][point.y_ind]) {
    //                 std::cout << "Loc-True: " << cnt_tops << std::endl;
    //                 Fill(mas2, point.x_ind, point.y_ind, 253, 255);
    //                 cnt_tops = 0;
    //             }
    //             if ((cnt_tops > 0) && !location_matrix[point.x_ind][point.y_ind]) {
    //                 std::cout << "Loc-False: " << cnt_tops << std::endl;
    //                 std::cout << cnt_tops << std::endl;
    //                 Fill(mas1, i, j, 253, 254);
    //                 cnt_tops = 0;
    //             }
    //             cnt_tops = 0;
    //         }
    //     }
    // }

    // for (int i = 0; i < HIGHT; i++) {
    //     for (int j = 0; j < WIDTH; j++) {
    //         if (mas1[i][j] == 253) { // если есть ТОП & True
    //             location_matrix[i][j] = true;
    //         }
    //         if (mas2[i][j] == 253) { // если есть ТОП & True
    //             location_matrix[i][j] = false;
    //         }
    //     }
    // }
}

int ParkingView::UpdateViewSpace(int index) {
    // cv::Mat curr = camera.GetImage();               // Камера сделает фото и пришлет
    // cv::Mat last = camera.GetLastImage();

    cv::Mat last = cv::imread("../parking_server/parking/dataset/img" + std::to_string(index) + ".jpg", 0);
    // задержка
    cv::Mat curr = cv::imread("../parking_server/parking/dataset/img" + std::to_string(index - 1) + ".jpg", 0);


    params.rgb_threshold = 20;                      // Как настроить? (от времени суток, погоды,...?)
    
    cv::Mat dif = params.Difference(curr, last);

    // params.PrintIMG(dif);


    unsigned int vsp_matrix[HIGHT][WIDTH];

    for (int i = 0; i < HIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            vsp_matrix[i][j] = params.counter_matrix[i][j];
            if (params.counter_matrix[i][j] > 0) {
                if (dif.at<unsigned char>(i, j) == 255) {
                    vsp_matrix[i][j]++;
                } 
            }
        }
    }

    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            vsp_matrix[i][j] = vsp_matrix[i][j] - params.counter_matrix[i][j];
        }
    }


    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (vsp_matrix[i][j] > 0) {
                params.location_matrix[i][j] = (params.location_matrix[i][j] == true ? false : true);
            }
        }
    }


    int result_places = params.CountPlaces("all");
    if (result_places == -1) {
        std::cerr << "ERROR: incorrect places counting";
        return -1;
    }

    space_info.space = result_places;

    // std::cout << "ALL_PLACES = " << space_info.space << std::endl;

    // params.UpdateLocationMatrix(dif);

    int result_free_places = params.CountPlaces("free");
    if (result_free_places == -1) {
        std::cerr << "ERROR: incorrect free places counting";
        return -1;
    }

    space_info.free_space = space_info.space - result_free_places;

    // std::cout << "FREE_PLACES = " << space_info.free_space << std::endl;


    // for (int i = 0; i < HIGHT; i++) {
    //     for (int j = 0; j < WIDTH; j++) {
    //         if (params.location_matrix[i][j]) {
    //             dif.at<unsigned char>(i, j) = 255;
    //         } else {
    //             dif.at<unsigned char>(i, j) = 0;
    //         }
    //     }
    // }


    cv::Mat dif1 = params.dataset.front().clone();

    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (params.location_matrix[i][j]) {
                dif1.at<unsigned char>(i, j) = 255;
            } else {
                dif1.at<unsigned char>(i, j) = 0;
            }
        }
    }

    // params.PrintIMG(dif1);

    // // params.PrintIMG();

    return 0;
}

int ParkingView::SetViewParams() {
    if (params.FitParams() == -1) {
        return -1;
    }

    return 0;
}

// ----------------------------------------------
SpaceInfo ParkingView::GetSpaceInfo() {
    return space_info;
}

// ----------------------------------------------
void Params::PrintIMG(cv::Mat res) {
    // cv::Mat res = dataset.front().clone();
    // /*  */

    // if (flag == 1) {
        // for (int i = 0; i < HIGHT; i++) {
        //     for (int j = 0; j < WIDTH; j++) {
        //         if (location_matrix[i][j]) {
        //             res.at<unsigned char>(i, j) = 255;
        //         } else {
        //             res.at<unsigned char>(i, j) = 0;
        //         }
        //     }
        // }
    // }

    // // return;

    // /* ОТМЕТКИ НА ФОТО */
    // if (flag == 2) {
        // for (int i = 0; i < HIGHT; i++) {
        //     for (int j = 0; j < WIDTH; j++) {
        //         if (counter_matrix[i][j] >= 4) {
        //             // res.at<unsigned char>(i, j) = counter_matrix[i][j] * 10;
        //             res.at<unsigned char>(i, j) = 255;
        //         }
        //     }
        // }
    // }
    
    // /* ЧЕРНО-БЕЛЫЕ СИЛУЭТЫ */
    // if (flag == 3) {
    //     for (int i = 0; i < HIGHT; i++) {
    //         for (int j = 0; j < WIDTH; j++) {
    //             res.at<unsigned char>(i, j) = counter_matrix[i][j] * 30;
    //         }
    //     }
    // }

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
        
        if (counter_matrix[x][y] > 4) {
            // std::cout << "FIND" << std::endl;
            point.x_ind = x;
            point.y_ind = y;
            cnt_tops++;
        }

        mas[x][y] = new_val;
        square++;
        Fill(mas, x + 1, y, new_val, old_val);
        Fill(mas, x - 1, y, new_val, old_val);
        Fill(mas, x, y + 1, new_val, old_val);
        Fill(mas, x, y - 1, new_val, old_val);
    }
}

void Params::Fill1(int (&mas)[HIGHT][WIDTH], int x, int y, int new_val = 254, int old_val = 255) {
    if(x >= 0 && x < HIGHT && y >= 0 && y < WIDTH && mas[x][y] == old_val && mas[x][y] != new_val) {
        
        if (counter_matrix[x][y] > 0) {
            counter_matrix[x][y] = 0;
        }
        mas[x][y] = new_val;
        
        Fill1(mas, x + 1, y, new_val, old_val);
        Fill1(mas, x - 1, y, new_val, old_val);
        Fill1(mas, x, y + 1, new_val, old_val);
        Fill1(mas, x, y - 1, new_val, old_val);
    }
}

cv::Mat Params::Difference(cv::Mat bg_frame, cv::Mat cam_frame) {

    int mas[HIGHT][WIDTH];
    cv::Mat diff_frame;

    resize(cam_frame, cam_frame, bg_frame.size());

    absdiff(bg_frame, cam_frame, diff_frame);   // разница между матрицами bg и cam

    cv::threshold(diff_frame, diff_frame, rgb_threshold, 255, cv::THRESH_BINARY);

    for (int i = 0; i < diff_frame.rows; i++) {
        for (int j = 0; j < diff_frame.cols; j++) {
            mas[i][j] = (diff_frame.at<unsigned char>(i, j) == 255 ? 255 : 0);
        }
    }

    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (mas[i][j] == 255) {
                
                Fill(mas, i, j, 254, 255);

                if (square > min_square) {
                    Fill(mas, i, j, 253, 254);
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
                diff_frame.at<unsigned char>(i, j) = 255;
            }
        }
    }

    return diff_frame;
}

int Params::ReadParams(std::string path) {
    std::ifstream file(path);
    std::string str;
    if (file) {        
        std::getline(file, str);
        if (str.size()) {
            int file_info = std::stoi(str);
            return file_info;
        } else {
            std::cerr << "File is empty" << std::endl;
        }

    } else {
        std::cerr << "File is not found..." << std::endl;
    }
    file.close();
    return -1;
}

int Params::ReadMatrix(std::string path, std::string matrix) {
    std::ifstream file(path);
    std::string str;
    if (file) {
        for (int i = 0; i < HIGHT; ++i) {
            int ind_i = 0;
            std::string new_str("");
            std::getline(file, str);
            for (size_t j = 0; j < str.size(); ++j) {
                if (str[j] != ' ' && (j != str.size())) {
                    new_str += std::to_string(str[j] - '0');
                } else {
                    if (matrix == "counter") {
                        counter_matrix[i][ind_i] = std::stoi(new_str);
                    } else if (matrix == "binary") {
                        location_matrix[i][ind_i] = std::stoi(new_str);
                    }
                    else {
                        std::cout << "incorrect matrix NAME" << std::endl;
                        file.close();
                        return -1;
                    }
                    new_str = "";
                    ind_i++;
                }
            }
            counter_matrix[i][ind_i] = std::stoi(new_str);
        }
    } else {
        std::cerr << "File is not found..." << std::endl;
    }
    file.close();
    return 0;
}

// void Params::BinaryMatrix(std::string path) {
//     std::ifstream file(path);
//     std::string str;
//     if (file) {
//         for (int i = 0; i < HIGHT; ++i) {
//             int ind_i = 0;
//             std::string new_str("");
//             std::getline(file, str);
//             for (size_t j = 0; j < str.size(); ++j) {
//                 if (str[j] != ' ' && (j != str.size())) {
//                     new_str += std::to_string(str[j] - '0');
//                 } else {
//                     location_matrix[i][ind_i] = std::stoi(new_str);
//                     new_str = "";
//                     ind_i++;
//                 }
//             }
//             location_matrix[i][ind_i] = std::stoi(new_str);
//         }
//     } else {
//         std::cerr << "File is not found..." << std::endl;
//     }
//     file.close();
// }

// void Params::BinaryCorrection() {
//     int mas[HIGHT][WIDTH];

//     for (int i = 0; i < HIGHT; i++) {
//         for (int j = 0; j < WIDTH; j++) {
//             mas[i][j] = location_matrix[i][j] == true ? 255 : 0;
//         }
//     }

//     cnt_tops = 0;
//     for (int i = 0; i < HIGHT; i++) {
//         for (int j = 0; j < WIDTH; j++) {
//             if (mas[i][j] == 255) { // если True
                
//                 Fill(mas, i, j, 254, 255);
//                 // min_pos_x = 2000, min_pos_y = 2000, max_pos_x = 0, max_pos_y = 0;
//                 // std::cout << cnt_tops << std::endl;
//                 if (cnt_tops > 0) {
//                     Fill(mas, i, j, 253, 254);
//                 }
//                 cnt_tops = 0;
//             }
//         }
//     }

    // for (int i = 0; i < HIGHT; i++) {
    //     for (int j = 0; j < WIDTH; j++) {
//             if (mas[i][j] != 253) { // если НЕ парковочное место
//                 location_matrix[i][j] = false;
//             }
//         }
//     }

//     // РЕЗУЛЬТАТ: корректная матрица T/F
// }

int Params::FitParams() {
    
    // Запустить EXE python -> создадутся 4 файла   (!!! ИЛИ ПЕРЕД ЗАПУСКОМ САМИМ ЗАПУСТИТЬ EXE !!!)
    // ПОДОЖДАТЬ, пока файл отработает

    /* min_square */
    unsigned int min_square_ = ReadParams("../min_square_file.txt");

    if (min_square_ == -1) {
        std::cerr << "MIN_square wasn't calculate" << std::endl;
        return -1;
    }

    min_square = min_square_;
    std::cout << min_square << std::endl;

    /* height_threshold */
    // unsigned int height_threshold_ = ReadParams("../height_threshold_file.txt");
    // height_threshold = (height_threshold_ != -1 ? height_threshold_ : 1000000);

    // height_threshold = 4;
    

    /* counter_matrix */
    ReadMatrix("../counter_matrix_file.txt", "counter");

    /* binary_matrix */
    ReadMatrix("../binary_matrix_file.txt", "binary");

    // for (int i = 0; i < HIGHT; i++) {
    //     for (int j = 0; j < WIDTH; j++) {
    //         if (counter_matrix[i][j] > 0) {
    //             location_matrix[i][j] = true;
    //         }
    //     }
    // }

    
    // BinaryCorrection(); // Корректируем матрицу T/F


    // cv::Mat dif = dataset.front().clone();

    // for (int i = 0; i < HIGHT; i++) {
    //     for (int j = 0; j < WIDTH; j++) {
    //         if (location_matrix[i][j]) {
    //             dif.at<unsigned char>(i, j) = 255;
    //         } else {
    //             // dif.at<unsigned char>(i, j) = 0;
    //         }
    //     }
    // }

    // PrintIMG(dif);

    return 0;
}

    // for (size_t index = 0; index < dataset.size() - 1; ++index) {
    //     // short cnt_cars[20];
    //     // for (short i = 0; i < 20; ++i) cnt_cars[i] = 0;
        
    //     cv::Mat bg_frame = dataset.at(index);
    //     cv::Mat cam_frame = dataset.at(index + 1);

    //     threshold = 20;
    //     Difference(bg_frame, cam_frame, false);
    // }

// }

int Params::SetDataset(cv::Mat img) {
    // dataset.push_back(img);

    /* Пока не работает камера, берем готовый датасет */
    std::string path("../parking_server/parking/dataset/"); 

    for (int i = 1; i < 11; ++i) {
        cv::Mat new_img1 = cv::imread(path + "img" + std::to_string(i) + ".jpg", 0);
        if (new_img1.empty()) {
            std::cout << "File 'img" << std::to_string(i) << ".jpg is not found" << std::endl;
        } else {
            dataset.push_back(new_img1);
        }
    }

    return 0;
}

// // void Params::CutPhotos() {
// //     cv::Mat last = cv::imread("../parking_server/parking/dataset/img8.jpg", 0);
// //     for (int i = 0; i < HIGHT; i++) {
// //         for (int j = 0; j < WIDTH; j++) {
// //             if (counter_matrix[i][j] > HIGHT_THRESHOLD) {
                
// //             }
// //         }
// //     }
// // }

cv::Mat Camera::GetLastImage() {
    return last_image;
}

cv::Mat Camera::GetImage() {
    // будет брать изображение с камеры

    // if (dataset.empty()) {
    //     std::cerr << "No photos" << std::endl;
    //     cv::Mat error;
    //     return error;
    // }
    // cv::Mat img = dataset.front();
    // std::vector<cv::Mat>::iterator it = dataset.begin();
    // dataset.erase(it);

    cv::Mat img = cv::imread("../parking_server/parking/dataset/img9.jpg", 0);

    return img;
}


int GetParkingID(std::string path) {
    std::ifstream file(path);
    std::string str;
    if (file) {        
        std::getline(file, str);
        if (str.size()) {
            int file_info = std::stoi(str);
            return file_info;
        } else {
            std::cerr << "File is empty" << std::endl;
        }

    } else {
        std::cerr << "File is not found..." << std::endl;
    }
    file.close();
    return -1;
}