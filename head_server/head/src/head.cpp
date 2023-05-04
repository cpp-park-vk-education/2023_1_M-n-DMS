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
