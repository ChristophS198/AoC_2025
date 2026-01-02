#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <ostream>

#include "../utility.h"


namespace Day9
{
    using TNum = int64_t;
    using TPos = Point<TNum>;
    
    // bundles data for a rectangle (area and corner tiles)
    struct RectTile {
        TNum area; 
        TPos corner_1;
        TPos corner_2;
    };

    std::vector<TPos> get_data(const std::string& file_path);
    std::vector<RectTile> get_sorted_rectangles(const std::vector<TPos>& red_tiles);
    bool is_tile_in_polygon(TPos tile, const std::vector<TPos>& red_tiles);
    bool is_rectangle_in_polygon(TPos corner_1, TPos corner_2, const std::vector<TPos>& red_tiles);
    std::pair<std::vector<TPos>, std::unordered_map<TPos,TPos,TPos::HashFunction>> reduce_map(const std::vector<TPos>& red_tiles);
    std::ostream& print_map(std::ostream& out, const std::vector<TPos>& red_tiles);


    TNum sol_9_1(const std::string &file_path)
    {
        auto red_tiles = get_data(file_path);
        auto rect_tiles = get_sorted_rectangles(red_tiles);

        return rect_tiles[0].area;
    }


    TNum sol_9_2(const std::string &file_path)
    {
        auto red_tiles = get_data(file_path);
        auto rect_tiles = get_sorted_rectangles(red_tiles);
        bool is_in_polygon = false;

        auto [reduced_tile_vec, tile_map] = reduce_map(red_tiles);
        // std::ofstream outfile("out.txt");
        // print_map(outfile, reduced_tile_vec);
        // outfile.close();

        for (auto rect : rect_tiles)
        {
            auto is_in_polygon = is_rectangle_in_polygon(tile_map[rect.corner_1], tile_map[rect.corner_2], reduced_tile_vec);
            if (is_in_polygon) 
            {
                return rect.area;
            }
        }


        return 0;
    }

    // Idea is to remove all rows and columns that have not red tile in it for they provide no additional information 
    // and are only required for the area calculation
    std::pair<std::vector<TPos>, std::unordered_map<TPos,TPos,TPos::HashFunction>> reduce_map(const std::vector<TPos>& red_tiles)
    {
        std::unordered_map<TPos,TPos,TPos::HashFunction> tile_map; // maps the reduced tile positions to their original positions
        std::vector<TPos> sorted_copy{ red_tiles };
       
        for (auto pos : red_tiles) 
        {
            tile_map.emplace(std::make_pair(pos, pos));
        }
        
        // sort based on x-coord and remove all intermediate points where only green tiles are, for they hold no information
        std::sort(sorted_copy.begin(), sorted_copy.end(), [](const TPos& t1, const TPos& t2)
        {
            return t1.x <= t2.x;
        });

        TNum last_x{ 0 };
        for (size_t i=0; i<sorted_copy.size(); ++i)
        {
            auto& r_1 = sorted_copy[i];

            auto dx = r_1.x - last_x - 1;
            if (dx > 0) 
            {
                // reduce for each tile having a larger x-coord the x-value in map
                for (size_t k=i; k<sorted_copy.size(); ++k)
                {
                    tile_map[sorted_copy[k]].x -= dx;
                }
            }

            last_x = r_1.x;
        }

        // reduce y-dimension
        std::sort(sorted_copy.begin(), sorted_copy.end(), [](const TPos& t1, const TPos& t2)
        {
            return t1.y <= t2.y;
        });

        TNum last_y{ 0 };
        for (size_t i=0; i<sorted_copy.size(); ++i)
        {
            auto& r_1 = sorted_copy[i];

            auto dy = r_1.y - last_y - 1;
            if (dy > 0) 
            {
                for (size_t k=i; k<sorted_copy.size(); ++k)
                {
                    tile_map[sorted_copy[k]].y -= dy;
                }
            }

            last_y = r_1.y;
        }

        // build vector with same order as red_tiles, but with reduced coordinates
        std::vector<TPos> reduced_tile_vec;
        for (auto pos : red_tiles) 
        {
            reduced_tile_vec.push_back(tile_map[pos]);
        }

        return { reduced_tile_vec, tile_map };
    }

    /*
    Checks for each border of a rectangle whether all borderpoints are inside (or on) the polygon shape
    */
    bool is_rectangle_in_polygon(TPos corner_1, TPos corner_2, const std::vector<TPos>& reduced_map)
    {

        // check for both x-borders whether borderpoints are inside or on polygon shape
        int dx = corner_2.x > corner_1.x ? 1 : -1;
        for (auto x=corner_1.x; x!=corner_2.x; x += dx)
        {
            auto border_tile   = TPos{x,corner_1.y};
            auto border_tile_2 = TPos{x,corner_2.y};
            if (!is_tile_in_polygon(border_tile, reduced_map) or !is_tile_in_polygon(border_tile_2, reduced_map))
            {
                return false;
            }
        }

        // check for both y-borders whether borderpoints are inside or on polygon shape
        int dy = corner_2.y > corner_1.y ? 1 : -1;
        for (auto y=corner_1.y; y!=corner_2.y; y += dy)
        {
            auto border_tile   = TPos{corner_1.x, y};
            auto border_tile_2 = TPos{corner_2.x, y};
            if (!is_tile_in_polygon(border_tile, reduced_map) or !is_tile_in_polygon(border_tile_2, reduced_map))
            {
                return false;
            }
        }

        return true;
    }

    /*
    Idea is to count for a tile position how many vertical polygon sections are to the left of this point
    If the number is even, the tile is outside of the polygon, if it is odd the tile is inside
    */
    bool is_tile_in_polygon(TPos tile, const std::vector<TPos>& red_tiles)
    {
        bool is_inside = false;

        for (size_t i=0, j=red_tiles.size()-1; i<red_tiles.size(); ++i)
        {
            auto r_1 = red_tiles[j];
            auto r_2 = red_tiles[i];

            // skip horizontal borders
            if (r_1.x == r_2.x) 
            {
                j = i;
                continue;
            }

            if (tile.x > r_1.x && tile.x <= r_2.x || tile.x <= r_1.x && tile.x > r_2.x)
            {
                // this vertical polygon section is interesting for us
                // if tile lies to the right of current vertical polygon border --> flip inside status
                if (tile.y > r_1.y) 
                {
                    is_inside = !is_inside;
                }

                // special treatment of tiles that lie on a polygon border --> borders are always inside
                if (tile.y == r_1.y || tile.y == r_2.y) 
                {
                    return true;
                }
            }

            // adapt j
            j = i;
        }

        return is_inside;
    }

    std::vector<RectTile> get_sorted_rectangles(const std::vector<TPos>& red_tiles)
    {
        std::vector<RectTile> rect_vec{ };

        for (size_t i=0; i<red_tiles.size(); ++i)
        {
            for (size_t j=i+1; j<red_tiles.size(); ++j)
            {
                auto width  = std::abs(red_tiles[j].x - red_tiles[i].x) + 1;
                auto height = std::abs(red_tiles[j].y - red_tiles[i].y) + 1;
                auto area = width * height;
                rect_vec.push_back( RectTile{ area,red_tiles[i], red_tiles[j] });
            }
        }

        std::sort(rect_vec.begin(), rect_vec.end(), [](RectTile r_1, RectTile r_2)
                                {
                                    return r_1.area > r_2.area;
                                });

        return rect_vec;
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

    std::ostream& print_map(std::ostream& out, const std::vector<TPos>& red_tiles)
    {
        size_t max_x{ 0 };
        size_t max_y{ 0 };
        std::unordered_set<TPos, TPos::HashFunction> red_tile_set;

        for (const auto& pos : red_tiles)
        {
            if (pos.x > max_x) max_x = pos.x;
            if (pos.y > max_y) max_y = pos.y;
            red_tile_set.emplace(pos);
        }

        std::vector<std::vector<char>> map(max_x+1, std::vector<char>(max_y+1, ' '));

        for (size_t i=0, j=red_tiles.size()-1; i<red_tiles.size(); ++i)
        {
            auto r_1 = red_tiles[j];
            auto r_2 = red_tiles[i];

            int dx = r_2.x > r_1.x ? 1 : -1;
            for (auto x=r_1.x; x!=r_2.x; x += dx)
            {
                map[x][r_1.y] = 'x';
            }

            int dy = r_2.y > r_1.y ? 1 : -1;
            for (auto y=r_1.y; y!=r_2.y; y += dy)
            {
                map[r_1.x][y] = 'x';
            }

            j = i;
        }

        map[30][154] = 'Q';
        map[219][124] = 'Q';

        for (size_t x=0; x<max_x; ++x) 
        {
            for (size_t y=0; y<max_y; ++y)
            {
                out << map[x][y];
            }
            out << "\n";
        }

        return out;
    }


} // namespace Day9
