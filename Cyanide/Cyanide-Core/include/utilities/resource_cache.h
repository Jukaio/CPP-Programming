// resource_cache.h

#ifndef INCLUDED_RESOURCE_CACHE_H
#define INCLUDED_RESOURCE_CACHE_H

#include <map>
#include <iostream>

// a wrapper for the standard libarary map 
// the most important method is its destructor 
// It ensures that whatever gets loaded into the cache,
// gets destroyed too

// this cache requires objects with a c++ destructor
// C-Style does not work

template<typename I, typename T>
struct resource_cache
{
	resource_cache() = default;
	~resource_cache()
	{
		int size = 0;
		for(auto&& item : m_cache)
		{
			if(item.second != nullptr)
				delete item.second;
			size++;
			std::cout << item.first << " deleted\n";
		}
		std::cout << size << " item/s deleted\n\n";
		m_cache.clear();
	}

	void add(I p_id, T* p_item)
	{
		if(exists(p_id))
			std::cout << p_id << " exists! ERROR!\n";
		else
			m_loaded++;

		m_cache.insert_or_assign(p_id, p_item);
		std::cout << p_id << " loaded in cache\n";
	}

	bool exists(I p_id)
	{
		auto it = m_cache.find(p_id);
		return it != m_cache.end();
	}

	T* get(I p_id)
	{
		return m_cache[p_id];
	}

	bool remove(I p_id)
	{
		auto it = m_cache.find(p_id);
		if(it != m_cache.end())
		{
			delete m_cache[p_id];
			m_cache.erase(it);
			m_cache.erase(p_id);
			return true;
		}
		return false;
	}

private:
	std::map<I, T*> m_cache;
	size_t m_loaded = 0;

	void* operator new[](size_t size) = delete;		// If bug in this file, delete/comment this line
													// Make it impossible to create an array of resource caches
													// Might bug a lot of shit, but whatever
};

#endif // !INCLUDED_RESOURCE_CACHE_H

