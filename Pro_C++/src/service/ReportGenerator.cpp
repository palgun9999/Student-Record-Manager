#include "service/ReportGenerator.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <filesystem>

namespace Service {

std::string ClassReportRow::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "{"
        << "\"rollNo\":\"" << Models::Student::escapeJson(rollNo) << "\","
        << "\"name\":\"" << Models::Student::escapeJson(name) << "\","
        << "\"percentage\":" << percentage << ","
        << "\"attendancePct\":" << attendancePct << ","
        << "\"grade\":\"" << Models::Student::escapeJson(grade) << "\""
        << "}";
    return oss.str();
}

ReportGenerator::ReportGenerator(std::shared_ptr<Repository::IStudentRepository> sRepo,
                                 std::shared_ptr<AttendanceManager> aMgr,
                                 std::shared_ptr<FeeManager> fMgr)
    : studentRepo(std::move(sRepo)), attendanceMgr(std::move(aMgr)), feeMgr(std::move(fMgr)) {
    loadAcademicsFromFile();
}

void ReportGenerator::loadAcademicsFromFile() {
    std::string filePath = "data/academics.csv";
    academicRecords.clear();

    try {
        std::filesystem::path p(filePath);
        if (p.has_parent_path()) {
            std::filesystem::create_directories(p.parent_path());
        }
    } catch (...) {}

    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        seedInitialAcademics();
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        Models::AcademicRecord rec = Models::AcademicRecord::fromFileFormat(line);
        if (!rec.getRollNo().empty()) {
            academicRecords.push_back(rec);
        }
    }
    inFile.close();

    if (academicRecords.empty()) {
        seedInitialAcademics();
    }
}

void ReportGenerator::saveAcademicsToFile() {
    std::ofstream outFile("data/academics.csv");
    if (!outFile.is_open()) return;

    for (const auto& r : academicRecords) {
        outFile << r.toFileFormat() << "\n";
    }
    outFile.close();
}

void ReportGenerator::seedInitialAcademics() {
    // Aditya University - B.Tech AIML Semester 2 Engineering Courses:
    // OOP_CPP, DSA, AIML_Foundations, Discrete_Maths, Digital_Logic
    Models::AcademicRecord r1("25B11AI102", "Priya Sharma");
    r1.addMarks("OOP_CPP", 95.0);
    r1.addMarks("DSA", 92.0);
    r1.addMarks("AIML_Foundations", 96.0);
    r1.addMarks("Discrete_Maths", 90.0);
    r1.addMarks("Digital_Logic", 94.0);
    academicRecords.push_back(r1);

    Models::AcademicRecord r2("25B11AI241", "D.R. Palgun");
    r2.addMarks("OOP_CPP", 96.0);
    r2.addMarks("DSA", 90.0);
    r2.addMarks("AIML_Foundations", 92.0);
    r2.addMarks("Discrete_Maths", 88.0);
    r2.addMarks("Digital_Logic", 91.0);
    academicRecords.push_back(r2);

    Models::AcademicRecord r3("25B11AI581", "K. Bala Aditya");
    r3.addMarks("OOP_CPP", 88.0);
    r3.addMarks("DSA", 85.0);
    r3.addMarks("AIML_Foundations", 89.0);
    r3.addMarks("Discrete_Maths", 82.0);
    r3.addMarks("Digital_Logic", 86.0);
    academicRecords.push_back(r3);

    Models::AcademicRecord r4("25B11AI361", "G. Dileep");
    r4.addMarks("OOP_CPP", 84.0);
    r4.addMarks("DSA", 82.0);
    r4.addMarks("AIML_Foundations", 85.0);
    r4.addMarks("Discrete_Maths", 80.0);
    r4.addMarks("Digital_Logic", 83.0);
    academicRecords.push_back(r4);

    Models::AcademicRecord r5("25B11AI765", "M. Mohith Naik");
    r5.addMarks("OOP_CPP", 82.0);
    r5.addMarks("DSA", 80.0);
    r5.addMarks("AIML_Foundations", 84.0);
    r5.addMarks("Discrete_Maths", 78.0);
    r5.addMarks("Digital_Logic", 81.0);
    academicRecords.push_back(r5);

    saveAcademicsToFile();
}

void ReportGenerator::addOrUpdateAcademicRecord(const Models::AcademicRecord& rec) {
    for (auto& r : academicRecords) {
        if (r.getRollNo() == rec.getRollNo()) {
            r = rec;
            saveAcademicsToFile();
            return;
        }
    }
    academicRecords.push_back(rec);
    saveAcademicsToFile();
}

bool ReportGenerator::getAcademicRecord(const std::string& rollNo, Models::AcademicRecord& outRec) {
    for (const auto& r : academicRecords) {
        if (r.getRollNo() == rollNo) {
            outRec = r;
            return true;
        }
    }
    return false;
}

std::vector<Models::AcademicRecord> ReportGenerator::getAllAcademicRecords() {
    return academicRecords;
}

std::vector<ClassReportRow> ReportGenerator::getClassReportRows(const std::string& className) {
    auto students = studentRepo->getAll();
    std::vector<ClassReportRow> rows;

    for (const auto& s : students) {
        if (!className.empty() && className != "all") {
            std::string c = s.getClassName();
            bool match = (c == className) || (c.find(className) != std::string::npos);
            if (!match) continue;
        }

        ClassReportRow row;
        row.rollNo = s.getRollNo();
        row.name = s.getName();

        Models::AcademicRecord aRec;
        if (getAcademicRecord(s.getRollNo(), aRec)) {
            row.percentage = aRec.calculatePercentage();
            row.grade = aRec.calculateGrade();
        } else {
            row.percentage = 0.0;
            row.grade = "N/A";
        }

        row.attendancePct = attendanceMgr->getAttendancePercentage(s.getRollNo());
        rows.push_back(row);
    }
    return rows;
}

std::vector<ClassReportRow> ReportGenerator::printTopperList(const std::string& className) {
    auto rows = getClassReportRows(className);
    std::sort(rows.begin(), rows.end(), [](const ClassReportRow& a, const ClassReportRow& b) {
        return a.percentage > b.percentage;
    });
    return rows;
}

bool ReportGenerator::exportToFile(const std::string& filename, const std::string& content) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) return false;
    outFile << content;
    outFile.close();
    return true;
}

std::string ReportGenerator::generateStudentReport(const std::string& rollNo) {
    Models::Student student;
    if (!studentRepo->getByRollNo(rollNo, student)) {
        return "Student with Roll No " + rollNo + " not found.";
    }

    std::ostringstream oss;
    oss << "\n======================================================\n";
    oss << "        ADITYA UNIVERSITY - COLLEGE OF ENGINEERING    \n";
    oss << "     STUDENT ACADEMIC DOSSIER & SEMESTER PROFILE      \n";
    oss << "======================================================\n";
    oss << "Registration No : " << student.getRollNo() << "\n";
    oss << "Student Name    : " << student.getName() << "\n";
    oss << "Date of Birth   : " << student.getDOB() << "\n";
    oss << "Contact Phone   : " << student.getContact() << "\n";
    oss << "Address         : " << student.getAddress() << "\n";
    oss << "Program / Sem   : " << student.getClassName() << "\n";
    oss << "Department      : Artificial Intelligence & Machine Learning\n";
    oss << "------------------------------------------------------\n";

    // Academic Performance
    Models::AcademicRecord aRec;
    if (getAcademicRecord(rollNo, aRec)) {
        oss << "Semester Academic Performance:\n";
        for (const auto& p : aRec.getSubjectMarks()) {
            oss << "   - " << std::left << std::setw(22) << p.first << ": " << static_cast<int>(p.second) << " / 100\n";
        }
        oss << std::fixed << std::setprecision(2);
        oss << "Total Marks     : " << static_cast<int>(aRec.calculateTotal()) << " / " << (aRec.getSubjectMarks().size() * 100) << "\n";
        oss << "Percentage      : " << aRec.calculatePercentage() << "%\n";
        oss << "Letter Grade    : " << aRec.calculateGrade() << "\n";
        oss << "SGPA / CGPA     : " << aRec.calculateCGPA() << " / 10.00\n";
    } else {
        oss << "Semester Academic Performance: No marks recorded yet.\n";
    }
    oss << "------------------------------------------------------\n";

    // Attendance Performance (University Condonation & Detention rules)
    auto attSummary = attendanceMgr->getSummary(rollNo);
    oss << "Semester Attendance & Exam Clearance:\n";
    oss << "Total Lectures  : " << attSummary.totalDays << "\n";
    oss << "Lectures Present: " << attSummary.presentDays << "\n";
    oss << std::fixed << std::setprecision(2);
    oss << "Attendance %    : " << attSummary.percentage << "%\n";
    std::string attClearance;
    if (attSummary.totalDays == 0) attClearance = "N/A (No records)";
    else if (attSummary.percentage >= 75.0) attClearance = "ELIGIBLE (Normal Clearance)";
    else if (attSummary.percentage >= 65.0) attClearance = "CONDONATION REQUIRED (Medical/Fee approval)";
    else attClearance = "DETAINED (Attendance below 65% - not eligible for exams)";
    oss << "Exam Clearance  : " << attClearance << "\n";
    oss << "------------------------------------------------------\n";

    // Fee Status
    auto feeStatus = feeMgr->getFeeStatus(rollNo, student.getName());
    oss << "College Fee Ledger:\n";
    oss << "Standard Fee    : \u20B9" << static_cast<long long>(feeStatus.totalFee) << "\n";
    oss << "Paid Amount     : \u20B9" << static_cast<long long>(feeStatus.totalPaid) << "\n";
    oss << "Pending Balance : \u20B9" << static_cast<long long>(feeStatus.pendingBalance) << "\n";
    oss << "Status          : " << (feeStatus.pendingBalance <= 0.0 ? "ALL DUES CLEARED" : "FEES OUTSTANDING") << "\n";
    oss << "======================================================\n\n";

    return oss.str();
}

std::string ReportGenerator::generateClassReport(const std::string& className, std::string& outFilename) {
    auto rows = printTopperList(className);

    std::string displayName = className;
    if (displayName.empty() || displayName == "10" || displayName == "Class 10") {
        displayName = "B.Tech AIML - Semester 2";
    }

    outFilename = "btech_aiml_sem2_report.csv";

    // Build text report
    std::ostringstream oss;
    oss << "\n========================================================================================\n";
    oss << "         ADITYA UNIVERSITY - " << displayName << " ACADEMIC MERIT & CLEARANCE REPORT        \n";
    oss << "========================================================================================\n";
    oss << "Reg No       | Name                 | Percentage | Attendance | Grade | Clearance Status\n";
    oss << "-------------+----------------------+------------+------------+-------+-----------------\n";

    std::ostringstream csvContent;
    csvContent << "Rank,RollNo,Name,Percentage,Attendance,Grade,ClearanceStatus\n";

    std::string topperName = "N/A";
    double topPct = -1.0;
    int rank = 1;

    for (const auto& r : rows) {
        std::string n = r.name.length() > 20 ? r.name.substr(0, 17) + "..." : r.name;
        std::string clearance;
        if (r.attendancePct >= 75.0) clearance = "Eligible";
        else if (r.attendancePct >= 65.0) clearance = "Condonation";
        else clearance = "Detained (<65%)";

        oss << std::left << std::setw(13) << r.rollNo << "| "
            << std::left << std::setw(21) << n << "| "
            << std::fixed << std::setprecision(2) << std::setw(10) << (std::to_string(r.percentage).substr(0, 5) + "%") << "| "
            << std::fixed << std::setprecision(2) << std::setw(10) << (std::to_string(r.attendancePct).substr(0, 5) + "%") << "| "
            << std::left << std::setw(6) << r.grade << "| "
            << clearance << "\n";

        csvContent << rank++ << "," << r.rollNo << ",\"" << r.name << "\","
                   << r.percentage << "%," << r.attendancePct << "%,"
                   << r.grade << "," << clearance << "\n";

        if (r.percentage > topPct) {
            topPct = r.percentage;
            topperName = r.name;
        }
    }
    oss << "----------------------------------------------------------------------------------------\n";
    if (topPct >= 0) {
        oss << std::fixed << std::setprecision(2);
        oss << "\u2B50 Department Topper (Rank 1): " << topperName << " (" << topPct << "%)\n";
    }
    oss << "Report exported to: " << outFilename << "\n\n";

    // Export CSV files to disk
    exportToFile(outFilename, csvContent.str());
    exportToFile("class10_report.csv", csvContent.str()); // Keep for compatibility

    return oss.str();
}

} // namespace Service
