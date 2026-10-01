#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <iostream>

namespace Models {

class Student {
protected:
    std::string rollNo;
    std::string name;
    std::string dob;
    std::string contact;
    std::string address;
    std::string className; // e.g. "Class 10" or "AIML"

public:
    Student();
    Student(std::string rollNo, std::string name, std::string dob,
            std::string contact, std::string address, std::string className = "B.Tech AIML - Sem 2");
    virtual ~Student() = default;

    // Getters
    const std::string& getRollNo() const;
    const std::string& getName() const;
    const std::string& getDOB() const;
    const std::string& getContact() const;
    const std::string& getAddress() const;
    const std::string& getClassName() const;
    std::string getDetails() const;

    // Setters
    void setRollNo(const std::string& roll);
    void setName(const std::string& n);
    void setDOB(const std::string& d);
    void setContact(const std::string& c);
    void setAddress(const std::string& addr);
    void setClassName(const std::string& cls);
    void setDetails(const std::string& roll, const std::string& n,
                    const std::string& d, const std::string& c,
                    const std::string& addr, const std::string& cls = "B.Tech AIML - Sem 2");

    // Display
    virtual void displayStudent() const;

    // Serialization for File I/O and JSON
    virtual std::string toFileFormat() const;
    static Student fromFileFormat(const std::string& line);
    virtual std::string toJson() const;
    static Student fromJson(const std::string& jsonString);

    // Helpers
    static std::string escapeJson(const std::string& s);
    static std::string escapeCsv(const std::string& s);
};

} // namespace Models

#endif // STUDENT_HPP
