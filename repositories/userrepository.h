#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <vector>
#include "../models/user.h"

class UserRepository
{
public:
    std::vector<User> getAllUsers();
};

#endif