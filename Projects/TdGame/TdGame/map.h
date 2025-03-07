#ifndef _MAP_H_
#define _MAP_H_

#include "tile.h"
#include "route.h"

#include <SDL.h>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

class Map
{
public:
	typedef std::unordered_map<int, Route> SpawnerRoutePool;
public:
	Map() = default;
	~Map() = default;

	// 加载地图数据
	bool load(const std::string& path)
	{
		std::ifstream file(path);
		if (!file.good())	return false;

		TileMap tile_map_temp;

		int idx_x = -1, idx_y = -1;

		std::string str_line;
		while (std::getline(file, str_line))
		{
			str_line = trim_str(str_line);
			//如果读取的行是空行，则跳过当前行
			if (str_line.empty())
				continue;

			idx_x = -1, idx_y ++;
			tile_map_temp.emplace_back();
			
			std::string str_tile;
			std::stringstream str_stream(str_line);
			while (std::getline(str_stream, str_tile, ','))
			{
				idx_x++;
				tile_map_temp[idx_y].emplace_back();
				Tile& tile = tile_map_temp[idx_y].back();
				load_tile_from_string(tile, str_tile);
			}
		}
		file.close();

		if (tile_map_temp.empty() || tile_map_temp[0].empty())
			return false;

		tile_map = tile_map_temp;

		// 地图缓存
		generate_map_cache();

		return true;
	}

	// 获取地图的宽度
	size_t get_width() const
	{
		if (tile_map.empty())
			return 0;

		return tile_map[0].size();
	}

	// 获取地图的高度
	size_t get_height() const
	{
		return tile_map.size();
	}

	// 获取地图信息
	const TileMap& get_tile_map() const
	{
		return tile_map;
	}

	// 获取防守位点的索引坐标
	const SDL_Point& get_idx_home() const
	{
		return idx_home;
	}

	// 获取路径池信息
	const SpawnerRoutePool& get_idx_spawner_pool() const
	{
		return spawner_route_pool;
	}

	// 放置塔防 
	void place_tower(const SDL_Point& idx_tile)
	{
		tile_map[idx_tile.y][idx_tile.x].has_tower = true;
	}

	void set_has_placed_towered(SDL_Point idx)
	{
		tile_map[idx.y][idx.x].has_tower = true;
	}

private:
	TileMap tile_map;						// 保存地图信息
	SDL_Point idx_home = { 0, 0 };			// 防守位点在地图中的索引位置
	SpawnerRoutePool spawner_route_pool;	// 生成器路径池，保存所有的线路

private:
	// 处理csv数据单元格中的首位的空白字符
	// eg: "  6\57\4\-1  "	->	"6\57\4\-1"
	std::string trim_str(const std::string& str)
	{
		size_t begin_idx = str.find_first_not_of(" \t");
		if (begin_idx == std::string::npos)
			return "";
		size_t end_idx = str.find_last_not_of(" \t");
		size_t idx_range = end_idx - begin_idx + 1;

		return str.substr(begin_idx, idx_range);
	}

	// 把读取的字符串转化为瓦片信息
	void load_tile_from_string(Tile& tile, const std::string& str)
	{
		std::string str_tidy = trim_str(str);

		std::string str_value;
		std::vector<int> values;
		std::stringstream str_stream(str_tidy);

		while (std::getline(str_stream, str_value, '\\'))
		{
			int value;
			try
			{
				value = std::stoi(str_value);
			}
			catch (const std::invalid_argument&)
			{
				value = -1;
			}
			values.push_back(value);
		}

		tile.terrian = (values.size() < 1 || values[0] < 0) ? 0 : values[0];
		tile.decoration = (values.size() < 2) ? -1 : values[1];
		tile.direction = (Tile::Direction)((values.size() < 3 || values[2] < 0) ? 0 : values[2]);
		tile.special_flag = (values.size() <= 3) ? -1 : values[3];
	}

	// 生成地图的缓存，包括房屋位点和敌人行进路线
	void generate_map_cache()
	{
		for (int y = 0; y < get_height(); y++)
		{
			for (int x = 0; x < get_width(); x++)
			{
				const Tile& tile = tile_map[y][x];
				if(tile.special_flag < 0)
					continue;

				if (tile.special_flag == 0)
				{
					idx_home.x = x;
					idx_home.y = y;
				}
				else
				{
					spawner_route_pool[tile.special_flag] = Route(tile_map, { x, y });
				}
			}
		}
	}
};

#endif // !_MAP_H_
