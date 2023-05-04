#include "parking.hpp"

int Parking::RunParking() {
    return 0;
}

int Parking::PushView(std::string ip) {
    return 0;
}

int Parking::PopView(std::string ip) {
    return 0;
}

int ViewsManager::MakeDataset() {
    return 0;
}

int ViewsManager::FitParams() {
    return 0;
}

int ViewsManager::SetParams() {
    return 0;
}

int ViewsManager::UpdateSpace() {
    return 0;
}

int ClientParking::Send() {
    std::cout << "Send from ClientParking" << std::endl;
    return 0;
}

int ParkingView::UpdateViewDataset() {
    return 0;
}
int ParkingView::UpdateViewSpace() {
    return 0;
}
int ParkingView::SetViewParams() {
    return 0;
}

SpaceInfo ParkingView::GetSpaceInfo() {
    SpaceInfo space_info;
    return space_info;
}

int Params::FitParams() {
    return 0;
}

int Params::SetDataset(cv::Mat img) {
    return 0;
}

cv::Mat Camera::GetImage() {
    cv::Mat img;
    return img;
}