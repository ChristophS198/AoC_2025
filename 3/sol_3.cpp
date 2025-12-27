#include <string>
#include <map>

#include "../utility.h"


namespace Day3
{
    using Joltage = int;
    using SumJoltage = uint64_t;

    SumJoltage get_max_joltage_in_bank(const std::vector<Joltage> &joltage_ratings, int to_be_activated);
    
    SumJoltage sol_3_1(const std::string &file_path)
    {
        auto joltage_ratings = read_2d_vec_from_file<Joltage>(file_path);
        SumJoltage sum_joltage_val{ 0 };

        for (const auto& bank : joltage_ratings)
        {
            sum_joltage_val += get_max_joltage_in_bank(bank, 2);
        }
    
        return sum_joltage_val;
    }
    
    
    SumJoltage sol_3_2(const std::string &file_path)
    {
        auto joltage_ratings = read_2d_vec_from_file<Joltage>(file_path);
        SumJoltage sum_joltage_val{ 0 };

        for (const auto& bank : joltage_ratings)
        {
            sum_joltage_val += get_max_joltage_in_bank(bank, 12);
        }
    
        return sum_joltage_val;
    }
    
    int num_active_batteries_after_idx(const auto& joltage_bank, size_t idx)
    {
        // count number of active batteries after newly activated battery
        int act_bat_count_in_remainder{ 0 };
        for (size_t j=idx; j<joltage_bank.size(); ++j)
        {
            if (joltage_bank[j] != -1) ++act_bat_count_in_remainder;
        }
        return act_bat_count_in_remainder;
    }
    
    /*
    Check for each already active battery position if at least to_be_activated batteries remain to its right. 
    By this we guarantee that the current sum joltage stays maximal, for new active battery values cannot get 
    larger than already active batteries --> we should add these new smaller digits to the right of the current
    sum joltage.
    Example: 4 batteries are still to be activated
        a a a     a means this battery is already active
        87859111

        here calc_start_idx returns 3, so the next battery to be activated has value 5 (index 3) resulting
        in 8859, which is larger than 8789 if we would start to search from 0. 
    */
    size_t calc_start_idx(const auto& joltage_bank, std::map<size_t, Joltage>& active_batteries, int to_be_activated)
    {
        size_t start_idx{ 0 };

        for (const auto& [idx, jolt] : active_batteries)
        {
            auto act_batteries_in_remainder = num_active_batteries_after_idx(joltage_bank, idx);
            if (act_batteries_in_remainder >= to_be_activated)
            {
                start_idx = idx + 1;
            }
            else 
            {
                // stop so we return the rightmost index, where at least to_be_activated batteries exist in the remainder
                break;
            }
        }

        return start_idx;
    }

    /*
    Idea is to iteratively activate an inactive battery with the largest value. In case of equality the first found
    battery is activated (so the search space for following activations is as large as possible). 
    For the next search the search space is adapted, because the largest battery values are already active. 
    Therefore, we try to activate only batteries coming after the currently active batteries so the resulting jolt value is 
    maximal (and new jolt digits are only added to the right of already active batteries). 
    The next start index is therefore chosen as the rightmost battery coming immediately after an already active battery, 
    with the condition that at least the number of to be activated batteries remains after this index.   
    */
    SumJoltage get_max_joltage_in_bank(const std::vector<Joltage> &joltage_ratings, int to_be_activated=2)
    {
        SumJoltage max_joltage_val{ 0 };
        std::map<size_t, Joltage> active_batteries; // map sorts automatically based on first entry (index)

        auto joltage_bank{ joltage_ratings }; // copy of original bank so we can set active batteries to -1 and no map lookup is required
        size_t start_idx{ 0 }; // during first search start at index 0
        
        // iteratively active batteries
        while (to_be_activated > 0)
        {
            std::pair<size_t, Joltage> max_joltage{ 0,0 };

            for (size_t i=start_idx; i<joltage_bank.size(); ++i)
            {
                // check that battery value is larger than current max value and battery is not already active
                if (joltage_bank[i] > max_joltage.second) 
                {
                    max_joltage = { i, joltage_bank[i]};
                }
            }

            active_batteries.emplace(max_joltage);
            
            // set battery value of newly activated battery to -1, to mark as "active" and reduce number of batteries to be activated
            joltage_bank[max_joltage.first] = -1;
            --to_be_activated;

            // new start_idx is either 0 or the smallest index after an already active battery. But this index must 
            // contain at least to_be_activated active batteries to his right
            start_idx = calc_start_idx(joltage_bank, active_batteries, to_be_activated);

        }

        // map sorts entries based on first element (index in battery bank here) --> concat values with increasing index
        for (const auto& act_bat : active_batteries)
        {
            max_joltage_val = max_joltage_val*10 + act_bat.second;
        }

        return max_joltage_val;
    }

}
