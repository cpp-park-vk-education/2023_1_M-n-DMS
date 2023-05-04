#include <parking.hpp>
#include <gtest/gtest.h>
#include <typeinfo>
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


TEST(GetSpaceInfoTest, Test1) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    vec_ip.push_back("192.168.1.2");
    boost::asio::io_context io_context;

    Parking parking(io_context, "server-ip", "port", vec_ip);

    ASSERT_EQ(1, parking.PopView(""));
}


TEST(ManagerMakeDatasetTest, Test1) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    ViewsManager manager(vec_ip);

    ASSERT_EQ(0, manager.MakeDataset());
}

TEST(ManagerMakeFitParamsTest, Test1) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    ViewsManager manager(vec_ip);

    ASSERT_EQ(0, manager.FitParams());
}

TEST(ManagerMakeSetParamsTest, Test1) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    vec_ip.push_back("192.168.1.2");
    ViewsManager manager(vec_ip);

    ASSERT_EQ(0, manager.SetParams());
}

TEST(ManagerMakeUpdateSpaceTest, Test1) {
    std::vector<std::string> vec_ip;
    vec_ip.push_back("192.168.1.1");
    vec_ip.push_back("192.168.1.2");
    ViewsManager manager(vec_ip);

    ASSERT_EQ(0, manager.UpdateSpace());
}

TEST(UpdateViewDatasetTest, Test1) {
    ParkingView parking_view("192.168.1.2");

    ASSERT_EQ(0, parking_view.UpdateViewDataset());
}

TEST(UpdateViewSpaceTest, Test1) {
    ParkingView parking_view("192.168.1.2");

    ASSERT_EQ(0, parking_view.UpdateViewDataset());
}

TEST(SetViewParamsTest, Test1) {
    ParkingView parking_view("192.168.1.2");

    ASSERT_EQ(0, parking_view.UpdateViewDataset());
}


TEST(GetSpaceInfoTests, Test1) {
    ParkingView parking_view("192.168.1.2");
    SpaceInfo inf;
    ASSERT_EQ(inf.free_space, parking_view.GetSpaceInfo().free_space);
}

TEST(GetSpaceInfoTests, Test2) {
    ParkingView parking_view("192.168.1.2");
    SpaceInfo inf;
    ASSERT_EQ(inf.space, parking_view.GetSpaceInfo().space);
}

TEST(GetSpaceInfoTests, Test3) {
    ParkingView parking_view("192.168.1.2");
    SpaceInfo inf;
    ASSERT_STREQ(typeid(inf).name(), typeid(parking_view.GetSpaceInfo()).name());
}


TEST(FitParamsTest, Test1) {
    Params params;
    ASSERT_EQ(0, params.FitParams());
}

TEST(CameraGetImageTest, Test1) {
    Camera camera;
    cv::Mat img;
    ASSERT_STREQ(typeid(img).name(), typeid(camera.GetImage()).name());
}




int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
