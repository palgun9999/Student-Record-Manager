#include "models/AcademicRecord.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

namespace Models {

AcademicRecord::AcademicRecord() : rollNo(""), studentName("") {}

AcademicRecord::AcademicRecord(std::string rollNo, std::string name)
    : rollNo(std::move(rollNo)), studentName(std::move(name)) {}

const std::string& AcademicRecord::getRollNo() const { return rollNo; }
const std::string& AcademicRecord::getStudentName() const { return studentName; }
const std::map<std::string, double>& AcademicRecord::getSubjectMarks() const { return subjectMarks; }

void AcademicRecord::setStudentName(const std::string& name) { studentName = name; }

void AcademicRecord::addMarks(const std::string& subject, double marks) {
    if (marks < 0.0) marks = 0.0;
    if (marks > 100.0) marks = 100.0;
    subjectMarks[subject] = marks;
}

void AcademicRecord::updateMarks(const std::string& subject, double marks) {
    addMarks(subject, marks);
}

double AcademicRecord::calculateTotal() const {
    double total = 0.0;
    for (const auto& pair : subjectMarks) {
        total += pair.second;
    }
    return total;
}

double AcademicRecord::calculatePercentage() const {
    if (subjectMarks.empty()) return 0.0;
    double total = calculateTotal();
    return total / (subjectMarks.size() * 100.0) * 100.0;
}

std::string AcademicRecord::calculateGrade() const {
    if (subjectMarks.empty()) return "N/A";
    double pct = calculatePercentage();
    if (pct >= 90.0) return "O";   // Outstanding (10)
    if (pct >= 80.0) return "A+";  // Excellent (9)
    if (pct >= 70.0) return "A";   // Very Good (8)
    if (pct >= 60.0) return "B+";  // Good (7)
    if (pct >= 50.0) return "B";   // Above Average (6)
    if (pct >= 40.0) return "C";   // Pass (5)
    return "F";                    // Fail / Backlog (0)
}

double AcademicRecord::calculateCGPA() const {
    if (subjectMarks.empty()) return 0.0;
    double totalGradePoints = 0.0;
    for (const auto& pair : subjectMarks) {
        double m = pair.second;
        if (m >= 90.0) totalGradePoints += 10.0;
        else if (m >= 80.0) totalGradePoints += 9.0;
        else if (m >= 70.0) totalGradePoints += 8.0;
        else if (m >= 60.0) totalGradePoints += 7.0;
        else if (m >= 50.0) totalGradePoints += 6.0;
        else if (m >= 40.0) totalGradePoints += 5.0;
        else totalGradePoints += 0.0;
    }
    return totalGradePoints / subjectMarks.size();
}

void AcademicRecord::displayResult() const {
    std::cout << "\n======================================================\n";
    std::cout << "         ADITYA UNIVERSITY - SEMESTER MARKSHEET       \n";
    std::cout << "         DEPARTMENT OF AIML - B.TECH PROGRAM          \n";
    std::cout << "======================================================\n";
    std::cout << "Roll No : " << rollNo << "\n";
    if (!studentName.empty()) {
        std::cout << "Student : " << studentName << "\n";
    }
    std::cout << "------------------------------------------------------\n";
    std::cout << "Course Title                       | Marks | Grade\n";
    std::cout << "-----------------------------------+-------+------\n";
    for (const auto& pair : subjectMarks) {
        std::string grade;
        double m = pair.second;
        if (m >= 90.0) grade = "O";
        else if (m >= 80.0) grade = "A+";
        else if (m >= 70.0) grade = "A";
        else if (m >= 60.0) grade = "B+";
        else if (m >= 50.0) grade = "B";
        else if (m >= 40.0) grade = "C";
        else grade = "F";

        std::cout << std::left << std::setw(35) << pair.first << "| "
                  << std::right << std::setw(5) << static_cast<int>(pair.second) << " | "
                  << std::left << std::setw(4) << grade << "\n";
    }
    std::cout << "------------------------------------------------------\n";
    std::cout << "Total Marks   : " << static_cast<int>(calculateTotal()) << " / "
              << (subjectMarks.size() * 100) << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Percentage    : " << calculatePercentage() << "%\n";
    std::cout << "Overall Grade : " << calculateGrade() << "\n";
    std::cout << "SGPA / CGPA   : " << calculateCGPA() << " / 10.00\n";
    std::cout << "Result Status : " << (calculateGrade() == "F" ? "FAILED / BACKLOG" : "PASSED") << "\n";
    std::cout << "======================================================\n\n";
}

std::string AcademicRecord::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "{"
        << "\"rollNo\":\"" << Student::escapeJson(rollNo) << "\","
        << "\"name\":\"" << Student::escapeJson(studentName) << "\","
        << "\"total\":" << calculateTotal() << ","
        << "\"maxMarks\":" << (subjectMarks.size() * 100) << ","
        << "\"percentage\":" << calculatePercentage() << ","
        << "\"grade\":\"" << calculateGrade() << "\","
        << "\"cgpa\":" << calculateCGPA() << ","
        << "\"subjects\":{";

    bool first = true;
    for (const auto& pair : subjectMarks) {
        if (!first) oss << ",";
        first = false;
        oss << "\"" << Student::escapeJson(pair.first) << "\":" << pair.second;
    }
    oss << "}}";
    return oss.str();
}

std::string AcademicRecord::toFileFormat() const {
    std::ostringstream oss;
    oss << Student::escapeCsv(rollNo) << ";" << Student::escapeCsv(studentName) << ";";
    bool first = true;
    for (const auto& pair : subjectMarks) {
        if (!first) oss << "|";
        first = false;
        oss << pair.first << "=" << pair.second;
    }
    return oss.str();
}

AcademicRecord AcademicRecord::fromFileFormat(const std::string& line) {
    AcademicRecord rec;
    size_t firstSemi = line.find(';');
    if (firstSemi == std::string::npos) return rec;

    rec.rollNo = line.substr(0, firstSemi);
    size_t secondSemi = line.find(';', firstSemi + 1);
    if (secondSemi == std::string::npos) return rec;

    rec.studentName = line.substr(firstSemi + 1, secondSemi - firstSemi - 1);
    std::string marksStr = line.substr(secondSemi + 1);

    std::istringstream mStream(marksStr);
    std::string token;
    while (std::getline(mStream, token, '|')) {
        size_t eq = token.find('=');
        if (eq != std::string::npos) {
            std::string sub = token.substr(0, eq);
            double marks = 0.0;
            try {
                marks = std::stod(token.substr(eq + 1));
            } catch (...) {}
            rec.addMarks(sub, marks);
        }
    }
    return rec;
}

} // namespace Models

