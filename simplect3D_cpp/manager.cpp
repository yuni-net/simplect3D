#include "si3_Manager.h"


namespace si3
{
	/*
	* @return true:���� false:���s
	*
	*/
	bool Manager::init()
	{
		return get_instance().init_dynamic();
	}

	/*
	* ���t���[���ŏ��ɌĂяo������
	* @return true:���s�\ false:���s�s�\
	*
	*/
	bool Manager::begin_frame()
	{
		return get_instance().begin_frame_dynamic();
	}

	/***
	@brief �\�����X�g�ɃI�u�W�F�N�g��o�^����B
	@detail ���̓o�^�͂��̃t���[���ł̂ݗL���ł��邽�߁A
	���t���[���`�悵������Ζ��t���[���o�^�������K�v������B
	*/
	void Manager::register_display_object(const DisplayObject & display_objecti)
	{
		get_instance().register_display_object_dynamic(display_objecti);
	}

	/***
	@brief �L�[�{�[�h�����Ǘ�����N���X�̃C���X�^���X�𓾂�B
	*/
	const ::si3::Key & Manager::key()
	{
		return get_instance().key_dynamic();
	}

	/***
	@brief �}�E�X�����Ǘ�����N���X�̃C���X�^���X�𓾂�B
	*/
	const ::si3::Mouse & Manager::mouse()
	{
		return get_instance().mouse_dynamic();
	}

	/***
	@brief �T�E���h�����Ǘ�����N���X�̃C���X�^���X�𓾂�B
	*/
	SoundManager & Manager::get_sound_manager()
	{
		return get_instance().get_sound_manager_dynamic();
	}

	/***
	@brief �J�������Ǘ�����N���X�̃C���X�^���X�𓾂�B
	*/
	::si3::Camera & Manager::camera()
	{
		return get_instance().camera_dynamic();
	}

	/***
	@brief �f�B�X�v���C���Ǘ�����N���X�̃C���X�^���X�𓾂�B
	*/
	::si3::DisplayManager & Manager::display_manager()
	{
		return get_instance().display_manager_dynamic();
	}


	/*
	* �`����e����ʂɔ��f����
	*
	*/
	void Manager::show()
	{
		get_instance().show_dynamic();
	}




	LPDIRECT3DDEVICE9 Manager::get_dxdevice()
	{
		return get_instance().get_dxdevice_dynamic();
	}
	Manager & Manager::get_instance()
	{
		static Manager manager;
		return manager;
	}




	bool Manager::init_dynamic()
	{
		fps60i.init();

		windowi.width(640).height(480);
		windowi.create();

		if (dxm.init(windowi.myhandle(), windowi.width(), windowi.height())==false) return false;

		if (soundm.init() == false) return false;
		displaym.init(dxm.get_present_parameters(), dxm.get_dxdevice());
		if(dxlighti.init(dxm.get_dxdevice())==false) return false;
		if(dxcamerai.init(dxm.get_dxdevice(), windowi.width(), windowi.height())==false) return false;
		if(dxinputi.init(windowi.myhandle())==false) return false;

		return true;
	}

	/*
	* ���t���[���ŏ��ɌĂяo������
	* ���s�\�Ȃ�true�A�s�\�Ȃ�false��Ԃ�
	*
	*/
	bool Manager::begin_frame_dynamic()
	{
		dxinputi.update();
		// todo
		return fw::Window::processage();	// �v���Z�X���b�Z�[�W����������
	}

	void Manager::register_display_object_dynamic(const DisplayObject & display_objecti)
	{
		displaym.register_display_object(display_objecti);
	}

	const ::si3::Key & Manager::key_dynamic() const
	{
		return dxinputi.key();
	}
	const ::si3::Mouse & Manager::mouse_dynamic() const
	{
		return dxinputi.mouse();
	}
	SoundManager & Manager::get_sound_manager_dynamic()
	{
		return soundm;
	}

	::si3::Camera & Manager::camera_dynamic()
	{
		return dxcamerai;
	}

	::si3::DisplayManager & Manager::display_manager_dynamic()
	{
		return displaym;
	}

	LPDIRECT3DDEVICE9 Manager::get_dxdevice_dynamic()
	{
		return dxm.get_dxdevice();
	}

	/*
	* �`����e����ʂɔ��f����
	*
	*/
	void Manager::show_dynamic()
	{
		dxcamerai.update();
		displaym.draw();

		fps60i.stop_if_need();	// �`��̃^�C�~���O��60fps�ɂ���
	}


}