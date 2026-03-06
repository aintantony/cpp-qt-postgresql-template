#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <vector>
#include "../models/user.h"

class UserService
{
public:
    std::vector<User> getUsers();
};

#endif