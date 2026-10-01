#include "service/FeeManager.hpp"
#include "models/Student.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <set>

namespace Service {

std::string FeePayment::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "{"
        << "\"rollNo\":\"" << Models::Student::escapeJson(rollNo) << "\","
        << "\"amount\":" << amount << ","
        << "\"date\":\"" << Models::Student::escapeJson(date) << "\""
        << "}";
    return oss.str();
}

std::string FeeStatus::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "{"
        << "\"rollNo\":\"" << Models::Student::escapeJson(rollNo) << "\","
        << "\"name\":\"" << Models::Student::escapeJson(studentName) << "\","
        << "\"totalFee\":" << totalFee << ","
        << "\"totalPaid\":" << totalPaid << ","
        << "\"pendingBalance\":" << pendingBalance
        << "}";
    return oss.str();
}

FeeManager::FeeManager(std::string filePath, double defaultTotal)
    : filePath(std::move(filePath)), defaultTotalFee(defaultTotal) {
    loadFromFile();
}

void FeeManager::loadFromFile() {
    std::lock_guard<std::mutex> lock(feeMutex);
    payments.clear();

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
        if (firstLine && line.find("rollNo") != std::string::npos) {
            firstLine = false;
            continue;
        }
        firstLine = false;

        std::istringstream ss(line);
        std::string roll, amtStr, date;
        if (std::getline(ss, roll, ',') &&
            std::getline(ss, amtStr, ',') &&
            std::getline(ss, date, ',')) {
            try {
                double amt = std::stod(amtStr);
                payments.push_back({roll, amt, date});
            } catch (...) {}
        }
    }
    inFile.close();

    if (payments.empty()) {
        seedInitialData();
    }
}

void FeeManager::saveToFile() {
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) return;

    outFile << "rollNo,amount,date\n";
    for (const auto& p : payments) {
        outFile << p.rollNo << "," << p.amount << "," << p.date << "\n";
    }
    outFile.close();
}

void FeeManager::seedInitialData() {
    // Aditya University B.Tech Semester Tuition & Lab Fee (Standard: 55,000)
    payments = {
        {"25B11AI241", 55000.0, "10-08-2026"},
        {"25B11AI102", 55000.0, "12-08-2026"},
        {"25B11AI581", 35000.0, "15-08-2026"},
        {"25B11AI361", 30000.0, "18-08-2026"},
        {"25B11AI765", 25000.0, "20-08-2026"},
        {"25B11AI101", 20000.0, "22-08-2026"},
        {"25B11AI130", 15000.0, "25-08-2026"}
    };
    saveToFile();
}

bool FeeManager::recordFeePayment(const std::string& rollNo, double amount, const std::string& date) {
    if (amount <= 0.0) return false;
    std::lock_guard<std::mutex> lock(feeMutex);
    payments.push_back({rollNo, amount, date});
    saveToFile();
    return true;
}

double FeeManager::getTotalPaid(const std::string& rollNo) {
    std::lock_guard<std::mutex> lock(feeMutex);
    double sum = 0.0;
    for (const auto& p : payments) {
        if (p.rollNo == rollNo) {
            sum += p.amount;
        }
    }
    return sum;
}

double FeeManager::getPendingFee(const std::string& rollNo) {
    std::lock_guard<std::mutex> lock(feeMutex);
    double total = defaultTotalFee;
    auto it = customTotalFees.find(rollNo);
    if (it != customTotalFees.end()) {
        total = it->second;
    }

    double paid = 0.0;
    for (const auto& p : payments) {
        if (p.rollNo == rollNo) {
            paid += p.amount;
        }
    }

    double pending = total - paid;
    return (pending < 0.0) ? 0.0 : pending;
}

std::string FeeManager::generateFeeReceipt(const std::string& rollNo, double amount, const std::string& date, const std::string& studentName) {
    double pending = getPendingFee(rollNo);
    std::ostringstream oss;
    oss << "\n======================================================\n";
    oss << "        ADITYA UNIVERSITY - COLLEGE OF ENGINEERING    \n";
    oss << "           OFFICIAL SEMESTER TUITION RECEIPT          \n";
    oss << "======================================================\n";
    oss << "Receipt No      : ADY-AIML-" << rollNo << "-2026\n";
    oss << "Date of Payment : " << date << "\n";
    oss << "Registration No : " << rollNo << "\n";
    if (!studentName.empty()) {
        oss << "Student Name    : " << studentName << "\n";
    }
    oss << "Program / Branch: B.Tech (Artificial Intelligence & ML)\n";
    oss << "Semester        : Semester 2\n";
    oss << "------------------------------------------------------\n";
    oss << "Standard Fee    : \u20B955,000\n";
    oss << "Amount Paid     : \u20B9" << static_cast<long long>(amount) << "\n";
    oss << "Pending Balance : \u20B9" << static_cast<long long>(pending) << "\n";
    oss << "Payment Status  : " << (pending <= 0.0 ? "PAID IN FULL (NO DUES)" : "PARTIAL PAYMENT") << "\n";
    oss << "Authorized By   : Accounts & Finance Dept, Aditya University\n";
    oss << "======================================================\n\n";
    return oss.str();
}

std::vector<FeePayment> FeeManager::getFeeHistory(const std::string& rollNo) {
    std::lock_guard<std::mutex> lock(feeMutex);
    std::vector<FeePayment> history;
    for (const auto& p : payments) {
        if (p.rollNo == rollNo) {
            history.push_back(p);
        }
    }
    return history;
}

FeeStatus FeeManager::getFeeStatus(const std::string& rollNo, const std::string& studentName) {
    std::lock_guard<std::mutex> lock(feeMutex);
    FeeStatus status;
    status.rollNo = rollNo;
    status.studentName = studentName;
    status.totalFee = defaultTotalFee;
    auto it = customTotalFees.find(rollNo);
    if (it != customTotalFees.end()) {
        status.totalFee = it->second;
    }

    status.totalPaid = 0.0;
    for (const auto& p : payments) {
        if (p.rollNo == rollNo) {
            status.totalPaid += p.amount;
        }
    }
    status.pendingBalance = (status.totalFee - status.totalPaid < 0.0) ? 0.0 : (status.totalFee - status.totalPaid);
    return status;
}

std::vector<FeeStatus> FeeManager::getAllFeeStatuses() {
    std::lock_guard<std::mutex> lock(feeMutex);
    std::set<std::string> rolls;
    for (const auto& p : payments) {
        rolls.insert(p.rollNo);
    }

    std::vector<FeeStatus> statuses;
    for (const auto& roll : rolls) {
        FeeStatus status;
        status.rollNo = roll;
        status.totalFee = defaultTotalFee;
        auto it = customTotalFees.find(roll);
        if (it != customTotalFees.end()) {
            status.totalFee = it->second;
        }

        status.totalPaid = 0.0;
        for (const auto& p : payments) {
            if (p.rollNo == roll) {
                status.totalPaid += p.amount;
            }
        }
        status.pendingBalance = (status.totalFee - status.totalPaid < 0.0) ? 0.0 : (status.totalFee - status.totalPaid);
        statuses.push_back(status);
    }
    return statuses;
}

void FeeManager::setTotalFee(const std::string& rollNo, double total) {
    std::lock_guard<std::mutex> lock(feeMutex);
    customTotalFees[rollNo] = total;
}

} // namespace Service

