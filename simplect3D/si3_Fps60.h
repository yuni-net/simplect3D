#pragma once

#include "si3_popular.h"

/*
 * �`���fps��60�ɂ���ړI�Ŏg�p�����
 * �S�Ă̕`����I������A�O��̕`�抮��������33.3ms�ȏ�o�߂��Ă����ꍇ�̂ݏ������ɐi�߂�p�r��z�肵�Ă���
 * �O��stop_if_need���Ă΂�Ă���33.3ms�ȏ�o�߂��Ă����stop_if_need�֐�����E�o����
 *
 */
namespace si3
{
	class Fps60
	{
	public:
		void init();
		void stop_if_need();


		~Fps60();
	private:
		DWORD old_ms;
	};
}