// External libs will be here

#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
// #include "../../pugixml/src/pugixml.hpp"

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
    virtual int Send(const std::string& message) = 0;

protected:
    int Connect();
    int Disconnect();
};
