#ifndef biringal3D_sound_data_h_
#define biringal3D_sound_data_h_

#include "popular.h"
#include "Manager.h"

namespace si3
{
	class sound_data
	{
	public:
		sound_data();
		sound_data(Manager & si3m, uint uID);

		bool load(Manager & si3m, uint uID);
		uint getID() const;



		bool create_buffer(IDirectSoundBuffer8 * & secondarysb) const;
	private:
		uint uID;
		Manager * si3mp;

		fw::Wav wav_data;
		WAVEFORMATEX wfe;
		DSBUFFERDESC dsbdesc;

		void construct();
	};
}

#endif