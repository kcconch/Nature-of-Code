#include "testApp.h"

void testApp::setup(){
    //画面の基本設定
    ofBackground(0, 0, 0);
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
    grayDiffSmall.allocate(60, 40);
    
    //変数の初期化
    bLearnBakground = true;
    showCvAnalysis = false;
    
    //ベクトル場の初期化
    VF.setupField(60, 40, ofGetWidth(), ofGetHeight());
    
    //パーティクル生成
    particleNum = 8000;
    for (int i = 0; i < particleNum; i++) {
        particle* p = new particle();
        p->setInitialCondition(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),0,0);
        particles.push_back(p);
    }
    
    //GUIの初期設定
    gui.setup();
    gui.config->gridSize.x = 250;
    gui.addContent("grayImage", grayImage);
    gui.addContent("grayDiff", grayDiff);
    gui.addContent("grayDiffSmall", grayDiffSmall);
    gui.addFPSCounter();
    gui.addSlider("threshold", threshold, 0, 400);
    gui.addToggle("use live video", liveVideo);
    gui.addToggle("draw video", drawColorImg);
    gui.addToggle("draw vector field", drawVectorFirld);
    gui.addToggle("force inward", bForceInward);
    gui.loadFromXML();
}

void testApp::update(){
    //新規フレームの取り込みをリセット
    bool bNewFrame = false;
    
    if (liveVideo) {
        vidPlayer.stop();
        //カメラ使用の場合はカメラから新規フレーム取り込み
        vidGrabber.grabFrame();
        //新規にフレームが切り替わったか判定
        bNewFrame = vidGrabber.isFrameNew();
    } else {
        vidPlayer.play();
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
        //縮小した差分イメージに画像をコピー
        grayDiffSmall.scaleIntoMe(grayDiff);
        //ぼかし
        grayDiffSmall.blur(5);
        //ベクトル場に差分イメージを適用
        VF.setFromPixels(grayDiffSmall.getPixels(), bForceInward, 0.05f);
    }
    
    //ベクトル場に発生した力を計算し、パーティクルにかかる力を算出
    for(vector <particle *>::iterator it = particles.begin(); it != particles.end(); ++it){
		(*it)->resetForce();
		ofxVec2f frc;
		frc = VF.getForceFromPos((*it)->pos.x, (*it)->pos.y);
		(*it)->addForce(frc.x, frc.y);
		(*it)->addDampingForce();
		(*it)->update();
        
	}
}

void testApp::draw(){
    //ofEnableAlphaBlending();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    if (drawColorImg) {
        //カラーイメージを描く
        ofSetColor(255, 255, 255, 63);
        colorImg.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    if (drawVectorFirld) {
        //ベクトル場を描く
        ofSetColor(255, 255, 255, 127);
        VF.draw();
    }
    
    //パーティクルを描く
    ofFill();
    ofSetColor(63, 127, 255, 200);
    for(vector <particle *>::iterator it = particles.begin(); it != particles.end(); ++it){
        (*it)->draw();
    }
    
    //GUIを描く
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
            
        case 't':
            //重力を反転
            bForceInward = !bForceInward;
            break;
            
        case ' ':
            //背景画像を新規に取り込む
            bLearnBakground = true;
            //particleをリセット
            for(vector <particle *>::iterator it = particles.begin(); it != particles.end(); ++it){
                (*it)->setInitialCondition(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight()), 0,0);
            }
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