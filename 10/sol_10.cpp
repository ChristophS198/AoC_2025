#include <string>
#include <limits>
#include <queue>

#include "../utility.h"

namespace Day10
{
    
    using TNum = int;

    constexpr char OFF{ '.' };
    constexpr char ON{ '#' };
    
    struct MachineState
    {
        std::string indicator_lights = "";
        std::string cur_indicator_lights = "";
        std::vector<std::vector<int>> button_wiring{ };
        std::vector<int> button_presses{ };
        std::vector<int> joltage_req{ };
    };
    

    std::vector<MachineState> get_endings(const std::string& file_path);
    MachineState parse_str_to_machine(const std::string& machine_description);
    TNum rec_min_presses(const MachineState& mach);
    TNum min_presses_bfs(const MachineState& mach);
    TNum rec_press_button(const std::string& target_state, const std::string& cur_state, const std::vector<std::vector<int>>& wirings, const std::vector<int>& button_presses);

    TNum sol_10_1(const std::string &file_path)
    {
        auto machine_vec = get_endings(file_path);
        TNum min_sum{ 0 };

        for (const auto& m : machine_vec)
        {
            // auto min_press_num = rec_min_presses(m);
            auto min_press_num = min_presses_bfs(m);
            min_sum += min_press_num;
            std::cout << "Num presses: " << min_press_num << " ----> Sum: " << min_sum << std::endl;
        }
        
        return min_sum;
    }


    int sol_10_2(const std::string &file_path)
    {

        
        return 0;
    }

    TNum min_presses_bfs(const MachineState& mach)
    {
        auto target_state = mach.indicator_lights;
        std::string cur_state(target_state.size(), OFF);
        auto wirings = mach.button_wiring;
        std::vector<int> button_presses(wirings.size(), 0); // counts for each wiring how often it was pressed

        // use queue for FIFO (breadth first search) so we can return after target state in indicator lights is achieved
        std::queue<MachineState> state_queue;
        state_queue.push(mach);

        while (!state_queue.empty())
        {
            // retrieve next state
            auto nxt_mach = state_queue.front();
            state_queue.pop();

            // check for target state
            if (nxt_mach.indicator_lights == nxt_mach.cur_indicator_lights)
            {
                TNum num_presses{ 0 };
                for (auto num : nxt_mach.button_presses) num_presses += num;
                return num_presses;
            }

            // press each non-active button once
            for (size_t i=0; i<nxt_mach.button_presses.size(); ++i)
            {
                if (nxt_mach.button_presses[i] == 0) 
                {
                    auto mach_copy(nxt_mach);

                    ++mach_copy.button_presses[i];

                    // switch indicator light status
                    for (auto idx : mach_copy.button_wiring[i]) 
                    {
                        if (mach_copy.cur_indicator_lights[idx] == OFF) mach_copy.cur_indicator_lights[idx] = ON;
                        else mach_copy.cur_indicator_lights[idx] = OFF;
                    }
                    
                    state_queue.push(mach_copy);
                }
            }

        }

        return 0;
    }


    TNum rec_press_button(const std::string& target_state, const std::string& cur_state, const std::vector<std::vector<int>>& wirings, const std::vector<int>& button_presses)
    {
        // check if target state is already active
        if (target_state == cur_state) 
        {
            TNum num_presses{ 0 };
            for (auto num : button_presses) num_presses += num;
            return num_presses;
        }

        // press each non-active button once
        TNum min_presses{ std::numeric_limits<TNum>::max() };
        for (size_t i=0; i<button_presses.size(); ++i)
        {
            if (button_presses[i] == 0) 
            {
                std::vector<int> new_button_presses(button_presses);
                ++new_button_presses[i];
                std::string new_light_state(cur_state);

                // switch indicator light status
                for (auto idx : wirings[i]) 
                {
                    if (new_light_state[idx] == OFF) new_light_state[idx] = ON;
                    else new_light_state[idx] = OFF;
                }
                auto tmp_res = rec_press_button(target_state, new_light_state, wirings, new_button_presses);
                if (tmp_res < min_presses) min_presses = tmp_res;
            }
        }

        return min_presses;
    }

    TNum rec_min_presses(const MachineState& mach)
    {
        auto target_state = mach.indicator_lights;
        std::string cur_state(target_state.size(), OFF);
        auto wirings = mach.button_wiring;
        std::vector<int> button_presses(wirings.size(), 0); // counts for each wiring how often it was pressed

        auto min_presses = rec_press_button(target_state, cur_state, wirings, button_presses);

        return min_presses;
    }

    std::vector<MachineState> get_endings(const std::string& file_path)
    {
        std::vector<MachineState> machine_vec{};
        std::fstream input_file;

        input_file.open(file_path,std::ios::in);

        if (input_file.is_open())
        {
            std::string input_line;

            while(getline(input_file, input_line))
            {
                auto mach = parse_str_to_machine(input_line);
                machine_vec.push_back(mach);
            }

            input_file.close();
        }
        return machine_vec;
    }

    MachineState parse_str_to_machine(const std::string& machine_description)
    {
        MachineState mach;
        size_t i = 1;

        // retrieve indicator light diagram
        while (machine_description.at(i) != ']')
        {
            mach.indicator_lights.push_back(machine_description.at(i));
            ++i;
        }

        // retrieve wiring schemantics
        ++i;
        std::vector<int> new_wiring{ };
        while (machine_description.at(i) != '{')
        {
            char c = machine_description.at(i);
            switch (c)
            {
                case ')':
                    // store schemantics
                    mach.button_wiring.push_back(new_wiring);

                    // start new schemantics
                    new_wiring.clear();
                    ++i;
                    break;
                case ',':
                    ++i;
                    break;
                case '(':
                    ++i;
                    break;
                case ' ':
                    ++i;
                    break;
                    
                default:
                    new_wiring.push_back(c - '0'); // indices only range from 0 to 9
                    ++i;
                    break;
            }

        }

        // retrieve joltage requirements
        auto jolt_str = machine_description.substr(i);
        mach.joltage_req = parse_string_to_number_vec<int>(jolt_str);

        mach.cur_indicator_lights = std::string(mach.indicator_lights.size(), OFF);
        mach.button_presses = std::vector<int>(mach.button_wiring.size(), 0);

        return mach;
    }


} // namespace Day10
