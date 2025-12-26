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

static void printAddUsage(){
    std::cout << R"(Usage: add --title "..." --amount 12.5 --category "..." --date YYYY-MM-DD)" << std::endl;
}

static void printDeleteUsage(){
    std::cout << R"(Usage: delete --id N)" << std::endl;
}

static std::vector<std::string> tokenize(const std::string& line){
    std::vector<std::string> token;
    std::string cur;
    bool inside = false;

    for(char c : line){
        if(c == '"'){
            inside = !inside;
            continue;
        }
        if(!inside && std::isspace(static_cast<unsigned char>(c))){
            if(!cur.empty()){
                token.emplace_back(cur);
                cur.clear();
            }
        }
        else{
            cur.push_back(c);
        }
    }

    if(!cur.empty())token.emplace_back(cur);
    return token;

}

void expense_tracker::cli::CommandHandler::handle(const std::string &line) {

    auto stringHolder = trim(line);
    if (stringHolder.empty()) return;
    auto tokens = tokenize(stringHolder);
    if(tokens.empty())return;
    auto cmd = tokens[0];



    if(cmd == "help"){
        std::cout<<"Commands:"<<std::endl
                 <<"  add"<<std::endl
                 <<"  delete"<<std::endl
                 <<"  help"<<std::endl
                 <<"  list"<<std::endl
                 <<"  exit"<<std::endl;
    }
    else if(cmd == "list"){
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
    else if(cmd == "add"){
        std::string title,category,date;
        double amount = 0;
        bool hasTitle = false; bool hasCategory = false; bool hasDate = false; bool hasAmount = false;

        for(size_t i = 1; i < tokens.size(); ++i){

            const auto& it = tokens[i];

            if(it == "--title" && (i+1)<tokens.size()){
                title = tokens[++i];
                hasTitle = true;
                continue;
            }
            else if(it == "--category" && (i+1)<tokens.size()){
                category = tokens[++i];
                hasCategory = true;
                continue;
            }
            else if(it == "--date" && (i+1)<tokens.size()){
                date = tokens[++i];
                hasDate  = true;
                continue;
            }
            else if(it == "--amount" && (i+1)<tokens.size()){
                try {
                    amount = std::stod(tokens[++i]);
                }
                catch(...){
                    std::cout<<"Invalid amount."<<std::endl;
                    printAddUsage();
                    return;
                }
                hasAmount = true;
                continue;
            }
            else{
                printAddUsage();
                return;
            }


        }

        if(!hasTitle){
            std::cerr<<"Title is missed or unknown."<<std::endl;
            return;
        }
        else if(!hasCategory){
            std::cerr<<"Category is missed or unknown."<<std::endl;
            return;
        }
        else if(!hasAmount){
            std::cerr<<"Amount is missed or unknown."<<std::endl;
            return;
        }
        else if(!hasDate){
            std::cerr<<"Date is missed or unknown."<<std::endl;
            return;
        }

        else{
            expense_tracker::domain::Expense exp;
            exp.title = title;
            exp.category = category;
            exp.date = date;
            exp.amount = amount;

            auto saved = expenseService.addExpense(exp);
            std::cout<<"Added expense with id = "<<saved.id<<std::endl;
        }
    }

    else if(cmd == "delete"){
        int id = 0;
        bool hasId = false;

        for(size_t i = 1; i < tokens.size(); ++i){
            const auto& it = tokens[i];

            if(it == "--id" && (i+1) < tokens.size()){
                try{
                    id = std::stoi(tokens[++i]);
                }
                catch(...){
                    std::cout<<"Invalid id."<<std::endl;
                    printDeleteUsage();
                    return;
                }
                hasId = true;
            }
            else{
                printDeleteUsage();
                return;
            }


        }
        if(!hasId){
            printDeleteUsage();
            return;
        }
        bool ok = expenseService.deleteExpense(id);

        if(ok){
            std::cout<<"Deleted."<<std::endl;
        }
        else
            std::cout<<"Not found."<<std::endl;
        return;
    }


    else if(cmd == "exit"){
        throw std::runtime_error("exit");
    }



    else
        std::cout<<"Unknown Command type 'help'."<<std::endl;




}
