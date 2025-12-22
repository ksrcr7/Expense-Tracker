#pragma once
#include <expense_tracker/storage/IExpenseStorage.h>
#include <vector>

namespace expense_tracker::storage {
    class InMemoryExpenseStorage final : public expense_tracker::storage::IExpenseStorage{
    private:
        int nextId_{1};
        std::vector<expense_tracker::domain::Expense> expenses_;
    public:
        expense_tracker::domain::Expense add(const expense_tracker::domain::Expense& exp)override;
        [[nodiscard]] std::vector<expense_tracker::domain::Expense> getAll()const override;
        bool removeById(int id)override;
    };
}













