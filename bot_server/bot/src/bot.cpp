#include "bot.hpp"

ClientBot::ClientBot(boost::asio::io_service& io_service, const std::string& host, const std::string& port)
    : io_service_(io_service), 
    resolver_(io_service), 
    socket_(io_service) {
    auto endpoints = resolver_.resolve({host, port});

    boost::asio::async_connect(socket_, endpoints,
        [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint ep) {
        if (ec) {
                std::cerr << "Failed to connect to " << ep << ": " << ec.message() << std::endl;
                return;
            }
            std::cout << "Connected to " << ep << std::endl;

        });
}

void ClientBot::Read() {
    
    boost::asio::async_read(socket_, response_, boost::asio::transfer_at_least(1),
        [this](boost::system::error_code ec, std::size_t bytes_transferred) {
            if (ec && ec != boost::asio::error::eof) {
                std::cerr << "Failed to read from server: " << ec.message() << std::endl;
                return;
            }
            std::istream is(&response_);
            std::string response_data;
            std::getline(is, response_data);
            std::cout << "Received response: " << response_data << std::endl;

            this->response_data_ = response_data;

            response_.consume(response_.size());
        });
        
}

int ClientBot::Send(const std::string& message) {
    boost::asio::async_write(socket_, boost::asio::buffer(message),
        [this, message](boost::system::error_code ec, std::size_t bytes_transferred) {
            if (ec) {
                std::cerr << "Failed to write to server: " << ec.message() << std::endl;
                return;
            }
            std::cout << "Sent message: " << message << std::endl;

            Read();
        });
    return 0;
}


int BotEvents::StartEvent(TgBot::Bot &bot) {
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "<b>Добро пожаловать в сервис Parktronic!</b>\n\nБот поможет быстро найти свободные парковочные места.\n\n/place - <b>Свободное парковочое место</b>", false, 0, nullptr, "HTML");
    });
    return 0;
}

int BotEvents::SpaceEvent(TgBot::Bot &bot, ClientBot& client_bot) {
    bot.getEvents().onCommand("space", [&bot, &client_bot](TgBot::Message::Ptr message) {
        // Формируем запрос
        std::string request =
	                            "{\"method\": \"GET\","
	                            "\"body\": {"
		                        "\"id\": \"" + std::to_string(1) + "\"}}";
        std::string request_size = std::to_string(request.size());
        request = request_size + " " + request;

        client_bot.Send(request);
        client_bot.Run();
        client_bot.Restart();
        bot.getApi().sendMessage(message->chat->id, client_bot.response_data_);
        //bot.getApi().sendPhoto(message->chat->id, TgBot::InputFile::fromFile("../example.jpg", "image/jpeg"));
    });

    return 0;
}

int BotEvents::AnyEvent(TgBot::Bot &bot) {
    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        std::cout << "User wrote " + message->text << std::endl;
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }

        if (StringTools::startsWith(message->text, "/space")) {
            return;
        }

        if (StringTools::startsWith(message->text, "/help")) {
            return;
        }

        bot.getApi().sendMessage(message->chat->id, "Такой команды не существует. Нажмите /help или воспользуйтесь меню, чтобы просмотреть доступный функционал.");
    });
    return 0;
}

int BotEvents::HelpEvent(TgBot::Bot &bot) {
    bot.getEvents().onCommand("help", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Бот поможет быстро найти свободные парковочные места.\n\n/place - <b>Свободное парковочое место</b>", false, 0, nullptr, "HTML");
    });

    return 0;
}

int ParktronicBot::RunBot() {
    
    events.StartEvent(bot);

    events.SpaceEvent(bot, events.client_bot);

    events.AnyEvent(bot);

    events.HelpEvent(bot);

    //events.client_bot.Run();

    try {
        std::cout << "Bot username: " + bot.getApi().getMe()->username << std::endl;
        TgBot::TgLongPoll longPoll(bot);
        while (true) {

            std::cout << "Long poll started" << std::endl;
            longPoll.start();
        }
    } 
    catch (TgBot::TgException& e) {
        std::cout << "error: " << e.what() << std::endl;
    }
    
    return 0;
}

int ClientBot::Run() {
    io_service_.run();
} 

int ClientBot::Restart() {
    io_service_.restart();
} 

int ParktronicBot::SetEvents() {
    return 0;
}
