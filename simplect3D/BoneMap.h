#ifndef si3_ModelMap_h_
#define si3_ModelMap_h_

#include <map>
#include <string>

namespace si3
{
	class BoneMap
	{
	public:
		void register_name(const char * name);
		int get_bone_index(const char * name) const;
		int get_bone_index(const std::string & name) const;



		BoneMap();
	private:
		std::map<std::string, int> mymap;
		int id;
	};
}

#endif