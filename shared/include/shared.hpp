// External libs will be here

#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "../../pugixml/src/pugixml.hpp"

#include <string>
#include <vector>
#include <iostream>

using boost::asio::ip::tcp;

struct SpaceInfo {
    cv::Mat Image;
    int space;
    int free_space;

    SpaceInfo() {}
};

class Client {
public:
    std::vector<SpaceInfo> answer;

    virtual int Send() = 0;

protected:
    int Connect();
    int Disconnect();
};
