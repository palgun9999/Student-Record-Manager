#ifndef ISTUDENT_REPOSITORY_HPP
#define ISTUDENT_REPOSITORY_HPP

#include "models/Student.hpp"
#include <vector>
#include <string>

namespace Repository {

class IStudentRepository {
public:
    virtual ~IStudentRepository() = default;

    virtual std::vector<Models::Student> getAll() = 0;
    virtual bool getByRollNo(const std::string& rollNo, Models::Student& outStudent) = 0;
    virtual bool add(Models::Student& student) = 0;
    virtual bool update(const Models::Student& student) = 0;
    virtual bool remove(const std::string& rollNo) = 0;
};

} // namespace Repository

#endif // ISTUDENT_REPOSITORY_HPP
