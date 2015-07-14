/*
 * WELCOME TO simplect3D LIBRARY !
 * 
 * Version: 5.0.1
 * FinalUpdate: Feb 8th 2015
 * Author: NH 30070 AT12A171 10
 */

#pragma once

#include "Manager.h"
#include "Model.h"
#include "model_data.h"
#include "Leaf.h"
#include "image_data.h"
#include "sphere.h"
#include "billboard.h"
#include "Anime.h"
#include "sound_data.h"
#include "unique_sound.h"
#include "se.h"
#include "bunisher.h"
#include "sky_dome.h"
#include "sky_dome_mini.h"
#include "box.h"
#include "sphere.h"
#include "Motion3D.h"

#ifndef simplect3D_developping_
#ifdef _DEBUG
#pragma comment(lib, "simplect3dd.lib")
#else
#pragma comment(lib, "simplect3d.lib")
#endif
#endif


// release note ***********************************************************

#if 0

[5.0.1]
*when Debug, bunisher does not sort

[5.0.0]
*bunisher was implemented

[3.1.0]
*unique_sound, sound_manager, sound_data, se ������

[3.0.0]
*x�t�@�C���̃T�|�[�g��p�~
*pmd�t�@�C���̃T�|�[�g�ɕύX
*Model ���ʕ`��A���C�e�B���OOFF�ݒ�ɑΉ�
*camera view�ϊ��s������o���֐�������

[2.0.0]
*Matrix�̃f�t�H���g�����l��P�ʍs��ɐݒ肷��悤�ύX
*Matrix��make_*�n�֐���static�ł͂Ȃ����̂ɕύX
*Model��plus_*�n�C���^�[�t�F�C�X��p�~
*Model�ɂ�xyz��]�̏��Ԃ��w��ł���@�\��ǉ�
*Matrix��D3DXMATRIX�ŏ������ł���@�\��ǉ�
*Leaf�����billboard��affin�֐���ǉ�
*Matrix�����������]�s����C��
*Leaf::image_data(), billboard::image_data()��p�����ۂ�Manager�̃|�C���^���擾����悤�ɏC��
*Model�ŃJ�����O�̗L���������w�肷��@�\��ǉ�
*Model��model_data���w�肷��Ƃ��ɂ�Manager�̃C���X�^���X���w�肷��悤�ύX

[1.2.0]
*context color�����f�����悤�ɏC��
*���f���ǂݍ��݂̍ہA���S�_���w��ł���@�\��ǉ�
*model_data draw_alpha()�̍ہA���[���h�ϊ��s��̎w�肪�����Ă����̂Œǉ�
*material�̒l��ς��Ă����f����Ă��Ȃ������悤�Ȃ̂ŏC��
*Leaf�ɂăJ�����O��ON, OFF��؂�ւ���@�\��ǉ�

[1.1.0]
*Leaf release()�̂Ƃ�ownership�ɉ����ď�����ς���悤�C��
*anime_data, Anime ������
*Anime, Leaf �ŁAcompute_world_mat�̌Ăяo���ʒu���C��
*billboard�Ɋ���̃R���X�g���N�^������
*DisplayManager�ɂ�context color��ݒ肷�邱�Ƃ��ł���悤�ɂȂ���

[1.0.2]
*Anime��ǉ��������ALeaf���R�s�[���������B

[1.0.1]
*Leaf�̕ύX�ɔ�����billboard���C���BLeaf���p����������ɂ���

[1.0.0]
*land�̃f�t�H���g������Z�}�C�i�X�����ɂ��A�n�ʂł͂Ȃ��Ȃ������ߖ��̂�Leaf�ɕύX

[0.1.0]
*Leaf�ŉ�]�@�\��ǉ�

[0.0.1]
*model_data��draw_alpha��alpha�l��1.0f�ƈ�v���Ă��邩�ǂ����̔�����A==�ł͂Ȃ�abs��p���Ĕ��肷��悤�ύX
*model_data�̓ǂݍ��݂̍ہA�ŏ���release()���ĂԂ悤�ɕύX

#endif