#include "service/StudentService.hpp"
#include <algorithm>
#include <cctype>

namespace Service 
{

static std::string toLower(const std::string& str) 
{
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lower;
}

StudentService::StudentService(std::shared_ptr<Repository::IStudentRepository> repo)
    : repository(std::move(repo)) {}

bool StudentService::validateStudent(const Models::Student& student, std::string& errorMsg) const {
    if (student.getRollNo().empty()) 
    {
        errorMsg = "Roll Number cannot be empty.";
        return false;
    }
    if (student.getName().empty()) 
    {
        errorMsg = "Student Name cannot be empty.";
        return false;
    }
    return true;
}

std::vector<Models::Student> StudentService::getAllStudents() const 
{
    return repository->getAll();
}

bool StudentService::getStudentByRollNo(const std::string& rollNo, Models::Student& outStudent) const
{
    return repository->getByRollNo(rollNo, outStudent);
}

std::vector<Models::Student> StudentService::searchStudents(const std::string& query) const {
    auto all = repository->getAll();
    if (query.empty()) return all;

    std::string q = toLower(query);
    std::vector<Models::Student> results;
    for (const auto& s : all)
    {
        if (toLower(s.getName()).find(q) != std::string::npos ||
            toLower(s.getRollNo()).find(q) != std::string::npos ||
            toLower(s.getAddress()).find(q) != std::string::npos ||
            toLower(s.getContact()).find(q) != std::string::npos ||
            toLower(s.getClassName()).find(q) != std::string::npos) 
        {
            results.push_back(s);
        }
    }
    return results;
}

bool StudentService::addStudent(Models::Student& student, std::string& errorMsg) {
    if (!validateStudent(student, errorMsg))
    {
        return false;
    }

    Models::Student existing;
    if (repository->getByRollNo(student.getRollNo(), existing)) 
    {
        errorMsg = "Student with Roll Number '" + student.getRollNo() + "' already exists.";
        return false;
    }

    return repository->add(student);
}

bool StudentService::updateStudent(const Models::Student& student, std::string& errorMsg) {
    if (!validateStudent(student, errorMsg)) 
    {
        return false;
    }

    Models::Student existing;
    if (!repository->getByRollNo(student.getRollNo(), existing)) 
    {
        errorMsg = "Student with Roll Number '" + student.getRollNo() + "' was not found.";
        return false;
    }

    return repository->update(student);
}

bool StudentService::deleteStudent(const std::string& rollNo) 
{
    return repository->remove(rollNo);
}

} // namespace Service
