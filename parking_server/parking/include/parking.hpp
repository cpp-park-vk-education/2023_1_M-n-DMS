#pragma once

#include <shared.hpp>

#define WIDTH 1280
#define HIGHT 720

class ClientParking;
class Parking;
class ParkingView;
class Camera;
class Params;
struct SpaceInfo;

class ClientParking : Client {
public:
    // ClientParking();
    ClientParking(boost::asio::io_context& io_context, const std::string& server, const std::string& port, std::vector<SpaceInfo> space_info)
                 : resolver_(io_context), socket_(io_context), has_info(false) {

    }

    ~ClientParking() {}

    int Send() override;

    bool GetHasInfo() { return has_info; }

private:
    tcp::resolver resolver_;
    tcp::socket socket_;
    // pugi::xml_document client_info;
    bool has_info;
};


class Params {
public:
    short threshold;
    int square;
    int min_square;
    bool location_matrix[HIGHT][WIDTH];
    unsigned int counter_matrix[HIGHT][WIDTH];

    int min_pos_x = 2000, min_pos_y = 2000, max_pos_x = 0, max_pos_y = 0;

    Params() : threshold(0), square(0), min_square(0) {
        for (int i = 0; i < HIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                location_matrix[i][j] = true;
                counter_matrix[i][j] = 0;
            }
        }
    }
    ~Params() {}
    
    int FitParams();
    int SetDataset(cv::Mat img);
    int Difference(cv::Mat bg_frame, cv::Mat cam_frame);  // +

private:
    void Fill(int (&mas)[HIGHT][WIDTH], int x, int y, int new_val, int old_val);
    void AssignWeight(int x, int y);
    void PrintIMG();                         // +

    std::vector<cv::Mat> dataset;
};


class Camera {
public:
    Camera() {}
    ~Camera() {}

    cv::Mat GetImage();

private:
    std::string ip;
    cv::Mat last_image;
};


class ParkingView {
public:
    ParkingView(std::string ip) {}
    ~ParkingView() {}

    int UpdateViewDataset();
    int UpdateViewSpace();
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
    int UpdateSpace();
    std::vector<SpaceInfo> GetSpaceInfo() {
        std::vector<SpaceInfo> vec;
        return vec;
    }

private:
    std::vector<ParkingView> parking_list;
};

class Parking {
public:
    Parking(boost::asio::io_context& io_context, const std::string& server, const std::string& port, std::vector<std::string> vec_ip)
                 : manager(vec_ip), 
                 client_parking(io_context, server, port, manager.GetSpaceInfo()) {

    }

    ~Parking() {}

    int RunParking();
    int PushView(std::string ip);
    int PopView(std::string ip);

private:
    ViewsManager manager;
    ClientParking client_parking;
};
