#include <string>
#include <vector>
#include <unordered_map>

#include "../utility.h"


namespace Day9
{
    using TNum = int64_t;
    using TPos = Point<TNum>;
    
    struct DiskElem {
        int id; // -1 indicates a free space
        int len;
    };

    std::vector<TPos> get_data(const std::string& file_path);
    TNum get_max_rect_area(const std::vector<TPos>& red_tiles);

    TNum sol_9_1(const std::string &file_path)
    {
        auto red_tiles = get_data(file_path);
        auto max_area = get_max_rect_area(red_tiles);

        return max_area;
    }


    TNum sol_9_2(const std::string &file_path)
    {
        auto red_tiles = get_data(file_path);

        return 1;
    }

    TNum get_max_rect_area(const std::vector<TPos>& red_tiles)
    {
        TNum max_area{ 0 };

        for (size_t i=0; i<red_tiles.size(); ++i)
        {
            for (size_t j=i+1; j<red_tiles.size(); ++j)
            {
                auto width  = std::abs(red_tiles[j].x - red_tiles[i].x) + 1;
                auto height = std::abs(red_tiles[j].y - red_tiles[i].y) + 1;
                auto area = width * height;
                if (area > max_area)
                {
                    max_area = area;
                }
            }
        }

        return max_area;
    }

    std::vector<TPos> get_data(const std::string& file_path)
    {
        std::vector<TPos> pos_vec;

        std::fstream input_file;
        input_file.open(file_path,std::ios::in);
        
        if (input_file.is_open())
        {
            std::string input_line;
            while(getline(input_file, input_line))
            {
                auto numbers = parse_string_to_number_vec<TNum>(input_line);

                pos_vec.push_back( TPos{ numbers[0],numbers[1] });
            }

            input_file.close();
        }

        return pos_vec;
    }

} // namespace Day9
