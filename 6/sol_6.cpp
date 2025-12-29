#include <string>
#include <algorithm>
#include <vector>
#include <set>

#include "../utility.h"

namespace Day6
{
    using TNum = int64_t;
    
    constexpr char ADD{ '+' };
    constexpr char MULTIPLY{ '*' };

    struct Problem {
        std::vector<TNum> values;
        char op;
    };


    std::vector<Problem> get_problems(const std::string& file_path);
    std::vector<Problem> get_cephalopods_problems(const std::string& file_path);
    TNum solve_problem(const Problem& p);

    TNum sol_6_1(const std::string &file_path)
    {
        auto problem_list = get_problems(file_path);
        TNum grand_total{ };
        
        for (const auto& p : problem_list)
        {
            grand_total += solve_problem(p);
        }

        return grand_total;
    }


    TNum sol_6_2(const std::string &file_path)
    {
        auto problem_list = get_cephalopods_problems(file_path);
        TNum grand_total{ };
        
        for (const auto& p : problem_list)
        {
            grand_total += solve_problem(p);
        }

        return grand_total;
    }

    TNum solve_problem(const Problem& p)
    {
        TNum solution{ };

        if (p.op == ADD)
        {
            for (auto v : p.values)
            {
                solution += v;
            }
        }
        else
        {
            solution = 1;
            for (auto v : p.values)
            {
                solution *= v;
            }
        }

        return solution;
    }

    std::vector<Problem> get_cephalopods_problems(const std::string& file_path)
    {
        std::vector<std::string> input_data{};
        
        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line;
            while(getline(input_file, input_line))
            {
                input_data.push_back(input_line);
            }
            input_file.close();
        }
        
        auto num_lines = input_data.size() - 1; // number of input rows containing numbers
        auto& operator_str = input_data[input_data.size()-1];
        std::vector<Problem> problem_list{ };
        std::vector<char> operator_list{ };
        Problem nxt_problem{ };
        for (auto i=0; i<operator_str.length(); ++i)
        {
            if (operator_str[i] != ' ')
            {
                // a new problem definition starts
                if (nxt_problem.values.size() > 0)
                {
                    problem_list.push_back(nxt_problem);
                }
                nxt_problem = Problem{{ }, operator_str[i]}; 
            }

            // vertically iterate the input data and add next number (if any digit is found in this column)
            bool is_empty{ true };
            TNum val{ 0 };
            for (int j=0; j<num_lines; ++j)
            {
                auto nxt_char = input_data[j][i];
                if (std::isdigit(static_cast<unsigned char>(nxt_char)))
                {
                    is_empty = false;
                    val = val*10 + nxt_char - '0';
                }
            }

            // add number if any digit was found in this column
            if (!is_empty)
            {
                nxt_problem.values.push_back(val);
            }
        }

        // insert last problem after loop
        problem_list.push_back(nxt_problem);

        return problem_list;
    }

    std::vector<Problem> get_problems(const std::string& file_path)
    {
        std::vector<std::string> input_data{};
        
        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line;
            while(getline(input_file, input_line))
            {
                input_data.push_back(input_line);
            }
            input_file.close();
        }
        
        // get number of problems and retrieve operators
        // retrieve operators
        auto& operator_str = input_data[input_data.size()-1];
        std::vector<Problem> problem_list{ };
        std::vector<char> operator_list{ };
        for (auto i=0; i<operator_str.length(); ++i)
        {
            if (operator_str[i] == ' ')
            {
                continue;
            }
            problem_list.push_back(Problem{{ },operator_str[i]});
        }
        
        // add numbers
        for (auto i=0; i<input_data.size()-1; ++i) {
            auto numbers = parse_string_to_number_vec<TNum>(input_data[i]);
            for (auto j=0; j<numbers.size(); ++j)
            {
                problem_list[j].values.push_back(numbers[j]);
            }
        }

        return problem_list;
    }
}
