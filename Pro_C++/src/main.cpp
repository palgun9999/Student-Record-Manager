#include "models/Student.hpp"
#include "models/AcademicRecord.hpp"
#include "repository/FileStudentRepository.hpp"
#include "service/StudentService.hpp"
#include "service/AttendanceManager.hpp"
#include "service/FeeManager.hpp"
#include "service/UserManager.hpp"
#include "service/ReportGenerator.hpp"
#include "http/HttpServer.hpp"
#include "ui/WebAssets.hpp"
#include "cli/CliController.hpp"

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <cctype>

static std::string extractJsonString(const std::string& json, const std::string& key) {
    std::string needle = "\"" + key + "\"";
    size_t pos = json.find(needle);
    if (pos == std::string::npos) return "";

    pos += needle.length();
    while (pos < json.size() && (std::isspace(json[pos]) || json[pos] == ':')) {
        pos++;
    }
    if (pos >= json.size() || json[pos] != '"') return "";

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
}

static double extractJsonDouble(const std::string& json, const std::string& key, double defaultVal = 0.0) {
    std::string needle = "\"" + key + "\"";
    size_t pos = json.find(needle);
    if (pos == std::string::npos) return defaultVal;

    pos += needle.length();
    while (pos < json.size() && (std::isspace(json[pos]) || json[pos] == ':')) {
        pos++;
    }
    if (pos >= json.size()) return defaultVal;

    size_t end = pos;
    while (end < json.size() && json[end] != ',' && json[end] != '}' && !std::isspace(json[end])) {
        end++;
    }
    try {
        return std::stod(json.substr(pos, end - pos));
    } catch (...) {
        return defaultVal;
    }
}

int main(int argc, char* argv[]) {
    int port = 8080;
    bool autoOpen = false;
    bool serverOnly = false;

    // Parse CLI arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--open" || arg == "-o") {
            autoOpen = true;
        } else if (arg == "--server" || arg == "-s" || arg == "--daemon") {
            serverOnly = true;
        } else {
            try {
                int parsedPort = std::stoi(arg);
                if (parsedPort > 0 && parsedPort <= 65535) {
                    port = parsedPort;
                }
            } catch (...) {}
        }
    }

    // Initialize PPT Modules
    auto repository = std::make_shared<Repository::FileStudentRepository>("data/students.csv");
    auto studentService = std::make_shared<Service::StudentService>(repository);
    auto attendanceMgr = std::make_shared<Service::AttendanceManager>("data/attendance.csv");
    auto feeMgr = std::make_shared<Service::FeeManager>("data/fees.csv", 55000.0);
    auto userMgr = std::make_shared<Service::UserManager>("data/users.csv");
    auto reportGen = std::make_shared<Service::ReportGenerator>(repository, attendanceMgr, feeMgr);

    auto httpServer = std::make_shared<Http::HttpServer>(port, "127.0.0.1");
    auto& router = httpServer->getRouter();

    // 1. Web UI Root
    router.addRoute("GET", "/", [](const Http::HttpRequest&) {
        return Http::HttpResponse::html(UI::getIndexHtml());
    });

    // 2. Student Endpoints
    router.addRoute("GET", "/api/students", [studentService](const Http::HttpRequest& req) {
        std::string roll = req.getQueryParam("rollNo");
        if (!roll.empty()) {
            Models::Student s;
            if (studentService->getStudentByRollNo(roll, s)) {
                return Http::HttpResponse::json(s.toJson());
            }
            return Http::HttpResponse::error(404, "Student not found.");
        }

        std::string query = req.getQueryParam("query");
        auto list = query.empty() ? studentService->getAllStudents() : studentService->searchStudents(query);
        std::ostringstream ss;
        ss << "[";
        for (size_t i = 0; i < list.size(); ++i) {
            if (i > 0) ss << ",";
            ss << list[i].toJson();
        }
        ss << "]";
        return Http::HttpResponse::json(ss.str());
    });

    router.addRoute("POST", "/api/students", [studentService](const Http::HttpRequest& req) {
        if (req.body.empty()) return Http::HttpResponse::error(400, "Empty body.");
        Models::Student s = Models::Student::fromJson(req.body);
        std::string error;
        if (!studentService->addStudent(s, error)) {
            return Http::HttpResponse::error(400, error);
        }
        return Http::HttpResponse::json(s.toJson(), 201);
    });

    router.addRoute("PUT", "/api/students", [studentService](const Http::HttpRequest& req) {
        if (req.body.empty()) return Http::HttpResponse::error(400, "Empty body.");
        Models::Student s = Models::Student::fromJson(req.body);
        std::string error;
        if (!studentService->updateStudent(s, error)) {
            return Http::HttpResponse::error(400, error);
        }
        return Http::HttpResponse::json(s.toJson(), 200);
    });

    router.addRoute("DELETE", "/api/students", [studentService](const Http::HttpRequest& req) {
        std::string roll = req.getQueryParam("rollNo");
        if (roll.empty()) return Http::HttpResponse::error(400, "Missing rollNo parameter.");
        if (studentService->deleteStudent(roll)) {
            return Http::HttpResponse::json("{\"success\":true}");
        }
        return Http::HttpResponse::error(404, "Student not found.");
    });

    // 3. Academic Records Endpoints
    router.addRoute("GET", "/api/academics", [reportGen](const Http::HttpRequest&) {
        auto records = reportGen->getAllAcademicRecords();
        std::ostringstream ss;
        ss << "[";
        for (size_t i = 0; i < records.size(); ++i) {
            if (i > 0) ss << ",";
            ss << records[i].toJson();
        }
        ss << "]";
        return Http::HttpResponse::json(ss.str());
    });

    router.addRoute("POST", "/api/academics", [reportGen, studentService](const Http::HttpRequest& req) {
        // Simple body extraction for rollNo and subject marks
        std::string b = req.body;
        size_t rollPos = b.find("\"rollNo\":\"");
        if (rollPos == std::string::npos) return Http::HttpResponse::error(400, "Missing rollNo.");
        size_t rollEnd = b.find("\"", rollPos + 10);
        std::string rollNo = b.substr(rollPos + 10, rollEnd - (rollPos + 10));

        Models::Student s;
        std::string sName = studentService->getStudentByRollNo(rollNo, s) ? s.getName() : "";
        Models::AcademicRecord aRec(rollNo, sName);
        reportGen->getAcademicRecord(rollNo, aRec);
        aRec.setStudentName(sName);

        // Parse B.Tech courses as well as general subjects
        auto parseMark = [&](const std::string& key) {
            size_t p = b.find("\"" + key + "\":");
            if (p != std::string::npos) {
                size_t start = p + key.length() + 3;
                size_t end = b.find_first_of(",}", start);
                try {
                    return std::stod(b.substr(start, end - start));
                } catch (...) {}
            }
            return -1.0;
        };

        std::vector<std::string> courses = {
            "OOP_CPP", "DSA", "AIML_Foundations", "Discrete_Maths", "Digital_Logic",
            "Maths", "Science", "English"
        };
        for (const auto& c : courses) {
            double m = parseMark(c);
            if (m >= 0) aRec.addMarks(c, m);
        }

        reportGen->addOrUpdateAcademicRecord(aRec);
        return Http::HttpResponse::json(aRec.toJson(), 200);
    });

    // 4. Attendance Endpoints
    router.addRoute("GET", "/api/attendance", [attendanceMgr](const Http::HttpRequest&) {
        auto summaries = attendanceMgr->getAllSummaries();
        std::ostringstream ss;
        ss << "[";
        for (size_t i = 0; i < summaries.size(); ++i) {
            if (i > 0) ss << ",";
            ss << summaries[i].toJson();
        }
        ss << "]";
        return Http::HttpResponse::json(ss.str());
    });

    router.addRoute("POST", "/api/attendance", [attendanceMgr](const Http::HttpRequest& req) {
        std::string roll = extractJsonString(req.body, "rollNo");
        std::string date = extractJsonString(req.body, "date");
        std::string status = extractJsonString(req.body, "status");
        if (roll.empty() || date.empty()) return Http::HttpResponse::error(400, "Missing fields.");
        attendanceMgr->markAttendance(roll, date, status.empty() ? "Present" : status);
        return Http::HttpResponse::json(attendanceMgr->getSummary(roll).toJson(), 200);
    });

    // 5. Fees Endpoints
    router.addRoute("GET", "/api/fees", [feeMgr, studentService](const Http::HttpRequest&) {
        auto students = studentService->getAllStudents();
        std::ostringstream ss;
        ss << "[";
        for (size_t i = 0; i < students.size(); ++i) {
            if (i > 0) ss << ",";
            auto st = feeMgr->getFeeStatus(students[i].getRollNo(), students[i].getName());
            ss << st.toJson();
        }
        ss << "]";
        return Http::HttpResponse::json(ss.str());
    });

    router.addRoute("POST", "/api/fees", [feeMgr](const Http::HttpRequest& req) {
        std::string roll = extractJsonString(req.body, "rollNo");
        std::string date = extractJsonString(req.body, "date");
        double amount = extractJsonDouble(req.body, "amount");
        if (roll.empty() || amount <= 0.0) return Http::HttpResponse::error(400, "Invalid payment.");
        feeMgr->recordFeePayment(roll, amount, date.empty() ? "Today" : date);
        return Http::HttpResponse::json(feeMgr->getFeeStatus(roll).toJson(), 200);
    });

    // 6. Reports Endpoints
    router.addRoute("GET", "/api/reports", [reportGen](const Http::HttpRequest& req) {
        std::string cls = req.getQueryParam("class", "");
        auto rows = reportGen->printTopperList(cls);
        std::ostringstream ss;
        ss << "[";
        for (size_t i = 0; i < rows.size(); ++i) {
            if (i > 0) ss << ",";
            ss << rows[i].toJson();
        }
        ss << "]";
        return Http::HttpResponse::json(ss.str());
    });

    router.addRoute("GET", "/api/reports/student", [reportGen](const Http::HttpRequest& req) {
        std::string roll = req.getQueryParam("rollNo");
        if (roll.empty()) return Http::HttpResponse::error(400, "Missing rollNo.");
        return Http::HttpResponse::plain(reportGen->generateStudentReport(roll));
    });

    router.addRoute("GET", "/api/reports/sem2.csv", [reportGen](const Http::HttpRequest&) {
        std::string filename;
        reportGen->generateClassReport("B.Tech AIML - Sem 2", filename);
        std::ifstream file(filename);
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        Http::HttpResponse res(200, "OK", "text/csv; charset=utf-8", content);
        res.setHeader("Content-Disposition", "attachment; filename=\"btech_aiml_sem2_report.csv\"");
        return res;
    });

    router.addRoute("GET", "/api/reports/class10.csv", [reportGen](const Http::HttpRequest&) {
        std::string filename;
        reportGen->generateClassReport("B.Tech AIML - Sem 2", filename);
        std::ifstream file(filename);
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        Http::HttpResponse res(200, "OK", "text/csv; charset=utf-8", content);
        res.setHeader("Content-Disposition", "attachment; filename=\"btech_aiml_sem2_report.csv\"");
        return res;
    });

    // 7. Auth Endpoints
    router.addRoute("GET", "/api/auth/users", [userMgr](const Http::HttpRequest&) {
        auto users = userMgr->getAllUsers();
        std::ostringstream ss;
        ss << "[";
        for (size_t i = 0; i < users.size(); ++i) {
            if (i > 0) ss << ",";
            ss << users[i].toJson();
        }
        ss << "]";
        return Http::HttpResponse::json(ss.str());
    });

    router.addRoute("POST", "/api/auth/login", [userMgr](const Http::HttpRequest& req) {
        std::string u = extractJsonString(req.body, "username");
        std::string p = extractJsonString(req.body, "password");
        if (userMgr->login(u, p)) {
            return Http::HttpResponse::json("{\"success\":true,\"accessLevel\":\"" + userMgr->checkAccessLevel() + "\"}");
        }
        return Http::HttpResponse::error(401, "Invalid username or password.");
    });

    // Start background HTTP web server thread
    if (!httpServer->start()) {
        std::cerr << "Failed to start HTTP server on port " << port << ". Exiting.\n";
        return 1;
    }

    if (autoOpen) {
        std::string openCmd = "cmd /c start http://" + httpServer->getHost() + ":" + std::to_string(httpServer->getPort());
        std::system(openCmd.c_str());
    }

    if (serverOnly) {
        std::cout << "[*] Running in server background mode on port " << port << "...\n";
        while (httpServer->running()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } else {
        // Launch CMD CLI Interface
        CLI::CliController cli(studentService, attendanceMgr, feeMgr, userMgr, reportGen, httpServer);
        cli.run();
    }

    // Clean shutdown
    httpServer->stop();
    return 0;
}
