#pragma once

#include <shared.hpp>
#include <tgbot/tgbot.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;

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
    BotEvents() {}
    // BotEvents(boost::asio::io_context& io_context, const std::string& server, const std::string& port) 
    //     : client_bot(io_context, server, port) {}

    int StartEvent(TgBot::Bot &bot);
    int SpaceEvent(TgBot::Bot &bot);
    int AnyEvent(TgBot::Bot &bot);
    int HelpEvent(TgBot::Bot &bot);

private:

    // ClientBot client_bot;
};

class ParktronicBot {
public:
    ParktronicBot() : bot(getenv("TOKEN")) {}
    // Bot(std::string TOKEN, boost::asio::io_context& io_context, const std::string& server, 
    //     const std::string& port) : events(io_context, server, port) {};

    int RunBot();
    int SetEvents();

    TgBot::Bot bot;
private:
    BotEvents events;
};
 