#ifndef WEB_ASSETS_HPP
#define WEB_ASSETS_HPP

#include <string>

namespace UI {

inline const char* getIndexHtml() {
    return R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Aditya University - Student Record Manager</title>
    <style>
        :root {
            --bg-body: #090d16;
            --bg-card: #131b2e;
            --bg-card-hover: #1c2742;
            --bg-input: #1e293b;
            --primary: #2563eb;
            --primary-hover: #1d4ed8;
            --accent: #f59e0b;
            --success: #10b981;
            --danger: #ef4444;
            --text-main: #f8fafc;
            --text-muted: #94a3b8;
            --border: #23304c;
            --radius: 12px;
        }

        * { box-sizing: border-box; margin: 0; padding: 0; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Arial, sans-serif; }
        body { background-color: var(--bg-body); color: var(--text-main); min-height: 100vh; display: flex; flex-direction: column; }

        /* Top University Navbar */
        .uni-navbar {
            background: linear-gradient(90deg, #0b1329 0%, #1e293b 100%);
            border-bottom: 2px solid #2563eb;
            padding: 14px 28px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            flex-wrap: wrap;
            gap: 12px;
        }

        .uni-brand {
            display: flex;
            align-items: center;
            gap: 14px;
        }

        .uni-logo-badge {
            background: linear-gradient(135deg, #f59e0b, #d97706);
            color: #000;
            font-weight: 900;
            font-size: 1.2rem;
            padding: 6px 12px;
            border-radius: 8px;
            letter-spacing: 0.05em;
        }

        .uni-title-group h1 {
            font-size: 1.35rem;
            font-weight: 800;
            letter-spacing: -0.01em;
            color: #fff;
        }

        .uni-title-group p {
            font-size: 0.8rem;
            color: #94a3b8;
        }

        .user-auth-badge {
            display: flex;
            align-items: center;
            gap: 12px;
            background: rgba(37, 99, 235, 0.15);
            border: 1px solid rgba(37, 99, 235, 0.4);
            padding: 6px 14px;
            border-radius: 9999px;
            font-size: 0.85rem;
        }

        .pulse-online {
            width: 8px;
            height: 8px;
            background: #22c55e;
            border-radius: 50%;
            box-shadow: 0 0 8px #22c55e;
        }

        /* Module Tabs */
        .tabs-nav {
            background: #0d1527;
            border-bottom: 1px solid var(--border);
            display: flex;
            gap: 4px;
            padding: 0 28px;
            overflow-x: auto;
        }

        .tab-btn {
            background: none;
            border: none;
            color: var(--text-muted);
            padding: 14px 20px;
            font-size: 0.92rem;
            font-weight: 600;
            cursor: pointer;
            border-bottom: 3px solid transparent;
            transition: all 0.2s;
            white-space: nowrap;
            display: flex;
            align-items: center;
            gap: 8px;
        }

        .tab-btn:hover { color: #fff; background: rgba(255,255,255,0.03); }
        .tab-btn.active { color: #60a5fa; border-bottom-color: #3b82f6; background: rgba(59,130,246,0.08); }

        /* Main Container */
        .main-content {
            flex: 1;
            max-width: 1300px;
            width: 100%;
            margin: 0 auto;
            padding: 24px;
        }

        .tab-pane { display: none; }
        .tab-pane.active { display: block; animation: fadeIn 0.25s ease-in-out; }

        @keyframes fadeIn { from { opacity: 0; transform: translateY(6px); } to { opacity: 1; transform: translateY(0); } }

        /* Cards & Grid */
        .card {
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: var(--radius);
            padding: 22px;
            margin-bottom: 24px;
            box-shadow: 0 6px 14px rgba(0,0,0,0.3);
        }

        .card-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 18px;
            flex-wrap: wrap;
            gap: 12px;
        }

        .card-title { font-size: 1.18rem; font-weight: 700; color: #fff; }

        /* Stats Row */
        .stats-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
            gap: 16px;
            margin-bottom: 24px;
        }

        .stat-box {
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: var(--radius);
            padding: 18px;
            display: flex;
            flex-direction: column;
            gap: 6px;
        }

        .stat-box .label { font-size: 0.8rem; color: var(--text-muted); text-transform: uppercase; font-weight: 600; }
        .stat-box .val { font-size: 1.8rem; font-weight: 800; color: #fff; }

        /* Controls */
        .toolbar {
            display: flex;
            justify-content: space-between;
            gap: 14px;
            margin-bottom: 18px;
            flex-wrap: wrap;
        }

        .search-field {
            background: var(--bg-input);
            border: 1px solid var(--border);
            border-radius: 8px;
            padding: 10px 16px;
            color: #fff;
            width: 100%;
            max-width: 380px;
            outline: none;
        }
        .search-field:focus { border-color: var(--primary); }

        .btn {
            background: var(--primary);
            color: #fff;
            border: none;
            border-radius: 8px;
            padding: 10px 18px;
            font-weight: 600;
            font-size: 0.88rem;
            cursor: pointer;
            transition: all 0.2s;
            display: inline-flex;
            align-items: center;
            gap: 6px;
        }
        .btn:hover { background: var(--primary-hover); }
        .btn-secondary { background: #334155; }
        .btn-secondary:hover { background: #475569; }
        .btn-danger { background: var(--danger); }
        .btn-danger:hover { background: #dc2626; }
        .btn-warning { background: var(--accent); color: #000; }
        .btn-warning:hover { background: #d97706; }
        .btn-sm { padding: 6px 12px; font-size: 0.8rem; }

        /* Tables */
        .table-responsive {
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: var(--radius);
            overflow-x: auto;
        }

        table { width: 100%; border-collapse: collapse; text-align: left; }
        th { background: #0e1526; color: var(--text-muted); font-size: 0.78rem; text-transform: uppercase; letter-spacing: 0.05em; padding: 14px 16px; border-bottom: 1px solid var(--border); }
        td { padding: 13px 16px; border-bottom: 1px solid var(--border); font-size: 0.9rem; }
        tr:hover td { background: rgba(255, 255, 255, 0.02); }

        .badge-roll { background: #1e293b; color: #38bdf8; font-family: monospace; font-weight: 700; padding: 4px 8px; border-radius: 6px; border: 1px solid #334155; }
        .badge-tag { padding: 3px 8px; border-radius: 6px; font-size: 0.82rem; font-weight: 700; }
        .tag-green { background: rgba(16,185,129,0.2); color: #34d399; }
        .tag-amber { background: rgba(245,158,11,0.2); color: #fbbf24; }
        .tag-red { background: rgba(239,68,68,0.2); color: #f87171; }

        /* Smooth Modern Scrollbars */
        html { scroll-behavior: smooth; }
        ::-webkit-scrollbar { width: 8px; height: 8px; }
        ::-webkit-scrollbar-track { background: #090d16; border-radius: 4px; }
        ::-webkit-scrollbar-thumb { background: #23304c; border-radius: 4px; }
        ::-webkit-scrollbar-thumb:hover { background: #3b82f6; }
        * { scrollbar-width: thin; scrollbar-color: #23304c #090d16; }

        /* Modals & Sheet Editors */
        .modal-bg {
            position: fixed; top: 0; left: 0; right: 0; bottom: 0;
            width: 100vw; height: 100vh;
            background: rgba(0, 0, 0, 0.82); display: none;
            justify-content: center; align-items: flex-start;
            z-index: 9999; backdrop-filter: blur(6px);
            overflow-y: auto; padding: 36px 16px;
            -webkit-overflow-scrolling: touch;
        }
        .modal-box {
            background: var(--bg-card); border: 1px solid var(--border); border-radius: var(--radius);
            width: 100%; max-width: 560px; padding: 24px; box-shadow: 0 25px 50px -12px rgba(0,0,0,0.7);
            margin: auto; position: relative;
            display: flex; flex-direction: column;
            max-height: calc(100vh - 72px);
        }
        .modal-head {
            display: flex; justify-content: space-between; align-items: center;
            padding-bottom: 14px; margin-bottom: 16px;
            border-bottom: 1px solid var(--border); flex-shrink: 0;
        }
        .modal-head h2 { font-size: 1.28rem; color: #fff; }
        .close-btn { background: none; border: none; color: var(--text-muted); font-size: 1.6rem; cursor: pointer; transition: color 0.2s; }
        .close-btn:hover { color: #fff; }
        .modal-form-body {
            overflow-y: auto; max-height: calc(100vh - 220px);
            padding-right: 8px; margin-bottom: 12px;
            flex: 1 1 auto; -webkit-overflow-scrolling: touch;
        }
        .form-row { margin-bottom: 14px; }
        .form-row label { display: block; font-size: 0.82rem; color: var(--text-muted); margin-bottom: 5px; font-weight: 600; }
        .input-ctrl { width: 100%; background: var(--bg-input); border: 1px solid var(--border); border-radius: 6px; padding: 10px 12px; color: #fff; font-size: 0.92rem; outline: none; transition: border-color 0.2s; }
        .input-ctrl:focus { border-color: var(--primary); }
        .modal-foot {
            display: flex; justify-content: flex-end; gap: 12px;
            padding-top: 14px; border-top: 1px solid var(--border);
            flex-shrink: 0; background: var(--bg-card);
        }

        /* Footer credits */
        footer {
            background: #0b1120;
            border-top: 1px solid var(--border);
            padding: 18px 28px;
            font-size: 0.82rem;
            color: var(--text-muted);
            text-align: center;
            line-height: 1.6;
        }

        #toast {
            position: fixed; bottom: 20px; right: 20px; padding: 12px 20px; border-radius: 8px;
            background: #1e293b; color: white; border: 1px solid var(--border); z-index: 2000;
            opacity: 0; transform: translateY(10px); transition: all 0.3s;
        }
        #toast.show { opacity: 1; transform: translateY(0); }
        #toast.success { border-color: var(--success); }
        #toast.error { border-color: var(--danger); }
    </style>
</head>
<body>

    <!-- University Header -->
    <header class="uni-navbar">
        <div class="uni-brand">
            <div class="uni-logo-badge">ADITYA</div>
            <div class="uni-title-group">
                <h1>ADITYA UNIVERSITY</h1>
                <p>Student Record Manager &bull; AIML Department &bull; C++ OOP Core</p>
            </div>
        </div>
        <div class="user-auth-badge">
            <span class="pulse-online"></span>
            <span id="activeUserLabel">Admin (Full Control)</span>
            <button class="btn btn-secondary btn-sm" onclick="promptLogin()">Switch User</button>
        </div>
    </header>

    <!-- Navigation Tabs for PPT Modules -->
    <nav class="tabs-nav">
        <button class="tab-btn active" onclick="switchTab('students')">🎓 Student Records</button>
        <button class="tab-btn" onclick="switchTab('academics')">📊 Academic Records</button>
        <button class="tab-btn" onclick="switchTab('attendance')">📅 Attendance Manager</button>
        <button class="tab-btn" onclick="switchTab('fees')">💳 Fee Manager</button>
        <button class="tab-btn" onclick="switchTab('reports')">📈 Report Generator &amp; Toppers</button>
        <button class="tab-btn" onclick="switchTab('admin')">🔐 Admin &amp; Access</button>
    </nav>

    <!-- Main Content Area -->
    <main class="main-content">

        <!-- 1. STUDENTS MODULE -->
        <section id="pane-students" class="tab-pane active">
            <div class="stats-grid">
                <div class="stat-box">
                    <span class="label">Total Enrolled</span>
                    <span class="val" id="statStudentCount">-</span>
                </div>
                <div class="stat-box">
                    <span class="label">Class</span>
                    <span class="val" style="font-size: 1.4rem; color: #60a5fa;">Class 10</span>
                </div>
                <div class="stat-box">
                    <span class="label">Average Attendance</span>
                    <span class="val" id="statAvgAtt">-</span>
                </div>
                <div class="stat-box">
                    <span class="label">Average Percentage</span>
                    <span class="val" id="statAvgPct">-</span>
                </div>
            </div>

            <div class="card">
                <div class="card-header">
                    <h2 class="card-title">Student Directory (Base Module)</h2>
                    <div class="toolbar" style="margin: 0;">
                        <input type="text" id="studentSearch" class="search-field" placeholder="Search by name, roll no, address..." oninput="filterStudents()">
                        <button class="btn btn-secondary" onclick="loadAllData()">Refresh</button>
                        <button class="btn" onclick="openAddStudentModal()">+ Add Student</button>
                    </div>
                </div>

                <div class="table-responsive">
                    <table>
                        <thead>
                            <tr>
                                <th>Roll No</th>
                                <th>Name</th>
                                <th>DOB</th>
                                <th>Contact</th>
                                <th>Address</th>
                                <th>Class</th>
                                <th>Actions</th>
                            </tr>
                        </thead>
                        <tbody id="studentsTableBody"></tbody>
                    </table>
                </div>
            </div>
        </section>

        <!-- 2. ACADEMIC RECORDS MODULE -->
        <section id="pane-academics" class="tab-pane">
            <div class="card">
                <div class="card-header">
                    <h2 class="card-title">Academic Records &amp; Marksheets (Slide 5 &amp; 7)</h2>
                    <button class="btn" onclick="openAddMarksModal()">+ Add/Update Marks</button>
                </div>
                <div class="table-responsive">
                    <table>
                        <thead>
                            <tr>
                                <th>Roll No</th>
                                <th>Student Name</th>
                                <th>Subjects &amp; Marks</th>
                                <th>Total</th>
                                <th>Percentage</th>
                                <th>Grade</th>
                                <th>CGPA</th>
                                <th>Marksheet</th>
                            </tr>
                        </thead>
                        <tbody id="academicsTableBody"></tbody>
                    </table>
                </div>
            </div>
        </section>

        <!-- 3. ATTENDANCE MANAGER MODULE -->
        <section id="pane-attendance" class="tab-pane">
            <div class="card">
                <div class="card-header">
                    <h2 class="card-title">Attendance Tracking &amp; Low Attendance Watchlist (Slide 5 &amp; 7)</h2>
                    <button class="btn" onclick="openMarkAttendanceModal()">+ Mark Attendance</button>
                </div>
                <div class="table-responsive">
                    <table>
                        <thead>
                            <tr>
                                <th>Roll No</th>
                                <th>Student Name</th>
                                <th>Total Days</th>
                                <th>Days Present</th>
                                <th>Attendance %</th>
                                <th>Status Alert</th>
                                <th>Action</th>
                            </tr>
                        </thead>
                        <tbody id="attendanceTableBody"></tbody>
                    </table>
                </div>
            </div>
        </section>

        <!-- 4. FEE MANAGER MODULE -->
        <section id="pane-fees" class="tab-pane">
            <div class="card">
                <div class="card-header">
                    <h2 class="card-title">Fee Manager &amp; Receipts (Slide 5 &amp; 8)</h2>
                    <button class="btn" onclick="openRecordPaymentModal()">+ Record Payment</button>
                </div>
                <div class="table-responsive">
                    <table>
                        <thead>
                            <tr>
                                <th>Roll No</th>
                                <th>Student Name</th>
                                <th>Total Course Fee</th>
                                <th>Amount Paid</th>
                                <th>Pending Balance</th>
                                <th>Status</th>
                                <th>Receipt</th>
                            </tr>
                        </thead>
                        <tbody id="feesTableBody"></tbody>
                    </table>
                </div>
            </div>
        </section>

        <!-- 5. REPORT GENERATOR & TOPPERS MODULE -->
        <section id="pane-reports" class="tab-pane">
            <div class="card">
                <div class="card-header">
                    <h2 class="card-title">Class 10 Report &amp; Topper Rankings (Slide 6 &amp; 8)</h2>
                    <button class="btn btn-warning" onclick="downloadCsvReport()">Export Class 10 Report (CSV)</button>
                </div>
                <div class="table-responsive">
                    <table>
                        <thead>
                            <tr>
                                <th>Rank</th>
                                <th>Roll No</th>
                                <th>Name</th>
                                <th>Academic Percentage</th>
                                <th>Attendance %</th>
                                <th>Grade</th>
                                <th>Dossier</th>
                            </tr>
                        </thead>
                        <tbody id="reportsTableBody"></tbody>
                    </table>
                </div>
            </div>
        </section>

        <!-- 6. ADMIN & USER MODULE -->
        <section id="pane-admin" class="tab-pane">
            <div class="card">
                <div class="card-header">
                    <h2 class="card-title">User Administration &amp; Access Levels (Slide 6 &amp; 8)</h2>
                    <button class="btn" onclick="openAddAdminModal()">+ Add New Admin</button>
                </div>
                <div class="table-responsive">
                    <table>
                        <thead>
                            <tr>
                                <th>Username</th>
                                <th>Access Level</th>
                                <th>Status</th>
                                <th>Action</th>
                            </tr>
                        </thead>
                        <tbody id="usersTableBody"></tbody>
                    </table>
                </div>
            </div>
        </section>

    </main>

    <!-- Student Modal (Add/Edit) -->
    <div class="modal-bg" id="studentModal" onclick="closeOnBackdrop(event, 'studentModal')">
        <div class="modal-box" onclick="event.stopPropagation()">
            <div class="modal-head">
                <h2 id="studentModalTitle">Add Student Record</h2>
                <button type="button" class="close-btn" onclick="closeModal('studentModal')">&times;</button>
            </div>
            <form onsubmit="submitStudentForm(event)" style="display:flex; flex-direction:column; min-height:0; flex:1;">
                <input type="hidden" id="editMode" value="false">
                <div class="modal-form-body">
                    <div class="form-row">
                        <label>Roll Number * (e.g. 101)</label>
                        <input type="text" id="mRollNo" class="input-ctrl" placeholder="e.g. 101" required>
                    </div>
                    <div class="form-row">
                        <label>Full Name *</label>
                        <input type="text" id="mName" class="input-ctrl" placeholder="e.g. Ravi Kumar" required>
                    </div>
                    <div class="form-row">
                        <label>Date of Birth (DD-MM-YYYY) *</label>
                        <input type="text" id="mDOB" class="input-ctrl" placeholder="12-05-2005" required>
                    </div>
                    <div class="form-row">
                        <label>Contact Number *</label>
                        <input type="text" id="mContact" class="input-ctrl" placeholder="9876543210" required>
                    </div>
                    <div class="form-row">
                        <label>Address *</label>
                        <input type="text" id="mAddress" class="input-ctrl" placeholder="Bobbili, AP" required>
                    </div>
                    <div class="form-row">
                        <label>Class</label>
                        <input type="text" id="mClass" class="input-ctrl" value="Class 10">
                    </div>
                </div>
                <div class="modal-foot">
                    <button type="button" class="btn btn-secondary" onclick="closeModal('studentModal')">Cancel</button>
                    <button type="submit" class="btn">Save Student</button>
                </div>
            </form>
        </div>
    </div>

    <!-- Mark Attendance Modal -->
    <div class="modal-bg" id="attendanceModal" onclick="closeOnBackdrop(event, 'attendanceModal')">
        <div class="modal-box" onclick="event.stopPropagation()">
            <div class="modal-head">
                <h2>Mark Attendance</h2>
                <button type="button" class="close-btn" onclick="closeModal('attendanceModal')">&times;</button>
            </div>
            <form onsubmit="submitAttendanceForm(event)" style="display:flex; flex-direction:column; min-height:0; flex:1;">
                <div class="modal-form-body">
                    <div class="form-row">
                        <label>Roll Number *</label>
                        <input type="text" id="attRollNo" class="input-ctrl" placeholder="e.g. 101" required>
                    </div>
                    <div class="form-row">
                        <label>Date (DD-MM-YYYY) *</label>
                        <input type="text" id="attDate" class="input-ctrl" value="26-08-2026" required>
                    </div>
                    <div class="form-row">
                        <label>Status</label>
                        <select id="attStatus" class="input-ctrl">
                            <option value="Present">Present</option>
                            <option value="Absent">Absent</option>
                        </select>
                    </div>
                </div>
                <div class="modal-foot">
                    <button type="button" class="btn btn-secondary" onclick="closeModal('attendanceModal')">Cancel</button>
                    <button type="submit" class="btn">Mark Attendance</button>
                </div>
            </form>
        </div>
    </div>

    <!-- Fee Payment Modal -->
    <div class="modal-bg" id="feeModal" onclick="closeOnBackdrop(event, 'feeModal')">
        <div class="modal-box" onclick="event.stopPropagation()">
            <div class="modal-head">
                <h2>Record Fee Payment</h2>
                <button type="button" class="close-btn" onclick="closeModal('feeModal')">&times;</button>
            </div>
            <form onsubmit="submitFeeForm(event)" style="display:flex; flex-direction:column; min-height:0; flex:1;">
                <div class="modal-form-body">
                    <div class="form-row">
                        <label>Roll Number *</label>
                        <input type="text" id="feeRollNo" class="input-ctrl" placeholder="e.g. 101" required>
                    </div>
                    <div class="form-row">
                        <label>Amount Paid (&#8377;) *</label>
                        <input type="number" id="feeAmount" class="input-ctrl" placeholder="5000" required>
                    </div>
                    <div class="form-row">
                        <label>Payment Date (DD-MM-YYYY) *</label>
                        <input type="text" id="feeDate" class="input-ctrl" value="20-08-2026" required>
                    </div>
                </div>
                <div class="modal-foot">
                    <button type="button" class="btn btn-secondary" onclick="closeModal('feeModal')">Cancel</button>
                    <button type="submit" class="btn">Record Payment</button>
                </div>
            </form>
        </div>
    </div>

    <!-- Add Marks Modal -->
    <div class="modal-bg" id="marksModal" onclick="closeOnBackdrop(event, 'marksModal')">
        <div class="modal-box" onclick="event.stopPropagation()">
            <div class="modal-head">
                <h2>Add / Update Academic Marks</h2>
                <button type="button" class="close-btn" onclick="closeModal('marksModal')">&times;</button>
            </div>
            <form onsubmit="submitMarksForm(event)" style="display:flex; flex-direction:column; min-height:0; flex:1;">
                <div class="modal-form-body">
                    <div class="form-row">
                        <label>Roll Number *</label>
                        <input type="text" id="mksRollNo" class="input-ctrl" placeholder="e.g. 101" required>
                    </div>
                    <div class="form-row">
                        <label>Maths Marks (0-100)</label>
                        <input type="number" id="mksMaths" class="input-ctrl" placeholder="85">
                    </div>
                    <div class="form-row">
                        <label>Science Marks (0-100)</label>
                        <input type="number" id="mksScience" class="input-ctrl" placeholder="78">
                    </div>
                    <div class="form-row">
                        <label>English Marks (0-100)</label>
                        <input type="number" id="mksEnglish" class="input-ctrl" placeholder="90">
                    </div>
                </div>
                <div class="modal-foot">
                    <button type="button" class="btn btn-secondary" onclick="closeModal('marksModal')">Cancel</button>
                    <button type="submit" class="btn">Save Marks</button>
                </div>
            </form>
        </div>
    </div>

    <!-- Add Admin Modal -->
    <div class="modal-bg" id="adminModal" onclick="closeOnBackdrop(event, 'adminModal')">
        <div class="modal-box" onclick="event.stopPropagation()">
            <div class="modal-head">
                <h2>Add New Admin User</h2>
                <button type="button" class="close-btn" onclick="closeModal('adminModal')">&times;</button>
            </div>
            <form onsubmit="submitAdminForm(event)" style="display:flex; flex-direction:column; min-height:0; flex:1;">
                <div class="modal-form-body">
                    <div class="form-row">
                        <label>Username *</label>
                        <input type="text" id="newAdminUser" class="input-ctrl" placeholder="e.g. faculty_aiml" required>
                    </div>
                    <div class="form-row">
                        <label>Password *</label>
                        <input type="password" id="newAdminPass" class="input-ctrl" placeholder="Enter password" required>
                    </div>
                    <div class="form-row">
                        <label>Access Level</label>
                        <select id="newAdminRole" class="input-ctrl">
                            <option value="Full Control">Full Control</option>
                            <option value="Staff">Staff</option>
                        </select>
                    </div>
                </div>
                <div class="modal-foot">
                    <button type="button" class="btn btn-secondary" onclick="closeModal('adminModal')">Cancel</button>
                    <button type="submit" class="btn">Create User</button>
                </div>
            </form>
        </div>
    </div>

    <!-- Dossier View Modal -->
    <div class="modal-bg" id="dossierModal" onclick="closeOnBackdrop(event, 'dossierModal')">
        <div class="modal-box" style="max-width: 680px;" onclick="event.stopPropagation()">
            <div class="modal-head">
                <h2 id="dossierTitle">Individual Student Dossier</h2>
                <button type="button" class="close-btn" onclick="closeModal('dossierModal')">&times;</button>
            </div>
            <div class="modal-form-body">
                <pre id="dossierContent" style="background:#0b1120; border:1px solid #334155; padding:18px; border-radius:8px; white-space:pre-wrap; font-family:monospace; color:#38bdf8; font-size:0.9rem; line-height:1.5;"></pre>
            </div>
            <div class="modal-foot">
                <button type="button" class="btn btn-secondary" onclick="closeModal('dossierModal')">Close</button>
                <button type="button" class="btn" onclick="window.print()">Print Dossier</button>
            </div>
        </div>
    </div>

    <!-- Toast Notification -->
    <div id="toast"></div>

    <!-- Presentation Team Footer -->
    <footer>
        <p><strong>ADITYA UNIVERSITY &bull; STUDENT RECORD MANAGER</strong></p>
        <p><strong>Presented by:</strong> 25B11AI241 (D.R.PALGUN) &bull; 25B11AI581 (K.BALA ADITYA) &bull; 25B11AI361 (G.DILEEP) &bull; 25B11AI765 (M.MOHITH NAIK)</p>
        <p><strong>Course Instructor:</strong> K.SYAMALA KALYANI, M.TECH, ASSISTANT PROFESSOR, AIML DEPARTMENT</p>
    </footer>

    <script>
        let cachedStudents = [];
        let cachedAcademics = [];
        let cachedAttendance = [];
        let cachedFees = [];
        let cachedReports = [];

        function showToast(msg, isErr = false) {
            const t = document.getElementById('toast');
            t.innerText = msg;
            t.className = 'show ' + (isErr ? 'error' : 'success');
            setTimeout(() => { t.className = ''; }, 3500);
        }

        function switchTab(tabId) {
            document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
            document.querySelectorAll('.tab-pane').forEach(p => p.classList.remove('active'));

            const activeBtn = Array.from(document.querySelectorAll('.tab-btn')).find(b => b.getAttribute('onclick').includes(tabId));
            if (activeBtn) activeBtn.classList.add('active');

            const target = document.getElementById('pane-' + tabId);
            if (target) target.classList.add('active');
        }

        function openModal(id) {
            const m = document.getElementById(id);
            if (m) {
                m.style.display = 'flex';
                document.body.style.overflow = 'hidden';
            }
        }

        function closeModal(id) {
            const m = document.getElementById(id);
            if (m) {
                m.style.display = 'none';
                document.body.style.overflow = '';
            }
        }

        function closeOnBackdrop(e, id) {
            if (e.target && e.target.classList && e.target.classList.contains('modal-bg')) {
                closeModal(id);
            }
        }

        window.addEventListener('keydown', (e) => {
            if (e.key === 'Escape') {
                document.querySelectorAll('.modal-bg').forEach(m => {
                    m.style.display = 'none';
                });
                document.body.style.overflow = '';
            }
        });

        function openAddAdminModal() {
            document.getElementById('newAdminUser').value = '';
            document.getElementById('newAdminPass').value = '';
            document.getElementById('newAdminRole').value = 'Full Control';
            openModal('adminModal');
        }

        async function submitAdminForm(e) {
            e.preventDefault();
            const username = document.getElementById('newAdminUser').value.trim();
            const password = document.getElementById('newAdminPass').value.trim();
            const accessLevel = document.getElementById('newAdminRole').value;

            try {
                const res = await fetch('/api/auth/register', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ username, password, accessLevel })
                });
                if (res.ok) {
                    showToast(`Admin user '${username}' created successfully!`);
                    closeModal('adminModal');
                    loadAllData();
                } else {
                    const data = await res.json();
                    showToast(data.error || 'Failed to create user', true);
                }
            } catch (err) {
                showToast('Error: ' + err.message, true);
            }
        }

        async function loadAllData() {
            try {
                const [sRes, aRes, attRes, fRes, rRes, uRes] = await Promise.all([
                    fetch('/api/students'),
                    fetch('/api/academics'),
                    fetch('/api/attendance'),
                    fetch('/api/fees'),
                    fetch('/api/reports?class=10'),
                    fetch('/api/auth/users')
                ]);

                if (sRes.ok) cachedStudents = await sRes.json();
                if (aRes.ok) cachedAcademics = await aRes.json();
                if (attRes.ok) cachedAttendance = await attRes.json();
                if (fRes.ok) cachedFees = await fRes.json();
                if (rRes.ok) cachedReports = await rRes.json();

                renderStudentsTable(cachedStudents);
                renderAcademicsTable();
                renderAttendanceTable();
                renderFeesTable();
                renderReportsTable();
                if (uRes.ok) renderUsersTable(await uRes.json());

                // Update KPI Metrics
                document.getElementById('statStudentCount').innerText = cachedStudents.length;
                let totalAtt = 0;
                cachedAttendance.forEach(a => totalAtt += a.percentage);
                document.getElementById('statAvgAtt').innerText = cachedAttendance.length ? (totalAtt / cachedAttendance.length).toFixed(1) + '%' : 'N/A';

                let totalPct = 0;
                cachedReports.forEach(r => totalPct += r.percentage);
                document.getElementById('statAvgPct').innerText = cachedReports.length ? (totalPct / cachedReports.length).toFixed(1) + '%' : 'N/A';

            } catch (err) {
                console.error(err);
            }
        }

        function renderStudentsTable(students) {
            const tbody = document.getElementById('studentsTableBody');
            if (!students.length) {
                tbody.innerHTML = '<tr><td colspan="7" style="text-align:center; padding:30px; color:#94a3b8;">No student records found.</td></tr>';
                return;
            }
            tbody.innerHTML = students.map(s => `
                <tr>
                    <td><span class="badge-roll">${escapeHtml(s.rollNo)}</span></td>
                    <td><strong>${escapeHtml(s.name)}</strong></td>
                    <td>${escapeHtml(s.dob)}</td>
                    <td>${escapeHtml(s.contact)}</td>
                    <td>${escapeHtml(s.address)}</td>
                    <td>${escapeHtml(s.className || 'Class 10')}</td>
                    <td>
                        <button class="btn btn-secondary btn-sm" onclick="editStudent('${s.rollNo}')">Edit</button>
                        <button class="btn btn-danger btn-sm" onclick="deleteStudent('${s.rollNo}')">Delete</button>
                    </td>
                </tr>
            `).join('');
        }

        function renderAcademicsTable() {
            const tbody = document.getElementById('academicsTableBody');
            tbody.innerHTML = cachedAcademics.map(a => {
                let subStr = Object.entries(a.subjects || {}).map(([k,v]) => `${k}: <strong>${v}</strong>`).join(', ');
                return `
                    <tr>
                        <td><span class="badge-roll">${escapeHtml(a.rollNo)}</span></td>
                        <td>${escapeHtml(a.name)}</td>
                        <td>${subStr || 'No marks'}</td>
                        <td>${a.total} / ${a.maxMarks}</td>
                        <td><strong>${a.percentage.toFixed(2)}%</strong></td>
                        <td><span class="badge-tag ${a.percentage >= 75 ? 'tag-green' : (a.percentage >= 50 ? 'tag-amber' : 'tag-red')}">${a.grade}</span></td>
                        <td>${a.cgpa.toFixed(2)}</td>
                        <td><button class="btn btn-secondary btn-sm" onclick="viewDossier('${a.rollNo}')">Marksheet</button></td>
                    </tr>
                `;
            }).join('');
        }

        function renderAttendanceTable() {
            const tbody = document.getElementById('attendanceTableBody');
            tbody.innerHTML = cachedAttendance.map(att => {
                const s = cachedStudents.find(x => x.rollNo === att.rollNo);
                const isLow = att.percentage < 75.0;
                return `
                    <tr>
                        <td><span class="badge-roll">${escapeHtml(att.rollNo)}</span></td>
                        <td>${escapeHtml(s ? s.name : 'Student')}</td>
                        <td>${att.totalDays}</td>
                        <td>${att.presentDays}</td>
                        <td><strong>${att.percentage.toFixed(2)}%</strong></td>
                        <td><span class="badge-tag ${isLow ? 'tag-red' : 'tag-green'}">${isLow ? 'Low (<75%)' : 'Satisfactory'}</span></td>
                        <td><button class="btn btn-secondary btn-sm" onclick="quickMark('${att.rollNo}')">Mark Today</button></td>
                    </tr>
                `;
            }).join('');
        }

        function renderFeesTable() {
            const tbody = document.getElementById('feesTableBody');
            tbody.innerHTML = cachedFees.map(f => {
                const isPaid = f.pendingBalance <= 0;
                return `
                    <tr>
                        <td><span class="badge-roll">${escapeHtml(f.rollNo)}</span></td>
                        <td>${escapeHtml(f.name || 'Student')}</td>
                        <td>&#8377;${f.totalFee}</td>
                        <td>&#8377;${f.totalPaid}</td>
                        <td><strong style="color: ${isPaid ? '#34d399' : '#f87171'}">&#8377;${f.pendingBalance}</strong></td>
                        <td><span class="badge-tag ${isPaid ? 'tag-green' : 'tag-amber'}">${isPaid ? 'Cleared' : 'Pending'}</span></td>
                        <td><button class="btn btn-secondary btn-sm" onclick="viewFeeReceipt('${f.rollNo}')">Receipt</button></td>
                    </tr>
                `;
            }).join('');
        }

        function renderReportsTable() {
            const tbody = document.getElementById('reportsTableBody');
            tbody.innerHTML = cachedReports.map((r, idx) => `
                <tr>
                    <td><strong>${idx === 0 ? '🥇 Topper' : '#' + (idx + 1)}</strong></td>
                    <td><span class="badge-roll">${escapeHtml(r.rollNo)}</span></td>
                    <td><strong>${escapeHtml(r.name)}</strong></td>
                    <td><strong>${r.percentage.toFixed(2)}%</strong></td>
                    <td>${r.attendancePct.toFixed(2)}%</td>
                    <td><span class="badge-tag tag-green">${r.grade}</span></td>
                    <td><button class="btn btn-secondary btn-sm" onclick="viewDossier('${r.rollNo}')">View Dossier</button></td>
                </tr>
            `).join('');
        }

        function renderUsersTable(users) {
            const tbody = document.getElementById('usersTableBody');
            tbody.innerHTML = users.map(u => `
                <tr>
                    <td><strong>${escapeHtml(u.username)}</strong></td>
                    <td><span class="badge-tag tag-amber">${escapeHtml(u.accessLevel)}</span></td>
                    <td>Active</td>
                    <td><button class="btn btn-secondary btn-sm" onclick="alert('User session verified.')">Verify</button></td>
                </tr>
            `).join('');
        }

        function filterStudents() {
            const q = document.getElementById('studentSearch').value.toLowerCase().trim();
            if (!q) { renderStudentsTable(cachedStudents); return; }
            const filtered = cachedStudents.filter(s =>
                s.name.toLowerCase().includes(q) ||
                s.rollNo.toLowerCase().includes(q) ||
                s.address.toLowerCase().includes(q)
            );
            renderStudentsTable(filtered);
        }

        function openAddStudentModal() {
            document.getElementById('studentModalTitle').innerText = 'Add Student Record';
            document.getElementById('editMode').value = 'false';
            document.getElementById('mRollNo').disabled = false;
            document.getElementById('mRollNo').value = '';
            document.getElementById('mName').value = '';
            document.getElementById('mDOB').value = '';
            document.getElementById('mContact').value = '';
            document.getElementById('mAddress').value = '';
            openModal('studentModal');
        }

        function editStudent(rollNo) {
            const s = cachedStudents.find(x => x.rollNo === rollNo);
            if (!s) return;
            document.getElementById('studentModalTitle').innerText = 'Edit Student #' + rollNo;
            document.getElementById('editMode').value = 'true';
            document.getElementById('mRollNo').disabled = true;
            document.getElementById('mRollNo').value = s.rollNo;
            document.getElementById('mName').value = s.name;
            document.getElementById('mDOB').value = s.dob;
            document.getElementById('mContact').value = s.contact;
            document.getElementById('mAddress').value = s.address;
            document.getElementById('mClass').value = s.className || 'Class 10';
            openModal('studentModal');
        }

        async function submitStudentForm(e) {
            e.preventDefault();
            const isEdit = document.getElementById('editMode').value === 'true';
            const student = {
                rollNo: document.getElementById('mRollNo').value.trim(),
                name: document.getElementById('mName').value.trim(),
                dob: document.getElementById('mDOB').value.trim(),
                contact: document.getElementById('mContact').value.trim(),
                address: document.getElementById('mAddress').value.trim(),
                className: document.getElementById('mClass').value.trim()
            };

            const method = isEdit ? 'PUT' : 'POST';
            const res = await fetch('/api/students', {
                method, headers: {'Content-Type': 'application/json'},
                body: JSON.stringify(student)
            });
            const data = await res.json();
            if (!res.ok) {
                showToast(data.error || 'Operation failed', true);
                return;
            }
            showToast(isEdit ? 'Student record updated.' : 'Student added successfully.');
            closeModal('studentModal');
            loadAllData();
        }

        async function deleteStudent(rollNo) {
            if (!confirm(`Are you sure you want to delete student Roll No: ${rollNo}?`)) return;
            const res = await fetch(`/api/students?rollNo=${rollNo}`, { method: 'DELETE' });
            if (res.ok) {
                showToast(`Student #${rollNo} deleted.`);
                loadAllData();
            } else {
                showToast('Failed to delete student.', true);
            }
        }

        function openMarkAttendanceModal() { openModal('attendanceModal'); }
        function quickMark(rollNo) {
            document.getElementById('attRollNo').value = rollNo;
            openModal('attendanceModal');
        }

        async function submitAttendanceForm(e) {
            e.preventDefault();
            const rollNo = document.getElementById('attRollNo').value.trim();
            const date = document.getElementById('attDate').value.trim();
            const status = document.getElementById('attStatus').value;

            const res = await fetch('/api/attendance', {
                method: 'POST', headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({rollNo, date, status})
            });
            if (res.ok) {
                showToast(`Attendance marked: ${status} for Roll #${rollNo}`);
                closeModal('attendanceModal');
                loadAllData();
            }
        }

        function openRecordPaymentModal() { openModal('feeModal'); }
        async function submitFeeForm(e) {
            e.preventDefault();
            const rollNo = document.getElementById('feeRollNo').value.trim();
            const amount = parseFloat(document.getElementById('feeAmount').value);
            const date = document.getElementById('feeDate').value.trim();

            const res = await fetch('/api/fees', {
                method: 'POST', headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({rollNo, amount, date})
            });
            if (res.ok) {
                showToast(`Payment of ₹${amount} recorded for Roll #${rollNo}`);
                closeModal('feeModal');
                loadAllData();
            }
        }

        function openAddMarksModal() { openModal('marksModal'); }
        async function submitMarksForm(e) {
            e.preventDefault();
            const rollNo = document.getElementById('mksRollNo').value.trim();
            const m1 = parseFloat(document.getElementById('mksMaths').value) || 0;
            const m2 = parseFloat(document.getElementById('mksScience').value) || 0;
            const m3 = parseFloat(document.getElementById('mksEnglish').value) || 0;

            const payload = {
                rollNo,
                subjects: { "Maths": m1, "Science": m2, "English": m3 }
            };

            const res = await fetch('/api/academics', {
                method: 'POST', headers: {'Content-Type': 'application/json'},
                body: JSON.stringify(payload)
            });
            if (res.ok) {
                showToast(`Academic marks saved for Roll #${rollNo}`);
                closeModal('marksModal');
                loadAllData();
            }
        }

        async function viewDossier(rollNo) {
            const res = await fetch(`/api/reports/student?rollNo=${rollNo}`);
            if (res.ok) {
                const text = await res.text();
                document.getElementById('dossierTitle').innerText = `Student Dossier (Roll #${rollNo})`;
                document.getElementById('dossierContent').innerText = text;
                openModal('dossierModal');
            }
        }

        async function viewFeeReceipt(rollNo) {
            const f = cachedFees.find(x => x.rollNo === rollNo);
            const receipt = `--- Fee Receipt ---\nRoll No: ${rollNo} | Name: ${f ? f.name : 'Student'}\nAmount Paid: ₹${f ? f.totalPaid : 0}\nPending Balance: ₹${f ? f.pendingBalance : 0}\n\n[Aditya University - Accounts Department]`;
            document.getElementById('dossierTitle').innerText = `Fee Receipt (Roll #${rollNo})`;
            document.getElementById('dossierContent').innerText = receipt;
            openModal('dossierModal');
        }

        function downloadCsvReport() {
            window.location.href = '/api/reports/class10.csv';
        }

        function promptLogin() {
            const u = prompt('Enter Admin Username:', 'admin');
            const p = prompt('Enter Admin Password:', 'admin123');
            if (u && p) {
                fetch('/api/auth/login', {
                    method: 'POST', headers: {'Content-Type':'application/json'},
                    body: JSON.stringify({username: u, password: p})
                }).then(r => r.json()).then(data => {
                    if (data.success) {
                        showToast(`Welcome, ${u}! Access: Full Control`);
                        document.getElementById('activeUserLabel').innerText = `${u} (${data.accessLevel})`;
                    } else {
                        showToast('Invalid credentials.', true);
                    }
                });
            }
        }

        function escapeHtml(s) {
            if (!s) return '';
            return s.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
        }

        setInterval(loadAllData, 4000);
        window.addEventListener('DOMContentLoaded', loadAllData);
    </script>
</body>
</html>
)rawliteral";
}

} // namespace UI

#endif // WEB_ASSETS_HPP
