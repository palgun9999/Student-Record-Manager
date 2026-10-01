#ifndef USER_MANAGER_HPP
#define USER_MANAGER_HPP

#include <string>
#include <vector>
#include <mutex>

namespace Service {

struct AdminUser {
    std::string username;
    std::string password;
    std::string accessLevel; // "Full Control", "Staff", "Student"

    std::string toJson() const;
};

class UserManager {
private:
    std::string filePath;
    std::vector<AdminUser> users;
    std::string activeUser;
    std::string activeAccessLevel;
    std::mutex userMutex;

    void loadFromFile();
    void saveToFile();
    void seedInitialData();

public:
    explicit UserManager(std::string filePath = "data/users.csv");

    // PPT Required Methods
    bool login(const std::string& username, const std::string& password);
    void logout();
    bool addNewAdmin(const std::string& username, const std::string& password, const std::string& accessLevel = "Full Control");
    bool changePassword(const std::string& username, const std::string& newPassword);
    std::string checkAccessLevel(const std::string& username = "");

    // Helpers
    bool isLoggedIn() const;
    const std::string& getActiveUser() const;
    std::vector<AdminUser> getAllUsers();
};

} // namespace Service

#endif // USER_MANAGER_HPP

