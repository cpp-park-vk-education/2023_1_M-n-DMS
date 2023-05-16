#pragma once

#include <shared.hpp>

#include <pqxx/pqxx>

class Database {
private:
    int result; // pqxx::result
    int connection; // pqxx::connection

public:
    Database() {}

    Database(const std::string& dbname, const std::string& user, const std::string& password,
             const std::string& host, const std::string& port);

    ~Database();

    int ExecuteQuery(std::string query);

    int ExecuteCommand(std::string command);

    int GetResult(); // pqxx::result
};

class Handler {
public:
    Handler(const std::string& dbname, const std::string& user, const std::string& password,
             const std::string& host, const std::string& port, boost::asio::io_context& io_context) : socket(io_context), database(dbname, user, password, host, port) {};

    ~Handler();

    int MakeDecision();

    int GetID();

    int WriteInDB();

    std::string FindInDB();

private:
    boost::asio::ip::tcp::socket socket;

    Database database;
};

class HeadServer {
public:
    HeadServer(const std::string& dbname, const std::string& user, const std::string& password,
             const std::string& host, const std::string& port, boost::asio::io_context& io_context) : handler(dbname, user, password, host, port, io_context), acceptor(io_context) {}

    ~HeadServer();

    int Listen();

    int SetAcceptor(unsigned short port);
private:
    Handler handler;

    boost::asio::ip::tcp::acceptor acceptor;
};
