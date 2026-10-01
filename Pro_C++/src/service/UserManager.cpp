#include "service/UserManager.hpp"
#include "models/Student.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace Service {

std::string AdminUser::toJson() const {
    std::ostringstream oss;
    oss << "{"
        << "\"username\":\"" << Models::Student::escapeJson(username) << "\","
        << "\"accessLevel\":\"" << Models::Student::escapeJson(accessLevel) << "\""
        << "}";
    return oss.str();
}

UserManager::UserManager(std::string filePath)
    : filePath(std::move(filePath)), activeUser(""), activeAccessLevel("") {
    loadFromFile();
}

void UserManager::loadFromFile() {
    std::lock_guard<std::mutex> lock(userMutex);
    users.clear();

    try {
        std::filesystem::path p(filePath);
        if (p.has_parent_path()) {
            std::filesystem::create_directories(p.parent_path());
        }
    } catch (...) {}

    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        seedInitialData();
        return;
    }

    std::string line;
    bool firstLine = true;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        if (firstLine && line.find("username") != std::string::npos) {
            firstLine = false;
            continue;
        }
        firstLine = false;

        std::istringstream ss(line);
        std::string u, p, a;
        if (std::getline(ss, u, ',') &&
            std::getline(ss, p, ',') &&
            std::getline(ss, a, ',')) {
            users.push_back({u, p, a});
        }
    }
    inFile.close();

    if (users.empty()) {
        seedInitialData();
    }
}

void UserManager::saveToFile() {
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) return;

    outFile << "username,password,accessLevel\n";
    for (const auto& u : users) {
        outFile << u.username << "," << u.password << "," << u.accessLevel << "\n";
    }
    outFile.close();
}

void UserManager::seedInitialData() {
    // Default admin matching PPT Slide 8: admin / admin123 -> Full Control
    users = {
        {"admin", "admin123", "Full Control"},
        {"faculty", "faculty123", "Staff"}
    };
    saveToFile();
}

bool UserManager::login(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(userMutex);
    for (const auto& u : users) {
        if (u.username == username && u.password == password) {
            activeUser = u.username;
            activeAccessLevel = u.accessLevel;
            return true;
        }
    }
    return false;
}

void UserManager::logout() {
    std::lock_guard<std::mutex> lock(userMutex);
    activeUser = "";
    activeAccessLevel = "";
}

bool UserManager::addNewAdmin(const std::string& username, const std::string& password, const std::string& accessLevel) {
    std::lock_guard<std::mutex> lock(userMutex);
    for (const auto& u : users) {
        if (u.username == username) {
            return false; // User exists
        }
    }
    users.push_back({username, password, accessLevel});
    saveToFile();
    return true;
}

bool UserManager::changePassword(const std::string& username, const std::string& newPassword) {
    std::lock_guard<std::mutex> lock(userMutex);
    for (auto& u : users) {
        if (u.username == username) {
            u.password = newPassword;
            saveToFile();
            return true;
        }
    }
    return false;
}

std::string UserManager::checkAccessLevel(const std::string& username) {
    std::lock_guard<std::mutex> lock(userMutex);
    std::string target = username.empty() ? activeUser : username;
    for (const auto& u : users) {
        if (u.username == target) {
            return u.accessLevel;
        }
    }
    return "None";
}

bool UserManager::isLoggedIn() const {
    return !activeUser.empty();
}

const std::string& UserManager::getActiveUser() const {
    return activeUser;
}

std::vector<AdminUser> UserManager::getAllUsers() {
    std::lock_guard<std::mutex> lock(userMutex);
    return users;
}

} // namespace Service

