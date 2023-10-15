#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <array>

using std::cout, std::string, std::endl; // namespace
const uint16_t header_size = 3;

struct Person {
    string name;
    uint16_t age;

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

    private:
        Person personalDetails;
        double salary;
};

std::vector<Payroll> parseCSV(const std::string& filename, std::array<std::string, header_size>& headers);

int main() 
{
    std::array<std::string, header_size> headers;
    std::vector<Payroll> employees = parseCSV("employees.txt", headers);

    for(const auto& employee : employees)
    {
        cout << "Name: " << employee.getPerson().name << "\nSalary: " << employee.getSalary() << "\n\n";
    }

}

std::vector<Payroll> parseCSV(const std::string& filename, std::array<std::string, header_size>& headers)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << endl;
        return std::vector<Payroll>();
    }

    std::vector<Payroll> temp;

    string line;

    getline(file, line);
    std::istringstream inputString(line);

    getline(inputString, headers[0], ',');
    getline(inputString, headers[1], ',');
    getline(inputString, headers[2], ',');

    while(getline(file, line)) {

        string name, strAge, strSalary;
        uint16_t age;
        double salary;

        std::istringstream inputString(line);

        getline(inputString, name, ',');
        getline(inputString, strAge, ',');
        getline(inputString, strSalary, ',');

        age = std::stoi(strAge.c_str());
        salary = std::stod(strSalary.c_str());

        temp.emplace_back(Payroll({name, age}, salary));
    }

    file.close();
    temp.shrink_to_fit();
    return temp;
}




