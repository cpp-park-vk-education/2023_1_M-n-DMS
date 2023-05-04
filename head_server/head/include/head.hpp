#pragma once

#include <shared.hpp>

class Database {
private:
    pqxx::result result;
    pqxx::connection connection;

public:
    Database(const std::string& dbname, const std::string& user, const std::string& password,
             const std::string& host, const std::string& port);

    ~Database()

    int ExecuteQuery(std::string query);

    int ExecuteCommand(std::string command);

    pqxx::result GetResult();
}

class Handler {
public:
    Handler(unsigned short port);

    ~Handler()

    int MakeDecision();

private:
    boost::asio::ip::tcp::socket socket;

    Database database;

    int GetID();

    int WriteInDB();

    std::string FindInDB();
}

class HeadServer {
public:
    HeadServer(unsigned short port);

    ~HeadServer();

    int Listen();

private:
    Handler handler;

    boost::asio::ip::tcp::acceptor acceptor

    int SetAcceptor(unsigned short port);
};
