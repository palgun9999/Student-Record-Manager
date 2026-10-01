#ifndef ACADEMIC_RECORD_HPP
#define ACADEMIC_RECORD_HPP

#include "models/Student.hpp"
#include <map>
#include <string>

namespace Models {

class AcademicRecord {
private:
    std::string rollNo;
    std::string studentName;
    std::map<std::string, double> subjectMarks; // Subject -> Marks (out of 100)

public:
    AcademicRecord();
    explicit AcademicRecord(std::string rollNo, std::string name = "");

    const std::string& getRollNo() const;
    const std::string& getStudentName() const;
    const std::map<std::string, double>& getSubjectMarks() const;

    void setStudentName(const std::string& name);

    // PPT Required Methods
    void addMarks(const std::string& subject, double marks);
    void updateMarks(const std::string& subject, double marks);
    double calculateTotal() const;
    double calculatePercentage() const;
    std::string calculateGrade() const;
    double calculateCGPA() const;
    void displayResult() const;

    // Serialization
    std::string toJson() const;
    std::string toFileFormat() const;
    static AcademicRecord fromFileFormat(const std::string& line);
};

} // namespace Models

#endif // ACADEMIC_RECORD_HPP

