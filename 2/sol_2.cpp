#include <string>
#include <set>

#include "../utility.h"


namespace Day2 
{
    using TIdVal = uint64_t;

    struct Range
    {
        TIdVal lower_limit;
        TIdVal upper_limit;
    };
        
    
    std::vector<Range> get_range_list(const std::string &file_path);
    TIdVal get_invalid_id_sum_in_range_sol_1(const Range range);
    TIdVal get_smallest_invalid_id_larger_equal(TIdVal val);
    TIdVal get_next_invalid_id(TIdVal prefix_sequence);
    
    TIdVal seq_pattern_in_range(const std::string& pattern, const Range range);
    std::set<TIdVal> get_invalid_ids_for_pattern_len(int seq_len, const Range range);
    std::set<TIdVal> get_invalid_ids_in_range(const Range range);


    TIdVal sol_2_1(const std::string &file_path)
    {
        std::vector<Range> report_list = get_range_list(file_path);
        
        TIdVal invalid_id_sum{ 0 };

        for (const auto& range : report_list) {
            invalid_id_sum += get_invalid_id_sum_in_range_sol_1(range);
        }

        return invalid_id_sum;
    }


    TIdVal sol_2_2(const std::string &file_path)
    {
        std::vector<Range> report_list = get_range_list(file_path);
        
        TIdVal invalid_id_sum{ 0 };
        std::set<TIdVal> invalid_id_set{};

        for (const auto& range : report_list) {
            auto invalid_ids_tmp = get_invalid_ids_in_range(range);
            invalid_id_set.insert(invalid_ids_tmp.begin(), invalid_ids_tmp.end());
        }

        for (auto val : invalid_id_set) 
        {
            invalid_id_sum += val;
        }

        return invalid_id_sum;
    }

    std::set<TIdVal> get_invalid_ids_in_range(const Range range)
    {
        std::set<TIdVal> invalid_id_set{};
        TIdVal invalid_id_sum{ 0 };
        
        // get max number of digits of a patter sequence (must be repeated at least twice for invalid id --> divide by 2)
        auto max_sequence_len = get_digit_count(range.upper_limit) / 2;
        

        for (auto seq_len=1; seq_len<=max_sequence_len; ++seq_len)
        {
            auto invalid_id_set_new = get_invalid_ids_for_pattern_len(seq_len, range);
            invalid_id_set.insert(invalid_id_set_new.begin(), invalid_id_set_new.end());
        }


        return invalid_id_set;
    }

    std::set<TIdVal> get_invalid_ids_for_pattern_len(int seq_len, const Range range)
    {
        std::set<TIdVal> invalid_id_set{  };
        
        std::string pattern_seq = "1";
        if (seq_len > 1)
        {
            pattern_seq += std::string(seq_len-1,'0');
        }

        TIdVal pattern_num = std::stoull(pattern_seq);
        
        // iterate through pattern sequences, starting with last index position
        while (pattern_seq.length() == seq_len)
        {
            invalid_id_set.emplace(seq_pattern_in_range(pattern_seq, range));

            // generate new pattern seq by increasing number
            ++pattern_num;
            pattern_seq = std::to_string(pattern_num);
        }

        return invalid_id_set;
    }

    /*
    check that a pattern is in the range by repeating it until it is at least as large as lower limit
    */
    TIdVal seq_pattern_in_range(const std::string& pattern, const Range range)
    {
        // start candidate is the orignal pattern concatenated twice
        auto invalid_candidate_str = pattern + pattern;
        auto invalid_candidate_num = std::stoull(invalid_candidate_str);

        // append pattern until our candidate exceeds or is equal to the lower limit
        while (invalid_candidate_num < range.lower_limit)
        {
            invalid_candidate_str += pattern;
            invalid_candidate_num = std::stoull(invalid_candidate_str);
        }

        // current candidate value is larger, equal lower limit, if it also is smaller, equal upper limit -> candidate is an invalid ID
        if (invalid_candidate_num <= range.upper_limit)
        {
            return invalid_candidate_num;
        }

        return 0ull;
    }

    TIdVal get_invalid_id_sum_in_range_sol_1(const Range range)
    {
        TIdVal invalid_id_sum{ 0 };

        
        TIdVal smallest_invalid_id = get_smallest_invalid_id_larger_equal(range.lower_limit);
        

        while (smallest_invalid_id <= range.upper_limit) {
            invalid_id_sum += smallest_invalid_id;
            auto prefix_len = std::to_string(smallest_invalid_id).length() / 2;
            auto prefix_str = std::to_string(smallest_invalid_id).substr(0, prefix_len);
            smallest_invalid_id = get_next_invalid_id(std::stoull(prefix_str));
        }

        return invalid_id_sum;
    }

    TIdVal get_smallest_invalid_id_larger_equal(TIdVal val)
    {
        auto val_str = std::to_string(val);
        size_t prefix_digit_count = val_str.length() / 2; // length of the first half (prefix) of the invalid ID.

        if ((val_str.length() % 2) == 0) 
        {
            TIdVal first_half = std::stoull(val_str.substr(0,prefix_digit_count));
            TIdVal second_half = std::stoull(val_str.substr(prefix_digit_count));
            
            if (first_half < second_half) 
            {
                // increase first half by 1 and return new invalid ID by concatenating first_half twice
                ++first_half;
                return std::stoull(std::to_string(first_half) + std::to_string(first_half));
            }
            else return std::stoull(std::to_string(first_half) + std::to_string(first_half));

        }
        else
        {
            // val_str.length() is an odd number --> return next largest base 10 value (1 followed by only zeros) 
            auto prefix_sequence = "1" + std::string(prefix_digit_count,'0');
            return std::stoull(prefix_sequence + prefix_sequence);
        }

    }

    TIdVal get_next_invalid_id(TIdVal prefix_sequence)
    {
        // increase prefix sequence by 1 
        ++prefix_sequence;

        // return new invalid ID by concatenating prefix_sequence twice
        return std::stoull(std::to_string(prefix_sequence) + std::to_string(prefix_sequence));;
    }



    std::vector<Range> get_range_list(const std::string &file_path)
    {
        std::vector<Range> range_list;

        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line;
            while(getline(input_file, input_line))
            {
                std::vector<char> c_vec{};
                TIdVal lower{};
                TIdVal upper{};

                for (auto c : input_line)
                {
                    if (std::isdigit(static_cast<unsigned char>(c)))
                    {
                        c_vec.push_back(c);
                    }
                    else
                    {
                        if (c == '-')
                        {
                            lower = convert_to_num<TIdVal>(c_vec);
                        }
                        else if (c == ',')
                        {
                            upper = convert_to_num<TIdVal>(c_vec);
                            range_list.push_back(Range(lower, upper));
                        }

                        c_vec.clear();
                    }
                    
                }
                    // add last range in current line
                    if (c_vec.size() > 0u)
                    {
                        upper = convert_to_num<TIdVal>(c_vec);
                        range_list.push_back(Range(lower, upper));
                        c_vec.clear();
                    }
            }
        }
        return range_list;
    }

} 
