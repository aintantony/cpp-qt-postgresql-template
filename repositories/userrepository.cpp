#include "userrepository.h"
#include "../database/db.h"

std::vector<User> UserRepository::getAllUsers()
{
    Database db;
    
    if (!db.connect())
    {
        throw std::runtime_error(db.lastError());
    }

    pqxx::work txn(db.getConnection());

    pqxx::result result =
        txn.exec("SELECT id, name, email FROM users");

    std::vector<User> users;

    for (auto row : result)
    {
        User u;

        u.id = row["id"].as<int>();
        u.name = row["name"].c_str();
        u.email = row["email"].c_str();

        users.push_back(u);
    }

    return users;
}