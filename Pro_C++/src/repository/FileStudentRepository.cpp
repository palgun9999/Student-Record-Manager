#include "repository/FileStudentRepository.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

namespace Repository 
{

FileStudentRepository::FileStudentRepository(std::string filePath)
    : filePath(std::move(filePath)) {
    loadFromFile();
}

void FileStudentRepository::loadFromFile() 
{
    std::lock_guard<std::mutex> lock(repoMutex);
    students.clear();

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
        if (firstLine && (line.find("rollNo") != std::string::npos || line.find("Roll") != std::string::npos)) {
            firstLine = false;
            continue;
        }
        firstLine = false;

        Models::Student s = Models::Student::fromFileFormat(line);
        if (!s.getRollNo().empty()) {
            students.push_back(s);
        }
    }
    inFile.close();

    if (students.empty()) {
        seedInitialData();
    }
}

void FileStudentRepository::saveToFile() {
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) return;

    outFile << "rollNo,name,dob,contact,address,className\n";
    for (const auto& s : students) {
        outFile << s.toFileFormat() << "\n";
    }
    outFile.close();
}

void FileStudentRepository::seedInitialData() {
    // PPT Slide 7 & 8 seed data
    students = {
        Models::Student("101", "Ravi Kumar", "12-05-2005", "9876543210", "Bobbili, AP", "Class 10"),
        Models::Student("102", "Priya Sharma", "15-08-2005", "9876543211", "Visakhapatnam, AP", "Class 10"),
        Models::Student("103", "Aditya Kumar", "22-01-2005", "9876543212", "Kakinada, AP", "Class 10"),
        Models::Student("104", "Sneha Reddy", "10-11-2005", "9876543213", "Rajahmundry, AP", "Class 10")
    };
    saveToFile();
}

std::vector<Models::Student> FileStudentRepository::getAll() {
    std::lock_guard<std::mutex> lock(repoMutex);
    return students;
}

bool FileStudentRepository::getByRollNo(const std::string& rollNo, Models::Student& outStudent) {
    std::lock_guard<std::mutex> lock(repoMutex);
    for (const auto& s : students) {
        if (s.getRollNo() == rollNo) {
            outStudent = s;
            return true;
        }
    }
    return false;
}

bool FileStudentRepository::add(Models::Student& student) {
    std::lock_guard<std::mutex> lock(repoMutex);
    for (const auto& s : students) {
        if (s.getRollNo() == student.getRollNo()) {
            return false; // Duplicate roll number
        }
    }
    students.push_back(student);
    saveToFile();
    return true;
}

bool FileStudentRepository::update(const Models::Student& student) {
    std::lock_guard<std::mutex> lock(repoMutex);
    for (auto& s : students) {
        if (s.getRollNo() == student.getRollNo()) {
            s = student;
            saveToFile();
            return true;
        }
    }
    return false;
}

bool FileStudentRepository::remove(const std::string& rollNo) {
    std::lock_guard<std::mutex> lock(repoMutex);
    auto it = std::remove_if(students.begin(), students.end(),
                             [&rollNo](const Models::Student& s) { return s.getRollNo() == rollNo; });
    if (it != students.end()) {
        students.erase(it, students.end());
        saveToFile();
        return true;
    }
    return false;
}

} // namespace Repository
