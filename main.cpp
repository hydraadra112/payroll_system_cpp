#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <array>

using namespace std;
constexpr uint8_t header_size = 4;
const string filename = "employees.txt";

struct Person {
    string name;
    uint16_t age;
    string id;
};

class Payroll {
    public:
        Payroll(const Person& person, double salary)
        : personalDetails(person), salary(salary) {}

        Person getPerson() const
        {
            return this->personalDetails;
        }

        double getSalary() const 
        {
            return this->salary;
        }

        void setSalary(double newSalary)
        {
            this->salary = newSalary;
        }

    private:
        Person personalDetails;
        double salary;
};

// Function prototypes
pair<vector<Payroll>, string> parseCSV(array<string, header_size>& headers);
void changePay(vector<Payroll>& employees, const string& header);
void displayEmployees(const vector<Payroll>& employees);
void addEmployee(vector<Payroll>& employees, const string& header);
void removeEmployee(vector<Payroll>& employees, const string& header);
void saveFile(const vector<Payroll>& employees, const string& header);
void searchEmployee(const vector<Payroll>& employees);
int menu();

int main() 
{
    array<string, header_size> headers;
    pair<vector<Payroll>, string> result = parseCSV(headers);

    vector<Payroll> employees = result.first;
    string header = result.second;

    // Necessary functions above
    int choice = menu();

    while(choice != 6)
    {
        switch(choice) {
            case 1: changePay(employees, header);
            system("cls");
            system("clear");
            break;
            case 2: addEmployee(employees, header);
            system("cls");
            system("clear");
            break; 
            case 3: removeEmployee(employees, header);
            system("cls");
            system("clear");
            break;
            case 4: searchEmployee(employees);
            system("cls");
            system("clear");
            break;
            case 5: // search specific employee by index
            system("cls");
            system("clear");
            break;
            default: return 0;
        }
        choice = menu();
    }
        
    
    return 0;
}

int menu()
{
    int choice;
    cout << "1. Change Payroll \n";
    cout << "2. Add Employee \n";
    cout << "3. Remove Emloyee \n";
    cout << "4. Display All Employees \n";
    cout << "5. Search Employee \n";
    cout << "6. Exit \n";
    cin >> choice;
    return choice;
}

// Function to parse a CSV file containing Payroll information
// and return a pair of vector of Payroll objects and a string of header
pair<vector<Payroll>, string> parseCSV(array<string, header_size>& headers)
{
    // Open the CSV file
    std::ifstream file(filename);
    if (!file.is_open()) {
        // If the file fails to open, print an error message and return an empty pair
        cerr << "Failed to open the file." << endl;
        return pair<vector<Payroll>, string>();
    }

    // Create a temporary vector to hold Payroll objects
    vector<Payroll> temp;

    string line;

    // Read the header line from the CSV
    getline(file, line);
    std::istringstream inputString(line);

    // Parse the header line to extract column headers
    getline(inputString, headers[0], ',');
    getline(inputString, headers[1], ',');
    getline(inputString, headers[2], ',');
    getline(inputString, headers[3], ',');

    // Loop through the remaining lines in the CSV
    while(getline(file, line)) {
        string name, strAge, strSalary, id;
        uint16_t age;
        double salary;

        std::istringstream inputString(line);

        // Parse the data in each line to extract name, age, and salary
        getline(inputString, name, ',');
        getline(inputString, strAge, ',');
        getline(inputString, strSalary, ',');
        getline(inputString, id, ',');

        // Convert string representations of age and salary to appropriate data types
        age = stoi(strAge);
        salary = stod(strSalary);

        // Create a Payroll object and add it to the vector
        temp.emplace_back(Payroll({name, age, id}, salary));
    }

    // Close the file and request to reduce vector capacity to match its size
    file.close();
    temp.shrink_to_fit();
    
    // Return a pair of vector objects and a string header
    return make_pair(temp, (headers[0] + ',' + headers[1] + ',' + headers[2] + ',' + headers[3]));
}

// Function to save employee data to a CSV file
void saveFile(const vector<Payroll>& employees, const string& header)
{
    // Open a file for writing
    ofstream write_file(filename);
    
    // Check if the file is successfully opened
    if (!write_file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return;
    }

    // Write the header line to the file
    write_file << header << "\n";

    // Iterate through the employees vector and write employee data to the file
        for (size_t i = 0; i < employees.size(); ++i) {
            write_file << employees[i].getPerson().name << ',' << employees[i].getPerson().age << ',' << employees[i].getSalary() << employees[i].getPerson().id;
            
            // Add a newline character if there is another employee data to follow
            if (i < employees.size() - 1) {
                write_file << "\n";
            }
    }

    // Close the file
    write_file.close();
}

void changePay(vector<Payroll>& employees, const string& header)
{
    int emp_id;
    double newSalary;
    cout << "Enter Employee Number: ";
    cin >> emp_id;

    cout << "\nEnter New Salary: ";
    cin >> newSalary;

    for(auto& employee : employees)
    {
        if(stoi(employee.getPerson().id) == emp_id) {
            employee.setSalary(newSalary);
            
            saveFile(employees, header);
            return;
        }
    }

    cout << "ID Not Found. Salary Not Changed \n";
    return;
}

void displayEmployees(const vector<Payroll>& employees)
{   
    for(const auto& employee : employees)
    {     
        cout << "ID: " << employee.getPerson().id << "\nName: " << employee.getPerson().name << "\nAge: " << employee.getPerson().age << "\nSalary: " << employee.getSalary() << "\n\n";
    }
}

void addEmployee(vector<Payroll>& employees, const string& header)
{
    string name, emp_id;
    uint16_t age;
    double initial_salary;

    cout << "Enter Name: ";
    cin >> name;

    cout << "\nEnter Age: ";
    cin >> age;
        
    if(age >= 60) {
        cout << "\nCant add senior citizens.";
        return;
    }
    
    cout << "\nEnter ID Num: ";
    cin >> emp_id;

    cout << "\nEnter Initial Salary: ";
    cin >> initial_salary;

    employees.emplace_back(Payroll({name, age, emp_id}, initial_salary));
    saveFile(employees, header);
}

void removeEmployee(vector<Payroll>& employees, const string& header)
{
    string emp;
    cout << "Enter Employee Number: ";
    cin >> emp;

    for(size_t i = 0; i < employees.size(); ++i)
    {
        if(employees[i].getPerson().id == emp)
        {
            employees.erase(employees.begin() + i);
            saveFile(employees, header);
            return;
        } 
    }    
}

void searchEmployee(const vector<Payroll>& employees)
{
    string emp;
    cout << "Enter Employee Number: ";
    cin >> emp;

    for(const auto& employee : employees)
    {
        if(employee.getPerson().id == emp)
        {
            cout << "ID: " << employee.getPerson().id << "\nName: " << employee.getPerson().name << "\nAge: " << employee.getPerson().age << "\nSalary: " << employee.getSalary() << "\n\n";
        }
    } 
}
