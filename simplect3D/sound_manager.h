#ifndef biringal3D_sound_manager_h_
#define biringal3D_sound_manager_h_

#include "popular.h"
#include <FantomWaves.h>

namespace si3
{
	class SoundManager
	{
	public:
		bool init();
		void reserve4ID_path(int reserve_size);
		void register_ID_path(uint id, const char * path);




		const char * get_path(uint id) const;
		LPDIRECTSOUND8 get_interface() const;
		~SoundManager();

	private:
		fw::Array<const char *> idpath_list;


		LPDIRECTSOUND8 dxs;
		LPDIRECTSOUNDBUFFER primarysb;
		fw::Window window_;
	};
}

#endif