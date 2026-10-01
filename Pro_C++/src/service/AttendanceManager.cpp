#include "service/AttendanceManager.hpp"
#include "models/Student.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <set>

namespace Service {

std::string AttendanceEntry::toJson() const {
    std::ostringstream oss;
    oss << "{"
        << "\"rollNo\":\"" << Models::Student::escapeJson(rollNo) << "\","
        << "\"date\":\"" << Models::Student::escapeJson(date) << "\","
        << "\"status\":\"" << Models::Student::escapeJson(status) << "\""
        << "}";
    return oss.str();
}

std::string AttendanceSummary::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "{"
        << "\"rollNo\":\"" << Models::Student::escapeJson(rollNo) << "\","
        << "\"totalDays\":" << totalDays << ","
        << "\"presentDays\":" << presentDays << ","
        << "\"percentage\":" << percentage
        << "}";
    return oss.str();
}

AttendanceManager::AttendanceManager(std::string filePath)
    : filePath(std::move(filePath)) {
    loadFromFile();
}

void AttendanceManager::loadFromFile() {
    std::lock_guard<std::mutex> lock(attMutex);
    entries.clear();

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
        std::string roll, date, status;
        if (std::getline(ss, roll, ',') &&
            std::getline(ss, date, ',') &&
            std::getline(ss, status, ',')) {
            entries.push_back({roll, date, status});
        }
    }
    inFile.close();

    if (entries.empty()) {
        seedInitialData();
    }
}

void AttendanceManager::saveToFile() {
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) return;

    outFile << "rollNo,date,status\n";
    for (const auto& e : entries) {
        outFile << e.rollNo << "," << e.date << "," << e.status << "\n";
    }
    outFile.close();
}

void AttendanceManager::seedInitialData() {
    // Seed matching PPT Slide 7: Roll 101 has 50 days, 46 present (92%)
    // Slide 8: Roll 102 has 88% attendance
    // Roll 103 (low attendance student, 60%)
    entries.clear();

    // 101 (Ravi Kumar)
    for (int i = 1; i <= 50; ++i) {
        std::string date = (i < 10 ? "0" : "") + std::to_string(i) + "-07-2026";
        std::string st = (i <= 46) ? "Present" : "Absent";
        entries.push_back({"101", date, st});
    }

    // 102 (Priya Sharma)
    for (int i = 1; i <= 50; ++i) {
        std::string date = (i < 10 ? "0" : "") + std::to_string(i) + "-07-2026";
        std::string st = (i <= 44) ? "Present" : "Absent";
        entries.push_back({"102", date, st});
    }

    // 103 (Aditya Kumar - Low Attendance example)
    for (int i = 1; i <= 50; ++i) {
        std::string date = (i < 10 ? "0" : "") + std::to_string(i) + "-07-2026";
        std::string st = (i <= 30) ? "Present" : "Absent";
        entries.push_back({"103", date, st});
    }

    saveToFile();
}

bool AttendanceManager::markAttendance(const std::string& rollNo, const std::string& date, const std::string& status) {
    std::lock_guard<std::mutex> lock(attMutex);
    // If already marked for rollNo and date, update it
    for (auto& e : entries) {
        if (e.rollNo == rollNo && e.date == date) {
            e.status = status;
            saveToFile();
            return true;
        }
    }
    entries.push_back({rollNo, date, status});
    saveToFile();
    return true;
}

AttendanceSummary AttendanceManager::getSummary(const std::string& rollNo) {
    std::lock_guard<std::mutex> lock(attMutex);
    AttendanceSummary sum;
    sum.rollNo = rollNo;

    for (const auto& e : entries) {
        if (e.rollNo == rollNo) {
            sum.totalDays++;
            if (e.status == "Present" || e.status == "present" || e.status == "P") {
                sum.presentDays++;
            }
        }
    }

    if (sum.totalDays > 0) {
        sum.percentage = (static_cast<double>(sum.presentDays) / sum.totalDays) * 100.0;
    }
    return sum;
}

double AttendanceManager::getAttendancePercentage(const std::string& rollNo) {
    return getSummary(rollNo).percentage;
}

std::vector<AttendanceEntry> AttendanceManager::viewAttendanceRecord(const std::string& rollNo) {
    std::lock_guard<std::mutex> lock(attMutex);
    std::vector<AttendanceEntry> list;
    for (const auto& e : entries) {
        if (e.rollNo == rollNo) {
            list.push_back(e);
        }
    }
    return list;
}

std::vector<AttendanceSummary> AttendanceManager::getAllSummaries() {
    std::lock_guard<std::mutex> lock(attMutex);
    std::set<std::string> rolls;
    for (const auto& e : entries) {
        rolls.insert(e.rollNo);
    }

    std::vector<AttendanceSummary> summaries;
    for (const auto& roll : rolls) {
        AttendanceSummary sum;
        sum.rollNo = roll;
        for (const auto& e : entries) {
            if (e.rollNo == roll) {
                sum.totalDays++;
                if (e.status == "Present" || e.status == "present" || e.status == "P") {
                    sum.presentDays++;
                }
            }
        }
        if (sum.totalDays > 0) {
            sum.percentage = (static_cast<double>(sum.presentDays) / sum.totalDays) * 100.0;
        }
        summaries.push_back(sum);
    }
    return summaries;
}

std::vector<AttendanceSummary> AttendanceManager::generateLowAttendanceList(double threshold) {
    auto all = getAllSummaries();
    std::vector<AttendanceSummary> lowList;
    for (const auto& s : all) {
        if (s.percentage < threshold) {
            lowList.push_back(s);
        }
    }
    return lowList;
}

} // namespace Service

