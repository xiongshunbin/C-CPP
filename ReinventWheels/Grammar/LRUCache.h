#pragma once
#include <iostream>
#include <stdexcept>
#include <list>
#include <unordered_map>
#include <string>

template<typename K, typename V>
class LRUCache
{
public:
	explicit LRUCache(size_t capacity) : cap_(capacity) 
	{
		if (capacity == 0)
		{
			throw std::invalid_argument("capacity must be > 0");
		}
	}
	~LRUCache(){ }

	// 命中返回true, 并把value写入out, 同时将当前节点移动到表头; 未命中返回false;.
	bool get(const K& key, V& out)
	{
		auto it = index_.find(key);
		if (it == index_.end())
		{
			return false;
		}
		
		items_.splice(items_.begin(), items_, it->second);
		out = it->second->second;
		return true;
	}

	// 插入或者更新: 存在则更新, 并把当前更新的节点挪到表头; 如果不存在则插入, 可能需要淘汰尾部的节点
	void put(const K& key, V value)
	{
		auto it = index_.find(key);
		if (it != index_.end())
		{
			it->second->second = std::move(value);
			items_.splice(items_.begin(), items_, it->second);
			return;
		}
		if (items_.size() == cap_)
		{
			auto& old = items_.back();
			index_.erase(old.first);
			items_.pop_back();
		}
		items_.emplace_front(key, std::move(value));
		index_[items_.front().first] = items_.begin();
	}

	void erase(const K& key)
	{
		auto it = index_.find(key);
		if (it == index_.end())
		{
			return;
		}
		items_.erase(it->second);
		index_.erase(key);
	}

	void clear()
	{
		items_.clear();
		index_.clear();
	}

	size_t size() const
	{
		return items_.size();
	}
	size_t capacity() const 
	{
		return cap_;
	}

	// 可选: 调整容量(若变小则立即淘汰多于元素)
	void resize(size_t newcap)
	{
		if (newcap == 0)
		{
			throw std::invalid_argument("newcap must be > 0");
		}
		cap_ = newcap;
		while (items_.size() > cap_)
		{
			auto& old = items_.back();
			index_.erase(old.first);
			items_.pop_back();
		}
	}

	// 用于演示打印当前从新到旧的顺序
	void debug_print() const
	{
		std::cout << "[MRU -> LRU]: ";
		for (auto it = items_.begin(); it != items_.end(); ++it)
		{
			std::cout << "(" << it->first << ":" << it->second << ") ";
		}
		std::cout << std::endl;
	}

private:
	size_t cap_;
	std::list<std::pair<K, V>> items_;
	std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> index_;
};

void testLRUCache()
{
	LRUCache<int, std::string> cache(3);
	cache.put(1, "A");
	cache.put(2, "B");
	cache.put(3, "C");
	cache.debug_print();

	std::string out;
	if (cache.get(2, out))
	{
		std::cout << "get 2 ->" << out << std::endl;
	}
	cache.debug_print();

	cache.put(4, "D");
	cache.debug_print();

	cache.put(2, "B2");
	cache.debug_print();

	cache.erase(3);
	cache.debug_print();

	cache.resize(1);
	cache.debug_print();
}