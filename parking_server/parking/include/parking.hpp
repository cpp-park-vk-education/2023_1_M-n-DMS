#pragma once

#include <shared.hpp>

#include <fstream>
#include <unistd.h>


#define WIDTH 1280
#define HIGHT 720
// #define HIGHT_THRESHOLD 4

class ClientParking;
class Parking;
class ParkingView;
class Camera;
class Params;
struct SpaceInfo;

class ClientParking : Client {
public:
    // ClientParking();
    ClientParking(boost::asio::io_context& io_context, const std::string& server, const std::string& port)
                 : io_service_(io_context), resolver_(io_context), socket_(io_context), has_info(false) {

        auto endpoints = resolver_.resolve({server, port});

        boost::asio::async_connect(socket_, endpoints,
        [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint ep) {
            if (ec) {
                std::cerr << "Failed to connect to " << ep << ": " << ec.message() << std::endl;
                return;
            }
            std::cout << "Connected to " << ep << std::endl;
        });
        // std::cout << "ClientParking is Activated" << std::endl;

    }

    ~ClientParking() {}

    int Run() {
        io_service_.run();
    }

    int Send(const std::string& message) override {
        boost::asio::async_write(socket_, boost::asio::buffer(message),
        [this, message](boost::system::error_code ec, std::size_t bytes_transferred) {
            if (ec) {
                std::cerr << "Failed to write to server: " << ec.message() << std::endl;
                return;
            }
            std::cout << "Sent message: " << message << std::endl;

            // Read();
        });
    return 0;
    }

    bool GetHasInfo() { return has_info; }

private:
    boost::asio::io_service& io_service_; // New
    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::asio::streambuf response_;
    // pugi::xml_document client_info;
    bool has_info;
};


class Params {
public:
    short rgb_threshold;
    unsigned int height_threshold;
    unsigned int square;
    unsigned int min_square;
    short cnt_tops;
    bool location_matrix[HIGHT][WIDTH];
    unsigned int counter_matrix[HIGHT][WIDTH];
    // int last_matrix[HIGHT][WIDTH];

    struct Point {
        int x_ind;
        int y_ind;

        Point() : x_ind(0), y_ind(0) {}
    };

    Point point;

    int min_pos_x = 2000, min_pos_y = 2000, max_pos_x = 0, max_pos_y = 0;

    Params() : rgb_threshold(0), height_threshold(0), square(0), min_square(0), cnt_tops(0), point() {
        for (int i = 0; i < HIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                location_matrix[i][j] = false;
                counter_matrix[i][j] = 0;
                // last_matrix[i][j] = 0;
            }
        }
    }
    ~Params() {}
    
    int FitParams();
    int ReadParams(std::string path);
    int ReadMatrix(std::string path, std::string matrix);
    // void BinaryMatrix(std::string path);
    // void BinaryCorrection();
    int SetDataset(cv::Mat img);
    cv::Mat Difference(cv::Mat bg_frame, cv::Mat cam_frame);
    void UpdateLocationMatrix(cv::Mat dif);
    void PrintIMG(cv::Mat res);
    int CountPlaces(std::string format);

    std::vector<cv::Mat> dataset;

private:
    void Fill(int (&mas)[HIGHT][WIDTH], int x, int y, int new_val, int old_val);
    void Fill1(int (&mas)[HIGHT][WIDTH], int x, int y, int new_val, int old_val);
    void AssignWeight(int x, int y);
};


class Camera {
public:
    Camera(std::string ip_) : ip(ip_) {
        std::string path("../parking_server/parking/dataset/");
        for (int i = 1; i < 11; ++i) {
            cv::Mat new_img = cv::imread(path + "img" + std::to_string(i) + ".jpg", 0);
            if (new_img.empty()) {
                std::cout << "File 'img" << std::to_string(i) << ".jpg is not found" << std::endl;
            } else {
                dataset.push_back(new_img);
            }
        }

        last_image = dataset.back();
        dataset.pop_back();
    }
    ~Camera() {}

    cv::Mat GetImage();
    cv::Mat GetLastImage();

private:
    std::string ip;
    cv::Mat last_image;

    std::vector<cv::Mat> dataset; // Временно, пока не работает камера
};


class ParkingView {
public:
    ParkingView(std::string ip) : camera(ip) {}
    ~ParkingView() {}

    int UpdateViewDataset();
    int UpdateViewSpace(int index);
    int SetViewParams();
    SpaceInfo GetSpaceInfo();

private:
    SpaceInfo space_info;
    Camera camera;
    Params params;
};


class ViewsManager {
public:
    // ViewsManager();
    ViewsManager(std::vector<std::string> vec_ip) {
        std::vector<ParkingView> vec;
        for (size_t i = 0; i < vec_ip.size(); i++) {
            ParkingView parking_view(vec_ip.at(i));
            vec.push_back(parking_view);
        }
        parking_list = vec;
    }

    ~ViewsManager() {}

    int MakeDataset();
    int FitParams();
    int SetParams();
    int UpdateSpace(int index);
    std::vector<SpaceInfo> GetSpaceInfo();

private:
    std::vector<ParkingView> parking_list;
};

class Parking {
public:
    Parking(boost::asio::io_context& io_context, const std::string& server, const std::string& port, std::vector<std::string> vec_ip)
                 : manager(vec_ip), 
                 client_parking(io_context, server, port) {

    }

    ~Parking() {}

    int RunParking();
    int PushView(std::string ip);
    int PopView(std::string ip);

private:
    ViewsManager manager;
    ClientParking client_parking;
};

int GetParkingID(std::string path);