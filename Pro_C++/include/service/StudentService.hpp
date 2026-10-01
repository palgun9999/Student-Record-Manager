#ifndef STUDENT_SERVICE_HPP
#define STUDENT_SERVICE_HPP

#include "models/Student.hpp"
#include "repository/IStudentRepository.hpp"
#include <memory>
#include <vector>

namespace Service {

class StudentService {
private:
    std::shared_ptr<Repository::IStudentRepository> repository;

    bool validateStudent(const Models::Student& student, std::string& errorMsg) const;

public:
    explicit StudentService(std::shared_ptr<Repository::IStudentRepository> repo);

    std::vector<Models::Student> getAllStudents() const;
    bool getStudentByRollNo(const std::string& rollNo, Models::Student& outStudent) const;
    std::vector<Models::Student> searchStudents(const std::string& query) const;
    bool addStudent(Models::Student& student, std::string& errorMsg);
    bool updateStudent(const Models::Student& student, std::string& errorMsg);
    bool deleteStudent(const std::string& rollNo);
};

} // namespace Service

#endif // STUDENT_SERVICE_HPP
