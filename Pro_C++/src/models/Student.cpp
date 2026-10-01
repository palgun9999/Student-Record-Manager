#include "models/Student.hpp"
#include <iomanip>
#include <sstream>
#include <vector>
#include <cctype>

namespace Models {

Student::Student()
    : rollNo(""), name(""), dob(""), contact(""), address(""), className("B.Tech AIML - Sem 2") {}

Student::Student(std::string rollNo, std::string name, std::string dob,
                 std::string contact, std::string address, std::string className)
    : rollNo(std::move(rollNo)),
      name(std::move(name)),
      dob(std::move(dob)),
      contact(std::move(contact)),
      address(std::move(address)),
      className(std::move(className)) {}

const std::string& Student::getRollNo() const { return rollNo; }
const std::string& Student::getName() const { return name; }
const std::string& Student::getDOB() const { return dob; }
const std::string& Student::getContact() const { return contact; }
const std::string& Student::getAddress() const { return address; }
const std::string& Student::getClassName() const { return className; }

std::string Student::getDetails() const {
    std::ostringstream oss;
    oss << "Roll No: " << rollNo << "\n"
        << "Name: " << name << "\n"
        << "DOB: " << dob << "\n"
        << "Contact: " << contact << "\n"
        << "Address: " << address << "\n"
        << "Class: " << className;
    return oss.str();
}

void Student::setRollNo(const std::string& roll) { rollNo = roll; }
void Student::setName(const std::string& n) { name = n; }
void Student::setDOB(const std::string& d) { dob = d; }
void Student::setContact(const std::string& c) { contact = c; }
void Student::setAddress(const std::string& addr) { address = addr; }
void Student::setClassName(const std::string& cls) { className = cls; }

void Student::setDetails(const std::string& roll, const std::string& n,
                         const std::string& d, const std::string& c,
                         const std::string& addr, const std::string& cls) {
    rollNo = roll;
    name = n;
    dob = d;
    contact = c;
    address = addr;
    className = cls;
}

void Student::displayStudent() const {
    std::cout << "Roll No: " << rollNo << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "DOB: " << dob << "\n";
    std::cout << "Contact: " << contact << "\n";
    std::cout << "Address: " << address << "\n";
    std::cout << "Class: " << className << "\n";
}

std::string Student::escapeJson(const std::string& s) {
    std::ostringstream o;
    for (char c : s) {
        switch (c) {
            case '"': o << "\\\""; break;
            case '\\': o << "\\\\"; break;
            case '\b': o << "\\b"; break;
            case '\f': o << "\\f"; break;
            case '\n': o << "\\n"; break;
            case '\r': o << "\\r"; break;
            case '\t': o << "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) <= 0x1f) {
                    o << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
                } else {
                    o << c;
                }
        }
    }
    return o.str();
}

std::string Student::escapeCsv(const std::string& s) {
    bool needsQuotes = false;
    for (char c : s) {
        if (c == ',' || c == '"' || c == '\n' || c == '\r') {
            needsQuotes = true;
            break;
        }
    }
    if (!needsQuotes) return s;

    std::string result = "\"";
    for (char c : s) {
        if (c == '"') result += "\"\"";
        else result += c;
    }
    result += "\"";
    return result;
}

std::string Student::toFileFormat() const {
    std::ostringstream oss;
    oss << escapeCsv(rollNo) << ","
        << escapeCsv(name) << ","
        << escapeCsv(dob) << ","
        << escapeCsv(contact) << ","
        << escapeCsv(address) << ","
        << escapeCsv(className);
    return oss.str();
}

Student Student::fromFileFormat(const std::string& line) {
    std::vector<std::string> tokens;
    std::string current;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                current += '"';
                ++i;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            tokens.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    tokens.push_back(current);

    Student s;
    if (tokens.size() >= 5) {
        s.rollNo = tokens[0];
        s.name = tokens[1];
        s.dob = tokens[2];
        s.contact = tokens[3];
        s.address = tokens[4];
        if (tokens.size() >= 6) {
            s.className = tokens[5];
        }
    }
    return s;
}

std::string Student::toJson() const {
    std::ostringstream oss;
    oss << "{"
        << "\"rollNo\":\"" << escapeJson(rollNo) << "\","
        << "\"name\":\"" << escapeJson(name) << "\","
        << "\"dob\":\"" << escapeJson(dob) << "\","
        << "\"contact\":\"" << escapeJson(contact) << "\","
        << "\"address\":\"" << escapeJson(address) << "\","
        << "\"className\":\"" << escapeJson(className) << "\""
        << "}";
    return oss.str();
}

static std::string extractJsonField(const std::string& json, const std::string& key) {
    std::string needle = "\"" + key + "\"";
    size_t pos = json.find(needle);
    if (pos == std::string::npos) return "";

    pos += needle.length();
    while (pos < json.size() && (std::isspace(json[pos]) || json[pos] == ':')) {
        pos++;
    }
    if (pos >= json.size()) return "";

    if (json[pos] == '"') {
        pos++;
        std::string val;
        while (pos < json.size()) {
            if (json[pos] == '\\' && pos + 1 < json.size()) {
                val += json[pos + 1];
                pos += 2;
            } else if (json[pos] == '"') {
                break;
            } else {
                val += json[pos++];
            }
        }
        return val;
    } else {
        size_t endPos = pos;
        while (endPos < json.size() && json[endPos] != ',' && json[endPos] != '}' && !std::isspace(json[endPos])) {
            endPos++;
        }
        return json.substr(pos, endPos - pos);
    }
}

Student Student::fromJson(const std::string& jsonString) {
    Student s;
    s.rollNo = extractJsonField(jsonString, "rollNo");
    s.name = extractJsonField(jsonString, "name");
    s.dob = extractJsonField(jsonString, "dob");
    s.contact = extractJsonField(jsonString, "contact");
    s.address = extractJsonField(jsonString, "address");
    std::string cls = extractJsonField(jsonString, "className");
    if (!cls.empty()) s.className = cls;
    return s;
}

} // namespace Models
