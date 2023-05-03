// External libs will be here

#pragma once

#include <string>
#include <vector>
#include <iostream>

struct SpaceInfo {
    std::string Image; // cv::Mat
    int space;
    int free_space;

    SpaceInfo();
};

class Client {
protected:
    std::string socket_; //

    int Connect();

    int Disconnect();
public:
    std::vector<SpaceInfo> answer;

    virtual int Send() = 0;
};
