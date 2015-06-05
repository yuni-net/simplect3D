#include <BoneMap.h>


namespace si3
{
	void BoneMap::register_name(const char * name)
	{
		std::string key(name);
		mymap.insert(make_pair(key, id));
		++id;
	}

	int BoneMap::get_bone_index(const char * name) const
	{
		return get_bone_index(std::string(name));
	}

	int BoneMap::get_bone_index(const std::string & name) const
	{
		auto iterator = mymap.find(name);
		if (iterator == mymap.end())
		{
			return -1;
		}
		return (*iterator).second;
	}


	BoneMap::BoneMap()
	{
		id = 0;
	}

}