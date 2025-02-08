#ifndef _TILE_H_
#define _TILE_H_

#include <vector>

#define SIZE_TILE 48	// ��Ƭ��С

struct Tile
{
	// ����
	enum class Direction
	{
		None = 0,
		Up,			// ��
		Down,		// ��
		Left,		// ��
		Right		// ��
	};
	// ��Ⱦ��
	int terrian = 0;						// ���β㣬ͼ�ο�ı��
	int decoration = -1;					// װ�Σ�-1��û��װ��

	// ���ݲ�
	int special_flag = -1;					// �����־��-1: û�У�0�����ݣ�> 0: ˢ�ֵ�λ��
	bool has_tower = false;					// �Ƿ��������
	Direction direction = Direction::None;	// ������о�����
};

typedef std::vector<std::vector<Tile>> TileMap;

#endif // !_TILE_H_
