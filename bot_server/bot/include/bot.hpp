#pragma once

#include <shared.hpp>
#include <tgbot/tgbot.h>

class ClientBot : public Client {
public:
    ClientBot(boost::asio::io_context& io_context, const std::string& server, const std::string& port)
        : resolver_(io_context), socket_(io_context) {
    }
    
    ~ClientBot() {}

    int Send() override;

private:
    tcp::resolver resolver_;
    tcp::socket socket_;
    // pugi::xml_document request;
};

class BotEvents {
public:
    BotEvents(boost::asio::io_context& io_context, const std::string& server, const std::string& port) 
        : client_bot(io_context, server, port) {}

    int SpaceEvent();
    int AnyEvent();
    int HelpEvent();
    int StartEvent();

private:
    std::string SpaceMessage();
    std::string AnyMessage();
    std::string HelpMessage();
    std::string StartMessage();

    ClientBot client_bot;
};

class Bot {
public:
    Bot(std::string TOKEN, boost::asio::io_context& io_context, const std::string& server, 
        const std::string& port) : events(io_context, server, port) {};

    int RunBot();
    void SetEvents();

private:
    BotEvents events;
};
 