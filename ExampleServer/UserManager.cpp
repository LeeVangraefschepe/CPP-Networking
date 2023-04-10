#include "pch.h"
#include "UserManager.h"

void UserManager::AddUser(int id, const User& user)
{
	m_users.insert(std::pair<int, std::unique_ptr<User>>{id, std::make_unique<User>(user)});
}
void UserManager::RemoveUser(int id)
{
	m_users.erase(id);
}

User* UserManager::GetUser(int id)
{
	return m_users[id].get();
}

int UserManager::GetUserAmount() const
{
	return static_cast<int>(m_users.size());
}
