#ifndef biringal3D_sound_manager_h_
#define biringal3D_sound_manager_h_

#include "popular.h"
#include <FantomWaves.h>

namespace si3
{
	class sound_manager
	{
	public:
		bool init();
		void reserve4ID_path(int nReserveSize);
		void register_ID_path(uint nID, const char * pszPath);




		const char * get_path(uint nID) const;
		LPDIRECTSOUND8 get_interface() const;
		~sound_manager();

	private:
		fw::Array<const char *> idpath_list;


		LPDIRECTSOUND8 dxs;
		LPDIRECTSOUNDBUFFER primarysb;
		fw::Window window_;
	};
}

#endif