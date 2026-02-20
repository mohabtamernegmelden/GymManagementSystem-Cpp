#include <iostream>
#include <string>
#include <cstdlib> // For system()
#include <set>     // For tracking available IDs
#include <fstream>
#include <sstream>

using namespace std;

// Function to clear the console screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int getIntegerInput() {
    int input;
    while (true) {
        cin >> input;
        if (cin.fail()) {  // If the input is not an integer
            cin.clear();  // Clear the error flag
            cin.ignore(1000, '\n');  // Ignore the invalid input
            cout << "Invalid input! Please enter an integer: ";
        }
        else {
            break;  // If valid, break out of the loop
        }
    }
    return input;
}

// Class for handling user login
class User {
private:
    string username;
    string password;

public:
    User(const string& user, const string& pass) : username(user), password(pass) {}

    bool login(const string& user, const string& pass) {
        return (username == user && password == pass);
    }
};

// Class to manage Member details
class Member {
protected:
    int id;
    string name;
    int age;
    string membershipType;
    double membershipCost;  // New attribute to store the cost of the membership

public:
    Member() : id(0), name("Unknown"), age(0), membershipType("Unknown"), membershipCost(0.0) {}

    Member(int newId, const string& newName, int newAge, const string& newMembershipType, double cost)
        : id(newId), name(newName), age(newAge), membershipType(newMembershipType), membershipCost(cost) {}

    void displayMember() const {
        cout << id << "\t" << name << "\t" << age << "\t" << membershipType 
             << "\t" << membershipCost << endl;  // Display cost
    }

    int getId() const { return id; }
    bool isEmpty() const { return id == 0; }
    string getMembershipType() const { return membershipType; }
    double getMembershipCost() const { return membershipCost; }  // Getter for cost
    string getName() const { return name; }
    int getAge() const { return age; }
};

// Class to manage Employee details
class Employee {
private:
    int id;
    string name;
    string jobTitle;
    double salary;

public:
    Employee() : id(0), name("Unknown"), jobTitle("Unknown"), salary(2000.0) {}

    Employee(int newId, const string& newName, const string& newJobTitle, double newSalary)
        : id(newId), name(newName), jobTitle(newJobTitle), salary(newSalary) {}

    void displayEmployee() const {
        cout << id << "\t" << name << "\t" << jobTitle << "\t \t" << salary << endl;
    }

    int getId() const { return id; }
    double getSalary() const { return salary; }
    string getName() const { return name; }
    string getJobTitle() const { return jobTitle; }
    bool isEmpty() const { return id == 0; }
};

// Class to manage the system
class GymManagementSystem {
private:
    static const int MAX_MEMBERS = 100; // Max number of members
    static const int MAX_EMPLOYEES = 50; // Max number of employees
    Member members[MAX_MEMBERS];
    Employee employees[MAX_EMPLOYEES];
    int memberCount;
    int employeeCount;
    double generalCost;  // Cost for general membership
    double privateCost;  // Cost for private membership
    set<int> availableEmployeeIds; // Track removed employee IDs
    set<int> availableMemberIds; // Track removed member IDs

public:
    GymManagementSystem() : memberCount(0), employeeCount(0), generalCost(500.0), privateCost(900.0)
    {
        loadMembers();
        loadEmployees();
        loadMembershipPrices();
    }

    void setGeneralCost(double cost) {
        generalCost = cost;
    }

    void setPrivateCost(double cost) {
        privateCost = cost;
    }

    // Function to get the next available ID for a new employee
    int getNextEmployeeId() {
        if (!availableEmployeeIds.empty()) {
            int id = *availableEmployeeIds.begin(); // Get the smallest available ID
            availableEmployeeIds.erase(availableEmployeeIds.begin()); // Remove it from the set
            return id;
        }
        else {
            return employeeCount + 1; // Assigns IDs starting from 1, based on count
        }
    }

    // Function to get the next available ID for a new member
    int getNextMemberId() {
        if (!availableMemberIds.empty()) {
            int id = *availableMemberIds.begin(); // Get the smallest available ID
            availableMemberIds.erase(availableMemberIds.begin()); // Remove it from the set
            return id;
        }
        else {
            return memberCount + 1; // Assigns IDs starting from 1, based on count
        }
    }

    void memberMenu() {
        while (true) {
            clearScreen();
            cout << "--- Member Management ---\n";
            cout << "Total Members: " << memberCount << endl;
            cout << "1. Add Member\n";
            cout << "2. Search Member\n";
            cout << "3. View Members\n";
            cout << "4. Edit Member\n";
            cout << "5. Remove Member\n"; // New option for removing member
            cout << "0. Back to Main Menu\n";
            cout << "Choose an option: ";

            int choice = getIntegerInput();  // Use the safe input function

            clearScreen();
            switch (choice) {
            case 1: addMember(); break;
            case 2: searchMember(); break;
            case 3: viewMembers(); break;
            case 4: editMember(); break;
            case 5: removeMember(); break; // Call to removeMember function
            case 0: return;
            default: cout << "Invalid option! Try again.\n";
            }
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    void addMember() {
    if (memberCount >= MAX_MEMBERS) {
        cout << "Cannot add more members. Maximum limit reached.\n";
        return;
    }

    int id = getNextMemberId();
    int age;
    string name, membershipType;
    double membershipCost;

    cout << "Enter Member Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Member Age: ";
    age=getIntegerInput();

    while (true) {
        cout << "Choose Membership Type:\n";
        cout << "1. General (Cost: " << generalCost << ")\n";
        cout << "2. Private (Cost: " << privateCost << ")\n";
        cout << "Enter your choice (1 or 2): ";
        int choice;
        choice=getIntegerInput();

        if (choice == 1) {
            membershipType = "General";
            membershipCost = generalCost;  // Store the current general cost
            break;
        } else if (choice == 2) {
            membershipType = "Private";
            membershipCost = privateCost;  // Store the current private cost
            break;
        } else {
            cout << "Invalid choice. Please enter 1 for General or 2 for Private.\n";
        }
    }

    members[memberCount] = Member(id, name, age, membershipType, membershipCost);
    memberCount++;

    saveMembers();
    cout << "Member added successfully! ID: " << id << endl;
    }

    void saveMembers() {
    ofstream file("members.txt", ios::trunc);

    if (file.is_open()) {
        for (int i = 0; i < memberCount; ++i) {
            if (!members[i].isEmpty()) {
                file << members[i].getId() << ","
                     << members[i].getName() << ","
                     << members[i].getAge() << ","
                     << members[i].getMembershipType() << ","
                     << members[i].getMembershipCost() << "\n";  // Save cost
            }
        }
        file.close();
    }
    }

    void loadMembers() {
    ifstream file("members.txt");
    string line;

    memberCount = 0;
    while (getline(file, line) && memberCount < MAX_MEMBERS) {
        stringstream ss(line);
        string idStr, name, ageStr, membershipType, costStr;

        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, ageStr, ',');
        getline(ss, membershipType, ',');
        getline(ss, costStr, ',');

        int id = stoi(idStr);
        int age = stoi(ageStr);
        double cost = stod(costStr);

        members[memberCount] = Member(id, name, age, membershipType, cost);
        memberCount++;
    }
    file.close();
    }
    void viewMembers() {
        if (memberCount == 0) {
            cout << "No members found.\n";
            return;
        }

        cout << "ID\tName\tAge\tMembership\n";
        cout << "------------------------------------\n";
        for (int i = 0; i < memberCount; ++i) {
            members[i].displayMember();
        }
    }

    void searchMember() {
        int searchId;
        cout << "Enter Member ID to search: ";
        searchId = getIntegerInput();

        bool found = false;
        for (int i = 0; i < memberCount; ++i) {
            if (members[i].getId() == searchId) {
                members[i].displayMember();
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Member with ID " << searchId << " not found.\n";
        }
    }

    void editMember() {
    int id;
    cout << "Enter Member ID to edit: ";
    cin >> id;

    for (int i = 0; i < memberCount; ++i) {
        if (members[i].getId() == id) {
            string name, membershipType;
            int age;
            double membershipCost;

            cout << "Enter New Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter New Age: ";
            age=getIntegerInput();
            cin.ignore();

            while (true) {
                cout << "Choose New Membership Type:\n";
                cout << "1. General (Cost: " << generalCost << ")\n";
                cout << "2. Private (Cost: " << privateCost << ")\n";
                cout << "Enter your choice (1 or 2): ";
                int choice;
                choice=getIntegerInput();
                cin.ignore();

                if (choice == 1) {
                    membershipType = "General";
                    membershipCost = generalCost; // Assign corresponding cost
                    break;
                } else if (choice == 2) {
                    membershipType = "Private";
                    membershipCost = privateCost; // Assign corresponding cost
                    break;
                } else {
                    cout << "Invalid choice. Please enter 1 for General or 2 for Private.\n";
                }
            }

            // Create the updated Member object with the cost
            members[i] = Member(id, name, age, membershipType, membershipCost);
            saveMembers(); // Save changes to file
            
            cout << "Member updated successfully!\n";
            return;
        }
    }

    cout << "Member not found!\n";
    }

    void removeMember() {
        int id;
        cout << "Enter Member ID to remove: ";
        id = getIntegerInput();

        bool found = false;
        for (int i = 0; i < memberCount; ++i) {
            if (members[i].getId() == id) {
                members[i] = Member(); // Remove member by resetting
                availableMemberIds.insert(id); // Track the ID as available
                --memberCount;
                found = true;
                saveMembers();
                cout << "Member with ID " << id << " removed.\n";
                break;
            }
        }

        if (!found) {
            cout << "Member not found.\n";
        }
    }


    void employeeMenu() {
        while (true) {
            clearScreen();
            cout << "--- Employee Management ---\n";
            cout << "1. Add Employee\n";
            cout << "2. Search Employee\n";
            cout << "3. View Employees\n";
            cout << "4. Change Employee Salary\n";
            cout << "5. Remove Employee\n";
            cout << "0. Back to Main Menu\n";
            cout << "Choose an option: ";

            int choice = getIntegerInput();  // Use the safe input function

            clearScreen();
            switch (choice) {
            case 1: addEmployee(); break;
            case 2: searchEmployee(); break;
            case 3: viewEmployees(); break;
            case 4: changeSalary(); break;
            case 5: removeEmployee(); break;
            case 0: return;
            default: cout << "Invalid option! Try again.\n";
            }
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    void addEmployee() {
        if (employeeCount >= MAX_EMPLOYEES) {
            cout << "Cannot add more employees. Maximum limit reached.\n";
            return;
        }

        int id = getNextEmployeeId(); // Get a unique ID for the new employee
        string name, jobTitle;
        double salary;

        cout << "Enter Employee Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Job Title: ";
        getline(cin, jobTitle);
        cout << "Enter Salary: ";
        cin >> salary;

        employees[employeeCount] = Employee(id, name, jobTitle, salary);
        employeeCount++;

        saveEmployees(); // Save data after adding a new employee
        cout << "Employee added successfully! ID: " << id << endl;
    }

    // Save employees to file
    void saveEmployees() {
        ofstream file("employees.txt", ios::trunc); // Use `ios::trunc` to overwrite the file

        if (file.is_open()) {
            for (int i = 0; i < employeeCount; ++i) {
                if (!employees[i].isEmpty()) {
                    file << employees[i].getId() << ","
                        << employees[i].getName() << ","
                        << employees[i].getJobTitle() << ","
                        << employees[i].getSalary() << "\n";
                }
            }
            file.close();
       
        }
      
    }

    // Load employees from file
    void loadEmployees() {
        ifstream file("employees.txt");
        string line;

        employeeCount = 0; // Reset employee count before loading
        while (getline(file, line) && employeeCount < MAX_EMPLOYEES) {
            stringstream ss(line);
            string idStr, name, jobTitle, salaryStr;

            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, jobTitle, ',');
            getline(ss, salaryStr, ',');

            int id = stoi(idStr);
            double salary = stod(salaryStr);

            employees[employeeCount] = Employee(id, name, jobTitle, salary);
            employeeCount++;
        }
        file.close();
    }

    void viewEmployees() {
        if (employeeCount == 0) {
            cout << "No employees found.\n";
            return;
        }

        cout << "ID\tName\tJob Title\tSalary\n";
        for (int i = 0; i < employeeCount; ++i) {
            if (!employees[i].isEmpty()) {
                employees[i].displayEmployee();
            }
        }
    }

    void searchEmployee() {
        int id;
        cout << "Enter Employee ID to search: ";
        id=getIntegerInput();

        for (int i = 0; i < employeeCount; ++i) {
            if (employees[i].getId() == id) {
                cout << "Employee found:\n";
                cout << "ID\tName\tJob Title\tSalary\n";
                employees[i].displayEmployee();
                return;
            }
        }

        cout << "Employee with ID " << id << " not found.\n";
    }

    // Function to change the salary of an employee by their ID

    void changeSalary() {
        int id;
        double newSalary;

        cout << "Enter Employee ID to change salary: ";
        id=getIntegerInput();

        // Search for the employee with the given ID
        for (int i = 0; i < employeeCount; ++i) {
            if (employees[i].getId() == id) {
                cout << "Enter new salary for employee " << id << ": ";
                cin >> newSalary;

                // Update the salary
                employees[i] = Employee(employees[i].getId(), employees[i].getName(),
                    employees[i].getJobTitle(), newSalary);

                saveEmployees(); // Save changes to file
                cout << "Salary updated successfully for Employee ID: " << id << endl;
                return;
            }
        }

        cout << "Employee with ID " << id << " not found.\n";
    }

    void removeEmployee() {
        int id;
        cout << "Enter Employee ID to remove: ";
        id=getIntegerInput();

        for (int i = 0; i < employeeCount; ++i) {
            if (employees[i].getId() == id) {
                // Shift elements to the left
                for (int j = i; j < employeeCount - 1; ++j) {
                    employees[j] = employees[j + 1]; // Move the next employee to current position
                }
                employees[employeeCount - 1] = Employee(); // Reset the last employee
                employeeCount--; // Reduce the employee count

                // Add the removed ID to the available IDs set
                availableEmployeeIds.insert(id);

                saveEmployees(); // Save data after removing an employee
                cout << "Employee removed successfully!\n";
                return;
            }
        }

        cout << "Employee not found!\n";
    }
    void saveMembershipPrices() {
        ofstream file("membership_prices.txt", ios::trunc); // Overwrite file

        if (file.is_open()) {
            file << generalCost << "\n" << privateCost << "\n";
            file.close();
         
        }
    }

    void loadMembershipPrices() {
        ifstream file("membership_prices.txt");
        if (file.is_open()) {
            file >> generalCost;
            file >> privateCost;
            file.close();
         
        }
       
        
    }

    void financeMenu() {
        while (true) {
            clearScreen();
            cout << "--- Finance ---\n";
            cout << "1. Set Membership Cost for General\n";
            cout << "2. Set Membership Cost for Private\n";
            cout << "3. Calculate Financials\n"; // New option to calculate financials
            cout << "0. Back to Main Menu\n";
            cout << "Choose an option: ";

            int choice = getIntegerInput();  // Use the safe input function

            clearScreen();
            switch (choice) {
            case 1: setGeneralCostMenu(); break;
            case 2: setPrivateCostMenu(); break;
            case 3: calculateFinancials(); break; // Call to calculateFinancials function
            case 0: return;
            default: cout << "Invalid option! Try again.\n";
            }
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    void setGeneralCostMenu() {
        double cost;
        cout << "Enter new General membership cost: ";
        cin >> cost;
        setGeneralCost(cost);
        cout << "General membership cost updated to " << cost << endl;
        saveMembershipPrices();
    }

    void setPrivateCostMenu() {
        double cost;
        cout << "Enter new Private membership cost: ";
        cin >> cost;
        setPrivateCost(cost);
        cout << "Private membership cost updated to " << cost << endl;
        saveMembershipPrices();
    }

    void calculateFinancials() {
    double totalIncome = 0.0;
    double totalOutcome = 0.0;

    // Calculate total income from members
    for (int i = 0; i < memberCount; ++i) {
        if (members[i].isEmpty()) continue; // Skip empty members
        totalIncome += members[i].getMembershipCost(); // Use the stored membership cost
    }

    // Calculate total salaries of employees
    for (int i = 0; i < employeeCount; ++i) {
        if (employees[i].isEmpty()) continue; // Skip empty employees
        totalOutcome += employees[i].getSalary();
    }

    // Save updated data
    saveMembers();
    saveEmployees();

    // Display the results
    cout << "Total Income from Members: $" << totalIncome << endl;
    cout << "Total Outcome to Employees: $" << totalOutcome << endl;
    cout << "Net Income: $" << (totalIncome - totalOutcome) << endl;
    }

    void mainMenu() {
        while (true) {
            clearScreen();
            cout << "--- Gym Management System ---\n";
            cout << "1. Member Details\n";
            cout << "2. Employee Details\n";
            cout << "3. Finance\n";
            cout << "0. Exit\n";
            cout << "Choose an option: ";

            int choice = getIntegerInput();  // Use the safe input function

            clearScreen();
            switch (choice) {
            case 1: memberMenu(); break;
            case 2: employeeMenu(); break;
            case 3: financeMenu(); break;
            case 0: cout << "Exiting the system. Goodbye!\n"; return;
            default: cout << "Invalid option! Try again.\n";
            }
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
};

// Main function
int main() {
    User admin("admin", "password");

    int attempts = 3;
    while (attempts > 0) {
        string username, password;
        cout << "Login to Gym Management System\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        if (admin.login(username, password)) {
            cout << "Login successful!\n";
            GymManagementSystem system;
            system.loadMembers();
            system.loadEmployees();
            system.loadMembershipPrices();
            system.mainMenu();
            return 0;
        }
        else {
            attempts--;
            cout << "Invalid credentials. Attempts remaining: " << attempts << endl;
        }
    }

    cout << "Too many failed attempts. Exiting program.\n";
    return 0;
}