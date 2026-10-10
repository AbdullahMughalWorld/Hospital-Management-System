# Hospital Management System (C++)

A console-based Hospital Management System built with **C++17** for an Object-Oriented Programming (OOP) university project. It demonstrates how classes, inheritance, encapsulation, polymorphism, and file handling can be used in a practical management application.

> **Project scope:** This is an educational demo. It is not intended for real clinical use and does not provide production-grade privacy, security, or medical-record safeguards.

## Features

- Register and view patients
- Add and view doctors with specializations and consultation fees
- Schedule appointments and prevent duplicate scheduled slots for the same doctor
- Cancel scheduled appointments
- Create bills and mark them as paid
- Dashboard with record counts and unpaid balance
- Persist records in text files so data remains after restarting the program
- Basic input validation and readable console menus

## OOP concepts demonstrated

| Concept | Example |
|---|---|
| Encapsulation | Person details are stored as protected fields; access is through methods |
| Inheritance | `Patient` and `Doctor` inherit from the abstract `Person` class |
| Abstraction | `Person` defines the common interface using pure virtual methods |
| Polymorphism | `getRole()` and `display()` are virtual and overridden in derived classes |
| Composition | `Hospital` owns collections of patients, doctors, appointments, and bills |
| File handling | Records are loaded from and saved to files in `data/` |

## Requirements

- A C++ compiler with C++17 support (GCC, Clang, or a recent MSVC)
- Windows, Linux, or macOS terminal

## Build and run

### Windows (MinGW g++)

Open PowerShell or Command Prompt in this folder:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o hospital.exe
.\hospital.exe
```

If using Command Prompt, run `hospital.exe`.

### Linux / macOS

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o hospital
./hospital
```

## First run

The program creates the text data files in `data/` when you save or exit. Keep the program's working directory at the project root so the relative `data/` path works correctly.

Suggested demo sequence:
1. Register a patient.
2. Add a doctor and note the displayed ID.
3. Schedule an appointment using the patient and doctor IDs.
4. Create a bill for the patient.
5. View the dashboard and mark the bill as paid.
6. Exit, reopen the app, and verify that records remain.

## Data files

- `data/patients.txt`
- `data/doctors.txt`
- `data/appointments.txt`
- `data/bills.txt`

These files use a simple pipe-delimited format for learning purposes. Avoid entering real patient information.

## Limitations and future improvements

- Date/time checks validate the format but do not fully validate calendar dates or opening hours.
- This is a single-user console app; it has no login roles, database, concurrency control, encryption, or audit trail.
- A future version could use SQLite, authentication, unit tests, appointment search, report export, and a graphical interface.

## Repository structure

```text
HospitalManagementSystem/
├── src/
│   └── main.cpp
├── data/
│   └── .gitkeep
├── .gitignore
├── LICENSE
└── README.md
```

## Author

Add your name, course, and GitHub profile here before publishing.
