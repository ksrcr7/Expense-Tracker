#pragma once
#include <expense_tracker/storage/IExpenseStorage.h>
#include <string>
#include <utility>

struct sqlite3;


namespace expense_tracker::storage{


    class SQLiteExpenseStorage : public IExpenseStorage{
    private:
        std::string dbPath_;
        sqlite3* db_ = nullptr;

    public:
        explicit SQLiteExpenseStorage(std::string);
        expense_tracker::domain::Expense add(const expense_tracker::domain::Expense& exp)override;
        [[nodiscard]] std::vector<expense_tracker::domain::Expense> getAll()const override;
        bool removeById(int id)override;
        [[nodiscard]] bool searchById(int id)const override;
        ~SQLiteExpenseStorage()override;


    };
}


