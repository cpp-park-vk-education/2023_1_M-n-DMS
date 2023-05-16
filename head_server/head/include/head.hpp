#pragma once

#include <shared.hpp>

#include <pqxx/pqxx>

using boost::asio::ip::tcp;

class Database {
public:
    Database() : connection("dbname=test_db user=postgres password=postgres hostaddr=127.0.0.1 port=5432") {
        std::string create_tables = R"(
                          CREATE TABLE IF NOT EXISTS Parking_Lots (
                          id SERIAL PRIMARY KEY,
                          address VARCHAR(255)
                          );

                          CREATE TABLE IF NOT EXISTS Views (
                          id SERIAL PRIMARY KEY,
                          parking_id INTEGER REFERENCES Parking_Lots(id),
                          image BYTEA
                          );)";
        ExecuteCommand(create_tables);
    }

    ~Database() {}

    // int GetResult();

private:
    int ExecuteCommand(std::string command) {
        try {
            pqxx::work txn(connection);
            txn.exec(command);
            txn.commit();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
        return 0;
    }

    int ExecuteQuery(std::string query) {
        try {
            pqxx::work txn(connection);
            result = txn.exec(query);
            txn.commit();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
        return 0;
    }

    pqxx::connection connection;
    pqxx::result result;
};

class Handler {
public:
    Handler() {};

    ~Handler() {}

    int HandleRequest(boost::asio::ip::tcp::socket& socket) {
        boost::asio::streambuf request;
        boost::asio::read_until(socket, request, "\r\n");

        // Обрабатываем запрос
        std::istream request_stream(&request);
        std::string request_line;
        std::getline(request_stream, request_line);
        std::cout << "Полученный запрос: " << request_line << std::endl;

        // Принимаем решение
        std::string response = MakeDecision(request_line);

        // Отправляем ответ
        boost::asio::write(socket, boost::asio::buffer(response));
        return 0;
    }

    // int GetID();

    // int WriteInDB();

    // std::string FindInDB();

private:
    std::string MakeDecision(const std::string& request) {
        if (request == "GET / HTTP/1.1") {
            return "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
        }
        std::cerr << "Error" << std::endl;
    }

    Database database;
};

class HeadServer {
public:
    HeadServer(const short port) : io_service_(), acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

    ~HeadServer() {}

    int Listen() {
        while (true) {
            boost::asio::ip::tcp::socket socket(io_service_);
            acceptor_.accept(socket);
            handler.HandleRequest(socket);
        }
    }

private:
    Handler handler;

    boost::asio::io_service io_service_;

    boost::asio::ip::tcp::acceptor acceptor_;
};
