#pragma once

#include <shared.hpp>

#include <pqxx/pqxx>

using boost::asio::ip::tcp;

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
                          image BYTEA,
                          free_places INTEGER
                          );)";
        ExecuteCommand(create_tables);
    }

    ~Database() {}

    int FreePlacesByID(int id) {
        std::string query = R"(
            SELECT free_places FROM views
            WHERE parking_id = )" + std::to_string(id) + ";";
        std::cout << query << std::endl;
        ExecuteQuery(query);
        int count = result[0][0].as<int>();
        return count;
    }

    void UpdateFreePlacesByID(int free_places, int id = 1) {
        std::string command = 
            "UPDATE views SET free_places = " + std::to_string(free_places) +  
            " WHERE parking_id = " + std::to_string(id) + ";";
        std::cout << command << std::endl;
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
    Connection(boost::asio::io_context& io_context)
            : socket_(io_context) {}

    tcp::socket& socket() {
        return socket_;
    }

    void start() {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&Connection::handle_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
        // Здесь обрабатываем запрос
        std::cout << "Полученный запрос: " << data_ << std::endl;

        std::cout << "Body: " << ParseBody(data_) << std::endl;

        std::cout << "Method: " << ParseMethod(data_) << std::endl;
    }

private:
    void handle_read(const boost::system::error_code& error,
                     size_t bytes_transferred) {
        if (!error) {
            // Собираем ответ
            std::string response = MakeDecision(data_);
            std::cout << response << std::endl;
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(response),
                                     boost::bind(&Connection::handle_write, this,
                                                 boost::asio::placeholders::error));
        } else {
            delete this;
        }
    }

    void handle_write(const boost::system::error_code& error) {
        if (!error) {
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                    boost::bind(&Connection::handle_read, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        } else {
            delete this;
        }
    }

    std::string ParseMethod(const std::string& http_request) {
        http::request_parser<http::string_body> parser;
        boost::beast::error_code ec;
        parser.put(boost::asio::buffer(http_request), ec);
        if (ec) {
            std::cerr << "Error parsing request: " << ec.message() << std::endl;
        } else {
            http::request<http::string_body> parsed = parser.release();
            return std::string(parsed.method_string());
        }
    }

    std::string ParseBody(const std::string& http_request) {
        std::string body;
        try {
            int body_pos = http_request.find("\r\n\r\n");
            body = http_request.substr(body_pos + 4);
        } catch (std::exception& ex) {
            std::cout << ex.what() << std::endl;
            return "";
        }
        return body;
    }

    std::string MakeDecision(const std::string& request) {
        switch (ChooseCase(request)) {
            case GET_DATA: {
                // nlohmann::json body = nlohmann::json::parse(ParseBody(request));
                int id, free_places;
                try {
                    id = stoi(ParseBody(request));
                    free_places = database.FreePlacesByID(id);
                } catch (std::exception& ex) {
                    std::cout << ex.what() << std::endl;
                    return "";
                }
                return std::to_string(free_places);
            }
            case POST_DATA: {
                // nlohmann::json body = nlohmann::json::parse(ParseBody(request));
                int free_places = stoi(ParseBody(request));
                database.UpdateFreePlacesByID(free_places);
                return "OK";
            }
        }
    }

    int ChooseCase(const std::string& http_request) {
        if (ParseMethod(http_request) == "GET") {
            return GET_DATA;
        }
        return POST_DATA;
    }

private:
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    Database database;
};

class Server {
public:
    Server(short port)
            : acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)) {
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

    void handle_accept(Connection* new_connection,
                       const boost::system::error_code& error) {
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
