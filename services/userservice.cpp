#include "userservice.h"
#include "../repositories/userrepository.h"

std::vector<User> UserService::getUsers()
{
    UserRepository repo;
    return repo.getAllUsers();
}