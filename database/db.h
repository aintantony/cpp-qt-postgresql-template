#ifndef DB_H
#define DB_H

#include <memory>
#include <string>
#include <pqxx/pqxx>

class Database
{
public:
    Database();

    bool connect();

    pqxx::connection& getConnection();

    std::string getVersion();

    std::string lastError();

private:
    std::unique_ptr<pqxx::connection> conn;
    std::string error;
};

#endif