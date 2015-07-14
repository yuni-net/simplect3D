#ifndef canimer_h_
#define canimer_h_

#include "fw_includes.h"

namespace si3
{
	class canimer
	{
	public:
		canimer();
		void config(int frame/*�A�j���R�}��*/, int resistor/*�A�j���[�V�����X�s�[�h�}���l�B�l���傫���قǃA�j���[�V�������x���Ȃ�*/);
		void frame(int v);
		void resistor(int v);
		void set(int No);		// �A�j���[�V�����ʒu�������I�ɐݒ肵�܂��B0�`maxNo()�܂ł̐����Ŏw�肵�܂�
		void move(int num);		// �A�j���[�V�����ʒu���w�萔�l���i�܂��܂��B�}�C�i�X�̒l���w��\�ł�
		int now() const;		// ���݉��R�}�ڂȂ̂���Ԃ��܂� ���A�j���[�V�����ʒu�ł͂���܂���I
		int nowNo() const;
		int maxNo() const;		// �A�j���[�V�����ʒu�̍ő�l��Ԃ��܂�

	private:
		int counter;
		int nFrame;
		int nResistor;
	};
}

#endif