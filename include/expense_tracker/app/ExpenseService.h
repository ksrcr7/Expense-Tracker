#pragma once
#include <expense_tracker/storage/IExpenseStorage.h>
#include <string>
#include <vector>


namespace expense_tracker::app{
    class ExpenseService{
    private:
        expense_tracker::storage::IExpenseStorage& ExpStorage;

    public:
        explicit ExpenseService(expense_tracker::storage::IExpenseStorage& storage):ExpStorage(storage){};
        expense_tracker::domain::Expense addExpense(const expense_tracker::domain::Expense& exp);
        [[nodiscard]] std::vector<expense_tracker::domain::Expense> getExpenses()const;
        bool deleteExpense(int id);
        [[nodiscard]] bool SearchById(int id)const;
    };
}



