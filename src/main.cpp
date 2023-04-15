#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

#include <tgbot/tgbot.h>

int main() 
{
    TgBot::Bot bot(getenv("TOKEN"));

    bot.getEvents().onCommand("place", [&bot](TgBot::Message::Ptr message) 
    {
        bot.getApi().sendMessage(message->chat->id, "Доступные парковочные места: ");
        bot.getApi().sendPhoto(message->chat->id, TgBot::InputFile::fromFile("../example.jpg", "image/jpeg"));
    });

    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) 
    {
        bot.getApi().sendMessage(message->chat->id, "<b>Добро пожаловать в сервис Parktronic!</b>\n\nБот поможет быстро найти свободные парковочные места.\n\n/place - <b>Свободное парковочое место</b>", false, 0, nullptr, "HTML");
    });

    bot.getEvents().onCommand("help", [&bot](TgBot::Message::Ptr message) 
    {
        bot.getApi().sendMessage(message->chat->id, "Бот поможет быстро найти свободные парковочные места.\n\n/place - <b>Свободное парковочое место</b>", false, 0, nullptr, "HTML");
    });
    
    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) 
    {
        std::cout << "User wrote " + message->text << std::endl;
        if (StringTools::startsWith(message->text, "/start")) 
        {
            return;
        }

        if (StringTools::startsWith(message->text, "/place")) 
        {
            return;
        }

        if (StringTools::startsWith(message->text, "/help")) 
        {
            return;
        }

        bot.getApi().sendMessage(message->chat->id, "Такой команды не существует. Нажмите /help или воспользуйтесь меню, чтобы просмотреть доступный функционал.");
    });

    try 
    {
        std::cout << "Bot username: " + bot.getApi().getMe()->username << std::endl;
        TgBot::TgLongPoll longPoll(bot);
        while (true) 
        {
            std::cout << "Long poll started" << std::endl;
            longPoll.start();
        }
    } 
    catch (TgBot::TgException& e) 
    {
        std::cout << "error: " << e.what() << std::endl;
    }
    
    return 0;
}
