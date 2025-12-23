#pragma once
#include <expense_tracker/storage/InMemoryExpenseStorage.h>
#include <expense_tracker/storage/IExpenseStorage.h>
#include <string>

class ExpenseService{
private:
    expense_tracker::storage::IExpenseStorage& ExpStorage;

public:
    explicit ExpenseService(expense_tracker::storage::IExpenseStorage& storage):ExpStorage(storage){};
    expense_tracker::domain::Expense addExpense(const expense_tracker::domain::Expense& exp);

};

