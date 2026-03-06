#include "db.h"
#include "../config/env.h"

Database::Database() {}

bool Database::connect()
{
    try {

        std::string connStr =
            "host=" + Env::get("DB_HOST") +
            " dbname=" + Env::get("DB_NAME") +
            " user=" + Env::get("DB_USER") +
            " password=" + Env::get("DB_PASSWORD");

        conn = std::make_unique<pqxx::connection>(connStr);

        if (!conn->is_open())
        {
            error = "Failed to open database connection";
            return false;
        }

        return true;

    } catch (const std::exception &e) {
        error = e.what();
        return false;
    }
}

pqxx::connection& Database::getConnection()
{
    if (!conn)
        throw std::runtime_error("Database not connected");

    return *conn;
}

std::string Database::getVersion()
{
    try {
        pqxx::work txn(*conn);
        pqxx::result r = txn.exec("SELECT version();");

        return r[0][0].c_str();

    } catch (const std::exception &e) {
        error = e.what();
        return "";
    }
}

std::string Database::lastError()
{
    return error;
}