#pragma once

#include <shared.hpp>
#include <fstream>

#include <pqxx/pqxx>

using boost::asio::ip::tcp;
using json = nlohmann::json;

namespace http = boost::beast::http;

const int GET_DATA = 0;
const int POST_DATA = 1;

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
                          image TEXT,
                          free_places INTEGER
                          );)";
        ExecuteCommand(create_tables);
    }

    ~Database() {}

    std::string FreePlacesByID(std::string id) {
        std::string query = R"(
            SELECT free_places FROM views
            WHERE parking_id = )" + id + ";";
        std::cout << query << std::endl;
        ExecuteQuery(query);
        std::string count = result[0][0].as<std::string>();
        return count;
    }

    std::string ImageByID(std::string id) {
        std::string query = R"(
            SELECT image FROM views
            WHERE parking_id = )" + id + ";";
        std::cout << query << std::endl;
        ExecuteQuery(query);
        std::string image = result[0][0].as<std::string>();
        return image;
    }

    void UpdateFreePlacesByID(std::string free_places, std::string id = "1") {
        std::string command = 
            "UPDATE views SET free_places = " + free_places +
            " WHERE parking_id = " + id + ";";
        std::cout << command << std::endl;
        ExecuteCommand(command);
    }

    void UpdateImageByID(std::string image, std::string id = "1") {
        std::string command = 
            "UPDATE views SET image = '" + image + "'" +
            " WHERE parking_id = " + id + ";";
        std::cout << "Save image" << std::endl;
        ExecuteCommand(command);
    }

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

class Connection {
public:
    Connection(boost::asio::io_context& io_context) : socket_(io_context) {}

    tcp::socket& socket() {
        return socket_;
    }

    void start() {
        socket_.async_read_some(boost::asio::buffer(data_), //
                                boost::bind(&Connection::handle_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }

private:
    void handle_read(const boost::system::error_code& error,
                     size_t bytes_transferred) {
        if (!error) {
            // Обрабатываем запрос
            std::string request = CutBuffer(data_);

            std::cout << "Received: " << request << std::endl;

            std::ofstream fs("log.txt");

            fs << request;

            std::cout << "Method: " << ParseMethod(request) << std::endl;

            std::cout << "Body: " << ParseBody(request) << std::endl;

            // Собираем ответ
            std::string response = MakeDecision(request);
            std::cout << "Response: " << response << std::endl; 
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(response), //
                                     boost::bind(&Connection::handle_write, this,
                                                 boost::asio::placeholders::error));
        } else {
            delete this;
        }
    }

    void handle_write(const boost::system::error_code& error) {
        if (!error) {
            socket_.async_read_some(boost::asio::buffer(data_), //
                                    boost::bind(&Connection::handle_read, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        } else {
            delete this;
        }
    }

    std::string ParseMethod(const std::string& http_request) {
        json request = json::parse(http_request);
        return request["method"];
    }

    std::string ParseBody(const std::string& http_request) {
        json request = json::parse(http_request);
        return request["body"].dump();
    }

    std::string MakeDecision(const std::string& request) {
        switch (ChooseCase(request)) {
            case GET_DATA: {
                json body = json::parse(ParseBody(request));
                std::string id = body["id"];
                std::string free_places = database.FreePlacesByID(id);
                std::string image_path = database.ImageByID(id);
                std::string response =
                              "{\"value\": \"" + free_places + "\","
                              "\"photo\": \"" + image_path + "\"}";
                return response;
            }
            case POST_DATA: {
                json body = json::parse(ParseBody(request));
                std::string id = body["id"];
                std::string value = body["value"];
                database.UpdateFreePlacesByID(value, id);
                std::string photo = body["photo"];
                database.UpdateImageByID(photo, id);
                return "OK";
            }
        }
        return "OK";
    }

    int ChooseCase(const std::string& http_request) {
        if (ParseMethod(http_request) == "GET") {
            return GET_DATA;
        }
        return POST_DATA;
    }

    std::string CutBuffer(std::string buffer) {
        int size = std::stoi(buffer);
        int pos = buffer.find(" ") + 1;
        return buffer.substr(pos, size);
    }

private:
    tcp::socket socket_;
    enum {max_length = 100000000};
    char data_[max_length];
    Database database;
};

class Server {
public:
    Server(short port) : acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
    }

    void run() {
        io_context_.run();
    }

private:
    void start_accept() {
        Connection* new_connection = new Connection(io_context_);
        acceptor_.async_accept(new_connection->socket(),
                               boost::bind(&Server::handle_accept,
                                                    this,
                                                    new_connection,
                                                    boost::asio::placeholders::error));
    }

    void handle_accept(Connection* new_connection, const boost::system::error_code& error) {
        if (!error) {
            new_connection->start();
        } else {
            std::cerr << "Connect error" << std::endl;
            delete new_connection;
        }

        start_accept();
    }

private:
    boost::asio::io_context io_context_;
    tcp::acceptor acceptor_;
};
