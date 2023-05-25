#include "bot.hpp"

void
fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

class session : public std::enable_shared_from_this<session>
{
    tcp::resolver resolver_;
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_; 
    http::request<http::empty_body> req_;
    http::response<http::string_body> res_;

public:
    explicit
    session(net::io_context& ioc)
        : resolver_(net::make_strand(ioc))
        , stream_(net::make_strand(ioc))
    {
    }

    void
    run(
        char const* host,
        char const* port,
        char const* target,
        int version)
    {
        req_.version(version);
        req_.method(http::verb::get);
        req_.target(target);
        req_.set(http::field::host, host);
        req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        resolver_.async_resolve(
            host,
            port,
            beast::bind_front_handler(
                &session::on_resolve,
                shared_from_this()));
    }

    void
    on_resolve(
        beast::error_code ec,
        tcp::resolver::results_type results)
    {
        if(ec)
            return fail(ec, "resolve");

        stream_.expires_after(std::chrono::seconds(30));

        stream_.async_connect(
            results,
            beast::bind_front_handler(
                &session::on_connect,
                shared_from_this()));
    }

    void
    on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
    {
        if(ec)
            return fail(ec, "connect");

        stream_.expires_after(std::chrono::seconds(30));

        http::async_write(stream_, req_,
            beast::bind_front_handler(
                &session::on_write,
                shared_from_this()));
    }

    void
    on_write(
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "write");
        
        http::async_read(stream_, buffer_, res_,
            beast::bind_front_handler(
                &session::on_read,
                shared_from_this()));
    }

    void
    on_read(
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "read");

        std::cout << res_ << std::endl;

        stream_.socket().shutdown(tcp::socket::shutdown_both, ec);

        if(ec && ec != beast::errc::not_connected)
            return fail(ec, "shutdown");

    }
};   

int ClientBot::Send() {
    return 0;
}

int BotEvents::StartEvent(TgBot::Bot &bot) {
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "<b>Добро пожаловать в сервис Parktronic!</b>\n\nБот поможет быстро найти свободные парковочные места.\n\n/place - <b>Свободное парковочое место</b>", false, 0, nullptr, "HTML");
    });

    return 0;
}

int BotEvents::SpaceEvent(TgBot::Bot &bot) {
    bot.getEvents().onCommand("place", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Доступные парковочные места: ");
        bot.getApi().sendPhoto(message->chat->id, TgBot::InputFile::fromFile("../example.jpg", "image/jpeg"));
    });

    return 0;
}

int BotEvents::AnyEvent(TgBot::Bot &bot) {
    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        std::cout << "User wrote " + message->text << std::endl;
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }

        if (StringTools::startsWith(message->text, "/place")) {
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

    events.SpaceEvent(bot);

    events.AnyEvent(bot);

    events.HelpEvent(bot);

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

int ParktronicBot::SetEvents() {
    return 0;
}
