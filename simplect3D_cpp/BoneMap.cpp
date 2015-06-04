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
		auto iterator = mymap.find(std::string(name));
		return (*iterator).second;
	}



	BoneMap::BoneMap()
	{
		id = 0;
	}

}