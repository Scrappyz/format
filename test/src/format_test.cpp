#include <gtest/gtest.h>
#include "format.hpp"

using namespace std;

TEST(maxColumnWidths, working)
{
    std::vector<std::vector<std::string>> strs = {
        {"Apple", "Banana", "Cherry"},
        {"Grapes", "Orange", "Pineapple"},
        {"Strawberry", "Kiwi", "Mango"}
    };

    std::vector<int> actual = format::_private_::maxColumnWidths(strs);
    std::vector<int> expected = {10, 6, 9};

    EXPECT_EQ(actual, expected);
}

TEST(maxColumnWidths, with_headers)
{
    std::vector<std::string> headers = {"Header1", "Header2", "Header3"};
    std::vector<std::vector<std::string>> strs = {
        {"Apple", "Banana", "Cherry"},
        {"Grapes", "Orange", "Pineapple"},
        {"Strawberry", "Kiwi", "Mango"}
    };

    std::vector<int> actual = format::_private_::maxColumnWidths(headers, strs);
    std::vector<int> expected = {10, 7, 9};

    EXPECT_EQ(actual, expected);
}