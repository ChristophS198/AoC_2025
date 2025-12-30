#include <string>
#include <unordered_set>
#include <stack>

#include "../utility.h"

namespace Day7
{
    using TCoord = Point<int>;

    constexpr char START_POS{ 'S' };
    constexpr char SPLITTER{ '^' };

    struct InputData
    {
        TCoord start{ }; // starting position of beam
        std::unordered_set<TCoord, TCoord::HashFunction> splitters{ }; // collects the coordinates of splitters
        int n_rows{ }; // number of rows in diagram
        int n_cols{ }; // number of cols in diagram

    };

    InputData get_data(const std::string& file_path);
    int count_splitter_hits(const InputData& data_in);

    int sol_7_1(const std::string &file_path)
    {
        auto data_in = get_data(file_path);

        

        return count_splitter_hits(data_in);
    }


    int sol_7_2(const std::string &file_path)
    {


        return 0;
    }

    int count_splitter_hits(const InputData& data_in)
    {
        // TCoord start_pos = data_in.start;
        std::stack<TCoord> beam_pos{ }; // track the position of all front beam positions
        const std::unordered_set<TCoord, TCoord::HashFunction>& splitters = data_in.splitters;
        int n_rows = data_in.n_rows;
        int n_cols = data_in.n_cols;
        TCoord move{ 1,0 }; // a beam always moves south

        std::unordered_set<TCoord, TCoord::HashFunction> hit_splitters; // contains the splitters that have been hit by a beam


        beam_pos.push(data_in.start);

        while (!beam_pos.empty()) 
        {
            // extract a beam and iterate its movement until boundary or slitter is hit
            auto cur_beam = beam_pos.top();
            beam_pos.pop();

            // check boundaries
            if (cur_beam.y < 0 || cur_beam.y >= n_cols || cur_beam.x >= n_rows) continue;

            // iterate beam until splitter or boundary is reached
            while (cur_beam.x < n_rows) 
            {
                cur_beam = cur_beam + move;

                // check if new position is a splitter 
                if (splitters.contains(cur_beam)) 
                {
                    // if splitter has already been hit --> stop further tracking of created beams
                    if (hit_splitters.contains(cur_beam))
                    {
                        break;
                    }
                    
                    // add splitter to set hit spitters
                    hit_splitters.emplace(cur_beam);

                    // create two new beams: one to the left and one to the right of splitter
                    beam_pos.push(TCoord{ cur_beam.x,cur_beam.y-1 });
                    beam_pos.push(TCoord{ cur_beam.x,cur_beam.y+1 });

                    // end this beam here
                    break;
                }
            }

        }

        return hit_splitters.size();
    }

    InputData get_data(const std::string& file_path)
    {
        InputData data_in{ };
        std::fstream input_file;

        input_file.open(file_path,std::ios::in);
        if (input_file.is_open())
        {
            std::string input_line;
            int row{ 0 };
            
            while(getline(input_file, input_line))
            {
                for (int col=0; col<input_line.length(); ++col)
                {
                    if (SPLITTER == input_line[col])
                    {
                        data_in.splitters.emplace(TCoord{ row,col });
                    }
                    else 
                    {
                        if (START_POS == input_line[col])
                        {
                            data_in.start = TCoord{ row,col };
                            data_in.n_cols = input_line.length();
                        }
                    }
                }

                ++row;
            }

            input_file.close();
            data_in.n_rows = row;
        }

        return data_in;
    }

}