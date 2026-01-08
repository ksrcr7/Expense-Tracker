#include <expense_tracker/storage/SQLiteExpenseStorage.h>
#include <expense_tracker/app/ExpenseService.h>
#include <expense_tracker/cli/CommandHandler.h>

#include <iostream>
#include <string>
#include <stdexcept>

int main() {
    expense_tracker::storage::SQLiteExpenseStorage SQLite("expenses.db");
    expense_tracker::app::ExpenseService ExpService(SQLite);
    expense_tracker::cli::CommandHandler Command(ExpService);

    std::cout<<"Expense Tracker (type 'help')\n";

    while (true){
        std::cout<<"> ";
        std::string line;
        if(!std::getline(std::cin,line))break;

        try{
            Command.handle(line);
        }
        catch (std::runtime_error& e) {
            if(std::string (e.what()) == "exit")break;
            std::cout<<"Error: "<<e.what()<<std::endl;
        }
        catch (const std::invalid_argument& e) {
            std::cout<<e.what()<<std::endl;
        }
        catch (const std::exception& e) {
            std::cout<<"Error: "<<e.what()<<std::endl;
        }
    }
    std::cout<<"Bye!\n";
    return 0;
}
