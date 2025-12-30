#include <string>
#include <unordered_set>
#include <stack>

#include "../utility.h"

namespace Day7
{
    using TCoord = Point<int>;
    using TNum = int64_t; 

    constexpr char START_POS{ 'S' };
    constexpr char SPLITTER{ '^' };
    constexpr TCoord BEAM_MOVE{ 1,0 }; // a beam always moves south

    struct InputData
    {
        TCoord start{ }; // starting position of beam
        std::unordered_set<TCoord, TCoord::HashFunction> splitters{ }; // collects the coordinates of splitters
        int n_rows{ }; // number of rows in diagram
        int n_cols{ }; // number of cols in diagram

    };

    InputData get_data(const std::string& file_path);
    int count_splitter_hits(const InputData& data_in);
    TNum count_beam_paths(TCoord beam_pos, std::vector<std::vector<TNum>>& memo, const std::unordered_set<TCoord, TCoord::HashFunction>& splitters);

    int sol_7_1(const std::string &file_path)
    {
        auto data_in = get_data(file_path);

        return count_splitter_hits(data_in);
    }

    /*
    Use dynamic programming with memoization to track for each diagram position the number of possible paths/timelines
    */
    TNum sol_7_2(const std::string &file_path)
    {
        auto data_in = get_data(file_path);
        std::vector<std::vector<TNum>> memo = std::vector(data_in.n_rows, std::vector<TNum>(data_in.n_rows, -1));

        return count_beam_paths(data_in.start, memo, data_in.splitters);
    }

    TNum count_beam_paths(TCoord beam_pos, std::vector<std::vector<TNum>>& memo, const std::unordered_set<TCoord, TCoord::HashFunction>& splitters)
    {
        // size of memo corresponds to size of diagram
        int n_rows = memo.size();
        int n_cols = memo[0].size();

        // check left/right boundaries
        if (beam_pos.y < 0 || beam_pos.y >= n_cols) return 0;

        // check if paths for this position are already known
        if (memo[beam_pos.x][beam_pos.y] != -1)
        {
            return memo[beam_pos.x][beam_pos.y];
        }

        // check bottom boundary
        if (beam_pos.x == n_rows-1) 
        {
            // set value in memo and return
            return memo[beam_pos.x][beam_pos.y] = 1;
        }

        // check if current pos is a splitter
        if (splitters.contains(beam_pos))
        {
            // sum up paths from left and right beams to calc possible paths for parent beam
            auto left_paths = count_beam_paths(TCoord{ beam_pos.x,beam_pos.y-1 }, memo, splitters);
            auto right_paths = count_beam_paths(TCoord{ beam_pos.x,beam_pos.y+1 }, memo, splitters);

            return memo[beam_pos.x][beam_pos.y] = left_paths + right_paths;
        }
        else
        {
            // move beam soutward
            auto paths = count_beam_paths(beam_pos + BEAM_MOVE, memo, splitters);
            
            return memo[beam_pos.x][beam_pos.y] = paths;
        }

    }

    int count_splitter_hits(const InputData& data_in)
    {
        std::stack<TCoord> beam_pos{ }; // track the position of all front beam positions
        const std::unordered_set<TCoord, TCoord::HashFunction>& splitters = data_in.splitters;
        int n_rows = data_in.n_rows;
        int n_cols = data_in.n_cols;

        std::unordered_set<TCoord, TCoord::HashFunction> hit_splitters; // contains the splitters that have been hit by a beam

        // push start position to stack
        beam_pos.push(data_in.start);

        // extract beams from stack until all beams reach boundaries
        while (!beam_pos.empty()) 
        {
            // extract a beam and iterate its movement until boundary or slitter is hit
            auto cur_beam = beam_pos.top();
            beam_pos.pop();

            // check boundaries (normally only left/right boundaries matter here)
            if (cur_beam.y < 0 || cur_beam.y >= n_cols || cur_beam.x >= n_rows) continue;

            // iterate beam until splitter or boundary is reached
            while (cur_beam.x < n_rows) 
            {
                cur_beam = cur_beam + BEAM_MOVE;

                // check if new position is a splitter 
                if (splitters.contains(cur_beam)) 
                {
                    // if splitter has already been hit --> stop further tracking of created beams
                    if (hit_splitters.contains(cur_beam))
                    {
                        // end beam tracking
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