#include <expense_tracker/storage/SQLiteExpenseStorage.h>
#include "sqlite3.h"
#include <stdexcept>


expense_tracker::domain::Expense
expense_tracker::storage::SQLiteExpenseStorage::add(const expense_tracker::domain::Expense &exp) {
    const char* sql = "INSERT INTO expenses (title,category,amount,date) "
                      "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db_,sql,-1,&stmt, nullptr);
    if(rc != SQLITE_OK){
        throw std::runtime_error(std::string("prepare failed: ") + sqlite3_errmsg(db_));
    }

    try {
        rc = sqlite3_bind_text(stmt,1,exp.title.c_str(),-1,SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) throw std::runtime_error(std::string("bind title failed: ") + sqlite3_errmsg(db_));

        rc = sqlite3_bind_text(stmt,2,exp.category.c_str(),-1,SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) throw std::runtime_error(std::string("bind category failed: ") + sqlite3_errmsg(db_));

        rc = sqlite3_bind_double(stmt,3,exp.amount);
        if (rc != SQLITE_OK) throw std::runtime_error(std::string("bind amount failed: ") + sqlite3_errmsg(db_));

        rc = sqlite3_bind_text(stmt,4,exp.date.c_str(),-1,SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) throw std::runtime_error(std::string("bind date failed: ") + sqlite3_errmsg(db_));

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE){
            throw std::runtime_error(std::string("step failed: ") + sqlite3_errmsg(db_));
        }

        int newId = static_cast<int>(sqlite3_last_insert_rowid(db_));

        expense_tracker::domain::Expense result = exp;
        result.id = newId;
        sqlite3_finalize(stmt);
        return result;
    }
    catch (...){
        sqlite3_finalize(stmt);
        throw;
    }

}

expense_tracker::storage::SQLiteExpenseStorage::SQLiteExpenseStorage(std::string path):
    dbPath_(std::move(path)){

    int rc = sqlite3_open(dbPath_.c_str(),&db_);
    if(rc != SQLITE_OK){
        std::string msg = db_ ? sqlite3_errmsg(db_) : "unknown error";

        if(db_){
            sqlite3_close(db_);
            db_ = nullptr;
        }
        throw std::runtime_error("Failed to open DB: " + msg);

    }

    const char* createTableSql =
            "CREATE TABLE IF NOT EXISTS expenses ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "title TEXT NOT NULL,"
            "category TEXT NOT NULL,"
            "amount REAL NOT NULL,"
            "date TEXT NOT NULL"
            ");";

    char* errMsg = nullptr;
    rc = sqlite3_exec(db_,createTableSql, nullptr, nullptr,&errMsg);
    if(rc != SQLITE_OK){
        std::string msg = errMsg ? errMsg : "unknown sqlite exec error";
        sqlite3_free(errMsg);

        sqlite3_close(db_);
        db_ = nullptr;
        throw std::runtime_error("Failed to create schema: " + msg);
    }

    



}

expense_tracker::storage::SQLiteExpenseStorage::~SQLiteExpenseStorage() {
        if(db_){
            sqlite3_close(db_);
            db_ = nullptr;
        }
}

std::vector<expense_tracker::domain::Expense> expense_tracker::storage::SQLiteExpenseStorage::getAll() const {

    const char* sql = "SELECT id, title, category, amount, date "
                      "FROM expenses "
                      "ORDER BY id DESC;";

    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db_,sql,-1,&stmt, nullptr);
    if(rc != SQLITE_OK){
        throw std::runtime_error(std::string("prepare failed: ") + sqlite3_errmsg(db_));
    }

    std::vector<expense_tracker::domain::Expense> result;

    try {
        while (true){
            rc = sqlite3_step(stmt);

            if(rc == SQLITE_ROW){
                expense_tracker::domain::Expense e;

                e.id = sqlite3_column_int(stmt,0);

                const unsigned char* title = sqlite3_column_text(stmt,1);
                e.title = title ? reinterpret_cast<const char*>(title) : "";

                const unsigned char* category = sqlite3_column_text(stmt,2);
                e.category = category ? reinterpret_cast<const char*>(category) : "";

                e.amount = sqlite3_column_double(stmt,3);

                const unsigned char* date = sqlite3_column_text(stmt,4);
                e.date = date ? reinterpret_cast<const char*>(date) : "";

                result.emplace_back(e);
                continue;
            }
            if(rc == SQLITE_DONE){
                break;
            }
            throw std::runtime_error(std::string("step failed: ") + sqlite3_errmsg(db_));
        }
        sqlite3_finalize(stmt);
        return result;
    }

    catch (...) {
        sqlite3_finalize(stmt);
        throw;

    }




}

bool expense_tracker::storage::SQLiteExpenseStorage::removeById(int id) {

}

bool expense_tracker::storage::SQLiteExpenseStorage::searchById(int id) const {
    const char* sql = "SELECT 1 FROM expenses WHERE id = ? LIMIT 1;";

    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db_,sql,-1,&stmt, nullptr);
    if(rc != SQLITE_OK){
        throw std::runtime_error(std::string("prepare failed: ")+ sqlite3_errmsg(db_));
    }
    try {
        rc = sqlite3_bind_int(stmt,1,id);
        if(rc != SQLITE_OK){
            throw std::runtime_error(std::string("bind id failed: ")+ sqlite3_errmsg(db_));
        }
        rc = sqlite3_step(stmt);

        if(rc == SQLITE_ROW){
            sqlite3_finalize(stmt);
            return true;
        }

        if(rc == SQLITE_DONE){
            sqlite3_finalize(stmt);
            return false;
        }


        throw std::runtime_error(std::string("step failed: ") + sqlite3_errmsg(db_));

    }
    catch (...) {
        sqlite3_finalize(stmt);
        throw;
    }

}
