#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <iomanip>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

const string FILENAME = "expences.csv";
const string BUDGET_FILE = "budget.csv";


struct Expense {
    double amount;
    string description;
    string category;

    Expense(double amt, string desc, string cat) : amount(amt), description(std::move(desc)),
                                                   category(std::move(cat)) {};
};
struct Budget{
    double monthlyLimit;
    string category;
    Budget(double limit, string cat) : monthlyLimit(limit),  category(std::move(cat)) {};
};

const vector<string> CATEGORIES = {
        "Living",
        "Eating and Drinking",
        "Communication",
        "Shopping",
        "Hobbies",
        "Car",
        "Cash",
        "Healthcare",
        "Others"
};

void showMenue(){
  cout << "\n" << string(40, '=') << endl;
  cout << "~~~Personal Expense Tracker~~~" << endl;
  cout << "\n" << string(40, '=') << endl;
  cout << "1. Add Expense" << endl;
  cout << "2. View all Expenses" << endl;
  cout << "3. Show Total Spending" << endl;
  cout << "4. View Spending by Category" << endl;
  cout << "5. Save Expenses" << endl;
  cout << "6. Load Expenses" << endl;
  cout << "7. Set Monthly Budget" << endl;
  cout << "8. View Budget Status" << endl;
  cout << "9. Budget Warnings" << endl;
  cout << "10. Exit" << endl;
  cout << string(40, '-') << endl;
  cout << "Choose an option (1-10)" << endl;
}

void saveExpenses(const vector<Expense>& expenses){
  ofstream file(FILENAME);
  if (!file.is_open()){
    cout << "Failed to open the file!!" << endl;
    return;
  }
  file << "Amount,Description,Category" << endl;
  for (auto& item : expenses) {
    file << item.amount << "," << item.description << "," << item.category << endl;
  }

  file.close();
  cout << "Expenses saved successfully to " << FILENAME << "!" << endl;
  cout << "Saved " << expenses.size() << " expenses." << endl;
}

void loadExpenses(vector<Expense>& expenses){
  ifstream file(FILENAME);
  if (!file.is_open()){
    cout << "Failed to open file!!" << endl;
    return;
  }
  string line;
  int loadedCount = 0;
  expenses.clear();

  getline(file, line);
  while(getline(file, line)){
    stringstream ss(line);
    string amountStr, description, category;

    if (getline(ss, amountStr, ',') &&
        getline(ss, description, ',') &&
        getline(ss, category, ','))
    {
      try {
        double amount = stod(amountStr);
        expenses.emplace_back(amount, description, category);
        loadedCount++;
      }catch (const exception& e){
        cout << "Warning: Spippend invalide line in file" << endl;
      }
    }
  }
  file.close();
  cout << "Scuccessfilly loaded " << loadedCount << " expenses from " <<
  FILENAME << "!!" << endl;

}

void autoLoadExpense(vector<Expense>& expenses){
  ifstream file(FILENAME);
  if (file.is_open()){
    file.close();
    cout << "Found existing expense file. Loading previous expenses... " << endl;
    loadExpenses(expenses);
  }
}

void autoSaveExpense(const vector<Expense>& expenses){
  ofstream file(FILENAME);
  if (!expenses.empty()){
    cout << "Auto-Saving expenses..." << endl;
    saveExpenses(expenses);
  }
}

void showCategories(){
  cout << "\nAvailable Categories: " << endl;
  for (int i = 0; i < CATEGORIES.size(); ++i) {
    cout << (i + 1) << ". " << CATEGORIES.at(i) << endl;
  }
}

string getCategory(){
  int choice;
  while(true){
    showCategories();
    cout << "Choose category ( 1 - " << CATEGORIES.size() << "): ";
    if (!(cin >> choice)){
      cout << "Invalid input! Please enter a number." << endl;
      cin.clear();
      cin.ignore();
      continue;
    }
    if (choice >= 1 && choice <= static_cast<int>(CATEGORIES.size())){
      return CATEGORIES.at(choice - 1);
    }else{
      cout << "Please enter a number between 1 and " << CATEGORIES.size() << endl;
    }

  }
}


void addExpense(vector<Expense> &expenses) {
  double amount;
  string description;
  string category;

  cout << "\nEnter expense amount: €";
  cin >> amount;
  cin.ignore();

  cout << "Enter expense description: ";
  getline(cin, description);

  category = getCategory();

  expenses.emplace_back(amount, description, category);
  cout << "Expense entered successfully!" << endl;
}

void viewExpenses(const vector<Expense> &expenses) {
  if (expenses.empty()){
    cout << "\n No expenses recorded yet." << endl;
    return;
  }
  int expenseSize = expenses.size();
  cout << "\n ### All Expenses ###" << endl;
  cout << left << setw(10) << "Amount" << setw(30) << "Description"  << setw(20) << "Category"<< endl;
  cout << string(55, '-') << endl;

  for (int i = 0; i < expenseSize; ++i) {
    cout << left <<  setprecision(2) << "€"
    << setw(10) << fixed << expenses.at(i).amount
    << setw(30) << expenses.at(i).description
    << setw(20) << expenses.at(i).category << endl;
  }
}

void showTotal(const vector<Expense> &expenses) {
  if (expenses.empty()){
    cout << "\n No expenses recorded yet." << endl;
    return;
  }
  double total = 0.0;

  for (const auto& item : expenses) {
    total += item.amount;
  }
  cout << "\nTotal spending: €" << fixed << setprecision(2) << total << endl;
}
static void showSpendingByCategory(const vector<Expense>& expenses){
  if (expenses.empty()){
    cout << "\nNo expenses recorded yet." << endl;
    return;
  }
  map<string, double> categoryTotals;

  for (auto &item: expenses) {
    categoryTotals[item.category] += item.amount;
  }
  cout << "\n### Spending by Category ###" << endl;
  cout << left << setw(25) << "Category" << setw(15) << "Total" << setw(10) << "Count" << endl;
  cout << string(50, '-') << endl;

  for (auto& pair : categoryTotals) {
    int count = 0;
    for (auto& item: expenses) {
      if (item.category == pair.first){
        count++;
      }
    }

    cout << left << setw(25) << pair.first
         << "€" << setw(14) << fixed << setprecision(2) << pair.second
         << setw(10) << count << " expenses" << endl;
  }
}

void setMonthlyBudget(vector<Budget> &budgets){
  cout << "\n### Set Monthly Budget ###" << endl;
  showCategories();
  string selectedCategory = getCategory();
  double budgetAmount = 0;

  cout << "Enter monthly budget for " << selectedCategory << ": €";
  if (!(cin >> budgetAmount) || budgetAmount <= 0){
    cout << "Invalid amount! Please enter a positive number." << endl;
    cin.clear();
    cin.ignore();
    return;
  }

  bool found = false;
  for (auto& item : budgets) {
    if (item.category == selectedCategory){
      cout << "Updating existing buget from €" << item.monthlyLimit
           << " to €" << budgetAmount << endl;
      item.monthlyLimit = budgetAmount;
      found = true;
      break;
    }
  }
  if (!found){
    budgets.emplace_back(budgetAmount, selectedCategory);
    cout << "New budget created!" << endl;
  }

  cout << "Monthly budget for " << selectedCategory << " ser to €"
  << fixed << setprecision(2) << budgetAmount << endl;
}

map<string, double> getCurrentMonthlySpending(const vector<Expense> & expenses){
  map<string, double> monthlySpending;

  for (auto &item : expenses) {
    monthlySpending[item.category] += item.amount;
  }
  return monthlySpending;
}

void showBudgetStatus(const vector<Expense> &expenses, const vector<Budget> &budgets){
  if (budgets.empty()){
    cout << "\nNo budgets set yet. Use option 7 to set budget" << endl;
    return;
  }
  map<string, double> monthlySpending = getCurrentMonthlySpending(expenses);

  cout << "\n ### Budget Status ###" << endl;
  cout << left << setw(20) << "Category" << setw(12) << "Spent" << setw(12)
  << "Budget" << setw(12) << "Remaining" << setw(10) << "Status" << endl;
  cout << string(66, '-') << endl;

  for (const auto& item : budgets) {
    double spent = monthlySpending[item.category];
    double remaining = item.monthlyLimit - spent;
    string status;

    if (spent > item.monthlyLimit){
      status = "OVER";
    }else if(spent > item.monthlyLimit * 0.8){
      status = "WARNING";
    }else{
      status = "OK";
    }

    cout << left << setw(20) << item.category << "€" << setw(11) << fixed << setprecision(2)
    << spent << "€" << setw(11) << item.monthlyLimit << "€" << setw(11) << remaining
    << setw(10) << status << endl;

  }

}

void checkBudgetWarnings(const vector<Expense> &expenses, const vector<Budget> &budgets){
  if (budgets.empty()){
    cout << "\nNo budget set yet." << endl;
  }
  map<string, double> monthlySpending = getCurrentMonthlySpending(expenses);
  bool hasWarnings = false;
  cout << "\n### Budget Warnings ###" << endl;


  for(const auto& item : budgets){
    double spent = monthlySpending[item.category];
    double percentUsed = (spent / item.monthlyLimit) * 100;

    if (spent > item.monthlyLimit) {
      cout << "🚨 OVER BUDGET: " << item.category
           << " (€" << fixed << setprecision(2) << spent
           << " / €" << item.monthlyLimit
           << " - " << percentUsed << "%)" << endl;
      hasWarnings = true;
    } else if (percentUsed >= 80) {
      cout << "⚠️  WARNING: " << item.category
           << " is at " << percentUsed << "% of budget"
           << " (€" << spent << " / €" << item.monthlyLimit << ")" << endl;
      hasWarnings = true;
    }
  }
  if (!hasWarnings) {
    cout << "✅All budges are looking good!" << endl;
  }
}

void saveBudget(const vector<Budget> &budgets){
  ofstream file(BUDGET_FILE);

  if(!file.is_open()){
    cout << "Error: failed to create budget file" << endl;
  }
  file << "Category, MonthlyLimit" << endl;

  for (const auto& item : budgets) {
    file << item.category << "," << item.monthlyLimit << endl;
  }
  file.close();
  cout << "Budget saved successfully!" << endl;
}

void loadBudget(vector<Budget> &budgets){
  ifstream file(BUDGET_FILE);

  if (!file.is_open()){
    cout << "No saved budget found" << endl;
  }
  string line;
  int loadedCount = 0;
  budgets.clear();
  getline(file, line);

  while (getline(file, line)){
    stringstream ss(line);
    string category, limitStr;
    if (getline(ss, category, ',') && getline(ss, limitStr)){
      try {
        double limit = stod(limitStr);
        budgets.emplace_back(limit, category);
        loadedCount++;
      }catch (const exception& e){
        cout << "Warning: Skippend invalid Budget line." << endl;
      }
    }

  }
  file.close();
  cout << "Loaded " << loadedCount << " budgets." << endl;

}



int main() {
  vector<Expense> expenses;
  vector<Budget> budgets;
  int choice = 0;

  cout << "Welcome to your Personal Expense Tracker" << endl;
  autoLoadExpense(expenses);
  loadBudget(budgets);

  while(true){
    showMenue();
    if (!(cin >> choice)){
      cout << "Invalid input! Please enter a number." << endl;
      cin.clear();
      cin.ignore();
      continue;
    }

    switch (choice) {
      case 1:
        addExpense(expenses);
        break;
      case 2:
        viewExpenses(expenses);
        break;
      case 3:
        showTotal(expenses);
        break;
      case 4:
        showSpendingByCategory(expenses);
        break;
      case 5:
        saveExpenses(expenses);
        break;
      case 6:
        loadExpenses(expenses);
        break;
      case 7:
        setMonthlyBudget(budgets);
        break;
      case 8:
        showBudgetStatus(expenses, budgets);
        break;
      case 9:
        checkBudgetWarnings(expenses, budgets);
        break;
      case 10:
        cout << "Thanks for using Expense Tracker. Goodbye!" << endl;
        autoSaveExpense(expenses);
        saveBudget(budgets);
        return 0;
      default:
        cout << "Invalid option. Please choose between numbers 1 to 7." << endl;

    }
  }

  return 0;
}