#ifndef ANALYSIS_HPP

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Movie {
private:
    VideoCapture cap; // 動画を読み込むためのバッファ
    Mat img; // 画像用のバッファ、動画から１フレームずつ取り出す
    int max_frame; // 動画の総フレーム数
    int cur_frame_num = 0; // 現在読み込んでいるフレーム

public:
    Movie() {
        // 動画の読み込み
        this->cap.open("C:\\opencv\\video.mp4");

        // 動画の総フレーム数を取得
        this->max_frame = cap.get(CAP_PROP_FRAME_COUNT);

        // オフセット分のフレームを飛ばす
        for (int i = 0; i < this->Get_FPS() * 3; i++) {
            if (cur_frame_num < max_frame) cap >> img;
        }
    }

    ~Movie() {}

    // 次のフレームを取得する関数
    Mat Get_Next_Frame() {
        if (cur_frame_num < max_frame) cap >> img;
        return this->img;
    }

    // 動画のフレームレートを取得する関数
    inline int Get_FPS() {
        return cap.get(CAP_PROP_FPS);
    }

    // 動画の総フレーム数を取得する関数
    inline int Get_Max_Frame() {
        return this->max_frame;
    }

    // 現在のフレーム番号を取得する関数
    inline int Cur_Frame_Num() {
        return this->cur_frame_num;
    }
};

class Analysis {
private:
    const static int threshold = 50;// 色がどれくらい変わったら変化したと認識するかの閾値

    std::ostringstream pos_msec;// 時間情報を保持するための文字列ストリーム

    // 座標の保持
    // 白鍵
    int key_white_y; // 縦軸
    int key_white_x[52]; // 横軸
    // 黒鍵
    int key_black_y; // 縦軸
    int key_black_x[36]; // 横軸
    // 88鍵盤全体
    int key_x[88];

    int active_key_sum = 0; // 画面変更の時の検知対策(変化量が多すぎると、書き出ししないフラグみたいな使い方)

    int first_key = 0; // 初めてのキーイベントのフラグ

    Movie movie; // 動画の処理を行うインスタンス

    cv::Mat frame; // フレームを保持するMatオブジェクト

    // 各鍵盤が押されてない初期状態のRGB値を保持する変数
    // 白鍵
    int def_w_clrB;
    int def_w_clrG;
    int def_w_clrR;
    // 黒鍵
    int def_b_clrB;
    int def_b_clrG;
    int def_b_clrR;

    // 各鍵盤の座標のイベント状態（押されたかどうか）を保持する配列
    bool key_w_event[52]; // 白鍵
    bool key_b_event[36]; // 黒鍵
    bool key_event[88]; // 88鍵盤全体のイベント状態を保持する配列
    bool key_active[88]; // 88鍵盤全体のアクティブ状態（押されている）かどうかを保持する配列

    string str = ""; // 1フレーム分を保持する文字列
    string str_ = ""; // その1フレーム分に問題がなければ、書き込み

public:
    Analysis(); // コンストラクタ
    void Set_Coodinates(); // 鍵盤の座標を設定する関数
    void Set_Color(); // 鍵盤が押されていない初期状態のRGBを記録する関数
    void Analyze(); // 映像の解析を行い、txtファイルを生成する関数
    void Check_Coodinates(); // 画面の座標をチェックする関数

    // 各鍵盤の色が変わったかチェックする関数(色が変わっていたらtrueを返す)
    bool Change_Color_w(int b, int g, int r);// 白鍵
    bool Change_Color_b(int b, int g, int r);// 黒鍵

    void Check_Key(); // 各鍵盤の状態をチェックする関数

    // 指定された座標の情報を取得する関数
    int Get_Color_b(int x, int y); // 青色
    int Get_Color_g(int x, int y); // 緑色
    int Get_Color_r(int x, int y); // 赤色

    bool True_White(int n); // 鍵盤が白鍵かどうかを判定する関数

    void Register_Event(int key, int event); // 各鍵盤のイベント（押されたかどうか）を登録する関数

    void Output_txt(); // 解析結果をテキストファイルにイベント情報を出力する関数
};

#endif ANALYSIS_HPP
