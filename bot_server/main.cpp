#include <bot.hpp>

#include <iostream>
#include <gtest/gtest.h>
#include <math.h>

TEST(ClientBotSendTest, Test1) {
    boost::asio::io_context io_context;

    ClientBot clientbot(io_context, "192.168.1.1", "1234");

    ASSERT_EQ(0, clientbot.Send());
}

TEST(ClientBotSendTest, Test2) {
    boost::asio::io_context io_context;

    ClientBot clientbot(io_context, "server-ip", "port");

    ASSERT_EQ(1, clientbot.Send());
}

TEST(SpaceEventTest, Test1) {
    boost::asio::io_context io_context;

    BotEvents events(io_context, "192.168.1.1", "1234");

    ASSERT_EQ(0, events.SpaceEvent());
}

TEST(SpaceEventTest, Test2) {
    boost::asio::io_context io_context;

    BotEvents events(io_context, "server-ip", "port");

    ASSERT_EQ(1, events.SpaceEvent());
}

TEST(AnyEventTest, Test1) {
    boost::asio::io_context io_context;

    BotEvents events(io_context, "192.168.1.1", "1234");

    ASSERT_EQ(0, events.AnyEvent());
}

TEST(AnyEventTest, Test2) {
    boost::asio::io_context io_context;

    BotEvents events(io_context, "server-ip", "port");

    ASSERT_EQ(1, events.AnyEvent());
}

TEST(HelpEventTest, Test1) {
    boost::asio::io_context io_context;

    BotEvents events(io_context, "192.168.1.1", "1234");

    ASSERT_EQ(0, events.HelpEvent());
}

TEST(HelpEventTest, Test2) {
    boost::asio::io_context io_context;

    BotEvents events(io_context, "server-ip", "port");

    ASSERT_EQ(1, events.HelpEvent());
}

TEST(StartEventTest, Test1) {
    boost::asio::io_context io_context;

    BotEvents events(io_context, "192.168.1.1", "1234");

    ASSERT_EQ(0, events.StartEvent());
}

TEST(StartEventTest, Test2) {
    boost::asio::io_context io_context;

    BotEvents events(io_context, "server-ip", "port");

    ASSERT_EQ(1, events.StartEvent());
}

TEST(BotSetEventsTest, Test1) {
    boost::asio::io_context io_context;
    std::string TOKEN("1");

    Bot bot(TOKEN, io_context, "192.168.1.1", "1234");

    ASSERT_EQ(0, bot.SetEvents());
}

TEST(BotSetEventsTest, Test2) {
    boost::asio::io_context io_context;
    std::string TOKEN("1");

    Bot bot(TOKEN, io_context, "server-ip", "port");

    ASSERT_EQ(0, bot.SetEvents());
}


TEST(RunBotTest, Test1) {
    boost::asio::io_context io_context;
    std::string TOKEN("1");

    Bot bot(TOKEN, io_context, "192.168.1.1", "1234");

    ASSERT_EQ(0, bot.RunBot());
}

TEST(RunBotTest, Test2) {
    boost::asio::io_context io_context;
    std::string TOKEN("1");
    
    Bot bot(TOKEN, io_context, "server-ip", "port");

    ASSERT_EQ(1, bot.RunBot());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
