#ifndef _TOWER_MANAGER_H_
#define _TOWER_MANAGER_H_

#include "tower.h"
#include "manager.h"
#include "archer_tower.h"
#include "axeman_tower.h"
#include "gunner_tower.h"
#include "config_manager.h"
#include "resources_manager.h"

#include <vector>

class TowerManager : public Manager<TowerManager>
{
	friend class Manager<TowerManager>;

public:
	void on_update(double delta)
	{
		for (Tower* tower : tower_list)
			tower->on_update(delta);
	}

	void on_render(SDL_Renderer* renderer)
	{
		for (Tower* tower : tower_list)
			tower->on_render(renderer);
	}

	double get_place_cost(TowerType type)
	{
		static ConfigManager* instance = ConfigManager::instance();

		switch (type)
		{
		case TowerType::Archer:
			return instance->archer_template.cost[instance->level_archer];
			break;
		case TowerType::Axeman:
			return instance->axeman_template.cost[instance->level_axeman];
			break;
		case TowerType::Gunner:
			return instance->gunner_template.cost[instance->level_gunner];
			break;
		}

		return 0;
	}

	double get_upgrade_cost(TowerType type)
	{
		static ConfigManager* instance = ConfigManager::instance();

		switch (type)
		{
		case TowerType::Archer:
			return instance->level_archer == 9 ? -1 : 
				instance->archer_template.upgrade_cost[instance->level_archer];
			break;
		case TowerType::Axeman:
			return instance->level_axeman == 9 ? -1 : 
				instance->axeman_template.upgrade_cost[instance->level_axeman];
			break;
		case TowerType::Gunner:
			return instance->level_gunner == 9 ? -1 : 
				instance->gunner_template.upgrade_cost[instance->level_gunner];
			break;
		}

		return 0;
	}

	double get_damage_range(TowerType type)
	{
		static ConfigManager* instance = ConfigManager::instance();

		switch (type)
		{
		case TowerType::Archer:
			return instance->archer_template.view_range[instance->level_archer];
			break;
		case TowerType::Axeman:
			return instance->axeman_template.view_range[instance->level_axeman];
			break;
		case TowerType::Gunner:
			return instance->gunner_template.view_range[instance->level_gunner];
			break;
		}

		return 0;
	}

	void place_tower(TowerType type, const SDL_Point& idx)
	{
		Tower* tower = nullptr;

		switch (type)
		{
		case TowerType::Archer:
			tower = new ArcherTower();
			break;
		case TowerType::Axeman:
			tower = new AxemanTower();
			break;
		case TowerType::Gunner:
			tower = new GunnerTower();
			break;
		default:
			tower = new ArcherTower();
			break;
		}

		static Vector2 position;
		static const SDL_Rect& rect = ConfigManager::instance()->rect_tile_map;

		position.x = rect.x + idx.x * SIZE_TILE + SIZE_TILE / 2;
		position.y = rect.y + idx.y * SIZE_TILE + SIZE_TILE / 2;
		tower->set_position(position);
		tower_list.push_back(tower);

		static const ResourcesManager::SoundPool& sound_pool 
			= ResourcesManager::instance()->get_sound_pool();

		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_PlaceTower)->second, 0);
	}

	void upgrade_tower(TowerType type)
	{
		static ConfigManager* instance = ConfigManager::instance();

		switch (type)
		{
		case TowerType::Archer:
			instance->level_archer = instance->level_archer >= 9 ? 9 : instance->level_archer + 1;
			break;
		case TowerType::Axeman:
			instance->level_axeman = instance->level_axeman >= 9 ? 9 : instance->level_axeman + 1;
			break;
		case TowerType::Gunner:
			instance->level_gunner = instance->level_gunner >= 9 ? 9 : instance->level_gunner + 1;
			break;
		}

		static const ResourcesManager::SoundPool& sound_pool
			= ResourcesManager::instance()->get_sound_pool();

		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_TowerLevelUp)->second, 0);
	}

protected:
	TowerManager() = default;
	~TowerManager() = default;

private:
	std::vector<Tower*> tower_list;
};


#endif // !_TOWER_MANAGER_H_
