#pragma once

#include <shared.hpp>

class ClientBot : public Client {
private:
    std::string request; //
public:
    ClientBot(std::string socket, const std::string& server, //
              const std::string& port);

    ~ClientBot();

    int Send();
};

class Bot {
private:
    ClientBot client_bot; 

    std::string SpaceMessage();

    std::string AnyMessage();

    std::string HelpMessage();

    std::string StartMessage();
public:
    Bot(std::string TOKEN);

    void SetEvents();
};
