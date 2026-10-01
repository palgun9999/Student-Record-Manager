#ifndef FILE_STUDENT_REPOSITORY_HPP
#define FILE_STUDENT_REPOSITORY_HPP

#include "repository/IStudentRepository.hpp"
#include <string>
#include <vector>
#include <mutex>

namespace Repository {

class FileStudentRepository : public IStudentRepository {
private:
    std::string filePath;
    std::vector<Models::Student> students;
    std::mutex repoMutex;

    void loadFromFile();
    void saveToFile();
    void seedInitialData();

public:
    explicit FileStudentRepository(std::string filePath = "data/students.csv");

    std::vector<Models::Student> getAll() override;
    bool getByRollNo(const std::string& rollNo, Models::Student& outStudent) override;
    bool add(Models::Student& student) override;
    bool update(const Models::Student& student) override;
    bool remove(const std::string& rollNo) override;
};

} // namespace Repository

#endif // FILE_STUDENT_REPOSITORY_HPP
