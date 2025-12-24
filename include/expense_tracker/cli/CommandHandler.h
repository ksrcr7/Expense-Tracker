#pragma once
#include <expense_tracker/app/ExpenseService.h>
#include <string>

namespace expense_tracker::cli{
    class CommandHandler{
    private:
        expense_tracker::app::ExpenseService& expenseService;

    public:
        explicit CommandHandler(expense_tracker::app::ExpenseService& expService): expenseService(expService){};
        void handle(const std::string& line);
    };
}