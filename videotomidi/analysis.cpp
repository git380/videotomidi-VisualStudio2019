#include "Analysis.hpp"

// Analysisクラスのコンストラクタ
Analysis::Analysis() {
    cout << "Analysisコンストラクタ呼び出し" << endl;
    this->Set_Coodinates(); // 鍵盤の座標を設定する関数の呼び出し
    this->Set_Color(); // 鍵盤が押されていない状態のRGB値を記録する関数の呼び出し

    // 各鍵盤のイベント（88鍵盤が押されたかどうか）のkey_event初期化
    for (int i = 0; i < 52; i++) {//白鍵
        this->key_w_event[i] = false;
    }
    for (int i = 0; i < 36; i++) {// 黒鍵
        this->key_b_event[i] = false;
    }
    for (int i = 0; i < 88; i++) {// 88鍵盤全体
        this->key_event[i] = false;
    }
}

// 鍵盤の座標を設定する関数
void Analysis::Set_Coodinates() {
    cout << "Set_Coodinates() 座標セット関数" << endl;

    // 720p 88鍵盤 の座標設定
    cout << "Mode = 720p 88 key" << endl;

    // 白鍵
    // Y座標(鍵盤は、横に並んでいるので縦軸は変更なし)
    this->key_white_y = 665;
    // X座標を計算
    for (int i = 0; i < 52; i++) {
        this->key_white_x[i] = (24.5 / 2.0) + i * 24.6;
    }

    // 黒鍵
    // Y座標(鍵盤は、横に並んでいるので縦軸は変更なし)
    this->key_black_y = 620;
    // X座標を計算
    this->key_black_x[0] = 28.5; // 一番最初の黒鍵座標設定
    int k = 0; // 1オクターブ分の黒鍵変数
    int def = 27; // 黒鍵座標変数
    double W_1 = 43.2; // 間に黒鍵がない
    double W_2 = 29.3; // 間に黒鍵がある
    for (int i = 1; i < 36; i++) {
        switch (k) {
        case 0: // #ド
            def += W_1;
            break;
        case 1: // #レ
            def += W_2;
            break;
        case 2: // #ファ
            def += W_1;
            break;
        case 3: // #ソ
            def += W_2;
            break;
        case 4: // #ラ
            def += W_2;
            k = -1;// 1オクターブ分完了 黒鍵リセット
            break;
        }
        k++; // 次の黒鍵
        this->key_black_x[i] = def; // 黒鍵座標を登録
    }

    // 88鍵盤全体の座標を保持する配列を設定
    int white = 0;
    int black = 0;
    for (int num = 0; num < 88; num++) {
        if (this->True_White(num)) {// 白鍵か黒鍵かを判定し、対応するX座標を設定
            // 白鍵
            key_x[num] = key_white_x[white];
            white++;
            cout << num << "w";
        } else {
            // 黒鍵
            key_x[num] = key_black_x[black];
            black++;
            cout << num << "b";
        }
    }

    cout << "" << endl;

    // X座標を昇順にソート
    sort(key_x, key_x + 88);

    // 座標の確認を行う関数を呼び出し
    this->Check_Coodinates();
}

// 鍵盤が押されていない初期状態のRGB値を記録する関数
void Analysis::Set_Color() {
    cout << "Set_Color() デフォルトカラー取得関数" << endl;

    // 各鍵盤の初期状態のRGB値を記録
    // 白鍵
    this->def_w_clrB = frame.at<Vec3b>(key_white_y, key_white_x[0])[0];
    this->def_w_clrG = frame.at<Vec3b>(key_white_y, key_white_x[0])[1];
    this->def_w_clrR = frame.at<Vec3b>(key_white_y, key_white_x[0])[2];
    // 黒鍵
    this->def_b_clrB = frame.at<Vec3b>(key_black_y, key_black_x[0])[0];
    this->def_b_clrG = frame.at<Vec3b>(key_black_y, key_black_x[0])[1];
    this->def_b_clrR = frame.at<Vec3b>(key_black_y, key_black_x[0])[2];

    cout << "B:" << def_w_clrB << ",G:" << def_w_clrG << ",R:" << def_w_clrR << endl;
    cout << "B:" << def_b_clrB << ",G:" << def_b_clrG << ",R:" << def_b_clrR << endl;
}

// 映像の解析を行い、txtファイルを生成する関数
void Analysis::Analyze() {
    const static double fps = movie.Get_FPS();
    // 1フレームずつ処理する
    for (int frame_count = 1;; frame = movie.Get_Next_Frame()) {
        if (frame.empty()) break;

        this->Check_Key(); // キーのイベントをアップデート

        double time_now = ((double)frame_count / fps); // 1フレームをfpsで割ることで、時間が出る

        // 同時発音数が一定以下でイベントを本登録
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
    this->Output_txt(); // 最後にファイルに書き出し
}

// 座標のチェックを行う関数
void Analysis::Check_Coodinates() {
    cout << "Check_Coodinates() 座標チェック関数" << endl;

    cv::namedWindow("movie", cv::WINDOW_AUTOSIZE);

    frame = movie.Get_Next_Frame();

    // 白鍵の座標を赤色の円で表示
    for (const auto &e: this->key_white_x) {
        cout << e << endl;
        cv::circle(frame, cv::Point(e, this->key_white_y), 3, cv::Scalar(0, 0, 200), 3, 4);
    }
    // 黒鍵の座標を緑色の円で表示
    for (const auto &e: this->key_black_x) {
        cout << e << endl;
        cv::circle(frame, cv::Point(e, this->key_black_y), 3, cv::Scalar(0, 200, 0), 3, 4);
    }
    int i = 0;
    // 88鍵盤全体の座標を黄色の円で表示
    for (const auto &e: this->key_x) {
        cout << i << "^" << e;
        cv::circle(frame, cv::Point(e, this->key_black_y + 30), 3, cv::Scalar(255, 255, 0), 3, 4);
        i += 1;
    }

    cv::imshow("movie", frame);

    cv::waitKey(0);
}

// 色が変わったかチェックする関数
// 白鍵
bool Analysis::Change_Color_w(int b, int g, int r) {
    int diff = abs(b - def_w_clrB) + abs(g - def_w_clrG) + abs(r - def_w_clrR);

    if (diff > threshold) {
        return true;
    } else {
        return false;
    }
}

// 黒鍵
bool Analysis::Change_Color_b(int b, int g, int r) {
    int diff = abs(b - def_b_clrB) + abs(g - def_b_clrG) + abs(r - def_b_clrR);

    if (diff > threshold) {
        return true;
    } else {
        return false;
    }
}

// キーのイベントをチェックする関数
void Analysis::Check_Key() {
    int x, y, key;

    for (key = 0; key < 88; key++) {
        x = key_x[key];
        if (True_White(key)) {
            // 白鍵
            y = key_white_y;
            if (this->key_event[key] == false) {
                // 白鍵の色が変わったとき
                if (Change_Color_w(Get_Color_b(x, y), Get_Color_g(x, y), Get_Color_r(x, y))) {
                    cout << "[" << key << "W:on]" << endl;
                    cv::circle(frame, cv::Point(x, y), 3, cv::Scalar(0, 0, 200), 3, 4);
                    this->Register_Event(key, 1);
                    key_event[key] = true;
                }
            } else {
                // 白鍵の色が元に戻ったとき
                if (!Change_Color_w(Get_Color_b(x, y), Get_Color_g(x, y), Get_Color_r(x, y))) {
                    cout << "[" << key << "W:off]" << endl;
                    cv::circle(frame, cv::Point(x, y), 3, cv::Scalar(0, 200, 0), 3, 4);
                    this->Register_Event(key, 0);
                    key_event[key] = false;
                }
            }
        } else {
            // 黒鍵
            y = key_black_y;
            if (this->key_event[key] == false) {
                // 黒鍵の色が変わったとき
                if (Change_Color_b(Get_Color_b(x, y), Get_Color_g(x, y), Get_Color_r(x, y))) {
                    cout << "[" << key << "B:on]" << endl;
                    cv::circle(frame, cv::Point(x, y), 3, cv::Scalar(0, 0, 200), 3, 4);
                    this->Register_Event(key, 1);
                    key_event[key] = true;
                }
            } else {
                // 黒鍵の色が元に戻ったとき
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

// 指定された座標の青色情報を取得する関数
int Analysis::Get_Color_b(int x, int y) {
    int b = frame.at<Vec3b>(y, x)[0];
    return b;
}

// 指定された座標の緑色情報を取得する関数
int Analysis::Get_Color_g(int x, int y) {
    int g = frame.at<Vec3b>(y, x)[1];
    return g;
}

// 指定された座標の赤色情報を取得する関数
int Analysis::Get_Color_r(int x, int y) {
    int r = frame.at<Vec3b>(y, x)[2];
    return r;
}

// 与えられたキー番号が白鍵かどうかを判定する関数
bool Analysis::True_White(int n) {
    n += 9;
    int a = n % 12;
    switch (a) {
    case 0: // ド
        return true;
    case 1: // #ド
        return false;
    case 2: // レ
        return true;
    case 3: // #レ
        return false;
    case 4: // ミ
        return true;
    case 5: // ファ
        return true;
    case 6: // #ファ
        return false;
    case 7: // ソ
        return true;
    case 8: // #ソ
        return false;
    case 9: // ラ
        return true;
    case 10: // #ラ
        return false;
    case 11: // シ
        return true;

    default:
        break;
    }
}

// イベントを登録する関数
void Analysis::Register_Event(int key, int event) {
    std::ostringstream key_;
    key_ << key;

    str += "-";
    str += to_string(event);
    str += key_.str();
    this->active_key_sum++;
}

// 解析結果をテキストファイルに書き出す関数
void Analysis::Output_txt() {
    ofstream outputfile("output.txt");
    outputfile << str_;
    outputfile.close();
}

int main() {
    Analysis analysis; // Analysisクラスのインスタンスを生成

    analysis.Check_Coodinates(); // 鍵盤の座標を確認する関数を呼び出す

    analysis.Analyze(); // 映像を解析してイベントを検知し、結果をテキストファイルに書き出す関数を呼び出す
}