#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace format {

    class Table {
        private:
            std::vector<std::string> headers_;
            std::vector<std::vector<std::string>> table_values_;

            std::vector<std::vector<std::string>> cleanTableValues(const std::vector<std::vector<std::string>>& table_values) const
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

            std::vector<int> maxColumnWidths(const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& table_values) const
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

            std::vector<int> maxColumnWidths(const std::vector<std::vector<std::string>>& table_values) const
            {
                return maxColumnWidths({}, table_values);
            }
        
        public:
            Table() : headers_(), table_values_() {}
            Table(const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& table_values)
            : headers_(headers), table_values_(table_values) {}

            Table(const std::vector<std::vector<std::string>>& table_values) : headers_(), table_values_(table_values) {}

            std::string getString(const std::string& column_border, const std::string& row_border, int left_margin) const
            {
                std::string table;
                std::vector<std::vector<std::string>> cleaned_table_values = cleanTableValues(table_values_);
                std::vector<int> col_widths = maxColumnWidths(headers_, cleaned_table_values);

                char row_char = row_border.empty() ? '\0' : row_border[0];

                for(int i = 0; i < headers_.size(); i++) {
                    table += headers_[i];
                    if(i < headers_.size()-1) {
                        table += std::string(col_widths[i] - headers_[i].size(), ' ') + column_border;
                    }
                }

                if(!headers_.empty()) {
                    table += '\n';
                    for(int i = 0; i < col_widths.size(); i++) {
                        table += std::string(col_widths[i], row_char);
                        if(i < col_widths.size()-1) {
                            table += column_border;
                        }
                    }
                    table += '\n';
                }

                if(cleaned_table_values.empty()) {
                    return table;
                }

                // Subtract the max width with the size of the string to get the correct amount of space
                for(int i = 0; i < cleaned_table_values.size(); i++) {
                    for(int j = 0; j < cleaned_table_values[i].size(); j++) {
                        std::string value = cleaned_table_values[i][j];
                        table += value;
                        if(j < cleaned_table_values[i].size()-1) {
                            table += std::string(col_widths[j] - value.size(), ' ') + column_border;
                        }
                    }
                    table += '\n';

                    
                }

                return table;
            }

            void print(const std::string& column_border, const std::string& row_border, int left_margin) const
            {
                std::cout << getString(column_border, row_border, left_margin);
            }
    };

    namespace _private_ {
        std::vector<std::vector<std::string>> cleanTableValues(const std::vector<std::vector<std::string>>& table_values);
        std::vector<int> maxColumnWidths(const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& table_values);
        std::vector<int> maxColumnWidths(const std::vector<std::vector<std::string>>& table_values);
    }

    inline std::string table(const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& table_values,
                             const std::string& column_border, const std::string& row_border, const std::string& left_margin)
    {
        std::string table;
        std::vector<std::vector<std::string>> cleaned_table_values = _private_::cleanTableValues(table_values);
        std::vector<int> col_widths = _private_::maxColumnWidths(headers, cleaned_table_values);

        for(int i = 0; i < headers.size(); i++) {
            table += headers[i];
            if(i < headers.size()-1) {
                table += std::string(col_widths[i] - headers[i].size(), ' ') + column_border;
            }
        }

        if(!headers.empty()) {
            table += '\n';
        }

        if(cleaned_table_values.empty()) {
            return table;
        }

        // Subtract the max width with the size of the string to get the correct amount of space
        for(int i = 0; i < cleaned_table_values.size(); i++) {
            for(int j = 0; j < cleaned_table_values[i].size(); j++) {
                std::string value = cleaned_table_values[i][j];
                table += value;
                if(j < cleaned_table_values[i].size()-1) {
                    table += std::string(col_widths[j] - value.size(), ' ') + column_border;
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

        return table(headers, table_values, std::string(space, ' '), "", "");
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