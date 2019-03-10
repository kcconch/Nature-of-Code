#include "testApp.h"

void testApp::setup(){
    //画面の基本設定
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    
    //カメラから映像を取り込んで表示
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(320,240);
    
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
    
    //GUIの初期設定
    gui.setup();
    gui.config->gridSize.x = 250;
    gui.addContent("grayImage", grayImage);
    gui.addContent("grayDiff", grayDiff);
    gui.addContent("grayDiffSmall", grayDiffSmall);
    gui.addFPSCounter();
    gui.addSlider("threshold", threshold, 0, 400);
    gui.addSlider("gravity", gravity, 0.0, 1.0);
    gui.addSlider("force", force, 0.0, 40.0);
    gui.addSlider("vector threshold", vectorThreshold, 0.0, 2.0);
    gui.addToggle("use live video", liveVideo);
    gui.addToggle("draw video", drawColorImg);
    gui.addToggle("draw vector field", drawVectorFirld);
    gui.addToggle("force inward", bForceInward);
    gui.loadFromXML();
    
    //Box2D初期設定
    box2d.init();
    box2d.setGravity(0, gravity);
    box2d.createBounds();
    box2d.setFPS(8);
    
    //パーティクル生成
    particleNum = 500;
    for (int i = 0; i < particleNum; i++) {
        CustomCircle* p = new CustomCircle();
        p->setPhysics(1.0, 0.0, 0.2);
        p->setup(box2d.getWorld(), 
                 ofRandom(0, ofGetWidth()), 
                 ofRandom(0, ofGetHeight()), 
                 ofRandom(7, 14), false);
        p->disableCollistion();
        particles.push_back(p);
    }
}


void testApp::update(){
    box2d.setGravity(0, gravity);
    box2d.update();
    
    //新規フレームの取り込みをリセット
    bool bNewFrame = false;
    //カメラから新規フレーム取り込み
    vidGrabber.grabFrame();
    //新規にフレームが切り替わったか判定
    bNewFrame = vidGrabber.isFrameNew();
    
    //フレームが切り替わった際のみ画像を解析
    if (bNewFrame){
        //取り込んだフレームを画像としてキャプチャ
        colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
        //左右反転
        colorImg.mirror(false, true);
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
        
        //ベクトル場に発生した力を計算し、パーティクルにかかる力を算出
        for(list <CustomCircle *>::iterator it = particles.begin(); it != particles.end(); ++it){
            ofxVec2f frc;
            frc = VF.getForceFromPos((*it)->getPosition().x, (*it)->getPosition().y);
            //設定した閾値を越えたら、VFの力を加える
            if (frc.length() > vectorThreshold) {
                (*it)->addForce(ofPoint(frc.x * force, frc.y * force), 1.0);
            }
            (*it)->update();        
        }
    }
}

void testApp::draw(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    if (drawColorImg) {
        //カラーイメージを描く
        ofSetColor(255, 255, 255, 127);
        colorImg.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    if (drawVectorFirld) {
        //ベクトル場を描く
        ofSetColor(255, 255, 255, 127);
        VF.draw();
    }
    
    //パーティクルを描く
    for(list <CustomCircle *>::iterator it = particles.begin(); it != particles.end(); ++it){
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