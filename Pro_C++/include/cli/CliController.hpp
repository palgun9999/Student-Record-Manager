#ifndef CLI_CONTROLLER_HPP
#define CLI_CONTROLLER_HPP

#include "service/StudentService.hpp"
#include "service/AttendanceManager.hpp"
#include "service/FeeManager.hpp"
#include "service/UserManager.hpp"
#include "service/ReportGenerator.hpp"
#include "http/HttpServer.hpp"
#include <memory>
#include <string>

namespace CLI {

class CliController {
private:
    std::shared_ptr<Service::StudentService> studentService;
    std::shared_ptr<Service::AttendanceManager> attendanceMgr;
    std::shared_ptr<Service::FeeManager> feeMgr;
    std::shared_ptr<Service::UserManager> userMgr;
    std::shared_ptr<Service::ReportGenerator> reportGen;
    std::shared_ptr<Http::HttpServer> server;
    bool running;

    void printBanner() const;
    void handleLogin();

    // PPT Required Menu Methods
    void showMainMenu() const;
    void showAdminMenu();
    void showStudentMenu();
    void handleUserChoice(int choice);
    void exitSystem();

    // PPT Module Handlers
    void handleAddStudent();
    void handleViewStudent();
    void handleUpdateRecord();
    void handleDeleteRecord();
    void handleAttendanceMenu();
    void handleFeesMenu();
    void handleReportsMenu();

    void handleOpenBrowser() const;

public:
    CliController(std::shared_ptr<Service::StudentService> sService,
                  std::shared_ptr<Service::AttendanceManager> aMgr,
                  std::shared_ptr<Service::FeeManager> fMgr,
                  std::shared_ptr<Service::UserManager> uMgr,
                  std::shared_ptr<Service::ReportGenerator> rGen,
                  std::shared_ptr<Http::HttpServer> server);

    void run();
};

} // namespace CLI

#endif // CLI_CONTROLLER_HPP

