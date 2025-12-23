#include <gtest/gtest.h>
#include <expense_tracker/storage/InMemoryExpenseStorage.h>
#include <expense_tracker/domain/Expense.h>

TEST(InMemoryStorageTest,ADD){
    expense_tracker::storage::InMemoryExpenseStorage InMemo;
    expense_tracker::domain::Expense exp1;
    expense_tracker::domain::Expense exp2;

    exp1.title = "Pizza";
    exp1.date = "1997-11-22";
    exp1.amount = 3;
    exp1.category = "Food";

    exp2.title = "Rice";
    exp2.date = "2025-08-10";
    exp2.amount = 5;
    exp2.category = "Food";

    auto saved1 = InMemo.add(exp1);
    auto saved2 = InMemo.add(exp2);
    auto all = InMemo.getAll();

    EXPECT_GT(saved1.id,0);
    EXPECT_EQ(saved2.id,saved1.id + 1);
    ASSERT_EQ(all.size(),2);




}
