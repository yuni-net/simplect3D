#pragma once

#include "si3_popular.h"
#include "si3_KeySystem.h"

namespace si3
{
	class Key
	{
	public:
		bool init(LPDIRECTINPUT8 & diinterface, HWND windle);
		void update();

		/**
		@brief I check whether the user is pushing the 'key.'
		@param Set the ID of key which was defined by DirectX.
				exp. DIC_RIGHT, DIC_9, DIC_A and so on.
		@return
			true: the user is pushing the 'key.'
			false: the user is NOT pushing the 'key.'
		*/
		bool pushing(unsigned char key) const;

		/**
		@brief I check whether the user is NOT pushing the 'key.'
		@param Set the ID of key which was defined by DirectX.
		@return
			true: the user is NOT pushing the 'key.'
			false: the user is pushing the 'key.'
		*/
		bool freeing(unsigned char key) const;

		/**
		@brief I check whether the user is pushing any key.
		@param Set the ID of key which was defined by DirectX.
		@return
			true: the user is pushing any key.
			false: the user is NOT pushing any key.
		*/
		bool any_pushing() const;

		/**
		@brief I check whether the user is pushing none of key.
		@param Set the ID of key which was defined by DirectX.
		@return
			true: the user is pushing none of key.
			false: the user is pushing any key.
		*/
		bool all_freeing() const;

		/**
		@brief I check whether the user has just pushed the 'key.'
		@param Set the ID of key which was defined by DirectX.
		@return
			true: the user has just pushed the 'key.'
			false: the user is pushing the 'key' since 1 frame ago, or the user is NOT pushing the 'key.'
		*/
		bool pushed(unsigned char key) const;

		/**
		@brief I check whether the user has just stop pushing the 'key.'
		@param Set the ID of key which was defined by DirectX.
		@return
			true: The user has just stop pushing the 'key.'
			false: The user is NOT pushing the 'key' since 1 frame ago, or the user is pushing the 'key.'
		*/
		bool clicked(unsigned char key) const;





	private:
		BYTE keystate[2][256];
		bool active;
		Key_system key_systemi;


	};
}