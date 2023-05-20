#pragma once

#include <shared.hpp>
#include <tgbot/tgbot.h>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>


class ClientBot : public Client {
public:
    ClientBot(boost::asio::io_service& io_service, const std::string& host, const std::string& port);

    ~ClientBot() {}

    int Send(const std::string& message) override;

    int Run();

    int Restart();

    std::string response_data_;

private:
    void Read();
    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf response_;
    // pugi::xml_document request;
};

class BotEvents {
public:
    BotEvents(boost::asio::io_service& io_service, const std::string& host, const std::string& port)
        : client_bot(io_service, host, port) {}

    int StartEvent(TgBot::Bot &bot);
    int SpaceEvent(TgBot::Bot &bot, ClientBot& client_bot);
    int AnyEvent(TgBot::Bot &bot);
    int HelpEvent(TgBot::Bot &bot);

    ClientBot client_bot;

};

class ParktronicBot {
public:

    ParktronicBot(boost::asio::io_service& io_service, const std::string& host, 
         const std::string& port) : bot(getenv("TOKEN")), events(io_service, host, port) {};

    int RunBot();
    int SetEvents();

    TgBot::Bot bot;
private:
    BotEvents events;
};
 