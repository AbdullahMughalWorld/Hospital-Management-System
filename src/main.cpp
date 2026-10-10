#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Hospital Management System | C++17
// OOP concepts: encapsulation, inheritance, polymorphism, abstraction,
// composition, and file handling.

namespace {
const string DATA_DIR = "data/";
const string PATIENTS_FILE = DATA_DIR + "patients.txt";
const string DOCTORS_FILE = DATA_DIR + "doctors.txt";
const string APPOINTMENTS_FILE = DATA_DIR + "appointments.txt";
const string BILLS_FILE = DATA_DIR + "bills.txt";

string cleanField(string value) {
    replace(value.begin(), value.end(), '|', '/');
    replace(value.begin(), value.end(), '\n', ' ');
    replace(value.begin(), value.end(), '\r', ' ');
    return value;
}

vector<string> split(const string& line, char delimiter = '|') {
    vector<string> fields;
    string field;
    stringstream stream(line);
    while (getline(stream, field, delimiter)) fields.push_back(field);
    return fields;
}

int readInt(const string& prompt, int minimum, int maximum) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) return minimum;
        stringstream stream(line);
        int value;
        char extra;
        if ((stream >> value) && !(stream >> extra) && value >= minimum && value <= maximum)
            return value;
        cout << "Invalid input. Enter a number from " << minimum << " to " << maximum << ".\n";
    }
}

double readDouble(const string& prompt, double minimum, double maximum) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) return minimum;
        stringstream stream(line);
        double value;
        char extra;
        if ((stream >> value) && !(stream >> extra) && value >= minimum && value <= maximum)
            return value;
        cout << "Invalid input. Enter a value from " << minimum << " to " << maximum << ".\n";
    }
}

string readText(const string& prompt) {
    while (true) {
        cout << prompt;
        string value;
        if (!getline(cin, value)) return "";
        if (value.find_first_not_of(" \t") != string::npos) return cleanField(value);
        cout << "This field cannot be empty.\n";
    }
}

void printLine(char c = '-', int width = 78) {
    cout << string(width, c) << '\n';
}
}

class Person {
protected:
    int id;
    string name;
    int age;
    string phone;
public:
    Person(int personId, string personName, int personAge, string personPhone)
        : id(personId), name(cleanField(personName)), age(personAge), phone(cleanField(personPhone)) {}
    virtual ~Person() = default;

    int getId() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }
    string getPhone() const { return phone; }

    virtual string getRole() const = 0;
    virtual void display() const = 0;
};

class Patient : public Person {
private:
    string condition;
public:
    Patient(int patientId, string patientName, int patientAge, string patientPhone, string patientCondition)
        : Person(patientId, patientName, patientAge, patientPhone),
          condition(cleanField(patientCondition)) {}

    string getCondition() const { return condition; }
    string getRole() const override { return "Patient"; }

    void display() const override {
        cout << left << setw(8) << id << setw(23) << name << setw(7) << age
             << setw(18) << phone << condition << '\n';
    }

    string serialize() const {
        ostringstream out;
        out << id << '|' << name << '|' << age << '|' << phone << '|' << condition;
        return out.str();
    }
};

class Doctor : public Person {
private:
    string specialization;
    double consultationFee;
public:
    Doctor(int doctorId, string doctorName, int doctorAge, string doctorPhone,
           string doctorSpecialization, double fee)
        : Person(doctorId, doctorName, doctorAge, doctorPhone),
          specialization(cleanField(doctorSpecialization)), consultationFee(fee) {}

    string getSpecialization() const { return specialization; }
    double getConsultationFee() const { return consultationFee; }
    string getRole() const override { return "Doctor"; }

    void display() const override {
        cout << left << setw(8) << id << setw(23) << name << setw(7) << age
             << setw(18) << phone << setw(22) << specialization
             << right << fixed << setprecision(2) << consultationFee << '\n';
    }

    string serialize() const {
        ostringstream out;
        out << id << '|' << name << '|' << age << '|' << phone << '|'
            << specialization << '|' << fixed << setprecision(2) << consultationFee;
        return out.str();
    }
};

struct Appointment {
    int id{};
    int patientId{};
    int doctorId{};
    string date;       // YYYY-MM-DD
    string time;       // HH:MM (24-hour format)
    string status{"Scheduled"};

    string serialize() const {
        ostringstream out;
        out << id << '|' << patientId << '|' << doctorId << '|'
            << cleanField(date) << '|' << cleanField(time) << '|' << status;
        return out.str();
    }
};

struct Bill {
    int id{};
    int patientId{};
    string description;
    double amount{};
    bool paid{false};

    string serialize() const {
        ostringstream out;
        out << id << '|' << patientId << '|' << cleanField(description) << '|'
            << fixed << setprecision(2) << amount << '|' << (paid ? 1 : 0);
        return out.str();
    }
};

class Hospital {
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    vector<Appointment> appointments;
    vector<Bill> bills;
    int nextPatientId = 1001;
    int nextDoctorId = 501;
    int nextAppointmentId = 2001;
    int nextBillId = 3001;

    Patient* findPatient(int patientId) {
        for (auto& patient : patients) if (patient.getId() == patientId) return &patient;
        return nullptr;
    }

    Doctor* findDoctor(int doctorId) {
        for (auto& doctor : doctors) if (doctor.getId() == doctorId) return &doctor;
        return nullptr;
    }

    Appointment* findAppointment(int appointmentId) {
        for (auto& appointment : appointments) if (appointment.id == appointmentId) return &appointment;
        return nullptr;
    }

    Bill* findBill(int billId) {
        for (auto& bill : bills) if (bill.id == billId) return &bill;
        return nullptr;
    }

    void savePatients() const {
        ofstream file(PATIENTS_FILE);
        if (!file) throw runtime_error("Could not save patient data.");
        for (const auto& patient : patients) file << patient.serialize() << '\n';
    }

    void saveDoctors() const {
        ofstream file(DOCTORS_FILE);
        if (!file) throw runtime_error("Could not save doctor data.");
        for (const auto& doctor : doctors) file << doctor.serialize() << '\n';
    }

    void saveAppointments() const {
        ofstream file(APPOINTMENTS_FILE);
        if (!file) throw runtime_error("Could not save appointment data.");
        for (const auto& appointment : appointments) file << appointment.serialize() << '\n';
    }

    void saveBills() const {
        ofstream file(BILLS_FILE);
        if (!file) throw runtime_error("Could not save bill data.");
        for (const auto& bill : bills) file << bill.serialize() << '\n';
    }

    void saveAll() const {
        savePatients();
        saveDoctors();
        saveAppointments();
        saveBills();
    }

    void loadPatients() {
        ifstream file(PATIENTS_FILE);
        string line;
        while (getline(file, line)) {
            auto f = split(line);
            if (f.size() != 5) continue;
            try {
                int id = stoi(f[0]), age = stoi(f[2]);
                patients.emplace_back(id, f[1], age, f[3], f[4]);
                nextPatientId = max(nextPatientId, id + 1);
            } catch (...) {}
        }
    }

    void loadDoctors() {
        ifstream file(DOCTORS_FILE);
        string line;
        while (getline(file, line)) {
            auto f = split(line);
            if (f.size() != 6) continue;
            try {
                int id = stoi(f[0]), age = stoi(f[2]);
                double fee = stod(f[5]);
                doctors.emplace_back(id, f[1], age, f[3], f[4], fee);
                nextDoctorId = max(nextDoctorId, id + 1);
            } catch (...) {}
        }
    }

    void loadAppointments() {
        ifstream file(APPOINTMENTS_FILE);
        string line;
        while (getline(file, line)) {
            auto f = split(line);
            if (f.size() != 6) continue;
            try {
                Appointment a;
                a.id = stoi(f[0]); a.patientId = stoi(f[1]); a.doctorId = stoi(f[2]);
                a.date = f[3]; a.time = f[4]; a.status = f[5];
                appointments.push_back(a);
                nextAppointmentId = max(nextAppointmentId, a.id + 1);
            } catch (...) {}
        }
    }

    void loadBills() {
        ifstream file(BILLS_FILE);
        string line;
        while (getline(file, line)) {
            auto f = split(line);
            if (f.size() != 5) continue;
            try {
                Bill b;
                b.id = stoi(f[0]); b.patientId = stoi(f[1]); b.description = f[2];
                b.amount = stod(f[3]); b.paid = (stoi(f[4]) == 1);
                bills.push_back(b);
                nextBillId = max(nextBillId, b.id + 1);
            } catch (...) {}
        }
    }

    bool appointmentSlotTaken(int doctorId, const string& date, const string& time) const {
        for (const auto& a : appointments) {
            if (a.doctorId == doctorId && a.date == date && a.time == time && a.status == "Scheduled")
                return true;
        }
        return false;
    }

    void addPatient() {
        cout << "\n--- Register Patient ---\n";
        string name = readText("Full name: ");
        int age = readInt("Age (1-120): ", 1, 120);
        string phone = readText("Phone number: ");
        string condition = readText("Reason / condition (brief): ");
        patients.emplace_back(nextPatientId++, name, age, phone, condition);
        saveAll();
        cout << "Patient registered. Patient ID: " << patients.back().getId() << "\n";
    }

    void listPatients() const {
        cout << "\n--- Patient Records ---\n";
        if (patients.empty()) { cout << "No patients registered yet.\n"; return; }
        cout << left << setw(8) << "ID" << setw(23) << "Name" << setw(7) << "Age"
             << setw(18) << "Phone" << "Condition\n";
        printLine();
        for (const auto& patient : patients) patient.display();
    }

    void addDoctor() {
        cout << "\n--- Add Doctor ---\n";
        string name = readText("Doctor name: ");
        int age = readInt("Age (23-100): ", 23, 100);
        string phone = readText("Phone number: ");
        string specialization = readText("Specialization: ");
        double fee = readDouble("Consultation fee (PKR, 0-1000000): ", 0, 1000000);
        doctors.emplace_back(nextDoctorId++, name, age, phone, specialization, fee);
        saveAll();
        cout << "Doctor added. Doctor ID: " << doctors.back().getId() << "\n";
    }

    void listDoctors() const {
        cout << "\n--- Doctor Directory ---\n";
        if (doctors.empty()) { cout << "No doctors added yet.\n"; return; }
        cout << left << setw(8) << "ID" << setw(23) << "Name" << setw(7) << "Age"
             << setw(18) << "Phone" << setw(22) << "Specialization" << "Fee (PKR)\n";
        printLine();
        for (const auto& doctor : doctors) doctor.display();
    }

    void bookAppointment() {
        cout << "\n--- Schedule Appointment ---\n";
        if (patients.empty() || doctors.empty()) {
            cout << "Register at least one patient and add at least one doctor first.\n";
            return;
        }
        listPatients();
        int patientId = readInt("Patient ID: ", 1, 1000000000);
        if (!findPatient(patientId)) { cout << "Patient ID not found.\n"; return; }

        listDoctors();
        int doctorId = readInt("Doctor ID: ", 1, 1000000000);
        if (!findDoctor(doctorId)) { cout << "Doctor ID not found.\n"; return; }

        string date = readText("Appointment date (YYYY-MM-DD): ");
        string time = readText("Appointment time (HH:MM, 24-hour): ");
        if (date.size() != 10 || date[4] != '-' || date[7] != '-' ||
            time.size() != 5 || time[2] != ':') {
            cout << "Please use date YYYY-MM-DD and time HH:MM format.\n";
            return;
        }
        if (appointmentSlotTaken(doctorId, date, time)) {
            cout << "That doctor already has a scheduled appointment at this time.\n";
            return;
        }
        Appointment a{nextAppointmentId++, patientId, doctorId, date, time, "Scheduled"};
        appointments.push_back(a);
        saveAll();
        cout << "Appointment booked. Appointment ID: " << a.id << "\n";
    }

    void listAppointments() const {
        cout << "\n--- Appointments ---\n";
        if (appointments.empty()) { cout << "No appointments found.\n"; return; }
        cout << left << setw(10) << "Appt ID" << setw(12) << "Patient ID"
             << setw(11) << "Doctor ID" << setw(14) << "Date" << setw(10) << "Time" << "Status\n";
        printLine();
        for (const auto& a : appointments) {
            cout << left << setw(10) << a.id << setw(12) << a.patientId << setw(11) << a.doctorId
                 << setw(14) << a.date << setw(10) << a.time << a.status << '\n';
        }
    }

    void cancelAppointment() {
        listAppointments();
        if (appointments.empty()) return;
        int id = readInt("Appointment ID to cancel: ", 1, 1000000000);
        Appointment* a = findAppointment(id);
        if (!a) { cout << "Appointment not found.\n"; return; }
        if (a->status != "Scheduled") { cout << "Only scheduled appointments can be cancelled.\n"; return; }
        a->status = "Cancelled";
        saveAll();
        cout << "Appointment cancelled.\n";
    }

    void createBill() {
        cout << "\n--- Create Patient Bill ---\n";
        if (patients.empty()) { cout << "Register a patient first.\n"; return; }
        listPatients();
        int patientId = readInt("Patient ID: ", 1, 1000000000);
        if (!findPatient(patientId)) { cout << "Patient ID not found.\n"; return; }
        string description = readText("Bill description: ");
        double amount = readDouble("Amount in PKR (0-100000000): ", 0, 100000000);
        Bill bill{nextBillId++, patientId, description, amount, false};
        bills.push_back(bill);
        saveAll();
        printLine('=');
        cout << "                 HOSPITAL BILL\n";
        printLine('=');
        cout << "Bill ID:       " << bill.id << "\nPatient ID:    " << bill.patientId
             << "\nDescription:   " << bill.description << "\nAmount:        PKR "
             << fixed << setprecision(2) << bill.amount << "\nPayment:       UNPAID\n";
        printLine('=');
    }

    void listBills() const {
        cout << "\n--- Billing Records ---\n";
        if (bills.empty()) { cout << "No bills found.\n"; return; }
        cout << left << setw(10) << "Bill ID" << setw(13) << "Patient ID"
             << setw(30) << "Description" << setw(16) << "Amount (PKR)" << "Status\n";
        printLine();
        for (const auto& b : bills) {
            cout << left << setw(10) << b.id << setw(13) << b.patientId << setw(30)
                 << b.description.substr(0, 28) << setw(16) << fixed << setprecision(2)
                 << b.amount << (b.paid ? "Paid" : "Unpaid") << '\n';
        }
    }

    void markBillPaid() {
        listBills();
        if (bills.empty()) return;
        int id = readInt("Bill ID to mark as paid: ", 1, 1000000000);
        Bill* bill = findBill(id);
        if (!bill) { cout << "Bill not found.\n"; return; }
        if (bill->paid) { cout << "This bill is already marked as paid.\n"; return; }
        bill->paid = true;
        saveAll();
        cout << "Payment status updated to PAID.\n";
    }

    void dashboard() const {
        int scheduled = 0, unpaid = 0;
        double unpaidTotal = 0;
        for (const auto& a : appointments) if (a.status == "Scheduled") ++scheduled;
        for (const auto& b : bills) if (!b.paid) { ++unpaid; unpaidTotal += b.amount; }

        cout << "\n";
        printLine('=');
        cout << "                  HOSPITAL DASHBOARD\n";
        printLine('=');
        cout << "Registered patients : " << patients.size() << '\n';
        cout << "Listed doctors      : " << doctors.size() << '\n';
        cout << "Scheduled visits    : " << scheduled << '\n';
        cout << "Total bills         : " << bills.size() << '\n';
        cout << "Unpaid bills        : " << unpaid << '\n';
        cout << "Unpaid balance      : PKR " << fixed << setprecision(2) << unpaidTotal << '\n';
        printLine('=');
    }

public:
    Hospital() {
        loadPatients();
        loadDoctors();
        loadAppointments();
        loadBills();
    }

    void run() {
        while (true) {
            cout << "\n";
            printLine('=');
            cout << "            HOSPITAL MANAGEMENT SYSTEM\n";
            cout << "                 C++17 | OOP Project\n";
            printLine('=');
            cout << " 1. Register patient\n"
                 << " 2. View patients\n"
                 << " 3. Add doctor\n"
                 << " 4. View doctors\n"
                 << " 5. Schedule appointment\n"
                 << " 6. View appointments\n"
                 << " 7. Cancel appointment\n"
                 << " 8. Create bill\n"
                 << " 9. View bills\n"
                 << "10. Mark bill as paid\n"
                 << "11. Dashboard\n"
                 << " 0. Save and exit\n";
            printLine();
            int choice = readInt("Choose an option: ", 0, 11);
            try {
                switch (choice) {
                    case 1: addPatient(); break;
                    case 2: listPatients(); break;
                    case 3: addDoctor(); break;
                    case 4: listDoctors(); break;
                    case 5: bookAppointment(); break;
                    case 6: listAppointments(); break;
                    case 7: cancelAppointment(); break;
                    case 8: createBill(); break;
                    case 9: listBills(); break;
                    case 10: markBillPaid(); break;
                    case 11: dashboard(); break;
                    case 0:
                        saveAll();
                        cout << "Data saved. Thank you for using the system!\n";
                        return;
                }
            } catch (const exception& ex) {
                cerr << "Operation failed: " << ex.what() << '\n';
            }
        }
    }
};

int main() {
    try {
        Hospital hospital;
        hospital.run();
    } catch (const exception& ex) {
        cerr << "Fatal error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
