#ifndef ANALYSIS_HPP

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Movie {
private:
    VideoCapture cap; // �����ǂݍ��ނ��߂̃o�b�t�@
    Mat img; // �摜�p�̃o�b�t�@�A���悩��P�t���[�������o��
    int max_frame; // ����̑��t���[����
    int cur_frame_num = 0; // ���ݓǂݍ���ł���t���[��

public:
    Movie() {
        // ����̓ǂݍ���
        this->cap.open("C:\\opencv\\video.mp4");

        // ����̑��t���[�������擾
        this->max_frame = cap.get(CAP_PROP_FRAME_COUNT);

        // �I�t�Z�b�g���̃t���[�����΂�
        for (int i = 0; i < this->Get_FPS() * 3; i++) {
            if (cur_frame_num < max_frame) cap >> img;
        }
    }

    ~Movie() {}

    // ���̃t���[�����擾����֐�
    Mat Get_Next_Frame() {
        if (cur_frame_num < max_frame) cap >> img;
        return this->img;
    }

    // ����̃t���[�����[�g���擾����֐�
    inline int Get_FPS() {
        return cap.get(CAP_PROP_FPS);
    }

    // ����̑��t���[�������擾����֐�
    inline int Get_Max_Frame() {
        return this->max_frame;
    }

    // ���݂̃t���[���ԍ����擾����֐�
    inline int Cur_Frame_Num() {
        return this->cur_frame_num;
    }
};

class Analysis {
private:
    const static int threshold = 50;// �F���ǂꂭ�炢�ς������ω������ƔF�����邩��臒l

    std::ostringstream pos_msec;// ���ԏ���ێ����邽�߂̕�����X�g���[��

    // ���W�̕ێ�
    // ����
    int key_white_y; // �c��
    int key_white_x[52]; // ����
    // ����
    int key_black_y; // �c��
    int key_black_x[36]; // ����
    // 88���ՑS��
    int key_x[88];

    int active_key_sum = 0; // ��ʕύX�̎��̌��m�΍�(�ω��ʂ���������ƁA�����o�����Ȃ��t���O�݂����Ȏg����)

    int first_key = 0; // ���߂ẴL�[�C�x���g�̃t���O

    Movie movie; // ����̏������s���C���X�^���X

    cv::Mat frame; // �t���[����ێ�����Mat�I�u�W�F�N�g

    // �e���Ղ�������ĂȂ�������Ԃ�RGB�l��ێ�����ϐ�
    // ����
    int def_w_clrB;
    int def_w_clrG;
    int def_w_clrR;
    // ����
    int def_b_clrB;
    int def_b_clrG;
    int def_b_clrR;

    // �e���Ղ̍��W�̃C�x���g��ԁi�����ꂽ���ǂ����j��ێ�����z��
    bool key_w_event[52]; // ����
    bool key_b_event[36]; // ����
    bool key_event[88]; // 88���ՑS�̂̃C�x���g��Ԃ�ێ�����z��
    bool key_active[88]; // 88���ՑS�̂̃A�N�e�B�u��ԁi������Ă���j���ǂ�����ێ�����z��

    string str = ""; // 1�t���[������ێ����镶����
    string str_ = ""; // ����1�t���[�����ɖ�肪�Ȃ���΁A��������

public:
    Analysis(); // �R���X�g���N�^
    void Set_Coodinates(); // ���Ղ̍��W��ݒ肷��֐�
    void Set_Color(); // ���Ղ�������Ă��Ȃ�������Ԃ�RGB���L�^����֐�
    void Analyze(); // �f���̉�͂��s���Atxt�t�@�C���𐶐�����֐�
    void Check_Coodinates(); // ��ʂ̍��W���`�F�b�N����֐�

    // �e���Ղ̐F���ς�������`�F�b�N����֐�(�F���ς���Ă�����true��Ԃ�)
    bool Change_Color_w(int b, int g, int r);// ����
    bool Change_Color_b(int b, int g, int r);// ����

    void Check_Key(); // �e���Ղ̏�Ԃ��`�F�b�N����֐�

    // �w�肳�ꂽ���W�̏����擾����֐�
    int Get_Color_b(int x, int y); // �F
    int Get_Color_g(int x, int y); // �ΐF
    int Get_Color_r(int x, int y); // �ԐF

    bool True_White(int n); // ���Ղ��������ǂ����𔻒肷��֐�

    void Register_Event(int key, int event); // �e���Ղ̃C�x���g�i�����ꂽ���ǂ����j��o�^����֐�

    void Output_txt(); // ��͌��ʂ��e�L�X�g�t�@�C���ɃC�x���g�����o�͂���֐�
};

#endif ANALYSIS_HPP
