#include "testApp.h"

void testApp::setup(){
    //画面の基本設定
    ofBackground(0, 0, 0);
    ofEnableAlphaBlending();
    ofSetFrameRate(60);

    //カメラから映像を取り込んで表示
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(320,240);

    vidPlayer.loadMovie("fingers.mov");
    vidPlayer.play();
    
    //使用する画像の領域を確保
    colorImg.allocate(320,240);
    grayImage.allocate(320,240);
    grayBg.allocate(320,240);
    grayDiff.allocate(320,240);
    
    //変数の初期化
    bLearnBakground = false;
    showCvAnalysis = false;

    //GUIを設定
    gui.setup();
    gui.config->gridSize.x = 250;
    gui.addContent("grayImage", grayImage);
    gui.addContent("grayDiff", grayDiff);
    gui.addContent("findConter", contourFinder);
    gui.addFPSCounter();
    gui.addSlider("threshold", threshold, 0, 400);
    gui.addToggle("use live video", liveVideo);
	gui.addToggle("findHoles", findHoles);
	gui.addToggle("useApproximation", useApproximation);
	gui.addSlider("minBlobSize", minBlobSize, 0, 1);
	gui.addSlider("maxBlobSize", maxBlobSize, 0, 1);
	gui.addSlider("maxNumBlobs", maxNumBlobs, 1, 100);
    gui.loadFromXML();
}


void testApp::update(){
    //新規フレームの取り込みをリセット
    bool bNewFrame = false;
    
    if (liveVideo) {
        //カメラ使用の場合はカメラから新規フレーム取り込み
        vidGrabber.grabFrame();
        //新規にフレームが切り替わったか判定
        bNewFrame = vidGrabber.isFrameNew();
    } else {
        vidPlayer.idleMovie();
        //新規にフレームが切り替わったか判定
        bNewFrame = vidPlayer.isFrameNew();
    }
    
    //フレームが切り替わった際のみ画像を解析
    if (bNewFrame){
        if (liveVideo) {
            //取り込んだフレームを画像としてキャプチャ
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
            //左右反転
            colorImg.mirror(false, true);
        } else {
            //取り込んだフレームを画像としてキャプチャ
            colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);    
        }

        //カラーのイメージをグレースケールに変換
        grayImage = colorImg;
        
        //まだ背景画像が記録されていなければ、現在のフレームを背景画像とする
        if (bLearnBakground == true){
            grayBg = grayImage;
            bLearnBakground = false;
        }
        
        //グレースケールのイメージと取り込んだ背景画像との差分を算出
        grayDiff.absDiff(grayBg, grayImage);
        //画像を2値化(白と黒だけに)する
        grayDiff.threshold(threshold);
        //2値化した画像から輪郭を抽出する
        contourFinder.findContours(grayDiff,
                                   minBlobSize * minBlobSize * grayDiff.getWidth() * grayDiff.getHeight(),
                                   maxBlobSize * maxBlobSize * grayDiff.getWidth() * grayDiff.getHeight(),
                                   maxNumBlobs, findHoles, useApproximation);
        
        //検出した解析結果からBlobの中心位置を求め
        //中心位置にパーティクルを追加
        for (int i = 0; i < contourFinder.nBlobs; i++){
            circles.push_back(new MyCircle(ofPoint(contourFinder.blobs[i].centroid.x, 
                                                   contourFinder.blobs[i].centroid.y), 
                                           ofRandom(0.5, 4.0), 0.4, 0.1, 10));
        }
    }
    
    //パーティクル更新
    for(vector <MyCircle *>::iterator it = circles.begin(); it != circles.end();){
        (*it)->update();
        if ((*it)->dead) {
            delete (*it);
            it = circles.erase(it);
        } else {
            ++it;
        }
    }
}

void testApp::draw(){
    ofFill();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    ofPushMatrix();
    //画面サイズいっぱいに表示されるようリスケール
    glScalef((float)ofGetWidth() / (float)grayDiff.width, 
             (float)ofGetHeight() / (float)grayDiff.height, 
             1.0f);
    //ビデオ映像表示
    ofSetColor(255, 255, 255, 127);
    colorImg.draw(0, 0);
    
    //パーティクルを表示
    for(vector <MyCircle *>::iterator it = circles.begin(); it != circles.end(); ++it){
        (*it)->draw();
    }
    ofPopMatrix();
    //GUIを表示
    gui.draw();
}

void testApp::keyPressed  (int key){
    //キー入力でモード切り替え
    switch (key){
        case 'f':
            //フルスクリーンのon/off
            ofToggleFullscreen();
            
        case 'g':
            //Gui表示のon/off
            gui.toggleDraw();
            break;
            
        case ' ':
            //背景画像を新規に取り込む
            bLearnBakground = true;
            break;
    }
}

void testApp::keyReleased(int key){
}

void testApp::mouseMoved(int x, int y){
}

void testApp::mouseDragged(int x, int y, int button){
}

void testApp::mousePressed(int x, int y, int button){
}

void testApp::mouseReleased(int x, int y, int button){
}

void testApp::windowResized(int w, int h){
}