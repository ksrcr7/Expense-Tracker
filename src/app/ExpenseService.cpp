#include <expense_tracker/app/ExpenseService.h>
#include <cctype>
#include <stdexcept>

std::string trim(const std::string& s){
    const std::string whitespace = " \t\n\r\f\v";

    size_t start = s.find_first_not_of(whitespace);
    if (start == std::string::npos)
        return "";

    size_t end = s.find_last_not_of(whitespace);
    return s.substr(start, end - start + 1);
}

static bool isLeap(int y){
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

static bool isValidDateYYYYMMDD(const std::string& s){
    if(s.size() != 10)return false;

    if(s[4] != '-' || s[7] != '-')return false;

    for(int i : {0,1,2,3,5,6,8,9}){
        if(!std::isdigit(static_cast<unsigned char>(s[i])))return false;
    }

    int y = (s[0]-'0')*1000 + (s[1]-'0')*100 + (s[2]-'0')*10 + (s[3]-'0');
    int m = (s[5]-'0')*10 + (s[6]-'0');
    int d = (s[8]-'0')*10 + (s[9]-'0');

    if(m < 1 || m > 12)return false;

    int mdays[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int maxd = mdays[m];
    if(m == 2 && isLeap(y)) maxd = 29;
    return d>=1 && d<=maxd;
}

expense_tracker::domain::Expense expense_tracker::app::ExpenseService::addExpense(const expense_tracker::domain::Expense &exp) {

    auto title = trim(exp.title);
    auto category = trim(exp.category);
    auto date = trim(exp.date);

    if(title.empty()){
        throw std::invalid_argument("Title must not be empty.");
    }
    if(category.empty()){
        throw std::invalid_argument("Category must not be empty.");
    }
    if(date.empty()){
        throw std::invalid_argument("Date must not be empty.");
    }
    if(!isValidDateYYYYMMDD(date)){
        throw std::invalid_argument("Date must be in YYYY-MM-DD format and be a real date.");
    }
    if(exp.amount <= 0){
        throw std::invalid_argument("Amount must be at least 1.");
    }

    expense_tracker::domain::Expense clean = exp;
    clean.title = title;
    clean.category = category;
    clean.date = date;

    return ExpStorage.add(clean);

}

std::vector<expense_tracker::domain::Expense> expense_tracker::app::ExpenseService::getExpenses() const {
    return ExpStorage.getAll();
}

bool expense_tracker::app::ExpenseService::deleteExpense(int id) {
    return ExpStorage.removeById(id);
}

bool expense_tracker::app::ExpenseService::SearchById(int id) const {
    return ExpStorage.searchById(id);
}
