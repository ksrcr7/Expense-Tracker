#include <expense_tracker/storage/InMemoryExpenseStorage.h>
#include <algorithm>


expense_tracker::domain::Expense expense_tracker::storage::InMemoryExpenseStorage::add(
        const expense_tracker::domain::Expense &exp) {

    auto temp = exp;
    if(temp.id == 0){
        temp.id = nextId_;
        ++nextId_;
    }
    expenses_.emplace_back(temp);
    return temp;

}

std::vector<expense_tracker::domain::Expense> expense_tracker::storage::InMemoryExpenseStorage::getAll() const {
    return expenses_;
}

bool expense_tracker::storage::InMemoryExpenseStorage::removeById(int id) {
    auto it = std::find_if(expenses_.begin(),expenses_.end(),
                           [id](const domain::Expense& exp){return id == exp.id;});

    if(it == expenses_.end())return false;

    expenses_.erase(it);
    return true;
}
