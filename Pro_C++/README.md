# Aditya University - Student Record Manager (OOP C++ Web & CMD System)

A comprehensive academic management system built in modern **Object-Oriented C++ (C++17)** with an embedded **Winsock2** HTTP web server and an interactive **CMD** console.

Designed and implemented in accordance with the project specification for **Aditya University (AIML Department)**.

---

## 👥 Project Team & Course Details

- **Institution**: Aditya University
- **Department**: Artificial Intelligence & Machine Learning (AIML)
- **Course Instructor**: K. Syamala Kalyani, M.Tech (Assistant Professor, AIML)
- **Project Team**:
  - `25B11AI241` — D.R. Palgun (Main C++ Program)
  - `25B11AI581` — K. Bala Aditya (Testing, Debugging & Improvement)
  - `25B11AI361` — G. Dileep (Project Design & Structure)
  - `25B11AI765` — M. Mohith Naik (Documentation, Flowchart & Presentation)

---

## 📦 Planned Modules (As per Presentation Slides 5–10)

| Module | Core Responsibilities & Methods |
| :--- | :--- |
| **1. Student (Base Class)** | `Set/Get Details`, `Roll No`, `Name`, `DOB`, `Contact`, `Address`, `Class`, `Display Student()`, `To/From File Format` |
| **2. Academic Record** | Inherits/has-a Student, `Add/Update Marks`, `Calculate Total`, `Calculate Percentage`, `Calculate Grade`, `Calculate CGPA`, `Display Result` |
| **3. Attendance Manager** | `Mark Attendance(rollNo, date, status)`, `Get Attendance Percentage`, `View Attendance Record`, `Generate Low Attendance List (<75%)` |
| **4. Fee Manager** | `Record Fee Payment(rollNo, amount, date)`, `Get Pending Fee`, `Generate Fee Receipt`, `Get Fee History` |
| **5. Admin / User** | `login(username, password)` (`admin`/`admin123`), `logout()`, `Add New Admin()`, `Change Password()`, `Check Access Level()` |
| **6. Report Generator** | `Generate Student Report(rollNo)`, `Generate Class Report(class)` (`class10_report.csv`), `Export To File`, `Print Topper List()` |
| **7. Menu / UI Controller** | Interactive 8-choice CMD Terminal + Multi-tab Web Portal with live synchronization |

---

## 🚀 How to Build and Run

### 1. Build via Command Prompt (CMD)
```cmd
build.bat
```

### 2. Run via Command Prompt (CMD)
```cmd
run.bat
```
*(Or `student_manager.exe --open` to automatically launch the web interface in your default browser)*

---

## 💻 Exact Inputs & Expected Outputs (Matching PPT Slides 7 & 8)

### 1. Student Record Addition (Slide 7)
**Input:**
```
Roll No: 101
Name: Ravi Kumar
DOB: 12-05-2005
Contact: 9876543210
Address: Bobbili, AP
```
**Output:**
```
--- Student Added Successfully ---
Roll No: 101
Name: Ravi Kumar
DOB: 12-05-2005
Contact: 9876543210
Address: Bobbili, AP
```

### 2. Academic Result (Slide 7)
**Input:**
```
Roll No: 101
Subject: Maths   Marks: 85
Subject: Science Marks: 78
Subject: English Marks: 90
```
**Output:**
```
--- Academic Result ---
Roll No: 101
Name: Ravi Kumar
Maths: 85
Science: 78
English: 90
Total: 253 / 300
Percentage: 84.33%
Grade: A
```

### 3. Attendance Manager (Slide 7)
**Input:**
```
Roll No: 101
Date: 26-08-2026
Status: Present
```
**Output:**
```
Attendance marked: Present (26-08-2026)
--- Attendance Summary ---
Roll No: 101 | Total Days: 50 | Present: 46
Attendance %: 92.00%
```

### 4. Fee Receipt (Slide 8)
**Input:**
```
Roll No: 101
Amount Paid: 5000
Date: 20-08-2026
```
**Output:**
```
--- Fee Receipt ---
Roll No: 101 | Name: Ravi Kumar
Amount Paid: ₹5000 | Date: 20-08-2026
Pending Balance: ₹2000
```

### 5. Admin Authentication (Slide 8)
**Input:**
```
Username: admin
Password: admin123
```
**Output:**
```
Login successful. Welcome, admin!
Access Level: Full Control
```

### 6. Class 10 Report & Topper (Slide 8)
**Input:**
```
Generate Report for Class: 10
```
**Output:**
```
--- Class 10 Report ---
Roll No | Name         | Percentage | Attendance
101     | Ravi Kumar   | 84.33%     | 92.00%
102     | Priya Sharma | 91.20%     | 88.00%
...
Topper: Priya Sharma (91.20%)
Report exported to: class10_report.csv
```

---

## 🌐 Web Portal (Slide 10 Flowchart)

Open your browser at:
```
http://localhost:8080
```
Includes dedicated tabs for:
- 🎓 **Student Records**: Base student CRUD and instant filtering.
- 📊 **Academic Records**: Subject mark entry, GPA/percentage calculator, and printable marksheets.
- 📅 **Attendance Manager**: Date-wise attendance logging with low-attendance warnings (<75%).
- 💳 **Fee Manager**: Payment tracking, pending balance ledger, and fee receipts with ₹ currency.
- 📈 **Report Generator**: Class 10 ranking table, gold/silver topper badges, and one-click CSV export (`class10_report.csv`).
- 🔐 **Admin & Access**: User credentials and access level administration.

