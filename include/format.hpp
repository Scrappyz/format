#pragma once

#include <string>
#include <vector>

namespace format {
    std::string table(const std::vector<std::vector<std::string>>& matrix, int space = 1) 
    {
        if(space < 0) {
            space = 0;
        }

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
        std::vector<std::vector<std::string>> cleaned_matrix(matrix.size(), std::vector<std::string>(matrix[0].size(), ""));
        for(int i = 0; i < matrix.size(); i++) {
            int max_endlines = 0;
            for(int j = 0; j < matrix[i].size(); j++) {
                int endlines = 0;

                for(int k = 0; k < matrix[i][j].size(); k++) {
                    char ch = matrix[i][j][k];

                    if(ch == '\n') {
                        endlines++;
                    }
                }

                max_endlines = std::max(max_endlines, endlines);
            }

            for(int j = 0; j < max_endlines; j++) {
                cleaned_matrix.push_back(std::vector<std::string>(matrix[i].size(), ""));
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
        for(int i = 0; i < matrix.size(); i++) {
            int max_endlines = 0;

            for(int j = 0; j < matrix[i].size(); j++) {
                int endlines = 0;

                for(int k = 0; k < matrix[i][j].size(); k++) {
                    char ch = matrix[i][j][k];

                    if(ch == '\n') {
                        endlines++;
                        continue;
                    }

                    cleaned_matrix[temp_i + endlines][j].push_back(ch);
                }

                max_endlines = std::max(max_endlines, endlines);
            }

            temp_i += max_endlines + 1;
        }

        // Determine the maximum width for all rows
        int max_width = 0;
        for(int i = 0; i < cleaned_matrix.size(); i++) {
            int max_length = 0;

            for(int j = 0; j < cleaned_matrix[i].size(); j++) {
                if(cleaned_matrix[i][j].size() > max_length) {
                    max_length = cleaned_matrix[i][j].size();
                }
            }

            if(max_length + space > max_width) {
                max_width = max_length + space;
            }
        }

        std::string table;

        // Subtract the max width with the size of the string to get the correct amount of space
        for(int i = 0; i < cleaned_matrix.size(); i++) {
            for(int j = 0; j < cleaned_matrix[i].size(); j++) {
                table += cleaned_matrix[i][j];
                if(j < cleaned_matrix[i].size()-1) {
                    table += std::string(max_width - cleaned_matrix[i][j].size(), ' ');
                }
            }
            table += '\n';
        }

        return table;
    }
}