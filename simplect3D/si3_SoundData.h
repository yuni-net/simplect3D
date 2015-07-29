#ifndef biringal3D_sound_data_h_
#define biringal3D_sound_data_h_

#include "si3_popular.h"
#include "si3_Manager.h"

namespace si3
{
	class SoundData
	{
	public:
		SoundData();
		SoundData(uint id);

		bool load(uint id);
		uint get_ID() const;



		bool create_buffer(IDirectSoundBuffer8 * & secondarysb) const;
	private:
		uint id;

		fw::Wav wav_data;
		WAVEFORMATEX wfe;
		DSBUFFERDESC dsbdesc;

		void construct();
	};
}

#endif