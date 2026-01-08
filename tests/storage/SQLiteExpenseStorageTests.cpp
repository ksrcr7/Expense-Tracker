#include <gtest/gtest.h>
#include <expense_tracker/storage/SQLiteExpenseStorage.h>
#include <expense_tracker/domain/Expense.h>

#include <filesystem>
#include <string>

namespace fs = std::filesystem;
using expense_tracker::storage::SQLiteExpenseStorage;
using expense_tracker::domain::Expense;

class SQLiteExpenseStorageFixture : public ::testing::Test {
protected:
    std::string dbPath;

    void SetUp()override{
        dbPath = (fs::temp_directory_path() / fs::path("expense_tracker_test.db")).string();
        std::error_code ec;
        fs::remove(dbPath,ec);

    }

    void TearDown() override{
        std::error_code ec;
        fs::remove(dbPath,ec);
    }
};

TEST_F(SQLiteExpenseStorageFixture,SearchById_NotFound){
    SQLiteExpenseStorage storage(dbPath);
    EXPECT_FALSE(storage.searchById(9999));
}

TEST_F(SQLiteExpenseStorageFixture,Add_ReturnsIdAndPersists){
    SQLiteExpenseStorage storage(dbPath);
    Expense exp;

    exp.title = "pizza";
    exp.category = "food";
    exp.amount = 3;
    exp.date = "2026-1-01";

    Expense saved = storage.add(exp);
    EXPECT_GT(saved.id,0);
    EXPECT_TRUE(storage.searchById(saved.id));
}

TEST_F(SQLiteExpenseStorageFixture,GetAll_ReturnsAllows){
    SQLiteExpenseStorage storage(dbPath);
    Expense a{0, "coffee", 3.5, "food", "2025-01-01"};
    Expense b{0, "uber", 12.0, "transport", "2025-01-02"};

    storage.add(a);
    storage.add(b);

    auto All = storage.getAll();
    EXPECT_EQ(All.size(),2u);
}

TEST_F(SQLiteExpenseStorageFixture, RemoveById_Deletes) {
    SQLiteExpenseStorage storage(dbPath);

    Expense e{0, "book", 10.0, "study", "2025-02-02"};
    Expense saved = storage.add(e);

    EXPECT_TRUE(storage.removeById(saved.id));
    EXPECT_FALSE(storage.removeById(saved.id));
    EXPECT_FALSE(storage.searchById(saved.id));
}
