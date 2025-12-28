#include <string>

#include "../utility.h"


namespace Day5
{

    using TId = uint64_t;

    struct IdRange
    {
        TId lower_bound{};
        TId upper_bound{};
        bool operator<(const IdRange &r){
        if (lower_bound != r.lower_bound) return lower_bound < r.lower_bound;
        else return upper_bound < r.upper_bound;
        }
    };

    struct InputDataDay5_1
    {
        std::vector<IdRange> range_list;
        std::vector<TId> id_vec;
    };

    InputDataDay5_1 get_input_5_1(const std::string &file_path);


    /*
    Brute force: check for each value if it is inside any range
    */
    int sol_5_1(const std::string &file_path)
    {
        InputDataDay5_1 data_in = get_input_5_1(file_path);
        int in_range_count{ 0 };

        for (const auto& id_val : data_in.id_vec) 
        {
            for (const auto& r : data_in.range_list)
            {
                if (id_val >= r.lower_bound && id_val <= r.upper_bound)
                {
                    ++in_range_count;
                    break;
                }
            }

        }

        return in_range_count;
    }


    /*
    Either merge ranges or what is even faster here is to keep track of the largest already considered Id value and then 
    check for each new range (ranges are sorted with inreasing lower_bound value) whether it adds new Id values based 
    on current largest considered Id.
    */
    TId sol_5_2(const std::string &file_path)
    {
        TId range_sum{ };
        InputDataDay5_1 data_in = get_input_5_1(file_path);
        auto& ranges = data_in.range_list;

        // sort ranges based on < operator (smaller lower_bound values)
        std::sort(ranges.begin(), ranges.end());

        TId current_upper_bound{ }; // tracks up to which value Ids are already considered inside the accumulated range sum

        for (const auto& r : ranges)
        {
            if (r.upper_bound < current_upper_bound)
            {
                // skip ranges that are completely contained in previous range
                // alternatively could also remove this check and only add positive (r.upper_bound - current_upper_bound) values
                continue;
            }

            if (r.lower_bound > current_upper_bound)
            {
                // increase current_upper_bound if next range has no overlap with previous range
                current_upper_bound = r.lower_bound;
            }
            
            range_sum += r.upper_bound - current_upper_bound + 1;
            current_upper_bound = r.upper_bound + 1;
        }

        return range_sum;
    }


    InputDataDay5_1 get_input_5_1(const std::string &file_path)
    {
        InputDataDay5_1 input_data{ };

        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line{ };

            // get ranges until an empty line is hit
            while(getline(input_file, input_line) && input_line != "")
            {
                auto nums = parse_string_to_number_vec<TId>(input_line);
                input_data.range_list.push_back( IdRange{ nums[0], nums[1] });
            }

            // get ID values
            while(getline(input_file, input_line))
            { 
                auto nums = std::stoull(input_line);
                input_data.id_vec.push_back(nums);
            }

            input_file.close();   
        }
        
        return input_data;
    }

}