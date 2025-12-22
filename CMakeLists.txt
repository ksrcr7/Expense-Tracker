cmake_minimum_required(VERSION 3.27)
project(Expense_Tracker VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------- Core library ----------------
add_library(expense_tracker_lib
        src/storage/InMemoryExpenseStorage.cpp
        src/storage/SQLiteExpenseStorage.cpp
        src/app/ExpenseService.cpp
        src/cli/CommandHandler.cpp

        # SQLite amalgamation (vendored)
        third_party/sqlite/sqlite3.c
)

target_include_directories(expense_tracker_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/sqlite
)

if (WIN32)
    target_compile_definitions(expense_tracker_lib PRIVATE SQLITE_THREADSAFE=1)
endif()

# ---------------- App executable ----------------
add_executable(Expense_Tracker
        src/main.cpp
        include/expense_tracker/storage/IExpenseStorage.h
        include/expense_tracker/storage/InMemoryExpenseStorage.h
        include/expense_tracker/domain/Expense.h
)

target_link_libraries(Expense_Tracker
        PRIVATE expense_tracker_lib
)

# ---------------- Tests ----------------
include(CTest)
enable_testing()
add_subdirectory(tests)
