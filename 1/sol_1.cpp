#include <string>
#include <algorithm>
#include<map>

#include "../utility.h"


constexpr int DIAL_SIZE{100};


std::vector<int> read_rotations_from_file(const std::string& file_path);
int get_num_x_dial_positions(const std::vector<int>& rotations, int start_pos, int x_dial_position);
int get_num_zero_passes(const std::vector<int>& rotations, int start_pos);

int sol_1_1(const std::string &file_path)
{
    auto rotations = read_rotations_from_file(file_path);
    auto num_0_dial_positions = get_num_x_dial_positions(rotations, 50, 0);
    
    return num_0_dial_positions;
}


int sol_1_2(const std::string &file_path)
{
    auto rotations = read_rotations_from_file(file_path);
    auto num_0_dial_positions = get_num_x_dial_positions(rotations, 50, 0);
    auto num_zero_passes = get_num_zero_passes(rotations, 50);
    
    return num_zero_passes;
}


/*
Returns the number of times the dial passes position 0. Left rotations are negative, right rotations are positive.
start_pos indicates the starting position of the dial (0 to DIAL_SIZE-1)
*/
int get_num_zero_passes(const std::vector<int>& rotations, int start_pos)
{
    int current_dial_pos = start_pos;
    int num_sign_changes{0};

    for (const auto& rot : rotations) {
        auto new_dial_pos = current_dial_pos + rot;

        // calculate integer quotient to account for full rotations over the dial size
        int num_passes = std::abs(new_dial_pos / DIAL_SIZE);

        // add number of sign changes if dial crosses 0 without landing on it
        if ((new_dial_pos < 0 && current_dial_pos > 0) || 
            (new_dial_pos > 0 && current_dial_pos < 0))
        {
            ++num_passes;
        }

        // add number of times dial lands exactly on 0
        if (new_dial_pos == 0) {
            ++num_passes;
        }

        num_sign_changes += std::abs(num_passes);

        // update current dial position within bounds of dial size
        current_dial_pos = (new_dial_pos + DIAL_SIZE) % DIAL_SIZE;
    }

    return num_sign_changes;
}

/*
Retrieves the number of times the dial reaches position x_dial_position. Left rotations are negative, right rotations are positive.
start_pos indicates the starting position of the dial (0 to DIAL_SIZE-1)
*/
int get_num_x_dial_positions(const std::vector<int>& rotations, int start_pos, int x_dial_position)
{
    int current_dial_pos = start_pos;
    int num_x_dial_positions{0};

    for (const auto& rot : rotations)
    {
        current_dial_pos = (current_dial_pos + rot + DIAL_SIZE) % DIAL_SIZE;
        if (current_dial_pos == x_dial_position)
        {
            ++num_x_dial_positions;
        }
    }
    return num_x_dial_positions;
}


std::vector<int> read_rotations_from_file(const std::string& file_path)
{
    std::vector<int> number_vec{};

    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open())
    {
        std::string input_line;
        while(getline(input_file, input_line))
        {
            int num{0};
            for (size_t i=1; i<input_line.length(); ++i) {
                num = num*10 + (input_line[i] - '0');
            }

            // left rotations evaluate to negative numbers
            if (input_line[0] == 'L') {
                num = -num;
            }

            number_vec.push_back(num);
        }

        input_file.close();   
    }
    return number_vec;
}

