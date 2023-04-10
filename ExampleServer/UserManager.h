#pragma once
#include <map>
#include <memory>
#include <map>
#include "User.h"

class UserManager final
{
public:
	UserManager() = default;
	~UserManager() = default;

	void AddUser(int id, const User& user);
	void RemoveUser(int id);
	User* GetUser(int id);

	int GetUserAmount() const;

private:
	std::map<int, std::unique_ptr<User>> m_users{};
};

