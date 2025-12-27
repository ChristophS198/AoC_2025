#include <string>

#include "../utility.h"

namespace Day4
{

    using Index = Point<int>;
    
    constexpr int MAX_ADJ_ROLLS{ 3 }; // at most 3 rolls are allowed in neighborhood
    constexpr char PAPERROLL{ '@' };
    constexpr char FREE{ '.' };

    int get_num_accessable_pos(const std::vector<std::vector<char>>& diagram);
    bool is_accessable_pos(const std::vector<std::vector<char>>& diagram, Index p);
    int remove_accessable_rolls(std::vector<std::vector<char>>& diagram);
    
    int sol_4_1(const std::string &file_path)
    {
        std::vector<std::vector<char>> diagram = read_2d_vec_from_file<char>(file_path);


        return get_num_accessable_pos(diagram);
    }


    int sol_4_2(const std::string &file_path)
    {
        std::vector<std::vector<char>> diagram = read_2d_vec_from_file<char>(file_path);

        int removed_rolls{ 0 };

        while (true)
        {
            auto newly_removed_rolls = remove_accessable_rolls(diagram);
            if (newly_removed_rolls == 0) break;
            removed_rolls += newly_removed_rolls;
        }

        return removed_rolls;
    }

    bool is_accessable_pos(const std::vector<std::vector<char>>& diagram, Index p)
    {
        static const std::vector<Index> dirs{ {1,0},{0,1},{-1,0},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} };
        
        int num_neighboring_rolls{ 0 };

        for (auto dir : dirs)
        {
            auto neigh = p + dir;
            if (neigh.x >= 0 && neigh.x < diagram.size() && neigh.y >= 0 && neigh.y < diagram[0].size() 
                && diagram[neigh.x][neigh.y] == PAPERROLL)
            {
                ++num_neighboring_rolls;
            }
        }

        return num_neighboring_rolls <= MAX_ADJ_ROLLS;
    }

    int get_num_accessable_pos(const std::vector<std::vector<char>>& diagram)
    {
        int num_accessable_pos{ 0 };

        for (int x=0; x<diagram.size(); ++x) 
        {
            for (int y=0; y<diagram[x].size(); ++y)
            {
                if (diagram[x][y] == PAPERROLL && is_accessable_pos(diagram, {x,y})) 
                {
                    ++num_accessable_pos;
                }
            }
        }

        return num_accessable_pos;
    }

    int remove_accessable_rolls(std::vector<std::vector<char>>& diagram)
    {
        std::vector<Index> accessable_indices{};

        for (int x=0; x<diagram.size(); ++x) 
        {
            for (int y=0; y<diagram[x].size(); ++y)
            {
                if (diagram[x][y] == PAPERROLL && is_accessable_pos(diagram, {x,y})) 
                {
                    accessable_indices.push_back({ x,y });
                }
            }
        }

        for (auto [x,y] : accessable_indices) 
        {
            diagram[x][y] = FREE;
        }

        return accessable_indices.size();
    }

}
