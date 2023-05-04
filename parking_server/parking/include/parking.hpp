#pragma once

#include <shared.hpp>

#define WIDTH 1280
#define HEIGHT 720

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
                 : resolver_(io_context), socket_(io_context) {

    }

    ~ClientParking() {}

    int Send() override;

private:
    tcp::resolver resolver_;
    tcp::socket socket_;
    // pugi::xml_document client_info;
};


class Params {
public:
    int threshold;
    int min_square;
    bool location_matrix[HEIGHT][WIDTH];
    int counter_matrix[HEIGHT][WIDTH];

    Params() {}
    ~Params() {}
    
    int FitParams();
    int SetDataset(cv::Mat img);

private:
    int Fill();

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
    ClientParking client_parking;
    ViewsManager manager;
};
