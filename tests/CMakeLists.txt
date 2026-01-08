include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(expense_tracker_tests
storage/InMemoryExpenseStorageTests.cpp
        app/ExpenseServiceTests.cpp
        storage/SQLiteExpenseStorageTests.cpp
        storage/SQLiteExpenseStorageTests.cpp
)

target_link_libraries(expense_tracker_tests
PRIVATE expense_tracker_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(expense_tracker_tests)