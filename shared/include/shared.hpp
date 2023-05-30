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
#include <nlohmann/json.hpp>

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

namespace base64 {
    inline std::string get_base64_chars() {
        static std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                    "abcdefghijklmnopqrstuvwxyz"
                                    "0123456789+/";
        return base64_chars;
    }

    inline std::string to_base64(std::string const &data) {
        int counter = 0;
        uint32_t bit_stream = 0;
        const std::string base64_chars = get_base64_chars();
        std::string encoded;
        int offset = 0;
        for (unsigned char c : data) {
            auto num_val = static_cast<unsigned int>(c);
            offset = 16 - counter % 3 * 8;
            bit_stream += num_val << offset;
            if (offset == 16) {
                encoded += base64_chars.at(bit_stream >> 18 & 0x3f);
            }
            if (offset == 8) {
                encoded += base64_chars.at(bit_stream >> 12 & 0x3f);
            }
            if (offset == 0 && counter != 3) {
                encoded += base64_chars.at(bit_stream >> 6 & 0x3f);
                encoded += base64_chars.at(bit_stream & 0x3f);
                bit_stream = 0;
            }
            counter++;
        }
        if (offset == 16) {
            encoded += base64_chars.at(bit_stream >> 12 & 0x3f);
            encoded += "==";
        }
        if (offset == 8) {
            encoded += base64_chars.at(bit_stream >> 6 & 0x3f);
            encoded += '=';
        }
        return encoded;
    }

    inline std::string from_base64(std::string const &data) {
        int counter = 0;
        uint32_t bit_stream = 0;
        std::string decoded;
        int offset = 0;
        const std::string base64_chars = get_base64_chars();
        for (unsigned char c : data) {
            auto num_val = base64_chars.find(c);
            if (num_val != std::string::npos) {
                offset = 18 - counter % 4 * 6;
                bit_stream += num_val << offset;
                if (offset == 12) {
                    decoded += static_cast<char>(bit_stream >> 16 & 0xff);
                }
                if (offset == 6) {
                    decoded += static_cast<char>(bit_stream >> 8 & 0xff);
                }
                if (offset == 0 && counter != 4) {
                    decoded += static_cast<char>(bit_stream & 0xff);
                    bit_stream = 0;
                }
            } else if (c != '=') {
                return std::string();
            }
            counter++;
        }
        return decoded;
    }
}
