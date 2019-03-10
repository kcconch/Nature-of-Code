#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxSimpleGuiToo.h"

//あらかじめ録画したビデオを使用する場合には、ここをコメントアウト
//#define _USE_LIVE_VIDEO

class testApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    //ライブカメラを使用する際には、カメラ入力を準備
    ofVideoGrabber 		vidGrabber;
    ofVideoPlayer 		vidPlayer;

    ofxCvColorImage colorImg; //オリジナルのカラー映像
    ofxCvGrayscaleImage grayImage; //グレースケールに変換後
    ofxCvGrayscaleImage grayBg; //キャプチャーした背景画像
    ofxCvGrayscaleImage grayDiff; //現在の画像と背景との差分
    ofxCvContourFinder contourFinder; //輪郭抽出のためのクラス
    
    bool bLearnBakground; //背景画像を登録したか否か
    bool showCvAnalysis; //解析結果を表示するか
    int threshold; //2値化の際の閾値
    bool liveVideo; //カメラ入力を使用するか否か

    //輪郭検出時のパラメータ
    float minBlobSize; //物体の最小サイズ
    float maxBlobSize; //物体の最大サイズ
    int maxNumBlobs; //認識する物体の最大数
    bool findHoles; //穴を検出するか
    bool useApproximation; //近似値を使用するか
    
    //GUI
    ofxSimpleGuiToo gui; 
};

#endif