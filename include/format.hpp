#pragma once

#include <string>
#include <vector>

namespace format {

    namespace _private_ {
        std::vector<std::vector<std::string>> cleanTableValues(const std::vector<std::vector<std::string>>& table_values);
        std::vector<int> maxColumnWidths(const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& table_values);
        std::vector<int> maxColumnWidths(const std::vector<std::vector<std::string>>& table_values);
    }

    inline std::string table(const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& table_values,
                             const std::string& border_between, const std::string& border_left, const std::string& border_right,
                             const std::string& left_margin)
    {
        std::string table;
        std::vector<std::vector<std::string>> cleaned_table_values = _private_::cleanTableValues(table_values);

        if(cleaned_table_values.empty()) {
            return table;
        }

        std::vector<int> col_widths = _private_::maxColumnWidths(cleaned_table_values);

        // Subtract the max width with the size of the string to get the correct amount of space
        for(int i = 0; i < cleaned_table_values.size(); i++) {
            for(int j = 0; j < cleaned_table_values[i].size(); j++) {
                std::string value = cleaned_table_values[i][j];
                table += value;
                if(j < cleaned_table_values[i].size()-1) {
                    table += std::string(col_widths[j] - value.size(), ' ') + border_between;
                }
            }
            table += '\n';
        }

        return table;
    }

    inline std::string table(const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& table_values,
                             int space = 1)
    {
        if(space < 0) {
            space = 0;
        }

        return table(headers, table_values, std::string(space, ' '), "", "", "");
    }

    inline std::string table(const std::vector<std::vector<std::string>>& table_values, int space = 1) 
    {
        return table({}, table_values, space);
    }

    namespace _private_ {
        inline std::vector<std::vector<std::string>> cleanTableValues(const std::vector<std::vector<std::string>>& table_values)
        {
            /*
                Add new rows based on the max number of endlines found in the row
                Eg:
                {
                    {"Apple", "Banana\nNana\nBan", "Cherry\nHenry"},
                    {"Grapes", "Orange", "Pineapple"}
                }
                to
                {
                    {"", "", ""},
                    {"", "", ""},
                    {"", "", ""},
                    {"", "", ""}
                }

                In this case, the max number of '\n' in the first row is 2. 
                So we add 2 more rows.
            */
            std::vector<std::vector<std::string>> cleaned_table_values(table_values.size(), std::vector<std::string>(table_values[0].size(), ""));
            for(int i = 0; i < table_values.size(); i++) {
                int max_endlines = 0;
                for(int j = 0; j < table_values[i].size(); j++) {
                    int endlines = 0;

                    for(int k = 0; k < table_values[i][j].size(); k++) {
                        char ch = table_values[i][j][k];

                        if(ch == '\n') {
                            endlines++;
                        }
                    }

                    max_endlines = std::max(max_endlines, endlines);
                }

                for(int j = 0; j < max_endlines; j++) {
                    cleaned_table_values.push_back(std::vector<std::string>(table_values[i].size(), ""));
                }
            }

            /*
                Put the strings next to the newline to the next row.
                Eg:
                {
                    {"Apple", "Banana\nNana\nBan", "Cherry\nHenry"},
                    {"Grapes", "Orange", "Pineapple"}
                }
                to
                {
                    {"Apple", "Banana", "Cherry"},
                    {"", "Nana", "Henry"},
                    {"", "Ban", ""},
                    {"Grapes", "Orange", "Pineapple"}
                }
            */
            int temp_i = 0;
            for(int i = 0; i < table_values.size(); i++) {
                int max_endlines = 0;

                for(int j = 0; j < table_values[i].size(); j++) {
                    int endlines = 0;

                    for(int k = 0; k < table_values[i][j].size(); k++) {
                        char ch = table_values[i][j][k];

                        if(ch == '\n') {
                            endlines++;
                            continue;
                        }

                        cleaned_table_values[temp_i + endlines][j].push_back(ch);
                    }

                    max_endlines = std::max(max_endlines, endlines);
                }

                temp_i += max_endlines + 1;
            }

            return cleaned_table_values;
        }

        inline std::vector<int> maxColumnWidths(const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& table_values)
        {
            // Determine the maximum width for all rows
            std::vector<int> col_widths;

            if(!headers.empty()) {
                col_widths = std::vector<int>(headers.size(), 0);
            } else if(!table_values.empty()) {
                col_widths = std::vector<int>(table_values[0].size(), 0);
            } else {
                return col_widths;
            }

            std::vector<std::vector<std::string>> combined;
            combined.push_back(headers);
            combined.insert(combined.end(), table_values.begin(), table_values.end());

            for(int i = 0; i < combined.size(); i++) {
                for(int j = 0; j < combined[i].size(); j++) {
                    if(combined[i][j].size() > col_widths[j]) {
                        col_widths[j] = combined[i][j].size();
                    }
                }
            }

            return col_widths;
        }

        inline std::vector<int> maxColumnWidths(const std::vector<std::vector<std::string>>& table_values)
        {
            return maxColumnWidths({}, table_values);
        }
    }
}