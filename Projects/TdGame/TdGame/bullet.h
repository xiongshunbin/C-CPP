#ifndef _BULLET_H_
#define _BULLET_H_

#include "vector2.h"
#include "enemy.h"
#include "animation.h"
#include "config_manager.h"
#include "resources_manager.h"

// 子弹基类

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	void set_velocity(const Vector2& velocity)
	{
		this->velocity = velocity;

		if (can_rotated)
		{
			double randian = std::atan2(velocity.y, velocity.x);
			angle_anim_rotated = randian * 180 / 3.14159265;
		}
	}

	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	void set_damage(double damage)
	{
		this->damage = damage;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	double get_damage() const
	{
		return damage;
	}

	double get_damage_range() const
	{
		return damage_range;
	}

	void disable_collide()
	{
		is_collisional = false;
	}

	bool can_collide() const
	{
		return is_collisional;
	}

	void make_invalid()
	{
		is_valid = false;
		is_collisional = false;
	}

	bool can_remove() const
	{
		return !is_valid;
	}

	virtual void on_update(double delta)
	{
		animation.on_update(delta);
		position += velocity * delta;

		// 处理子弹飞出地图边界
		static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;

		if (position.x - size.x / 2 <= rect_tile_map.x
			|| position.x + size.x / 2 >= rect_tile_map.x + rect_tile_map.w
			|| position.y - size.y / 2 <= rect_tile_map.y
			|| position.y + size.y / 2 >= rect_tile_map.y + rect_tile_map.h)
		{
			is_valid = false;
		}
	}

	virtual void on_render(SDL_Renderer* renderer)
	{
		static SDL_Point point;

		point.x = (int)(position.x - size.x / 2);
		point.y = (int)(position.y - size.y / 2);

		animation.on_render(renderer, point, angle_anim_rotated);
	}

	virtual void on_collide(Enemy* enemy)
	{
		is_valid = false;
		is_collisional = false;
	}

protected:
	Vector2 size;					// 子弹的大小
	Vector2 velocity;				// 子弹的速度
	Vector2 position;				// 子弹的位置

	Animation animation;			// 子弹动画
	bool can_rotated = false;		// 子弹是否可以旋转

	double damage = 0;				// 子弹的伤害
	double damage_range = -1;		// 子弹的伤害范围

private:
	bool is_valid = true;			// 子弹是否有效
	bool is_collisional = true;		// 子弹是否可以发生碰撞
	double angle_anim_rotated = 0;	// 子弹的旋转角度

};



#endif // !_BULLET_H_
