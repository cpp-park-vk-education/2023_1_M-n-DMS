#include <head.hpp>

Database::Database(const std::string& dbname, const std::string& user, const std::string& password,
         const std::string& host, const std::string& port) {}

Database::~Database() {}

int Database::ExecuteQuery(std::string query) {
    return 0;
}

int Database::ExecuteCommand(std::string command) {
    return 0;
}

int Database::GetResult() {
    return 0;
} // pqxx::result

Handler::~Handler() {}

int Handler::MakeDecision() {
    return 0;
}

int Handler::GetID() {
    return 0;
}

int Handler::WriteInDB() {
    return 0;
}

std::string Handler::FindInDB() {
    return "";
}

HeadServer::~HeadServer() {}

int HeadServer::Listen() {
    return 0;
}

int HeadServer::SetAcceptor(unsigned short port) {
    return 0;
}
