#include <parking.hpp>
#include <gtest/gtest.h>
#include <math.h>
// #include <gmock/gmock.h>

#include <iostream>


TEST(RunParkingTest, Test1) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    boost::asio::io_context io_context;

    Parking parking(io_context, "server-ip", "port", vec_ip);

    ASSERT_EQ(0, parking.RunParking());
}

TEST(RunParkingTest, Test2) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1.0.0.0.0");
    boost::asio::io_context io_context;

    Parking parking(io_context, "server-ip", "port", vec_ip);

    ASSERT_EQ(1, parking.RunParking());
}

TEST(RunParkingTest, Test3) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("hello");
    boost::asio::io_context io_context;

    Parking parking(io_context, "server-ip", "port", vec_ip);

    ASSERT_EQ(1, parking.RunParking());
}

TEST(RunParkingTest, Test4) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("");
    boost::asio::io_context io_context;

    Parking parking(io_context, "server-ip", "port", vec_ip);

    ASSERT_EQ(1, parking.RunParking());
}


TEST(PushViewTest, Test1) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    boost::asio::io_context io_context;

    Parking parking(io_context, "server-ip", "port", vec_ip);

    ASSERT_EQ(0, parking.PushView("192.168.1.255"));
}

TEST(PushViewTest, Test2) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    boost::asio::io_context io_context;

    Parking parking(io_context, "server-ip", "port", vec_ip);

    ASSERT_EQ(1, parking.PushView("192.168.1.255.0.0.0.0"));
}

TEST(PushViewTest, Test3) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    boost::asio::io_context io_context;

    Parking parking(io_context, "server-ip", "port", vec_ip);

    ASSERT_EQ(1, parking.PushView("hello"));
}

TEST(PushViewTest, Test4) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    boost::asio::io_context io_context;

    Parking parking(io_context, "server-ip", "port", vec_ip);

    ASSERT_EQ(1, parking.PushView(""));
}


TEST(PopViewTest, Test1) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    boost::asio::io_context io_context;

    Parking parking(io_context, "server-ip", "port", vec_ip);

    ASSERT_EQ(0, parking.PopView("192.168.1.255"));
}

TEST(PopViewTest, Test2) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    boost::asio::io_context io_context;

    Parking parking(io_context, "server-ip", "port", vec_ip);

    ASSERT_EQ(1, parking.PopView("192.168.1.255.0.0.0"));
}

TEST(PopViewTest, Test3) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    boost::asio::io_context io_context;

    Parking parking(io_context, "server-ip", "port", vec_ip);

    ASSERT_EQ(1, parking.PopView("hello"));
}

TEST(PopViewTest, Test4) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    boost::asio::io_context io_context;

    Parking parking(io_context, "server-ip", "port", vec_ip);

    ASSERT_EQ(1, parking.PopView(""));
}


TEST(ManagerMakeDataset, Test1) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    ViewsManager manager(vec_ip);

    ASSERT_EQ(0, manager.MakeDataset());
}

TEST(ManagerMakeFitParams, Test1) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    ViewsManager manager(vec_ip);

    ASSERT_EQ(0, manager.FitParams());
}

TEST(ManagerMakeSetParams, Test1) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    vec_ip.push_back("192.168.1.2");
    ViewsManager manager(vec_ip);

    ASSERT_EQ(0, manager.SetParams());
}

TEST(ManagerMakeUpdateSpace, Test1) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    vec_ip.push_back("192.168.1.2");
    ViewsManager manager(vec_ip);

    ASSERT_EQ(0, manager.UpdateSpace());
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
