#include "Analysis.hpp"

// Analysis�N���X�̃R���X�g���N�^
Analysis::Analysis() {
    cout << "Analysis�R���X�g���N�^�Ăяo��" << endl;
    this->Set_Coodinates(); // ���Ղ̍��W��ݒ肷��֐��̌Ăяo��
    this->Set_Color(); // ���Ղ�������Ă��Ȃ���Ԃ�RGB�l���L�^����֐��̌Ăяo��

    // �e���Ղ̃C�x���g�i�����ꂽ���ǂ����j��key_event������
    for (int i = 0; i < 52; i++) {
        this->key_w_event[i] = false;
    }
    for (int i = 0; i < 36; i++) {
        this->key_b_event[i] = false;
    }
    for (int i = 0; i < 88; i++) {
        this->key_event[i] = false;
    }
}

// ���Ղ̍��W��ݒ肷��֐�
void Analysis::Set_Coodinates() {
    cout << "Set_Coodinates() ���W�Z�b�g�֐�" << endl;

    // 720p 88���� �̍��W�ݒ�
    cout << "Mode = 720p 88 key" << endl;

    // ������Y���W
    this->key_white_y = 665;
    // �����̍��W���v�Z
    for (int i = 0; i < 52; i++) {
        this->key_white_x[i] = (24.5 / 2.0) + i * 24.6;
    }

    // ������Y���W
    this->key_black_y = 620;
    // �����̍��W���v�Z
    this->key_black_x[0] = 28.5;

    int k = 0;
    int def = 27;
    double W_1 = 43.2;
    double W_2 = 29.3;

    for (int i = 1; i < 36; i++) {
        switch (k) {
        case 0:
            def += W_1;
            break;
        case 1:
            def += W_2;
            break;
        case 2:
            def += W_1;
            break;
        case 3:
            def += W_2;
            break;
        case 4:
            def += W_2;
            k = -1;
            break;
        }
        k++;
        this->key_black_x[i] = def;
    }


    int num = 0;
    int white = 0;
    int black = 0;

    for (; num < 88; num++) {// 88���ՑS�̂̍��W��ێ�����z���ݒ�
        if (this->True_White(num)) {// �������������𔻒肵�A�Ή�����X���W��ݒ�
            // ����
            key_x[num] = key_white_x[white];
            white++;
            cout << num << "w";
        } else {
            // ����
            key_x[num] = key_black_x[black];
            black++;
            cout << num << "b";
        }
    }

    cout << "" << endl;

    // X���W�������Ƀ\�[�g
    sort(key_x, key_x + 88);

    // ���W�̊m�F���s���֐����Ăяo��
    this->Check_Coodinates();
}

// ���Ղ�������Ă��Ȃ�������Ԃ�RGB�l���L�^����֐�
void Analysis::Set_Color() {
    cout << "Set_Color() �f�t�H���g�J���[�擾�֐�" << endl;

    // �e���Ղ̏�����Ԃ�RGB�l���L�^
    // ����
    this->def_w_clrB = frame.at<Vec3b>(key_white_y, key_white_x[0])[0];
    this->def_w_clrG = frame.at<Vec3b>(key_white_y, key_white_x[0])[1];
    this->def_w_clrR = frame.at<Vec3b>(key_white_y, key_white_x[0])[2];
    // ����
    this->def_b_clrB = frame.at<Vec3b>(key_black_y, key_black_x[0])[0];
    this->def_b_clrG = frame.at<Vec3b>(key_black_y, key_black_x[0])[1];
    this->def_b_clrR = frame.at<Vec3b>(key_black_y, key_black_x[0])[2];

    cout << "B:" << def_w_clrB << ",G:" << def_w_clrG << ",R:" << def_w_clrR << endl;
    cout << "B:" << def_b_clrB << ",G:" << def_b_clrG << ",R:" << def_b_clrR << endl;
}

// �f���̉�͂��s���Atxt�t�@�C���𐶐�����֐�
void Analysis::Analyze() {
    const static double fps = movie.Get_FPS();

    int frame_count = 1;

    for (;; frame = movie.Get_Next_Frame()) {
        if (frame.empty()) break;

        this->Check_Key(); // �L�[�̃C�x���g���A�b�v�f�[�g

        double time_now = ((double)frame_count / fps);

        // ���������������ȉ��ŃC�x���g��{�o�^
        if (str.size() > 0 && str.size() < 30) {
            cout << std::to_string(time_now) << endl;

            this->str_ += std::to_string(time_now);
            this->str_ += "ms";
            this->str_ += str;
            this->str_ += "\n";
        }
        str = "";

        frame_count++;
        this->first_key = 0;
        this->active_key_sum = 0;
        cv::imshow("movie", frame);
    }

    // �Ō�Ƀt�@�C���ɏ����o��
    this->Output_txt();
}

// ���W�̃`�F�b�N���s���֐�
void Analysis::Check_Coodinates() {
    cout << "Check_Coodinates() ���W�`�F�b�N�֐�" << endl;

    cv::namedWindow("movie", cv::WINDOW_AUTOSIZE);

    frame = movie.Get_Next_Frame();

    // �����̍��W��ԐF�̉~�ŕ\��
    for (const auto &e: this->key_white_x) {
        cout << e << endl;
        cv::circle(frame, cv::Point(e, this->key_white_y), 3, cv::Scalar(0, 0, 200), 3, 4);
    }
    // �����̍��W��ΐF�̉~�ŕ\��
    for (const auto &e: this->key_black_x) {
        cout << e << endl;
        cv::circle(frame, cv::Point(e, this->key_black_y), 3, cv::Scalar(0, 200, 0), 3, 4);
    }
    int i = 0;
    // 88���ՑS�̂̍��W�����F�̉~�ŕ\��
    for (const auto &e: this->key_x) {
        cout << i << "^" << e;
        cv::circle(frame, cv::Point(e, this->key_black_y + 30), 3, cv::Scalar(255, 255, 0), 3, 4);
        i += 1;
    }

    cv::imshow("movie", frame);

    cv::waitKey(0);
}

// �F���ς�������`�F�b�N����֐�
// ����
bool Analysis::Change_Color_w(int b, int g, int r) {
    int diff = abs(b - def_w_clrB) + abs(g - def_w_clrG) + abs(r - def_w_clrR);

    if (diff > threshold) {
        return true;
    } else {
        return false;
    }
}

// ����
bool Analysis::Change_Color_b(int b, int g, int r) {
    int diff = abs(b - def_b_clrB) + abs(g - def_b_clrG) + abs(r - def_b_clrR);

    if (diff > threshold) {
        return true;
    } else {
        return false;
    }
}

// �L�[�̃C�x���g���`�F�b�N����֐�
void Analysis::Check_Key() {
    int x, y, key;

    for (key = 0; key < 88; key++) {
        x = key_x[key];
        if (True_White(key)) {
            // ����
            y = key_white_y;
            if (this->key_event[key] == false) {
                // �����̐F���ς�����Ƃ�
                if (Change_Color_w(Get_Color_b(x, y), Get_Color_g(x, y), Get_Color_r(x, y))) {
                    cout << "[" << key << "W:on]" << endl;
                    cv::circle(frame, cv::Point(x, y), 3, cv::Scalar(0, 0, 200), 3, 4);
                    this->Register_Event(key, 1);
                    key_event[key] = true;
                }
            } else {
                // �����̐F�����ɖ߂����Ƃ�
                if (!Change_Color_w(Get_Color_b(x, y), Get_Color_g(x, y), Get_Color_r(x, y))) {
                    cout << "[" << key << "W:off]" << endl;
                    cv::circle(frame, cv::Point(x, y), 3, cv::Scalar(0, 200, 0), 3, 4);
                    this->Register_Event(key, 0);
                    key_event[key] = false;
                }
            }
        } else {
            // ����
            y = key_black_y;
            if (this->key_event[key] == false) {
                // �����̐F���ς�����Ƃ�
                if (Change_Color_b(Get_Color_b(x, y), Get_Color_g(x, y), Get_Color_r(x, y))) {
                    cout << "[" << key << "B:on]" << endl;
                    cv::circle(frame, cv::Point(x, y), 3, cv::Scalar(0, 0, 200), 3, 4);
                    this->Register_Event(key, 1);
                    key_event[key] = true;
                }
            } else {
                // �����̐F�����ɖ߂����Ƃ�
                if (!Change_Color_b(Get_Color_b(x, y), Get_Color_g(x, y), Get_Color_r(x, y))) {
                    cout << "[" << key << "B:off]" << endl;
                    cv::circle(frame, cv::Point(x, y), 3, cv::Scalar(0, 200, 0), 3, 4);
                    this->Register_Event(key, 0);
                    key_event[key] = false;
                }
            }
        }
    }
}

// �w�肳�ꂽ���W�̐F�����擾����֐�
int Analysis::Get_Color_b(int x, int y) {
    int b = frame.at<Vec3b>(y, x)[0];
    return b;
}

// �w�肳�ꂽ���W�̗ΐF�����擾����֐�
int Analysis::Get_Color_g(int x, int y) {
    int g = frame.at<Vec3b>(y, x)[1];
    return g;
}

// �w�肳�ꂽ���W�̐ԐF�����擾����֐�
int Analysis::Get_Color_r(int x, int y) {
    int r = frame.at<Vec3b>(y, x)[2];
    return r;
}

// �^����ꂽ�L�[�ԍ����������ǂ����𔻒肷��֐�
bool Analysis::True_White(int n) {
    n += 9;
    int a = n % 12;
    switch (a) {
    case 0:
        return true;
    case 1:
        return false;
    case 2:
        return true;
    case 3:
        return false;
    case 4:
        return true;
    case 5:
        return true;
    case 6:
        return false;
    case 7:
        return true;
    case 8:
        return false;
    case 9:
        return true;
    case 10:
        return false;
    case 11:
        return true;

    default:
        break;
    }
}

// �C�x���g��o�^����֐�
void Analysis::Register_Event(int key, int event) {
    std::ostringstream key_;
    key_ << key;

    str += "-";
    str += to_string(event);
    str += key_.str();
    this->active_key_sum++;
}

// ��͌��ʂ��e�L�X�g�t�@�C���ɏ����o���֐�
void Analysis::Output_txt() {
    ofstream outputfile("output.txt");
    outputfile << str_;
    outputfile.close();
}

int main() {
    Analysis analysis; // Analysis�N���X�̃C���X�^���X�𐶐�

    analysis.Check_Coodinates(); // ���Ղ̍��W���m�F����֐����Ăяo��

    analysis.Analyze(); // �f������͂��ăC�x���g�����m���A���ʂ��e�L�X�g�t�@�C���ɏ����o���֐����Ăяo��
}