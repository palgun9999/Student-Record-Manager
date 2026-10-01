#ifndef ATTENDANCE_MANAGER_HPP
#define ATTENDANCE_MANAGER_HPP

#include <string>
#include <vector>
#include <map>
#include <mutex>

namespace Service {

struct AttendanceEntry {
    std::string rollNo;
    std::string date;
    std::string status; // "Present" or "Absent"

    std::string toJson() const;
};

struct AttendanceSummary {
    std::string rollNo;
    int totalDays = 0;
    int presentDays = 0;
    double percentage = 0.0;

    std::string toJson() const;
};

class AttendanceManager {
private:
    std::string filePath;
    std::vector<AttendanceEntry> entries;
    std::mutex attMutex;

    void loadFromFile();
    void saveToFile();
    void seedInitialData();

public:
    explicit AttendanceManager(std::string filePath = "data/attendance.csv");

    // PPT Required Methods
    bool markAttendance(const std::string& rollNo, const std::string& date, const std::string& status);
    double getAttendancePercentage(const std::string& rollNo);
    std::vector<AttendanceEntry> viewAttendanceRecord(const std::string& rollNo);
    std::vector<AttendanceSummary> generateLowAttendanceList(double threshold = 75.0);

    // Helpers
    AttendanceSummary getSummary(const std::string& rollNo);
    std::vector<AttendanceSummary> getAllSummaries();
};

} // namespace Service

#endif // ATTENDANCE_MANAGER_HPP

