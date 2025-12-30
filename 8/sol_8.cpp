#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../utility.h"

namespace Day8
{

    using TNum = int64_t;
    using TPos = Point3D<TNum>;
    using TCircleId = int;
    using CircleMap = std::unordered_map<TCircleId, std::vector<TPos>>;
    using JunctionMap = std::unordered_map<TPos, TCircleId, TPos::HashFunction>;


    /*
    Connections struct for bundling up information about a connection: distance between nodes and node positions
    */
    struct Connection
    {
        TNum dist_val;
        std::pair<TPos,TPos> nodes;

        bool operator<(const Connection &r)
        {
            if (dist_val == r.dist_val) 
            {
                if (nodes.first == r.nodes.first) return nodes.second < r.nodes.second;
                else return nodes.first < r.nodes.first;
            }
            else return dist_val < r.dist_val;
        }
    };

    JunctionMap get_data(const std::string& file_path);
    std::vector<Connection> get_sorted_connections(const JunctionMap& j_map);
    void connect_junction_boxes(JunctionMap& junction_map, CircleMap& circle_map, TPos n1, TPos n2);

    size_t sol_8_1(const std::string &file_path)
    {
        auto junction_map = get_data(file_path);
        auto shortest_connections = get_sorted_connections(junction_map);

        CircleMap circle_map;
        for (const auto& j : junction_map) 
        {
            auto p = std::make_pair(j.second, std::vector<TPos>{j.first});
            circle_map.emplace(p);
        }

        for (size_t i=0; i<1000; ++i) 
        {
            const auto& connection = shortest_connections[i];
            auto [n1, n2] = connection.nodes;

            connect_junction_boxes(junction_map, circle_map, n1, n2);
        }

        // certainly not the best solution, but copy unordered_map to vector and sort it based on circle sizes
        std::vector<std::pair<TCircleId,std::vector<TPos>>> elems(circle_map.begin(), circle_map.end());
        std::sort(elems.begin(), elems.end(), [](std::pair<TCircleId,std::vector<TPos>> a, std::pair<TCircleId,std::vector<TPos>> b)
                                {
                                    return a.second.size() > b.second.size();
                                });

        return elems[0].second.size() * elems[1].second.size() * elems[2].second.size();
    }


    TNum sol_8_2(const std::string &file_path)
    {
        auto junction_map = get_data(file_path);
        auto shortest_connections = get_sorted_connections(junction_map);

        CircleMap circle_map;
        for (const auto& j : junction_map) 
        {
            auto p = std::make_pair(j.second, std::vector<TPos>{j.first});
            circle_map.emplace(p);
        }

        size_t i=0;

        while(circle_map.size() > 1) 
        {
            const auto& connection = shortest_connections[i];
            auto [n1, n2] = connection.nodes;
            auto circle_n1 = junction_map[n1];
            auto circle_n2 = junction_map[n2];

            // check that junctions are in different circles
            if (circle_n1 != circle_n2) 
            {
                // move elements of n2's circle to circle of n1
                for (auto elem : circle_map[circle_n2])
                {
                    circle_map[circle_n1].push_back(elem);
                    // set new circle ID for moved junction box
                    junction_map[elem] = circle_n1;
                }

                // remove circle of n2 from circle_map
                circle_map.erase(circle_n2);
            }
            ++i;
        }

        auto [n1, n2] = shortest_connections[i-1].nodes;        

        return n1.x * n2.x;
    }


    /*
    Connect the two junction boxes and also merge their circles + update their circle IDs in junction map
    */
    void connect_junction_boxes(JunctionMap& junction_map, CircleMap& circle_map, TPos n1, TPos n2)
    {
        auto circle_n1 = junction_map[n1];
        auto circle_n2 = junction_map[n2];

        // check that junctions are in different circles
        if (circle_n1 != circle_n2) 
        {
            // move elements of n2's circle to circle of n1
            for (auto elem : circle_map[circle_n2])
            {
                circle_map[circle_n1].push_back(elem);
                // set new circle ID for moved junction box
                junction_map[elem] = circle_n1;
            }

            // remove circle of n2 from circle_map
            circle_map.erase(circle_n2);
        }
    }

    /*
    returns a sorted vector of connections, shortest connections come first in vector
    */
    std::vector<Connection> get_sorted_connections(const JunctionMap& j_map)
    {
        int num_junctions = j_map.size();
        std::vector<Connection> dist_vec;

        for (const auto& n1 : j_map)
        {
            for (const auto& n2 : j_map)
            {
                if (n1.first == n2.first) 
                {
                    // stop on diagonal entries, i.e., when junction maps are identical so distances are only inserted once
                    break;
                }

                auto distance = (n2.first-n1.first).dist_squ();
                dist_vec.push_back(Connection{ distance, std::make_pair(n1.first,n2.first) });
            }
        }

        std::sort(dist_vec.begin(), dist_vec.end());

        return dist_vec;
    }

    JunctionMap get_data(const std::string& file_path)
    {
        JunctionMap junction_map{};
        std::fstream input_file;
        input_file.open(file_path,std::ios::in);

        
        if (input_file.is_open())
        {
            std::string input_line;
            TCircleId circle_id{ 0 };
            while(getline(input_file, input_line))
            {
                auto numbers = parse_string_to_number_vec<TNum>(input_line);
                auto position = TPos{ numbers[0],numbers[1],numbers[2] };

                junction_map.emplace(std::make_pair(position, circle_id));
                ++circle_id;
            }

            input_file.close();
        }

        return junction_map;
    }
}