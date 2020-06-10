/***************************************
Note to self.
For some reason if the K template parameter is an std::string
This doesn't work. I cannot be bothered to debug it though.
Something to do with the hash function....

***************************************/
#pragma once
#include <unordered_map>
#include <string>











template <typename K, typename T, typename HashK, typename HashT> 
class BiMap
{
public:
	bool insert(K key, T val)
	{
		if(map1.find(key) == map1.end() && map2.find(val) == map2.end())
		{
			map1.insert(std::make_pair(key, val));
			map2.insert(std::make_pair(val, key));
			return true;
		}
		
		return false;
	}
	/*
	std::unordered_map<K, T, std::hash<HashK> >::iterator begin()
	{
		return map1.begin();
	}

	std::unordered_map<K, T, std::hash<HashK> >::iterator end()
	{
		return map1.end();
	}*/


//operator[] should not be used! Deprecated!
	T* operator[](const K& key)
	{
		auto finder1 = map1.find(key);
		if(finder1 == map1.end())
			return nullptr;
		return &(finder1->second);
	}

	K* operator[](const T& key)
	{
		auto finder2 = map2.find(key);
		if(finder2 == map2.end())
			return nullptr;
		return &(finder2->second);

	}




	typename std::unordered_map<K, T, std::hash<HashK> >::iterator 
	find(const K& key)
	{
		return map1.find(key);
	}

	bool isEnd(typename std::unordered_map<K, T, std::hash<HashK> >::iterator& it)
	{
		return it == map1.end();
	}

	typename std::unordered_map<T, K, std::hash<HashT> >::iterator  
	find(const T& key)
	{
		return map2.find(key);
	}

	bool isEnd(typename std::unordered_map<T, K, std::hash<HashT> >::iterator& it)
	{
		return it == map2.end();
	}

	bool contains(const T& key)
	{
		auto finder2 = map2.find(key);
		return finder2 != map2.end();
	}

	bool contains(const K& key)
	{
		auto finder1 = map1.find(key);
		return finder1 != map1.end();
	}

	

	//typename std::unordered_map<K, T, std::hash<HashK> >::iterator finder1;
	//typename std::unordered_map<T, K, std::hash<HashT> >::iterator finder2;
	std::unordered_map<K, T, std::hash<HashK> > map1;
	std::unordered_map<T, K, std::hash<HashT> > map2;
};