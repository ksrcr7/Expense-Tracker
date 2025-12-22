#ifndef EXPENSE_TRACKER_IEXPENSESTORAGE_H
#define EXPENSE_TRACKER_IEXPENSESTORAGE_H

#pragma once
#include <expense_tracker/domain/Expense.h>
#include <vector>

namespace expense_tracker::storage{
    class IExpenseStorage{
    public:
        virtual ~IExpenseStorage() = default;
        virtual expense_tracker::domain::Expense add(const expense_tracker::domain::Expense&) = 0;
        [[nodiscard]] virtual std::vector<expense_tracker::domain::Expense> getAll() const = 0;
        virtual bool removeById(int id) = 0;
    };
}




















#endif //EXPENSE_TRACKER_IEXPENSESTORAGE_H
