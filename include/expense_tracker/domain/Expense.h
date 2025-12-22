#ifndef EXPENSE_TRACKER_EXPENSE_H
#define EXPENSE_TRACKER_EXPENSE_H

#pragma once
#include <string>

namespace expense_tracker::domain {
    struct Expense{
        int id{0};
        std::string title;
        double amount{0.0};
        std::string category;
        std::string date;
    };
}













#endif //EXPENSE_TRACKER_EXPENSE_H
