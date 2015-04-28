#ifndef biringal3D_sound_data_h_
#define biringal3D_sound_data_h_

#include "popular.h"
#include "manager.h"

namespace si3
{
	class sound_data
	{
	public:
		sound_data();
		sound_data(manager & si3m, uint uID);

		bool load(manager & si3m, uint uID);
		uint getID() const;



		bool create_buffer(IDirectSoundBuffer8 * & secondarysb) const;
	private:
		uint uID;
		manager * si3mp;

		fw::wav wav_data;
		WAVEFORMATEX wfe;
		DSBUFFERDESC dsbdesc;

		void construct();
	};
}

#endif