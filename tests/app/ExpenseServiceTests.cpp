#include <gtest/gtest.h>
#include <expense_tracker/storage/InMemoryExpenseStorage.h>
#include <expense_tracker/app/ExpenseService.h>

TEST(ExpenseServiceTests,Add){
    expense_tracker::storage::InMemoryExpenseStorage InMemo;
    expense_tracker::app::ExpenseService expenseService(InMemo);
    expense_tracker::domain::Expense exp;

    exp.title = "Pizza";
    exp.category = "Food";
    exp.amount = 12.5;
    exp.date = "2025-03-15";

    auto saved = expenseService.addExpense(exp);
    EXPECT_GT(saved.id,0);

    auto all = expenseService.getExpenses();
    ASSERT_EQ(all.size(),1u);
    EXPECT_EQ(all[0].title,"Pizza");

}

TEST(ExpenseServiceTests,TitleThrowErrorException){
    expense_tracker::storage::InMemoryExpenseStorage InMemo;
    expense_tracker::app::ExpenseService expenseService(InMemo);
    expense_tracker::domain::Expense exp;

    exp.title = " ";
    exp.category = "Food";
    exp.amount = 12.5;
    exp.date = "2025-03-15";

    auto all = expenseService.getExpenses();
    EXPECT_THROW(expenseService.addExpense(exp),std::invalid_argument);
    EXPECT_EQ(all.size(),0u);
}

TEST(ExpenseServiceTests,DateThrowErrorException){
    expense_tracker::storage::InMemoryExpenseStorage InMemo;
    expense_tracker::app::ExpenseService expenseService(InMemo);
    expense_tracker::domain::Expense exp1;
    expense_tracker::domain::Expense exp2;

    exp1.title = "Pizza";
    exp1.category = "Food";
    exp1.amount = 12.5;
    exp1.date = "2025-02-30";

    auto all = expenseService.getExpenses();
    EXPECT_THROW(expenseService.addExpense(exp1),std::invalid_argument);
    all = expenseService.getExpenses();
    EXPECT_EQ(all.size(),0u);

    exp2.title = "Pizza";
    exp2.category = "Food";
    exp2.amount = 12.5;
    exp2.date = "30-02-2025";

    all = expenseService.getExpenses();
    EXPECT_THROW(expenseService.addExpense(exp2),std::invalid_argument);
    all = expenseService.getExpenses();
    EXPECT_EQ(all.size(),0u);

}

TEST(ExpenseServiceTests,AmountThrowErrorException){
    expense_tracker::storage::InMemoryExpenseStorage InMemo;
    expense_tracker::app::ExpenseService expenseService(InMemo);
    expense_tracker::domain::Expense exp1;

    exp1.title = "Pizza";
    exp1.category = "Food";
    exp1.amount = -3;
    exp1.date = "2025-02-11";

    auto all = expenseService.getExpenses();
    EXPECT_THROW(expenseService.addExpense(exp1),std::invalid_argument);
    all = expenseService.getExpenses();
    EXPECT_EQ(all.size(),0u);
}

TEST(ExpenseServiceTests,DeleteExpense){
    expense_tracker::storage::InMemoryExpenseStorage InMemo;
    expense_tracker::app::ExpenseService expenseService(InMemo);
    expense_tracker::domain::Expense exp1;
    expense_tracker::domain::Expense exp2;

    exp1.title = "Pizza";
    exp1.category = "Food";
    exp1.amount = 3;
    exp1.date = "2025-02-11";

    exp2.title = "Pc";
    exp2.category = "Electronic";
    exp2.amount = 4;
    exp2.date = "2025-05-08";

    auto saved1 = expenseService.addExpense(exp1);
    auto saved2 = expenseService.addExpense(exp2);
    auto all = expenseService.getExpenses();

    EXPECT_EQ(all.size(),2u);
    EXPECT_TRUE(expenseService.deleteExpense(saved1.id));
    all = expenseService.getExpenses();
    EXPECT_EQ(all.size(),1u);
    EXPECT_EQ(all[0].title,"Pc");

}

TEST(ExpenseService,DeleteMissing){
    expense_tracker::storage::InMemoryExpenseStorage InMemo;
    expense_tracker::app::ExpenseService expenseService(InMemo);
    expense_tracker::domain::Expense exp1;
    expense_tracker::domain::Expense exp2;

    exp1.title = "Pizza";
    exp1.category = "Food";
    exp1.amount = 3;
    exp1.date = "2025-02-11";

    exp2.title = "Pc";
    exp2.category = "Electronic";
    exp2.amount = 4;
    exp2.date = "2025-05-08";

    auto saved1 = expenseService.addExpense(exp1);
    auto saved2 = expenseService.addExpense(exp2);
    auto all = expenseService.getExpenses();

    EXPECT_FALSE(expenseService.deleteExpense(999));
}


