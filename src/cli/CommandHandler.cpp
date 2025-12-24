#include <expense_tracker/cli/CommandHandler.h>
#include <iostream>
#include <stdexcept>

static std::string trim(const std::string& s){
    const std::string whitespace = " \t\n\r\f\v";

    size_t start = s.find_first_not_of(whitespace);
    if(start == std::string::npos){
        return "";
    }
    size_t end = s.find_last_not_of(whitespace);
    return s.substr(start,end - start + 1);
}

void expense_tracker::cli::CommandHandler::handle(const std::string &line) {

    auto stringHolder = trim(line);
    if(stringHolder.empty())return;

    if(stringHolder == "help"){
        std::cout<<"Commands:"<<std::endl
                 <<"  help"<<std::endl
                 <<"  list"<<std::endl
                 <<"  exit"<<std::endl;
    }
    else if(stringHolder == "list"){
        auto VecExpenses = expenseService.getExpenses();
        if(VecExpenses.empty()){
            std::cout<<"No Expenses."<<std::endl;
            return;
        }

        for(const auto& it : VecExpenses){
            std::cout<<"["<<it.id<<"] "<<it.title<<" | "<<it.category
            <<" | "<<it.amount<<" | "<<it.date<<std::endl;
        }


    }
    else if(stringHolder == "exit"){
        throw std::runtime_error("exit");
    }

    else
        std::cout<<"Unknown Command type 'help'."<<std::endl;




}
