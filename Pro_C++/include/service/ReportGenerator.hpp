#ifndef REPORT_GENERATOR_HPP
#define REPORT_GENERATOR_HPP

#include "models/Student.hpp"
#include "models/AcademicRecord.hpp"
#include "service/AttendanceManager.hpp"
#include "service/FeeManager.hpp"
#include "repository/IStudentRepository.hpp"
#include <string>
#include <vector>
#include <memory>

namespace Service {

struct ClassReportRow {
    std::string rollNo;
    std::string name;
    double percentage;
    double attendancePct;
    std::string grade;

    std::string toJson() const;
};

class ReportGenerator {
private:
    std::shared_ptr<Repository::IStudentRepository> studentRepo;
    std::shared_ptr<AttendanceManager> attendanceMgr;
    std::shared_ptr<FeeManager> feeMgr;
    std::vector<Models::AcademicRecord> academicRecords;

    void loadAcademicsFromFile();
    void saveAcademicsToFile();
    void seedInitialAcademics();

public:
    ReportGenerator(std::shared_ptr<Repository::IStudentRepository> sRepo,
                    std::shared_ptr<AttendanceManager> aMgr,
                    std::shared_ptr<FeeManager> fMgr);

    // Academic Record Management
    void addOrUpdateAcademicRecord(const Models::AcademicRecord& rec);
    bool getAcademicRecord(const std::string& rollNo, Models::AcademicRecord& outRec);
    std::vector<Models::AcademicRecord> getAllAcademicRecords();

    // PPT Required Methods
    std::string generateStudentReport(const std::string& rollNo);
    std::string generateClassReport(const std::string& className, std::string& outFilename);
    bool exportToFile(const std::string& filename, const std::string& content);
    std::vector<ClassReportRow> printTopperList(const std::string& className = "");

    // JSON export for Web Dashboard
    std::vector<ClassReportRow> getClassReportRows(const std::string& className = "");
};

} // namespace Service

#endif // REPORT_GENERATOR_HPP

