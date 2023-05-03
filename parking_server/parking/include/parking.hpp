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
    ClientParking(std::string socket, const std::string& server,                // tcp::socket socket
                 const std::string& port, std::vector<SpaceInfo> space_info);

    ~ClientParking();

    int Send();
};

class Parking {
public:
    Parking(std::vector<std::string> vec_ip);
    ~Parking() {}

    int MakeDataset();
    int SetParams();
    int UpdateSpace();
    int PushView(std::string ip);
    int PopView(std::string ip);
    std::vector<SpaceInfo> GetSpaceInfo();

private:
    std::vector<ParkingView> parking_list;
};


class Params {
public:
    int threshold;
    int min_square;
    bool location_matrix[HEIGHT][WIDTH];
    int counter_matrix[HEIGHT][WIDTH];

    Params();
    ~Params();
    
    int FitParams();
    int SetDataset(std::string); // std::string -> cv::Mat


private:
    int Fill();

    std::vector<std::string> dataset; // std::vector<cv::Mat>
};


class Camera {
public:
    Camera();
    ~Camera();

    std::string GetImage(); // cv::Mat

private:
    std::string ip;
    std::string last_image; // cv::Mat
};


class ParkingView {
public:
    ParkingView(std::string ip);
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
