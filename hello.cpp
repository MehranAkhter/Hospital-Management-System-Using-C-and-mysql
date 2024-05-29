#include <iostream>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cstdlib> 

int choice;

using namespace std;
using namespace sql;

class Appointment 
{
    static int assignAppointmentId;
    const int appointmentId;
    string patientId;
    string doctorId;
    string appointmentDate;
    string appointmentTime;

public:
    Appointment(int id = 0) : appointmentId(id) {}

    static string generateAppointmentId() {
        try {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            Statement* stmt = con->createStatement();
            ResultSet* rs = stmt->executeQuery("SELECT appointment_id FROM appointments ORDER BY appointment_id DESC LIMIT 1");
            if (rs->next()) 
            {
                string maxID = rs->getString(1);
                if (!maxID.empty()) {
                    int maxNumericID = stoi(maxID.substr(4));
                    assignAppointmentId = maxNumericID + 1;
                }
            }
            delete rs;
            delete stmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        return "APPT" + to_string(assignAppointmentId++);
    }

    void addAppointment() {
        cout << "Adding appointment details\n";
        cout << "Enter patient ID: ";
        cin >> patientId;
        cout << "Enter doctor ID: ";
        cin >> doctorId;
        cout << "Enter appointment date (YYYY-MM-DD): ";
        cin >> appointmentDate;
        cout << "Enter appointment time (HH:MM): ";
        cin >> appointmentTime;

        try {
            string appointmentID = generateAppointmentId();
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* psmt = con->prepareStatement("INSERT INTO appointments(appointment_id, patient_id, doctor_id, appointment_date, appointment_time) VALUES (?, ?, ?, ?, ?)");
            psmt->setString(1, appointmentID);
            psmt->setString(2, patientId);
            psmt->setString(3, doctorId);
            psmt->setString(4, appointmentDate);
            psmt->setString(5, appointmentTime);

            int rowsAffected = psmt->executeUpdate();
            if (rowsAffected > 0)
                cout << "Appointment added successfully!" << endl;
            else
                cerr << "Error: No rows affected." << endl;

            delete psmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e) {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }

    void listAppointmentsByPatientId() {
        try {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* psmt = con->prepareStatement("SELECT * FROM appointments WHERE patient_id = ?");
            psmt->setString(1, patientId);
            ResultSet* rs = psmt->executeQuery();

            cout << "Appointments for Patient ID " << patientId << ":\n";
            while (rs->next()) {
                cout << "Appointment ID: " << rs->getString("appointment_id") << ", Doctor ID: " << rs->getString("doctor_id") << ", Date: " << rs->getString("appointment_date") << ", Time: " << rs->getString("appointment_time") << endl;
            }

            delete rs;
            delete psmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e) {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }

    void listAppointmentsByDoctorId() {
        try {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* psmt = con->prepareStatement("SELECT * FROM appointments WHERE doctor_id = ?");
            psmt->setString(1, doctorId);
            ResultSet* rs = psmt->executeQuery();

            cout << "Appointments for Doctor ID " << doctorId << ":\n";
            while (rs->next()) {
                cout << "Appointment ID: " << rs->getString("appointment_id") << ", Patient ID: " << rs->getString("patient_id") << ", Date: " << rs->getString("appointment_date") << ", Time: " << rs->getString("appointment_time") << endl;
            }

            delete rs;
            delete psmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e) {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }
};

int Appointment::assignAppointmentId = 1;


class patient 
{
    static int assignregular;
    const int ID;
    string cnic;
    string name;
    string fname;
    string age;
    string gender;
    int dept_ID;
    string doctor_id;
public:
    patient(int id = 0) : ID(id) {}

    static string generateregular() {
        try {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            Statement* stmt = con->createStatement();
            ResultSet* rs = stmt->executeQuery("SELECT ID FROM regular_patients ORDER BY ID DESC LIMIT 1");
            if (rs->next()) {
                string maxID = rs->getString(1);
                if (!maxID.empty()) {
                    int maxNumericID = stoi(maxID.substr(4));
                    assignregular = maxNumericID + 1;
                }
            }
            delete rs;
            delete stmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        return "REGP" + to_string(assignregular++);
    }

    static void listWaitingPatients(const string& doctorID) {
        try {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* psmt = con->prepareStatement("SELECT ID, name, dept_id FROM regular_patients WHERE doctor_id = ? AND checked = 0");
            psmt->setString(1, doctorID);
            ResultSet* rs = psmt->executeQuery();

            cout << "Patients waiting for Doctor ID " << doctorID << ":\n";
            while (rs->next()) {
                cout << "Patient ID: " << rs->getString("ID") << ", Name: " << rs->getString("name") << ", Department ID: " << rs->getInt("dept_id") << endl;
            }

            delete rs;
            delete psmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e) {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }

    static void markAsChecked(const string& patientID)
    {
        try {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* psmt = con->prepareStatement("UPDATE regular_patients SET checked = 1 WHERE ID = ?");
            psmt->setString(1, patientID);
            int rowsAffected = psmt->executeUpdate();
            if (rowsAffected > 0)
                cout << "Patient marked as checked successfully!" << endl;
            else
                cerr << "Error: No rows affected." << endl;

            delete psmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e) {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }

    void referToAnotherDoctor()
    {
        string patientID;
        int newDeptID;
        string newDoctorID;
        cout << "Enter the patient ID to refer: ";
        cin >> patientID;
        cout << "Enter the new department ID: ";
        cin >> newDeptID;
        try {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");

            PreparedStatement* stmt = con->prepareStatement("SELECT doctor_id FROM doctors WHERE dept_id = ? ORDER BY RAND() LIMIT 1");
            stmt->setInt(1, newDeptID);
            ResultSet* rs = stmt->executeQuery();
            if (rs->next()) {
                newDoctorID = rs->getString("doctor_id");
            }
            delete rs;
            delete stmt;

            PreparedStatement* psmt = con->prepareStatement("UPDATE regular_patients SET doctor_id = ?, dept_id = ? WHERE ID = ?");
            psmt->setString(1, newDoctorID);
            psmt->setInt(2, newDeptID);
            psmt->setString(3, patientID);
            int rowsAffected = psmt->executeUpdate();
            if (rowsAffected > 0)
                cout << "Patient referred to another doctor successfully!" << endl;
            else
                cerr << "Error: No rows affected." << endl;

            delete psmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e) {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }

    void addregular() {
        cout << "Adding patient details \n";
        cout << "Enter the CNIC of the patient: ";
        cin >> cnic;
        cout << "Enter the name of the patient: ";
        cin >> name;
        cout << "Enter the father's name of the patient: ";
        cin >> fname;
        cout << "Enter the age of the patient: ";
        cin >> age;
        cout << "Enter the gender of the patient: ";
        cin >> gender;
        cout << "Enter the department ID of the patient: ";
        cin >> dept_ID;
        try {
            string regID = generateregular();
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");

            PreparedStatement* stmt = con->prepareStatement("SELECT doctor_id FROM doctors WHERE dept_id = ? ORDER BY RAND() LIMIT 1");
            stmt->setInt(1, dept_ID);
            ResultSet* rs = stmt->executeQuery();
            if (rs->next()) {
                doctor_id = rs->getString("doctor_id");
            }
            delete rs;
            delete stmt;
            PreparedStatement* psmt = con->prepareStatement("INSERT INTO regular_patients(ID, cnic, name, fname, age, gender, dept_id, doctor_id, checked, date) VALUES (?, ?, ?, ?, ?, ?, ?, ?, 0, NOW())");
            psmt->setString(1, regID);
            psmt->setString(2, cnic);
            psmt->setString(3, name);
            psmt->setString(4, fname);
            psmt->setString(5, age);
            psmt->setString(6, gender);
            psmt->setInt(7, dept_ID);
            psmt->setString(8, doctor_id);

            int rowsAffected = psmt->executeUpdate();
            if (rowsAffected > 0)
                cout << "Patient details inserted successfully!" << endl;
            else
                cerr << "Error: No rows affected." << endl;

            delete psmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e) {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }
};

int patient::assignregular = 1;


class diagnosisreport
{
    static int assigndiagnose;
public:


    static string generatedoctor()
    {
        try
        {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            Statement* stmt = con->createStatement();
            ResultSet* rs = stmt->executeQuery("SELECT report_id FROM diagnosis_reports ORDER BY report_id DESC LIMIT 1");
            if (rs->next())
            {
                string maxID = rs->getString(1);
                if (!maxID.empty())
                {
                    int maxNumericID = stoi(maxID.substr(6));
                    assigndiagnose = maxNumericID + 1;
                }
            }

            delete rs;
            delete stmt;
            delete con;
        }
        catch (SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
        return "REPORT" + to_string(assigndiagnose++);
    }

    void addDiagnosisReport(const string& doctorID) {
        string patientID, diagnosis, medicines, precautions, tests;
        cout << "Enter the patient ID: ";
        cin >> patientID;
        cout << "Enter diagnosis details: ";
        cin.ignore();
        getline(cin, diagnosis);
        cout << "Enter prescribed medicines: ";
        getline(cin, medicines);
        cout << "Enter precautions: ";
        getline(cin, precautions);
        cout << "Enter recommended tests: ";
        getline(cin, tests);

        try {
            string getid;
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* psmt = con->prepareStatement("INSERT INTO diagnosis_reports (report_id,patient_id, doctor_id, diagnosis, medicines, precautions, tests) VALUES (?, ?, ?, ?, ?, ?,?)");
            psmt->setString(1, patientID);
            psmt->setString(2, patientID);
            psmt->setString(3, doctorID);
            psmt->setString(4, diagnosis);
            psmt->setString(5, medicines);
            psmt->setString(6, precautions);
            psmt->setString(7, tests);

            int rowsAffected = psmt->executeUpdate();
            if (rowsAffected > 0)
                cout << "Diagnosis report submitted successfully!" << endl;
            else
                cerr << "Error: No rows affected." << endl;

            delete psmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e) {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }

    void displayDiagnosisReport(const string& patientID) {
        try {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* psmt = con->prepareStatement("SELECT * FROM diagnosis_reports WHERE patient_id = ?");
            psmt->setString(1, patientID);
            ResultSet* rs = psmt->executeQuery();

            while (rs->next()) {
                cout << "Report ID: " << rs->getInt("report_id") << endl;
                cout << "Patient ID: " << rs->getString("patient_id") << endl;
                cout << "Doctor ID: " << rs->getString("doctor_id") << endl;
                cout << "Diagnosis: " << rs->getString("diagnosis") << endl;
                cout << "Medicines: " << rs->getString("medicines") << endl;
                cout << "Precautions: " << rs->getString("precautions") << endl;
                cout << "Tests: " << rs->getString("tests") << endl;
                cout << "Date: " << rs->getString("date") << endl;
            }

            delete rs;
            delete psmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e) {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }
};
int diagnosisreport::assigndiagnose = 1;
class laboratory
{
    static int assignlab;
    string cnic;
    string testname, result;
public:

    static string generatelab()
    {
        try
        {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            Statement* stmt = con->createStatement();
            ResultSet* rs = stmt->executeQuery("SELECT report_id FROM laboratory_reports ORDER BY report_id DESC LIMIT 1");
            if (rs->next())
            {
                string maxID = rs->getString(1);
                if (!maxID.empty())
                {
                    int maxNumericID = stoi(maxID.substr(3));
                    assignlab = maxNumericID + 1;
                }
            }

            delete rs;
            delete stmt;
            delete con;
        }
        catch (SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
        return "LAB" + to_string(assignlab++);
    }

    void addLabTest()
    {
        string cnic;
        cout << "Enter the patient's CNIC: ";
        cin >> cnic;
        cout << "Enter the test name: ";
        cin.ignore();
        getline(cin, testname);
        cout << "Enter the test result: ";
        getline(cin, result);

        try {
            string getid = generatelab();
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* psmt = con->prepareStatement("INSERT INTO laboratory_reports (report_id,patient_cnic, test_name, result) VALUES (?, ?, ?,?)");
            psmt->setString(1, getid);
            psmt->setString(2, cnic);
            psmt->setString(3, testname);
            psmt->setString(4, result);

            int rowsAffected = psmt->executeUpdate();
            if (rowsAffected > 0)
                cout << "Lab test added successfully!" << endl;
            else
                cerr << "Error: No rows affected." << endl;

            delete psmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e) {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }

    void viewLabReport() 
    {
        string patientCNIC;
        cout << "Enter the patient's CNIC: ";
        cin >> patientCNIC;

        try {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* psmt = con->prepareStatement("SELECT * FROM laboratory_reports WHERE patient_cnic = ?");
            psmt->setString(1, patientCNIC);
            ResultSet* rs = psmt->executeQuery();

            while (rs->next()) {
                cout << "Report ID: " << rs->getInt("report_id") << endl;
                cout << "Test Name: " << rs->getString("test_name") << endl;
                cout << "Result: " << rs->getString("result") << endl;
                cout << "Date: " << rs->getString("date") << endl;
                cout << "-----------------------------" << endl;
            }

            delete rs;
            delete psmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e) {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }
};

int laboratory::assignlab = 1;
class doctor
{
    string pass;
    static int assigndoctor;
    const string id;
    string cnic;
    string name;
    string fname;
    string dob;
    string gender;
    string degree;
    string number;
    string adress;
    string dept_id;

public:
    doctor(string i, string id="") :pass(i), id(i)
    {
        /*if (!checkDoctorIdExists(i)) {
            cerr << "Error: Doctor ID does not exist in the database. Exiting program." << endl;
            exit(EXIT_FAILURE);
        }*/

    }

    bool checkDoctorIdExists(const string& doctorId) {
        bool exists = false;
        try {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");

            PreparedStatement* pstmt = con->prepareStatement("SELECT COUNT(*) FROM doctors WHERE ID = ?");
            pstmt->setString(1, doctorId);
            ResultSet* rs = pstmt->executeQuery();

            if (rs->next()) {
                int count = rs->getInt(1);
                if (count > 0) {
                    exists = true;
                }
            }

            delete rs;
            delete pstmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        return exists;
    }

    void writeDiagnosisReport() {
        string patientID, diagnosis, medicines, precautions, tests;
        cout << "Enter the patient ID: ";
        cin >> patientID;
        cout << "Enter diagnosis details: ";
        cin.ignore();
        getline(cin, diagnosis);
        cout << "Enter prescribed medicines: ";
        getline(cin, medicines);
        cout << "Enter precautions: ";
        getline(cin, precautions);
        cout << "Enter recommended tests: ";
        getline(cin, tests);

        try {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* psmt = con->prepareStatement("INSERT INTO diagnosis_reports (patient_id, doctor_id, diagnosis, medicines, precautions, tests) VALUES (?, ?, ?, ?, ?, ?)");
            psmt->setString(1, patientID);
            psmt->setString(2, id);
            psmt->setString(3, diagnosis);
            psmt->setString(4, medicines);
            psmt->setString(5, precautions);
            psmt->setString(6, tests);

            int rowsAffected = psmt->executeUpdate();
            if (rowsAffected > 0)
                cout << "Diagnosis report submitted successfully!" << endl;
            else
                cerr << "Error: No rows affected." << endl;

            delete psmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e) {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }
    static string generatedoctor()
    {
        try
        {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            Statement* stmt = con->createStatement();
            ResultSet* rs = stmt->executeQuery("SELECT id FROM doctors ORDER BY id DESC LIMIT 1");
            if (rs->next())
            {
                string maxID = rs->getString(1);
                if (!maxID.empty())
                {
                    int maxNumericID = stoi(maxID.substr(3));
                    assigndoctor = maxNumericID + 1;
                }
            }

            delete rs;
            delete stmt;
            delete con;
        }
        catch (SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
        return "DOC" + to_string(assigndoctor++);
    }

    void adddoctor()
    {
        cout << "Adding doctor details" << endl;
        cout << "Enter the CNIC of the doctor:";
        cin >> cnic;
        cout << "Enter the Name of the doctor:";
        cin >> name;
        cout << "Enter the father name of the doctor:";
        cin >> fname;
        cout << "Enter the date of birth of the doctor:";
        cin >> dob;
        cout << "Enter the gender of the doctor:";
        cin >> gender;
        cout << "Enter the degree of the doctor:";
        cin >> degree;
        cout << "Enter the number of the doctor:";
        cin >> number;
        cout << "Enter the address of the doctor:";
        cin >> adress;
        cout << "Enter the department ID for the doctor: ";
        cin >> dept_id;

        try
        {
            string regID = generatedoctor();
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* psmt;
            psmt = con->prepareStatement("insert into doctors(ID, cnic, name,fname, dob,gender,degree,number,adress) values (?,?,?,?,?,?,?,?,?)");
            psmt->setString(1, regID);
            psmt->setString(2, cnic);
            psmt->setString(3, name);
            psmt->setString(4, fname);
            psmt->setString(5, dob);
            psmt->setString(6, gender);
            psmt->setString(7, degree);
            psmt->setString(8, number);
            psmt->setString(9, adress);
            psmt->setString(10, dept_id);

            int rowsAffected = psmt->executeUpdate();
            if (rowsAffected > 0)
                cout << "Doctor details inserted successfully!" << endl;
            else
                cerr << "Error: No rows affected." << endl;
            delete psmt;
            delete con;

        }
        catch (SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e)
        {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }

    void doctorMenu()
    {
        int choice;
        patient p;
        diagnosisreport dr;
        do {
            cout << "\nDoctor Menu:\n";
            cout << "1. View Waiting Patients\n";
            cout << "2. Mark Patient as Checked\n";
            cout << "3. Refer Patient to Another Doctor\n";
            cout << "4. Write Diagnosis Report\n";
            cout << "5. View Diagnosis Report\n";
            cout << "6. Check the lab report\n";
            cout << "Enter 7 to check your appointments:";
            cout << "8. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
            case 1: {
                string doctorID;
                cout << "Enter your doctor ID: ";
                cin >> doctorID;
                patient::listWaitingPatients(doctorID);
                break;
            }
            case 2: {
                string patientID;
                cout << "Enter the patient ID to mark as checked: ";
                cin >> patientID;
                patient::markAsChecked(patientID);
                break;
            }
            case 3:
                p.referToAnotherDoctor();
                break;
            case 4:
                dr.addDiagnosisReport(id);
                break;
            case 5: {
                string patientID;
                cout << "Enter the patient ID to view the report: ";
                cin >> patientID;
                dr.displayDiagnosisReport(patientID);
                break;
            }
            case 6:
            {
                laboratory l;
                l.viewLabReport();
               
                break;
            }
            case 7:
            {
                Appointment p;
                p.listAppointmentsByDoctorId();
                break;
            }
            case 8:
            {
                cout << "Exiting doctor menu.\n";
                break;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 6);
    }

};

int doctor::assigndoctor = 1;


class department
{
    static int assignid;
    const string dept_id;
    string dept_name;

public:

    department(string i = "") :dept_id(i) {}
    static string generatedepartment()
    {
        try
        {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            Statement* stmt = con->createStatement();
            ResultSet* rs = stmt->executeQuery("SELECT dept_id FROM department ORDER BY dept_id DESC LIMIT 1");
            if (rs->next())
            {
                string maxID = rs->getString(1);
                if (!maxID.empty())
                {
                    int maxNumericID = stoi(maxID.substr(4));
                    assignid = maxNumericID + 1;
                }
            }

            delete rs;
            delete stmt;
            delete con;
        }
        catch (SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
        return "DEPT" + to_string(assignid++);
    }

    void adddepartment()
    {
        cout << "Adding department details" << endl;

        cout << "Enter the Name of the Department:";
        cin >> dept_name;

        try
        {
            string regID = generatedepartment();
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* psmt;
            psmt = con->prepareStatement("insert into department(dept_id, dept_name) values (?,?)");
            psmt->setString(1, regID);
            psmt->setString(2, dept_name);

            int rowsAffected = psmt->executeUpdate();
            if (rowsAffected > 0)
                cout << "Department details inserted successfully!" << endl;
            else
                cerr << "Error: No rows affected." << endl;
            delete psmt;
            delete con;

        }
        catch (SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e)
        {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }

};

int department::assignid = 1;



class paramedicalstaff
{
    static int assignstaff;
    const string ID;
    string cnic;
    string name;
    string fname;
    string dob;
    string gender;
    string number;
    string adress;

public:


    paramedicalstaff(string id) :ID(id) {}
    static string generatestaff()
    {
        try
        {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            Statement* stmt = con->createStatement();
            ResultSet* rs = stmt->executeQuery("SELECT ID FROM satff ORDER BY ID DESC LIMIT 1");
            if (rs->next())
            {
                string maxID = rs->getString(1);
                if (!maxID.empty())
                {
                    int maxNumericID = stoi(maxID.substr(3));
                    assignstaff = maxNumericID + 1;
                }
            }
            delete rs;
            delete stmt;
            delete con;
        }
        catch (SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
        return "LAB" + to_string(assignstaff++);
    }

    void addstaff()
    {
        cout << "Adding doctor details" << endl;
        cout << "Enter the CNIC :";
        cin >> cnic;
        cout << "Enter the Name :";
        cin >> name;
        cout << "Enter the father name:";
        cin >> fname;
        cout << "Enter the date of birth:";
        cin >> dob;
        cout << "Enter the gender:";
        cin >> gender;
       
        cout << "Enter the number:";
        cin >> number;
        cout << "Enter the address of the doctor:";
        cin >> adress;
        

        try
        {
            string regID = generatestaff();
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* psmt;
            psmt = con->prepareStatement("insert into satff(ID, cnic, name,fname, dob,gender,number,adress) values (?,?,?,?,?,?,?,?)");
            psmt->setString(1, regID);
            psmt->setString(2, cnic);
            psmt->setString(3, name);
            psmt->setString(4, fname);
            psmt->setString(5, dob);
            psmt->setString(6, gender);
            psmt->setString(7, number);
            psmt->setString(8, adress);
            


            int rowsAffected = psmt->executeUpdate();
            if (rowsAffected > 0)
                cout << "Doctor details inserted successfully!" << endl;
            else
                cerr << "Error: No rows affected." << endl;
            delete psmt;
            delete con;

        }
        catch (SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
        catch (runtime_error& e)
        {
            cerr << "Runtime Error: " << e.what() << endl;
        }
    }

};
int paramedicalstaff::assignstaff = 1;


class pharmacy 
{
    static int assignpharmacy;
    const string id;
    string name;
    string expiry_date;
    int quantity;

public:

    pharmacy(string id = "1") :id(id) {}
    static string generatemedicine()
    {
        try
        {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            Statement* stmt = con->createStatement();
            ResultSet* rs = stmt->executeQuery("SELECT id FROM medicines ORDER BY id DESC LIMIT 1");
            if (rs->next())
            {
                string maxID = rs->getString(1);
                if (!maxID.empty())
                {
                    int maxNumericID = stoi(maxID.substr(3));
                    assignpharmacy = maxNumericID + 1;
                }
            }
            delete rs;
            delete stmt;
            delete con;
        }
        catch (SQLException& e)
        {
            cerr << "SQL Error: " << e.what() << endl;
        }
        return "LAB" + to_string(assignpharmacy++);
    }
    void addMedicine() 
    {
        cout << "Enter the name of the medicine :";
        cin >> name;
        cout << "Enter the expiry date of the medicine:";
        cin >> expiry_date;
        cout << "Enter the quantity of the medicine:";
        cin >> quantity;
        try {
            string idd = generatemedicine();
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* pstmt = con->prepareStatement("INSERT INTO medicines (id,name, expiry_date, quantity) VALUES (?,?, ?, ?)");
            pstmt->setString(1, idd);
            pstmt->setString(1, name);
            pstmt->setString(2, expiry_date);
            pstmt->setInt(3, quantity);
            pstmt->executeUpdate();

            cout << "Medicine added successfully!" << endl;

            delete pstmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }

    void updateMedicineQuantity(const string& medicineName, int newQuantity) {
        try {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* pstmt = con->prepareStatement("UPDATE medicines SET quantity = ? WHERE name = ?");
            pstmt->setInt(1, newQuantity);
            pstmt->setString(2, medicineName);
            pstmt->executeUpdate();

            cout << "Medicine quantity updated successfully!" << endl;

            delete pstmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }

    void retrieveMedicines() {
        try {
            Driver* driver = get_driver_instance();
            Connection* con = driver->connect("tcp://localhost:3306/hospital", "root", "Haji1122@");
            PreparedStatement* pstmt = con->prepareStatement("SELECT name, expiry_date, quantity FROM medicines");
            ResultSet* rs = pstmt->executeQuery();

            cout << "Medicines available in pharmacy:" << endl;
            while (rs->next()) {
                cout << "Name: " << rs->getString("name") << ", Expiry Date: " << rs->getString("expiry_date") << ", Quantity: " << rs->getInt("quantity") << endl;
            }

            delete rs;
            delete pstmt;
            delete con;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }
};

int pharmacy::assignpharmacy = 1;

int main()
{
    cout << "Enter your password:";
    string pass;
    cin >> pass;
    if (pass == "admin123")
    {
        cout << "Enter 1 to go to the patient database\n";
            cout << "Enter 2 to go to the Doctor database\n";
            cout << "Enter 3 to go to the Staff database\n";
            cout << "Enter 4 to go to the Pharmacy database\n";
            cout << "Enter 5 to go to the Laboratory database\n";
            cout << "Enter 6 to check the appointments\n";
            cout << "Enter 7 to go to the medical history\n";
            cout << "Enter your choice:";
            cin >> choice;
            switch (choice)
            {
            case 1:
            {
                patient p;
                p.addregular();
                break;
            }
            case 2:
            {
                
                doctor d("2","1");
                d.adddoctor();
                break;
                
                
            }
            case 3:
            {
                paramedicalstaff p("id");
                p.addstaff();
                break;
                
            }
            case 4:
            {
                cout << "Enter 1 to add medicine\n";
                cout << "Enter 2 to retrive medicine\n";
                cout << "Enter 3 to update medicine\n";
                cin >> choice;
                pharmacy p;
                switch (choice)
                {
                case 1:
                {
                    p.addMedicine();
                }
                case 2:
                {
                    p.retrieveMedicines();
                }
                case 3:
                {
                    string str;
                    cout << "enter the name of the medicine\n";
                    cin >> str;
                    cout << "Enter the quantity if the medicine\n";
                    int a;
                    cin >> a;

                    p.updateMedicineQuantity(str,a);
                }
                }
               
                
                
            }
            case 5:
            {
                int a;
                laboratory l;
                cout << "Enter 1 to add lab task\n";
                cout << "Enter 2 to lab report\n";
                cin >> a;
                switch (a)
                {
                case 1:
                {
                    l.addLabTest();
                    break;
                }
                case 2:
                {
                    l.viewLabReport();
                    break;
                }
                }
               
               
            }
            case 6:
            {
                Appointment a;
                 int c;
                cout << "Enter 1 to add appointment\n";
                cout << "Enter 2 to check doctor appointments \n";
                cout << "Enter 3 to check patient appoitments\n";
                cout << "Enter your choice:";
                cin >> c;
                switch (c)
                {
                case 1:
                {
                    a.addAppointment();
                    break;
                }

                case 2:
                {
                    a.listAppointmentsByDoctorId();
                    break;
                }
                case 3:
                {
                    a.listAppointmentsByPatientId();
                    break;
                }
                }
            }
           /* case 7:
            {
                diagnosisreport d;
                d.addDiagnosisReport();
                d.displayDiagnosisReport();
                break;
            }*/
            }
    }
    else if (pass == "reception123")
    {
        do
        {
            cout << "Enter 1 to register patient\n";
            cout << "Enter 2 to check appoinment\n";
            cout << "Enter 3 to add appoitment\n";

            cout << "Enter your choice:";
            cin >> choice;
            switch (choice)
            {
            case 1:
            {
                patient p;
                p.addregular();
            }
            case 2:
            {
                Appointment p;
                p.listAppointmentsByPatientId();
                break;
            }
            case 3:
            {
                Appointment p;
                p.addAppointment();
                break;
            }

            }
        } while (choice != 0);
    }
    else if (pass == "doctorf22raptor")
    {
        string id;
        cout << "Enter Your id :";
        cin >> id;
        doctor d(id, "");
        d.doctorMenu();
    }
    else if (pass == "lab")
    {
        laboratory l;
        l.addLabTest();
    }
    else if (pass == "medicine")
    {
        pharmacy p;
        cout << "Enter 1 to add medicine \n";
        cout << "Enter 2 to retrive medicines from the database\n";
        cout << "Enter 3 to update any medicine\n";
        cout << "Enter your choice:";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            p.addMedicine();
            break;
        }
        case 2:
        {
            p.retrieveMedicines();
            break;
        }
        case 3:
        {
            string name;
            int quantity;
            cout << "Enter the name of the medicine:";
            cin >> name;
            cout << "Enter the quantity of the medicine:";
            cin >> quantity;
            p.updateMedicineQuantity(name, quantity);
        }
      }  
    }
    else
    {
        cout << "Wrong Password.... Exiting\n";
    }
    return 0;
}



