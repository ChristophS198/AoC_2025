#include <string>
#include <algorithm>
#include<map>

#include "../utility.h"


constexpr int DIAL_SIZE{100};


std::vector<int> read_rotations_from_file(const std::string& file_path);
int get_num_x_dial_positions(const std::vector<int>& rotations, int start_pos, int x_dial_position);

int sol_1_1(const std::string &file_path)
{
    auto rotations = read_rotations_from_file(file_path);
    auto num_0_dial_positions = get_num_x_dial_positions(rotations, 50, 0);
    

    return num_0_dial_positions;
}


int sol_1_2(const std::string &file_path)
{


    return 1;
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
            if (input_line[0] == 'L') {
                num = -num;
            }
            std::cout << num << std::endl;
            number_vec.push_back(num);
        }

        input_file.close();   
    }
    return number_vec;
}

