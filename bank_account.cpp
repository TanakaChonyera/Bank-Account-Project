#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

void deposit(double& balance, double& amount) { balance += amount; }

//=================================================================================
//=================================================================================

bool withdraw(double& balance, double& amount) {
  if (amount < balance) {
    balance -= amount;
    return true;
  }
  return false;
}

//=================================================================================
//=================================================================================

void overdraft(double& balance, double& amount) {
  balance = balance - amount - 35.0;
}

//=================================================================================
//=================================================================================

double interest_for_month(double& balance, const double& apr) {
  std::ostringstream convert_ss;
  convert_ss << (balance * (apr / 12.0 / 100.0)) * 100;
  std::string temp_str = convert_ss.str(), interest_str = "";
  for (char c : temp_str) {
    if (c == '.') {
      break;
    } else {
      interest_str.push_back(c);
    }
  }
  double interest = std::stod(interest_str);
  interest /= 100.0;
  return interest;
}

//=================================================================================
//=================================================================================

void string_date_to_int_ptrs(std::string& date, int* const year,
                             int* const month, int* const day) {
  std::stringstream iss(date);
  std::stringstream date_ss;
  std::string token;
  while (std::getline(iss, token, '-')) {
    date_ss << token << " ";
  }
  date_ss >> *year;
  date_ss >> *month;
  date_ss >> *day;
}

//=================================================================================
//=================================================================================

void string_date_to_ints(const std::string& date, 
                         int& year, int& month, int& day) {
  std::stringstream iss(date);
  std::stringstream date_ss;
  std::string token;
  while (std::getline(iss, token, '-')) {
    date_ss << token << " ";
  }
  date_ss >> year;
  date_ss >> month;
  date_ss >> day;
}

//=================================================================================
//=================================================================================

int number_of_first_of_months(std::string& earlier_date,
                              const std::string& later_date) {
  int earlier_year, earlier_month, earlier_day;
  int later_year, later_month, later_day;
  int number_of_months = 0;
  string_date_to_ints(earlier_date, earlier_year, earlier_month, earlier_day);
  string_date_to_ints(later_date, later_year, later_month, later_day);
  if (earlier_year == later_year) {
    number_of_months += later_month - earlier_month;
  } else {
    number_of_months += (later_year - (earlier_year + 1)) * 12;
    number_of_months += 12 - earlier_month;
    number_of_months += later_month;
  }
  return number_of_months;
}

//=================================================================================
//=================================================================================

double interest_earned(double& balance, const double& apr, 
                       std::string& earlier_date, 
                       const std::string& later_date) {
  int periods_elapsed = number_of_first_of_months(earlier_date, later_date);
  double interest_accrued = balance;
  for (int i = 0; i < periods_elapsed; i++) {
    interest_accrued += interest_for_month(interest_accrued, apr);
  }
  return interest_accrued - balance;
}

//=================================================================================
//=================================================================================

void pc_deposit(std::string& previous_date, const std::string& current_date,
                std::string& amount, double& balance, double& interest,
                int& number_of_months, std::stringstream& oss) {
  double amount_double = std::stod(amount.substr(1));
  if (number_of_months == 0) {
    deposit(balance, amount_double);
    oss << "On " << current_date << ": Instructed to perform \"Deposit "
        << amount << "\"" << "\nBalance: " << balance << "\n";
  } else {
    if (balance < 0) {
      interest = 0.0;
    }
    deposit(balance, amount_double);
    deposit(balance, interest);
    oss << "On " << current_date << ": Instructed to perform \"Deposit "
        << amount << "\"" << "\nSince " << previous_date 
        << ", interest has accrued "<< number_of_months << " times."
        << "\n$" << interest << " interest has been earned."<< "\nBalance: " 
        << balance << "\n";
  }
}

//=================================================================================
//=================================================================================

void pc_withdraw_r(std::string& previous_date, const std::string& current_date,
                   std::string& amount, double& balance, double& interest,
                   int& number_of_months, std::stringstream& oss) {
  double amount_double = std::stod(amount.substr(1));
  if (number_of_months == 0) {
    withdraw(balance, amount_double);
    oss << "On " << current_date << ": Instructed to perform \"Withdraw "
        << amount << "\"" << "\nBalance: " << balance << "\n";
  } else {
    deposit(balance, interest);
    withdraw(balance, amount_double);
    oss << "On " << current_date << ": Instructed to perform \"Withdraw "
        << amount<< "\""<< "\nSince " << previous_date 
        << ", interest has accrued " << number_of_months << " times." 
        << "\n$" << interest << " interest has been earned." 
        << "\nBalance: " << balance << "\n";
  }
}

//=================================================================================
//=================================================================================

void pc_withdraw_o(std::string& previous_date, const std::string& current_date,
                   std::string& amount, double& balance, double& interest,
                   int& number_of_months, std::stringstream& oss) {
  double amount_double = std::stod(amount.substr(1));
  if (number_of_months == 0) {
    overdraft(balance, amount_double);
    oss << "On " << current_date << ": Instructed to perform \"Withdraw "
        << amount << "\"" << "\nOverdraft!" << "\nBalance: " << balance << "\n";
  } else {
    deposit(balance, interest);
    overdraft(balance, amount_double);
    oss << "On " << current_date << ": Instructed to perform \"Withdraw "
        << amount << "\"" << "\nSince " << previous_date 
        << ", interest has accrued " << number_of_months << " times." 
        << "\n$" << interest << " interest has been earned." 
        << "\nOverdraft!" << "\nBalance: " << balance << "\n";
  }
}

//=================================================================================
//=================================================================================

void pc_withdraw(std::string& previous_date, const std::string& current_date,
                 std::string& amount, double& balance, double& interest,
                 int& number_of_months, std::stringstream& oss) {
  double amount_double = std::stod(amount.substr(1));
  if (amount_double > balance) {
    pc_withdraw_o(previous_date, current_date, amount, balance, interest,
                  number_of_months, oss);
  } else {
    pc_withdraw_r(previous_date, current_date, amount, balance, interest,
                  number_of_months, oss);
  }
}

//=================================================================================
//=================================================================================

std::string process_command(const std::string& process, std::string& previous_date,
                            double& balance, const double& apr) {
  std::stringstream processed_ss(process), oss;
  oss << std::fixed << std::setprecision(2);
  std::string current_date, command, amount;
  processed_ss >> current_date >> command >> amount;
  if (previous_date == "") {
    previous_date = current_date;
  }
  int number_of_months = number_of_first_of_months(previous_date, current_date);
  double interest = interest_earned(balance, apr, previous_date, current_date);
  if (command == "Deposit") {
    pc_deposit(previous_date, current_date, amount, balance, interest,
               number_of_months, oss);
  } else if (command == "Withdraw") {
    pc_withdraw(previous_date, current_date, amount, balance, interest,
                number_of_months, oss);
  }
  previous_date = current_date;
  return oss.str();
}

//=================================================================================
//=================================================================================

std::string process_commands(const std::string& instrctions, const double apr) {
  std::string instruction, return_string, previous_date = "";
  double balance = 0.0;
  std::stringstream lines_ss(instrctions), output_ss;
  while (std::getline(lines_ss, instruction)) {
    return_string = process_command(instruction, previous_date, balance, apr);
    output_ss << return_string;
    previous_date = instruction.substr(0, 10);
  }
  std::string final_output = output_ss.str();
  return final_output;
}

//=================================================================================
//=================================================================================

int main (){
    std::string input = 
    "2016-09-02 Deposit $600\n"
    "2016-09-02 Withdraw $23.54\n"
    "2016-09-04 Withdraw $58\n"
    "2016-11-02 Deposit $200\n"
    "2017-03-02 Withdraw $1200\n"
    "2019-11-02 Deposit $1500\n"
    "2020-03-02 Withdraw $100\n"
    "2021-03-02 Deposit $100\n";
    std::string result = process_commands(input, 7);
    std::cout << result << std::endl;;
    
}
