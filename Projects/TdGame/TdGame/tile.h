#ifndef _TILE_H_
#define _TILE_H_

#include <vector>

#define SIZE_TILE 48	// 瓦片大小

struct Tile
{
	// 方向
	enum class Direction
	{
		None = 0,
		Up,			// 上
		Down,		// 下
		Left,		// 左
		Right		// 右
	};
	// 渲染层
	int terrian = 0;						// 地形层，图形块的编号
	int decoration = -1;					// 装饰，-1：没有装饰

	// 数据层
	int special_flag = -1;					// 特殊标志，-1: 没有，0：房屋，> 0: 刷怪的位点
	bool has_tower = false;					// 是否放置塔防
	Direction direction = Direction::None;	// 怪物的行经方向
};

typedef std::vector<std::vector<Tile>> TileMap;

#endif // !_TILE_H_
