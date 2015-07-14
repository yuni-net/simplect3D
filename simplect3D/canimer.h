#ifndef canimer_h_
#define canimer_h_

#include "fw_includes.h"

namespace si3
{
	/***
	�A�j���[�V�����̕\���R�}���Ǘ�����N���X�ł��B
	*/
	class AniCounter
	{
	public:
		AniCounter();

		/***
		@brief �A�j���[�V�����̐ݒ���s���܂��B
		@param
			frame: �A�j���R�}��
			resistor: �A�j���[�V�����X�s�[�h�}���l�B�l���傫���قǃA�j���[�V�������x���Ȃ�܂��B
		*/
		void config(int frame, int resistor);

		void frame(int v);
		void resistor(int v);
		void set(int No);		// �A�j���[�V�����ʒu�������I�ɐݒ肵�܂��B0�`maxNo()�܂ł̐����Ŏw�肵�܂�
		void move(int num);		// �A�j���[�V�����ʒu���w�萔�l���i�܂��܂��B�}�C�i�X�̒l���w��\�ł�
		int now() const;		// ���݉��R�}�ڂȂ̂���Ԃ��܂� ���A�j���[�V�����ʒu�ł͂���܂���I
		int now_No() const;
		int max_No() const;		// �A�j���[�V�����ʒu�̍ő�l��Ԃ��܂�

	private:
		int counter;
		int frame_;
		int resistor_;
	};
}

#endif