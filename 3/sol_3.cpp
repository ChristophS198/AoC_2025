#include <string>
#include <map>

#include "../utility.h"


namespace Day3
{
    using Joltage = int;

    Joltage get_max_joltage_in_bank(const std::vector<Joltage> &joltage_ratings, int to_be_activated);
    
    Joltage sol_3_1(const std::string &file_path)
    {
        auto joltage_ratings = read_2d_vec_from_file<Joltage>(file_path);
        Joltage sum_joltage_val{ 0 };

        for (const auto& bank : joltage_ratings)
        {
            sum_joltage_val += get_max_joltage_in_bank(bank, 2);
        }
    
        return sum_joltage_val;
    }
    
    
    Joltage sol_3_2(const std::string &file_path)
    {
        auto joltage_ratings = read_2d_vec_from_file<Joltage>(file_path);
    
        return 1;
    }
    
    
    Joltage get_max_joltage_in_bank(const std::vector<Joltage> &joltage_ratings, int to_be_activated=2)
    {
        Joltage max_joltage_val{ 0 };
        std::map<size_t, Joltage> active_batteries;

        auto joltage_bank{ joltage_ratings };
        size_t start_idx{ 0 }; // during first search start at index 0
        
        while (to_be_activated > 0)
        {
            std::pair<size_t, Joltage> max_joltage{ 0,0 };

            for (size_t i=start_idx; i<joltage_bank.size(); ++i)
            {

                // check that battery value is larger than current max value and battery is not already active
                if (joltage_bank[i] > max_joltage.second && active_batteries.find(i) == active_batteries.end()) 
                {
                    max_joltage = { i, joltage_bank[i]};
                }
            }

            active_batteries.emplace(max_joltage);
            
            // if after the current max value index at least to_be_activated batteries remain, we increase the 
            // start_idx so future searches only include batteries after our current max value index
            if (joltage_bank.size() - max_joltage.first > to_be_activated)
            {
                start_idx = max_joltage.first;
            }

            --to_be_activated;
        }

        // map sorts entries based on first element (index in battery bank here) --> sum up values with increasing index
        for (const auto val : joltage_ratings)
        {
            std::cout << val;
        }
        std::cout << std::endl;

        for (const auto& act_bat : active_batteries)
        {
            max_joltage_val = max_joltage_val*10 + act_bat.second;
            std::cout << act_bat.first << "; " << act_bat.second << std::endl;
        }

        std::cout << "--> " << max_joltage_val << "\n" << std::endl;

        return max_joltage_val;
    }

}
