#include <parking.hpp>

#include <iostream>

int main() {

    std::vector<std::string> vec_ip;
    vec_ip.push_back("123");
    vec_ip.push_back("456");
    boost::asio::io_context io_context;

    Parking parking(io_context, "1", "2", vec_ip);

    std::cout << "Server for parking" << std::endl;
    return 0;
}
