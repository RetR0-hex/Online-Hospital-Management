#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <random>
#include <iomanip>
#include <regex>
#include <windows.h>
#include <chrono>
#include <filesystem>
using namespace std;


class User{

protected:

    int user_id, user_permission;
    string name, password, username;

public:

    // Constructor for User class
    User(int user_id, int user_permission, string name, string password, string username) {
        this->user_id = user_id;
        this->user_permission = user_permission;
        this->name = std::move(name);
        this->password = std::move(password);
        this->username = std::move(username);
    }

    static void show_menu() {
        cout << "You don't have any roles, please contact an administrator" << endl;
        cout << "Press any key to exit" << endl;
        _getch();
    }

    static void illegal_input() {
        cout << "Invalid choice!" << endl;
        cout << "Please try again." << endl;
    }

    virtual void main_function() {
        User::show_menu();
        exit(0);
    }

};


class DBHandling {

private:

    static int generate_random_user_id(void) {
        random_device rand_hard_num;
        mt19937 seed(rand_hard_num());
        uniform_int_distribution<> distr(0, 9999999);
        return distr(seed);

    }

    static string role_from_user_permission(const string& user_permission){
        if (user_permission == "0"){
            return "Administrator";
        }
        else if (user_permission == "1") {
            return "Doctor";
        }
        else if (user_permission == "2"){
            return "Patient";
        }
        else {
            return "No Role Assigned";
        }
    }


public:
    static void add_new_user() {
        fstream database_file;
        database_file.open("database.txt", ios::app | ios::out | ios::in);
        string name, username, password;
        int user_permission, age;
        char gender;
        cout << "++++++++++++++++ New User Menu +++++++++++++++++" << endl;
        cout << "Enter their name:";
        std::getline(std::cin >> std::ws, name);
        cout << "Enter their username (NO SPACES ALLOWED):";
        cin >> username;
        cout << "Enter their password:";
        std::getline(std::cin >> std::ws, password);
        // 0 for admin, 1 for doctor, 2 for patient, 3 for no permissions
        cout << "Enter their User Permission Identifier (0 or 1 or 2 or 3):";
        cin >> user_permission;
        if (user_permission == 0 || user_permission == 3) {
            database_file << "|" << "user id:" << setfill('0') << setw(7) << DBHandling::generate_random_user_id() << "|" << "user permission:" << user_permission
                          << "|" << "username:" << username << "|" << "password:" << password << "|" << "name:" << name << "|" << endl;
            database_file.close();
        }

        else if (user_permission == 1) {
            int salary;
            string specialization;
            cout << endl << "Enter their Salary:";
            cin >> salary;
            cout << endl << "Enter their specialization:";
            std::getline(std::cin >> std::ws, specialization);
            cout << "Enter their Gender (M/F), and age:";
            cin >> gender >> age;
            database_file << "|" << "user id:" << setfill('0') << setw(7) << DBHandling::generate_random_user_id() << "|" << "user permission:" << user_permission
                          << "|" << "username:" << username << "|" << "password:" << password << "|" << "name:" << name << "|" << "doctor id:" << setfill('0')
                          << setw(7) << DBHandling::generate_random_user_id() << "|" << "age:" << age << "|" << "salary:" << salary << "|" << "gender:" << gender
                          << "|" << "specialization:" << specialization << "|" << endl;
            database_file.close();
        }

        else if (user_permission == 2) {
            int insurance_plan;
            string medical_history;
            // 0 for Diamond, 1 for Gold, 2 for Silver and 3 for No insurance_plan
            cout << "Enter their insurance_plan identifier(0,1,2,3):";
            cin >> insurance_plan;
            cout << "Enter their Medical History:";
            std::getline(std::cin >> std::ws, medical_history);
            cout << "Enter their Gender (M/F), and age:";
            cin >> gender >> age;
            database_file << "|" << "user id:" << setfill('0') << setw(7) << DBHandling::generate_random_user_id() << "|" << "user permission:" << user_permission
                          << "|" << "username:" << username << "|" << "password:" << password << "|" << "name:" << name << "|" << "patient id:" << setfill('0')
                          << setw(7) << DBHandling::generate_random_user_id() << "|" << "age:" << age << "|" << "insurance_plan_id:" << insurance_plan << "|" << "gender:"
                          << gender << "|" << "medical history:" << medical_history << "|" << endl;
            database_file.close();
        }

        else {
            return;
        }
    }

    static void delete_user() {
        fstream database_file, temp_file;
        database_file.open("database.txt", ios::app | ios::out | ios::in);
        temp_file.open("temp.txt", ios::trunc | ios::in | ios::out);
        string user_id, line;
        cout << "Enter the user id of the account that you want to remove: ";
        cin >> user_id;
        while(database_file) {
            getline(database_file, line);
            if (!line.empty()) {
                if (string_searcher(line, "user id:") != user_id){
                    temp_file << line << endl;
                }
            }
        }
        database_file.close();
        temp_file.close();
        remove("database.txt");
        rename("temp.txt", "database.txt");
    }

    static void show_users_list() {
        fstream database_file;
        database_file.open("database.txt", ios::app | ios::out | ios::in);
        string line;
        while (database_file) {
            getline(database_file, line);
            if (!line.empty()) {
                // user id finder
                cout << "|User ID:" << string_searcher(line, "|user id:") << " ";
                // role finder
                cout << "|Role:" << role_from_user_permission(string_searcher(line, "|user permission:")) << "| ";
                // username finder
                cout << "|Username:" << string_searcher(line, "|username:") << "| ";
                // name finder
                cout << "|Name:" << string_searcher(line, "|name:") << "|" << endl;
            }
        }
        database_file.close();
    }

    static void edit_user() {
        int user_choice;
        string line, user_id_to_change, info_to_replace;
        fstream database_file, temp_file;
        database_file.open("database.txt", ios::app | ios::out | ios::in);
        temp_file.open("temp.txt", ios::trunc | ios::out | ios::in);
        cout << "Enter the user id of the account account whose details you want to change: ";
        cin >> user_id_to_change;
        cout << endl << "What do you want to edit:\n"
             << "1. Username\n"
             << "2. Password\n"
             << "3. Name\n";
        cin >> user_choice;
        if (user_choice == 1) {
            cout << "Enter the new username:";
            getline(std::cin >> std::ws, info_to_replace);
            while (database_file) {
                getline(database_file, line);
                if (!line.empty()) {

                    if (string_searcher(line, "|user id:") == user_id_to_change) {
                        auto to_replace = string_searcher(line, "|username:");
                        line = regex_replace(line, regex(to_replace), info_to_replace);
                        temp_file << line << endl;
                    } else {
                        temp_file << line << endl;
                    }
                }
            }
        }
        else if (user_choice == 2){
            cout << "Enter the new password:";
            getline(std::cin >> std::ws, info_to_replace);
            while (database_file) {
                getline(database_file, line);
                if (!line.empty()) {

                    if (string_searcher(line, "|user id:") == user_id_to_change) {
                        auto to_replace = string_searcher(line, "|password:");
                        line = regex_replace(line, regex(to_replace), info_to_replace);
                        temp_file << line << endl;
                    } else {
                        temp_file << line << endl;
                    }
                }
            }
        }
        else if (user_choice == 3){
            cout << "Enter the new name:";
            getline(std::cin >> std::ws, info_to_replace);
            while (database_file) {
                getline(database_file, line);
                if (!line.empty()) {

                    if (string_searcher(line, "|user id:") == user_id_to_change) {
                        auto to_replace = string_searcher(line, "|name:");
                        line = regex_replace(line, regex(to_replace), info_to_replace);
                        temp_file << line << endl;
                    } else {
                        temp_file << line << endl;
                    }
                }
            }
        }
        else {
            cout << "wrong input";
            database_file.close();
            temp_file.close();
            return;
        }
        database_file.close();
        temp_file.close();
        remove("database.txt");
        rename("temp.txt", "database.txt");
    }

    static bool check_login(const string& username, const string& password){
        fstream database_file;
        database_file.open("database.txt", ios::in);
        string line;
        while(database_file){
            getline(database_file, line);
            auto username_database = string_searcher(line, "|username:");
            if (username_database == username && !username_database.empty()){
                auto password_database = string_searcher(line, "|password:");
                database_file.close();
                return password_database == password;
            }
        }
        database_file.close();
        return false;
    }

    static string retrieve_account_details(const string& username){
        string line;
        fstream database_file;
        database_file.open("database.txt", ios::in);
        while(database_file) {
            getline(database_file, line);
            if(!line.empty()){
                auto username_database = string_searcher(line, "|username:");
                if(username_database == username && !username_database.empty()){
                    database_file.close();
                    return line;
                }
            }
        }
        database_file.close();
        return "";
    }

    static string string_searcher(const string& line, const string& search_string) {
        auto start = line.find(search_string);
        if (start != string::npos) {
            start = start + search_string.length();
            auto end = line.find('|', start);
            return line.substr(start, (end - start));
        }
        else {
            return "";
        }
    }

    static void make_default_database_file(void){
        fstream database_file;
        database_file.open("database.txt", ios::app | ios::out | ios::in);
        database_file << "|" << "user id:" << setfill('0') << setw(7) << DBHandling::generate_random_user_id() << "|" << "user permission:" << '0'
                      << "|" << "username:" << "retr0" << "|" << "password:" << "test123" << "|" << "name:" << "Abdul-Mateen" << "|" << endl;
        database_file.close();
    }
};


// set at userpermission 0
class Administrator : public User {

private:
    friend DBHandling;

    void show_menu(void) {

        cout << " ================= Welcome Administrator " << this->name << " ================= " << endl;
        cout
                << "You have the following privileges\n"
                << "1. Add a user to the database\n"
                << "2. Delete a user from the database\n"
                << "3. Show Current Users List\n"
                << "4. Edit details of a user\n"
                << "5. Exit\n";
    }

    int get_user_input() {
        int user_choice;
        cout << "Enter your choice:";
        cin >> user_choice;
        if (user_choice >= 1 && user_choice <= 5) {
            return user_choice;
        }
        else {
            User::illegal_input();
            this->get_user_input();
        }
    }

    static void show_choice_menu(int& user_choice) {

        switch (user_choice) {

            case 1:
                DBHandling::add_new_user();
                break;

            case 2:
                DBHandling::delete_user();
                break;

            case 3:
                DBHandling::show_users_list();
                break;

            case 4:
                DBHandling::edit_user();
                break;
        }
    }

public:

    // Administrator constructor
    Administrator(int user_id, int user_permission, string name, string password, string username)
            : User(user_id, user_permission, std::move(name), std::move(password), std::move(username)) {
    }

    void main_function() override {
        while(true) {
            this->show_menu();
            auto user_choice = this->get_user_input();
            Administrator::show_choice_menu(user_choice);
            if (user_choice == 5) {
                break;
            }
        }
        exit(0);
    }

};

class BillPayment{

private:
    int total_bill, final_bill, ccv_number;
    float total_discount;
    int64_t credit_card_number;

    void show_menu(){
        cout
                << "You have the following payment methods\n"
                << "1. Credit/Debit Card\n"
                << "2. Wire Transfer (Coming soon)\n"
                << "3. Paypal (Coming soon)\n"
                << "4. Bitcoin Payments (Coming Soon)\n"
                << "5. Exit\n";
    }

    int get_user_input() {
        int user_choice;
        cout << "Enter your choice: ";
        cin >> user_choice;
        if (user_choice >= 1 && user_choice <= 5) {
            return user_choice;
        }
        else {
            User::illegal_input();
            this->get_user_input();
        }
    }

    void show_choice_menu(int& user_choice){
        char user_choice_1;
        cout << "Your Total bill is: " << this->total_bill << endl;
        cout << "Your Total Discount is: " << this->total_discount << endl;
        cout << "Your final balance after discount:" << this->final_bill << endl;
        cout << "Do you want to pay? (Y/N): ";
        cin >> user_choice_1;
        if (user_choice_1 == 'Y' || user_choice_1 == 'y'){
            switch (user_choice) {
                case 1:
                    cout << "Please Enter your credit_card_number or debit_card_number: " << endl;
                    cin >> credit_card_number;
                    cout << "Please enter the ccv_number of your credit_card: " << endl;
                    cin >> ccv_number;
                    cout << "Please Wait, ......\n";
                    Sleep(2000);
                    cout << "Payment was successful, Thanks for using our service." << endl;
                    break;

                case 2:
                case 3:
                case 4:
                    cout << "Sorry this payment method is not supported yet." << endl;
                    break;
            }
        }
    }

public:
    // BillPayment Constructor
    BillPayment(int& appointment_count, float& insurance_plan_discount){
        this->total_bill = 9000 * appointment_count;
        this->total_discount = float(this->total_bill) * insurance_plan_discount;
        this->final_bill = this->total_bill - int(round(this->total_discount));
    }

    void main_function(){
        this->show_menu();
        auto user_choice = this->get_user_input();
        this->show_choice_menu(user_choice);
    }
};

class Patient : public User {

private:
    int patient_number, age, appointment_count = 0, insurance_plan;
    float insurance_plan_discount;
    string medical_history, gender;

    void calc_insurance_discount(){
        // 0 for Diamond, 1 for Gold, 2 for Silver and 3 for No insurance_plan
        if (insurance_plan == 0) {
            insurance_plan_discount = 0.45;
        }

        else if (insurance_plan == 1) {
            insurance_plan_discount = 0.30;
        }

        else if (insurance_plan == 2) {
            insurance_plan_discount = 0.15;
        }

        else {
            insurance_plan_discount = 0;
        }

    }

    void show_menu(){
        cout << " ================= Welcome Patient " << this->name << " ================= " << endl;
        cout
                << "You have the following options\n"
                << "1. Reserve an Appointment\n"
                << "2. View your Medical History\n"
                << "3. Pay your Bills\n"
                << "4. Show number of appointments you have made\n"
                << "5. Show your Information\n"
                << "6. Exit\n";
    }

    int get_user_input() {
        int user_choice;
        cout << "Enter your choice: ";
        cin >> user_choice;
        if (user_choice >= 1 && user_choice <= 6) {
            return user_choice;
        } else {
            User::illegal_input();
            this->get_user_input();
        }
    }

    void show_choice_menu(int& user_choice){

        string time_date;
        int department_identifier;

        if (user_choice == 1) {
            cout
                    << "To Reserve an appointment please enter the time you would like to "
                    << "schedule the appointment (format: 13:30:24-05-2020): " << endl;
            cin >> time_date;
            cout << "Which department you would like to reserve the appointment for?" << endl;
            cout
                    << "1. General Medicine\n"
                    << "2. E.N.T\n"
                    << "3. Psychiatry\n"
                    << "4. Orthopaedics\n";
            cin >> department_identifier;
            cout << "An Appointment has been reserved for " << this->name << " at time " << time_date << endl;
            cout << "We will notify you when a doctor is available, Thank you for using our service!" << endl;
            this->appointment_count += 1;
        }

        else if (user_choice == 2) {
            cout << "Your Medical History: " << this->medical_history << endl;
        }

        else if (user_choice == 3) {
            BillPayment payment_object = BillPayment(this->appointment_count, this->insurance_plan_discount);
            payment_object.main_function();
        }

        else if (user_choice == 4) {
            cout << "Number of appointment you have made: " << this->appointment_count;
        }

        else if (user_choice == 5){
            cout
                    << "Name:" << this->name << endl
                    << "Gender:" << this->gender << endl
                    << "Age:" << this->age << endl
                    << "Patient ID:" << this->patient_number << endl
                    << "Insurance Plan:" << this->insurance_plan << endl
                    << "Insurance Plan Discount:" << this->insurance_plan_discount << endl
                    << "Total Appointments:" << this->appointment_count << endl;
        }
        else {
            return;
        }
    }

public:
// Patient Constructor
    Patient(int user_id, int user_permission, int patient_number, int age, int insurance_plan, string name, string password, string username, string gender,
            string medical_history) : User(user_id, user_permission, std::move(name), std::move(password), std::move(username)) {

        this->patient_number = patient_number;
        this->age = age;
        this->insurance_plan = insurance_plan;
        this->medical_history = std::move(medical_history);
        this->gender = std::move(gender);

    }

    void main_function() override{
        while (true) {
            this->show_menu();
            auto user_choice = this->get_user_input();
            this->calc_insurance_discount();
            this->show_choice_menu(user_choice);
            if(user_choice == 6){
                break;
            }
        }
        exit(0);
    }
};

class Doctor : public User {

private:
    int doctor_number, age, salary;
    string specialization, gender;

    void show_menu() {
        cout << " ================= Welcome Doctor " << this->name << " ================= " << endl;
        cout
                << "You have the following options\n"
                << "1. Perform a medical Checkup\n"
                << "2. View your salary\n"
                << "3. View your Information\n"
                << "4. Exit\n";
    }

    int get_user_input() {
        int user_choice;
        cout << "Enter your choice: ";
        cin >> user_choice;
        if (user_choice >= 1 && user_choice <= 4) {
            return user_choice;
        } else {
            User::illegal_input();
            this->get_user_input();
        }
    }

    void show_choice_menu(int &user_choice) {
        string diagnosis;
        switch (user_choice) {
            char participate_choice;
            case 1:
                cout << "Do you want to participate in a medical checkup with your patient? (Y/N)";
                cin >> participate_choice;
                if (participate_choice == 'Y'||participate_choice == 'y'){
                    cout << "Please write your diagnosis:";
                    getline(cin >> ws, diagnosis);
                    auto time = std::chrono::system_clock::now();
                    time_t diagnosis_time = std::chrono::system_clock::to_time_t(time);
                    cout << "\nDiagnoses:" << diagnosis << "\nBy Doctor:" << this->name << "\nBy Hospital:Ghazi Online Hospital" << "\nAt time:" << std::ctime(&diagnosis_time);

                }
                break;

            case 2:
                cout << "Your Salary: " << this->salary << endl;
                break;

            case 3:
                cout
                        << "Name:" << this->name << endl
                        << "Gender:" << this->gender << endl
                        << "Age:" << this->age << endl
                        << "Doctor ID:" << this->doctor_number << endl
                        << "Salary:" << this->salary << endl
                        << "Specialization:" << this->specialization << endl;
                break;
        }
    }

public:
// Doctor Constructor
    Doctor(int user_id, int user_permission, string name, string password, string username, int doctor_number, int age, int salary, const string &gender,
           const string &specialization) : User(user_id, user_permission, std::move(name), std::move(password), std::move(username)) {
        this->doctor_number = doctor_number;
        this->age = age;
        this->salary = salary;;
        this->gender = gender;
        this->specialization = specialization;
    }

    void main_function() override {
        while(true) {
            this->show_menu();
            auto user_choice = this->get_user_input();
            this->show_choice_menu(user_choice);
            if (user_choice == 4) {
                break;
            }
        }
        exit(0);
    }
};

int main(){
    string username, password;

    //If database file doesn't exists create a new default database file
    if (!std::filesystem::exists("database.txt")){
        DBHandling::make_default_database_file();
    }

    cout << "================================= Welcome to Ghazi Online Hospital ========================================="
         << endl;
    cout << "Please enter your account details to login." << endl;
    while(true) {
        cout << "Username:";
        cin >> username;
        cout << "Password:";
        cin >> password;
        if (DBHandling::check_login(username, password)) {
            cout << "You have successfully logged in" << endl;
            break;
        } else {
            cout << "Incorrect username or password, Please try again." << endl;
        }
    }

    auto account_line = DBHandling::retrieve_account_details(username);
    if (!account_line.empty()) {
        auto user_id = stoi(DBHandling::string_searcher(account_line, "|user id:"));
        auto user_permission = stoi(DBHandling::string_searcher(account_line, "|user permission:"));
        auto name = DBHandling::string_searcher(account_line, "|name:");
        if (user_permission == 0) {
            Administrator admin_obj = Administrator(user_id, user_permission, name, password, username);
            admin_obj.main_function();
        }
        else if (user_permission == 1) {
            auto age = stoi(DBHandling::string_searcher(account_line, "|age:"));
            auto gender = DBHandling::string_searcher(account_line, "|gender:");
            auto doctor_id = stoi(DBHandling::string_searcher(account_line, "|doctor id:"));
            auto salary = stoi(DBHandling::string_searcher(account_line, "|salary:"));
            auto specialization = DBHandling::string_searcher(account_line, "|specialization:");
            Doctor doctor_obj = Doctor(user_id, user_permission, name, password, username, doctor_id, age, salary,
                                       gender, specialization);
            doctor_obj.main_function();;
        }
        else if (user_permission == 2) {
            auto patient_number = stoi(DBHandling::string_searcher(account_line, "|patient id:"));
            auto age = stoi(DBHandling::string_searcher(account_line, "|age:"));
            auto insurance_plan = stoi(DBHandling::string_searcher(account_line, "|insurance_plan_id:"));
            auto gender = DBHandling::string_searcher(account_line, "|gender:");
            auto medical_history = DBHandling::string_searcher(account_line, "|medical history:");
            Patient patient_obj = Patient(user_id, user_permission, patient_number, age, insurance_plan, name, password,
                                          username, gender, medical_history);
            patient_obj.main_function();
        }
        else {
            User user_obj = User(user_id, user_permission, name, password, username);
            user_obj.main_function();
        }
    }
    system("pause");
    return 0;
}

