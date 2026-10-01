#include "cli/CliController.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

namespace CLI {

CliController::CliController(std::shared_ptr<Service::StudentService> sService,
                             std::shared_ptr<Service::AttendanceManager> aMgr,
                             std::shared_ptr<Service::FeeManager> fMgr,
                             std::shared_ptr<Service::UserManager> uMgr,
                             std::shared_ptr<Service::ReportGenerator> rGen,
                             std::shared_ptr<Http::HttpServer> server)
    : studentService(std::move(sService)),
      attendanceMgr(std::move(aMgr)),
      feeMgr(std::move(fMgr)),
      userMgr(std::move(uMgr)),
      reportGen(std::move(rGen)),
      server(std::move(server)),
      running(true) {}

void CliController::printBanner() const {
    std::cout << "\n======================================================================\n";
    std::cout << "                 ADITYA UNIVERSITY - AIML DEPARTMENT                  \n";
    std::cout << "                      STUDENT RECORD MANAGER                          \n";
    std::cout << "======================================================================\n";
    std::cout << " Presented by: 25B11AI241 (D.R.PALGUN), 25B11AI581 (K.BALA ADITYA)   \n";
    std::cout << "               25B11AI361 (G.DILEEP), 25B11AI765 (M.MOHITH NAIK)     \n";
    std::cout << " Course Guide: K.SYAMALA KALYANI, M.TECH, ASST PROFESSOR (AIML)       \n";
    std::cout << "----------------------------------------------------------------------\n";
    std::cout << " Web Portal  : http://" << server->getHost() << ":" << server->getPort() << "\n";
    std::cout << " (Type 'open' at any prompt to launch web interface in your browser) \n";
    std::cout << "======================================================================\n\n";
}

void CliController::handleLogin() {
    std::cout << "--- Admin Login ---\n";
    std::string username, password;
    std::cout << "Username: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    if (userMgr->login(username, password)) {
        std::cout << "Login successful. Welcome, " << username << "!\n";
        std::cout << "Access Level: " << userMgr->checkAccessLevel() << "\n\n";
    } else {
        std::cout << "[-] Invalid username or password.\n\n";
    }
}

void CliController::showMainMenu() const {
    std::cout << "\nMenu:\n";
    std::cout << "1. Add Student\n";
    std::cout << "2. View Student\n";
    std::cout << "3. Update Record\n";
    std::cout << "4. Delete Record\n";
    std::cout << "5. Attendance\n";
    std::cout << "6. Fees\n";
    std::cout << "7. Reports\n";
    std::cout << "8. Exit\n";
    std::cout << "Enter choice: ";
}

void CliController::handleAddStudent() {
    std::cout << "\nInput:\n";
    std::string roll, name, dob, contact, addr, cls;
    std::cout << "Registration / Roll No (e.g. 25B11AI241): ";
    std::getline(std::cin, roll);
    std::cout << "Name: ";
    std::getline(std::cin, name);
    std::cout << "DOB (DD-MM-YYYY): ";
    std::getline(std::cin, dob);
    std::cout << "Contact: ";
    std::getline(std::cin, contact);
    std::cout << "Address: ";
    std::getline(std::cin, addr);
    std::cout << "Program & Semester (default: B.Tech AIML - Sem 2): ";
    std::getline(std::cin, cls);
    if (cls.empty()) cls = "B.Tech AIML - Sem 2";

    Models::Student student(roll, name, dob, contact, addr, cls);
    std::string error;
    if (studentService->addStudent(student, error)) {
        std::cout << "\nOutput:\n";
        std::cout << "--- Student Added Successfully ---\n";
        std::cout << "Roll No : " << student.getRollNo() << "\n";
        std::cout << "Name    : " << student.getName() << "\n";
        std::cout << "DOB     : " << student.getDOB() << "\n";
        std::cout << "Contact : " << student.getContact() << "\n";
        std::cout << "Address : " << student.getAddress() << "\n";
        std::cout << "Program : " << student.getClassName() << "\n";
    } else {
        std::cout << "[-] Failed to add student: " << error << "\n";
    }
}

void CliController::handleViewStudent() {
    std::string roll;
    std::cout << "Enter Roll No to view (or 'all' to list all): ";
    std::getline(std::cin, roll);

    if (roll == "all" || roll.empty()) {
        auto all = studentService->getAllStudents();
        std::cout << "\n+---------+----------------------+------------+------------+---------------------------+\n";
        std::cout << "| Roll No | Name                 | DOB        | Contact    | Address                   |\n";
        std::cout << "+---------+----------------------+------------+------------+---------------------------+\n";
        for (const auto& s : all) {
            std::cout << "| " << std::left << std::setw(8) << s.getRollNo()
                      << "| " << std::left << std::setw(21) << (s.getName().length() > 20 ? s.getName().substr(0,17)+"..." : s.getName())
                      << "| " << std::left << std::setw(11) << s.getDOB()
                      << "| " << std::left << std::setw(11) << s.getContact()
                      << "| " << std::left << std::setw(26) << (s.getAddress().length() > 25 ? s.getAddress().substr(0,22)+"..." : s.getAddress())
                      << "|\n";
        }
        std::cout << "+---------+----------------------+------------+------------+---------------------------+\n";
        std::cout << "Total Students: " << all.size() << "\n";
    } else {
        Models::Student s;
        if (studentService->getStudentByRollNo(roll, s)) {
            std::cout << "\nOutput:\n";
            std::cout << "Roll No: " << s.getRollNo() << "\n";
            std::cout << "Name: " << s.getName() << "\n";
            std::cout << "DOB: " << s.getDOB() << "\n";
            std::cout << "Contact: " << s.getContact() << "\n";
            std::cout << "Address: " << s.getAddress() << "\n";
            std::cout << "Class: " << s.getClassName() << "\n";
        } else {
            std::cout << "[-] Student with Roll No '" << roll << "' not found.\n";
        }
    }
    std::cout << "Returning to Main Menu...\n";
}

void CliController::handleUpdateRecord() {
    std::string roll;
    std::cout << "Enter Roll No to update: ";
    std::getline(std::cin, roll);

    Models::Student s;
    if (!studentService->getStudentByRollNo(roll, s)) {
        std::cout << "[-] Student not found.\n";
        return;
    }

    std::cout << "\nUpdating Student: " << s.getName() << " (Press Enter to keep current value)\n";
    std::string input;
    std::cout << "Name [" << s.getName() << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) s.setName(input);

    std::cout << "DOB [" << s.getDOB() << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) s.setDOB(input);

    std::cout << "Contact [" << s.getContact() << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) s.setContact(input);

    std::cout << "Address [" << s.getAddress() << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) s.setAddress(input);

    std::cout << "Class [" << s.getClassName() << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) s.setClassName(input);

    std::string error;
    if (studentService->updateStudent(s, error)) {
        std::cout << "[+] Student record updated successfully!\n";
    } else {
        std::cout << "[-] Failed to update: " << error << "\n";
    }
}

void CliController::handleDeleteRecord() {
    std::string roll;
    std::cout << "Enter Roll No to delete: ";
    std::getline(std::cin, roll);

    Models::Student s;
    if (!studentService->getStudentByRollNo(roll, s)) {
        std::cout << "[-] Student not found.\n";
        return;
    }

    std::cout << "Are you sure you want to delete Roll No: " << roll << " (" << s.getName() << ")? (y/N): ";
    std::string confirm;
    std::getline(std::cin, confirm);
    if (confirm == "y" || confirm == "Y") {
        if (studentService->deleteStudent(roll)) {
            std::cout << "[+] Student deleted successfully.\n";
        } else {
            std::cout << "[-] Failed to delete record.\n";
        }
    } else {
        std::cout << "Deletion cancelled.\n";
    }
}

void CliController::handleAttendanceMenu() {
    std::cout << "\n--- Attendance Manager Module ---\n";
    std::cout << "1. Mark Attendance\n";
    std::cout << "2. View Attendance Summary & Record\n";
    std::cout << "3. Generate Low Attendance List (<75%)\n";
    std::cout << "4. Return to Main Menu\n";
    std::cout << "Choice: ";

    std::string chStr;
    std::getline(std::cin, chStr);
    int ch = 0;
    try { ch = std::stoi(chStr); } catch (...) {}

    if (ch == 1) {
        std::cout << "\nInput:\n";
        std::string roll, date, status;
        std::cout << "Roll No: ";
        std::getline(std::cin, roll);
        std::cout << "Date (DD-MM-YYYY): ";
        std::getline(std::cin, date);
        std::cout << "Status (Present / Absent): ";
        std::getline(std::cin, status);

        if (status.empty()) status = "Present";
        attendanceMgr->markAttendance(roll, date, status);

        auto sum = attendanceMgr->getSummary(roll);
        std::cout << "\nOutput:\n";
        std::cout << "Attendance marked: " << status << " (" << date << ")\n";
        std::cout << "--- Attendance Summary ---\n";
        std::cout << "Roll No: " << roll << " | Total Days: " << sum.totalDays << " | Present: " << sum.presentDays << "\n";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Attendance %: " << sum.percentage << "%\n";
    } else if (ch == 2) {
        std::string roll;
        std::cout << "Enter Roll No: ";
        std::getline(std::cin, roll);
        auto sum = attendanceMgr->getSummary(roll);
        std::cout << "\n--- Attendance Summary ---\n";
        std::cout << "Roll No: " << roll << " | Total Days: " << sum.totalDays << " | Present: " << sum.presentDays << "\n";
        std::cout << std::fixed << std::setprecision(2) << "Attendance %: " << sum.percentage << "%\n";
        if (sum.percentage < 75.0 && sum.totalDays > 0) {
            std::cout << "[!] WARNING: Attendance is below 75% threshold!\n";
        }
    } else if (ch == 3) {
        auto lowList = attendanceMgr->generateLowAttendanceList(75.0);
        std::cout << "\n--- Low Attendance List (<75%) ---\n";
        if (lowList.empty()) {
            std::cout << "All students have satisfactory attendance (>= 75%).\n";
        } else {
            for (const auto& l : lowList) {
                Models::Student s;
                std::string name = studentService->getStudentByRollNo(l.rollNo, s) ? s.getName() : "N/A";
                std::cout << "Roll: " << std::left << std::setw(8) << l.rollNo
                          << " | Name: " << std::left << std::setw(18) << name
                          << " | Attendance: " << std::fixed << std::setprecision(2) << l.percentage << "%\n";
            }
        }
    }
}

void CliController::handleFeesMenu() {
    std::cout << "\n--- Fee Manager Module ---\n";
    std::cout << "1. Record Fee Payment\n";
    std::cout << "2. View Pending Fee & Status\n";
    std::cout << "3. View Payment History\n";
    std::cout << "4. Return to Main Menu\n";
    std::cout << "Choice: ";

    std::string chStr;
    std::getline(std::cin, chStr);
    int ch = 0;
    try { ch = std::stoi(chStr); } catch (...) {}

    if (ch == 1) {
        std::cout << "\nInput:\n";
        std::string roll, date;
        double amount = 0.0;
        std::cout << "Roll No: ";
        std::getline(std::cin, roll);
        std::cout << "Amount Paid (\u20B9): ";
        std::string amtStr;
        std::getline(std::cin, amtStr);
        try { amount = std::stod(amtStr); } catch (...) {}
        std::cout << "Date (DD-MM-YYYY): ";
        std::getline(std::cin, date);

        feeMgr->recordFeePayment(roll, amount, date);

        Models::Student s;
        std::string name = studentService->getStudentByRollNo(roll, s) ? s.getName() : "";
        std::string receipt = feeMgr->generateFeeReceipt(roll, amount, date, name);
        std::cout << "\nOutput:" << receipt;
    } else if (ch == 2) {
        std::string roll;
        std::cout << "Enter Roll No: ";
        std::getline(std::cin, roll);
        Models::Student s;
        std::string name = studentService->getStudentByRollNo(roll, s) ? s.getName() : "";
        auto status = feeMgr->getFeeStatus(roll, name);
        std::cout << "\n--- Fee Status for Roll #" << roll << " (" << name << ") ---\n";
        std::cout << "Total Fee       : \u20B9" << static_cast<long long>(status.totalFee) << "\n";
        std::cout << "Total Paid      : \u20B9" << static_cast<long long>(status.totalPaid) << "\n";
        std::cout << "Pending Balance : \u20B9" << static_cast<long long>(status.pendingBalance) << "\n";
    } else if (ch == 3) {
        std::string roll;
        std::cout << "Enter Roll No: ";
        std::getline(std::cin, roll);
        auto hist = feeMgr->getFeeHistory(roll);
        std::cout << "\n--- Payment History for Roll #" << roll << " ---\n";
        if (hist.empty()) {
            std::cout << "No payment records found.\n";
        } else {
            for (const auto& h : hist) {
                std::cout << "Date: " << h.date << " | Amount: \u20B9" << static_cast<long long>(h.amount) << "\n";
            }
        }
    }
}

void CliController::handleReportsMenu() {
    std::cout << "\n--- Report Generator Module ---\n";
    std::cout << "1. Generate Individual Student Dossier\n";
    std::cout << "2. Generate Class Report (Export to CSV)\n";
    std::cout << "3. View Academic Result (Subject Marks & CGPA)\n";
    std::cout << "4. Add / Update Subject Marks\n";
    std::cout << "5. Print Topper List\n";
    std::cout << "6. Return to Main Menu\n";
    std::cout << "Choice: ";

    std::string chStr;
    std::getline(std::cin, chStr);
    int ch = 0;
    try { ch = std::stoi(chStr); } catch (...) {}

    if (ch == 1) {
        std::string roll;
        std::cout << "Enter Roll No: ";
        std::getline(std::cin, roll);
        std::string rep = reportGen->generateStudentReport(roll);
        std::cout << rep;
    } else if (ch == 2) {
        std::cout << "\nInput:\n";
        std::string cls;
        std::cout << "Generate Report for Program/Semester (default: B.Tech AIML - Sem 2): ";
        std::getline(std::cin, cls);
        if (cls.empty()) cls = "B.Tech AIML - Sem 2";

        std::string filename;
        std::cout << "\nOutput:\n";
        std::string out = reportGen->generateClassReport(cls, filename);
        std::cout << out;
    } else if (ch == 3) {
        std::string roll;
        std::cout << "Enter Registration / Roll No: ";
        std::getline(std::cin, roll);
        Models::AcademicRecord aRec;
        if (reportGen->getAcademicRecord(roll, aRec)) {
            aRec.displayResult();
        } else {
            std::cout << "[-] No academic marks found for Roll No " << roll << ".\n";
        }
    } else if (ch == 4) {
        std::cout << "\nInput:\n";
        std::string roll;
        std::cout << "Registration / Roll No: ";
        std::getline(std::cin, roll);
        Models::Student s;
        std::string sName = studentService->getStudentByRollNo(roll, s) ? s.getName() : "";

        Models::AcademicRecord aRec(roll, sName);
        reportGen->getAcademicRecord(roll, aRec);
        aRec.setStudentName(sName);

        std::string sub, marksStr;
        std::cout << "Course 1 (e.g. OOP_CPP): ";
        std::getline(std::cin, sub);
        std::cout << "Marks (0-100): ";
        std::getline(std::cin, marksStr);
        if (!sub.empty() && !marksStr.empty()) {
            aRec.addMarks(sub, std::stod(marksStr));
        }

        std::cout << "Course 2 (e.g. DSA): ";
        std::getline(std::cin, sub);
        std::cout << "Marks (0-100): ";
        std::getline(std::cin, marksStr);
        if (!sub.empty() && !marksStr.empty()) {
            aRec.addMarks(sub, std::stod(marksStr));
        }

        std::cout << "Course 3 (e.g. AIML_Foundations): ";
        std::getline(std::cin, sub);
        std::cout << "Marks (0-100): ";
        std::getline(std::cin, marksStr);
        if (!sub.empty() && !marksStr.empty()) {
            aRec.addMarks(sub, std::stod(marksStr));
        }

        reportGen->addOrUpdateAcademicRecord(aRec);
        std::cout << "\nOutput:\n";
        aRec.displayResult();
    } else if (ch == 5) {
        auto toppers = reportGen->printTopperList();
        std::cout << "\n====================================================================\n";
        std::cout << "         DEPARTMENT OF AIML - B.TECH SEMESTER MERIT RANKINGS        \n";
        std::cout << "====================================================================\n";
        int rank = 1;
        for (const auto& t : toppers) {
            std::cout << "#" << rank++ << " | Reg: " << std::left << std::setw(12) << t.rollNo
                      << " | " << std::left << std::setw(20) << t.name
                      << " | Percentage: " << std::fixed << std::setprecision(2) << t.percentage << "%"
                      << " | Grade: " << t.grade << "\n";
        }
        std::cout << "====================================================================\n";
    }
}

void CliController::handleOpenBrowser() const {
    std::string url = "http://" + server->getHost() + ":" + std::to_string(server->getPort());
    std::cout << "[*] Launching website in your default browser: " << url << "\n";
    std::string cmd = "cmd /c start " + url;
    std::system(cmd.c_str());
}

void CliController::handleUserChoice(int choice) {
    switch (choice) {
        case 1: handleAddStudent(); break;
        case 2: handleViewStudent(); break;
        case 3: handleUpdateRecord(); break;
        case 4: handleDeleteRecord(); break;
        case 5: handleAttendanceMenu(); break;
        case 6: handleFeesMenu(); break;
        case 7: handleReportsMenu(); break;
        case 8: exitSystem(); break;
        default:
            std::cout << "[!] Invalid choice. Please enter a number between 1 and 8.\n";
            break;
    }
}

void CliController::exitSystem() {
    std::cout << "\nSaving records and safely shutting down...\n";
    std::cout << "Thank You for using Aditya University Student Record Manager!\n";
    running = false;
}

void CliController::run() {
    printBanner();

    std::string input;
    while (running) {
        showMainMenu();
        if (!std::getline(std::cin, input)) {
            break;
        }

        // Trim input
        size_t start = input.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue;
        size_t end = input.find_last_not_of(" \t\r\n");
        input = input.substr(start, end - start + 1);

        if (input == "open" || input == "web") {
            handleOpenBrowser();
            continue;
        }
        if (input == "login" || input == "admin") {
            handleLogin();
            continue;
        }

        int choice = 0;
        try {
            choice = std::stoi(input);
        } catch (...) {
            choice = -1;
        }

        handleUserChoice(choice);
    }
}

} // namespace CLI
