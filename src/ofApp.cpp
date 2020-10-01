#include "ofApp.h"
#include "CbusGuiTheme.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
	//ofSetVerticalSync(false);

	margin_top = 30;
	margin_left = 30;

	kinect.setRegistration(true);
	kinect.init();
	kinect.open();
	kinect.setCameraTiltAngle(0);

	calibrating = 1;
	calibrationStep = 1;
	shift = ofPoint(354, margin_top);

	valPixels.allocate(OUT_WIDTH, OUT_HEIGHT, OF_IMAGE_GRAYSCALE);
	valCalibPixels.allocate(OUT_WIDTH, OUT_HEIGHT, OF_IMAGE_GRAYSCALE);
	
	oscSender.setup(DEF_HOST, DEF_SEND_PORT);
	
	calib_configs.setName("Calibration");
	unity_configs.setName("Unity");
	
	guiSetup();
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate(), 0));
	kinect.update();
	if (calibrating) {
		if (calibrationStep == 1) {
			if (corners.size() == 4) {
				nextCalibBtn->setTheme(new CbusBlueButton());
				nextCalibBtn->setEnabled(true);
			}
			else {
				nextCalibBtn->setTheme(new CbusBlueButtonDisabled());
				nextCalibBtn->setEnabled(false);
			}

			if (corners.size() == 0) {
				resetCornerBtn->setTheme(new CbusRedButtonDisabled());
				resetCornerBtn->setWidth(100);
				resetCornerBtn->setEnabled(false);
			}
			else {
				resetCornerBtn->setTheme(new CbusRedButton());
				resetCornerBtn->setWidth(100);
				resetCornerBtn->setEnabled(true);
			}

			boxMapColor->update();
			resetCornerBtn->update();
			nextCalibBtn->update();
		}
		else if (calibrationStep == 2) {

			ofShortPixels &inputCal = kinect.getRawDepthPixels();

			int inpWidth = inputCal.getWidth();
			int inpHeight = inputCal.getHeight();

			for (int Y = 0; Y < OUT_HEIGHT; Y++) {
				for (int X = 0; X < OUT_WIDTH; X++) {
					int x = int(warpPoints[X][Y].x);
					int y = int(warpPoints[X][Y].y);

					if (x > 0 && x < inpWidth && y > 0 && y < inpHeight) {
						int idx = x + inpWidth * y;
						int inpZ = inputCal.getPixels()[idx];
						int backgroundZ = backgroundDepth.getPixels()[idx];

						int delta = -1;
						if (inpZ != 0 && backgroundZ != 0) {
							delta = backgroundZ - inpZ;
						}

						if (minDistance <= delta && delta <= maxDistance) {
							valCalibPixels.getPixels()[X + OUT_WIDTH * Y] = 255;
						}
						else {
							valCalibPixels.getPixels()[X + OUT_WIDTH * Y] = 0;
						}
					}
				}
			}
			valCalibImage.setFromPixels(valCalibPixels);

			contour.setMinAreaRadius(minBlobArea * 10);
			contour.setMaxAreaRadius((OUT_WIDTH * OUT_HEIGHT) / (2000 / maxBlobArea));//102400
			contour.findContours(valCalibImage);
			contour.setFindHoles(false);

			prevCalibBtn->update();
			playBtn->update();
			oscInputAddress->update();
			saveConfigBtn->update();
			loadConfigBtn->update();
		}
		//if (calibrationStep < 2) nextCalibBtn->update();
	}
	else {
		saveUnityBtn->update();
		loadUnityBtn->update();
		recalibrateBtn->update();
		exitUnityBtn->update();

		ofShortPixels &input = kinect.getRawDepthPixels();

		int inpWidth = input.getWidth();
		int inpHeight = input.getHeight();

		for (int Y = 0; Y < OUT_HEIGHT; Y++) {
			for (int X = 0; X < OUT_WIDTH; X++) {
				int x = int(warpPoints[X][Y].x);
				int y = int(warpPoints[X][Y].y);

				if (x > 0 && x < inpWidth && y > 0 && y < inpHeight) {
					int idx = x + inpWidth * y;
					int inpZ = input.getPixels()[idx];
					int backgroundZ = backgroundDepth.getPixels()[idx];

					int delta = -1;
					if (inpZ != 0 && backgroundZ != 0) {
						delta = backgroundZ - inpZ;
					}

					if (minDistance <= delta && delta <= maxDistance) {
						valPixels.getPixels()[X + OUT_WIDTH * Y] = 255;
					}
					else {
						valPixels.getPixels()[X + OUT_WIDTH * Y] = 0;
					}
				}
			}
		}
		valImage.setFromPixels(valPixels);

		contour.setMinAreaRadius(minBlobArea * 10);
		contour.setMaxAreaRadius((OUT_WIDTH * OUT_HEIGHT) / (2000 / maxBlobArea));//102400
		contour.findContours(valImage);
		contour.setFindHoles(false);

		//if (detectPoints.size() > 0) detectPoints.clear();
		int numBlobs = contour.size();
		for (int i = 0; i < numBlobs; i++) {
			ofVec2f centroid = toOf(contour.getCentroid(i));
			sendContourCentroid(centroid);

			/*float circleRadius;
			ofVec2f circleCenter = toOf(contour.getMinEnclosingCircle(i, circleRadius));
			sendContourCircle(circleCenter, circleRadius);*/
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(35, 39, 43);
	if (calibrating) {
		if (calibrationStep == 1) {
			boxMapColor->draw();
			resetCornerBtn->draw();
			nextCalibBtn->draw();
			ofSetColor(255);
			kinect.draw(shift.x, shift.y, OUT_WIDTH, OUT_HEIGHT);

			ofSetColor(boxMapColor->getColor());
			ofFill();
			for (int i = 0; i < corners.size(); i++) {
				ofDrawCircle(corners[i] + shift, 10);
			}
			if (corners.size() == 4) {
				ofDrawLine(corners[0] + shift, corners[1] + shift);
				ofDrawLine(corners[1] + shift, corners[2] + shift);
				ofDrawLine(corners[2] + shift, corners[3] + shift);
				ofDrawLine(corners[3] + shift, corners[0] + shift);
			}
		}
		else if (calibrationStep == 2) {
			//ofSetColor(0);
			//ofDrawRectangle(shift.x, shift.y, OUT_WIDTH, OUT_HEIGHT);
			ofSetColor(255);
			valCalibImage.draw(shift.x, shift.y);

			//ofFill();
			int numBlobs = contour.size();
			//cout << "BLOB: " << ofToString(numBlobs, 0) << endl;
			for (int i = 0; i < numBlobs; i++) {
				//cout << "BLOB: " << ofToString(numBlobs, 0) << endl;
				//detectPoints.push_back(centroid);

				/*ofVec2f centroid = toOf(contour.getCentroid(i));
				ofVec2f average = toOf(contour.getAverage(i));
				ofVec2f center = toOf(contour.getCenter(i));
				ofSetColor(cyanPrint);
				ofDrawCircle(centroid + shift, 10);
				ofSetColor(magentaPrint);
				ofDrawCircle(average + shift, 10);
				ofSetColor(yellowPrint);
				ofDrawCircle(center + shift, 10);*/
				ofSetColor(ofColor::red);
				ofNoFill();
				float circleRadius;
				ofPoint circleCenter = toOf(contour.getMinEnclosingCircle(i, circleRadius));
				ofDrawCircle(circleCenter + shift, circleRadius/2);
			}
			prevCalibBtn->draw();
			playBtn->draw();
			oscInputAddress->draw();
			saveConfigBtn->draw();
			loadConfigBtn->draw();
		}
		//if (calibrationStep < 2) nextCalibBtn->draw();
	}
	else {
		saveUnityBtn->draw();
		loadUnityBtn->draw();
		recalibrateBtn->draw();
		exitUnityBtn->draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		/*case ' ':
			calibrating = 0;
			calibrationStep = 1;
			guiPlay->setVisible(true);
			guiCalib2->setVisible(false);
			break;*/
		case '0':
			oscMessage.setAddress("/AppController");
			oscMessage.addIntArg(0);
			oscSender.sendMessage(oscMessage);
			oscMessage.clear();
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (calibrating && corners.size() < 4 && shift.x <= x && x <= (shift.x + OUT_WIDTH) && shift.y <= y && y <= (shift.y + OUT_HEIGHT)) {
		corners.push_back(ofPoint(x, y) - shift);
	}
}

void ofApp::exit() {
	kinect.setCameraTiltAngle(0);
	kinect.close();
}

void ofApp::sendContourCentroid(ofVec2f centroid)
{
	oscMessage.setAddress("/KinectXZ");
	oscMessage.addFloatArg(centroid.x);
	oscMessage.addFloatArg(centroid.y);
	oscMessage.addIntArg(OUT_WIDTH);
	oscMessage.addIntArg(OUT_HEIGHT);

	oscSender.sendMessage(oscMessage);
	oscMessage.clear();
}

void ofApp::sendContourCircle(ofVec2f center, float radius)
{
	oscMessage.setAddress("/KinectXZ");
	oscMessage.addFloatArg(center.x);
	oscMessage.addFloatArg(center.y);
	oscMessage.addIntArg(OUT_WIDTH);
	oscMessage.addIntArg(OUT_HEIGHT);
	cout << "radius: " << ofToString(radius, 0) << endl;
	cout << "minBlobArea: " << ofToString(minBlobArea, 0) << endl;
	float scale = ofMap(radius, minBlobArea * 10, (OUT_WIDTH * OUT_HEIGHT) / 200, 0.05, 1);
	cout << "scale: " << ofToString(scale) << endl;
	oscMessage.addIntArg(scale);

	oscSender.sendMessage(oscMessage);
	oscMessage.clear();
}

void ofApp::onResetCornerClicked(ofxDatGuiButtonEvent e) {
	//cout << corners.size() << " corner" << endl;
	if (corners.size() > 0) corners.clear();
	//cout << corners.size() << " corner" << endl;

}

void ofApp::onMaxDistanceChanged(ofxDatGuiSliderEvent e) {
	//cout << e.target->getLabel() << " was changed!" << endl;
}

void ofApp::onPrevCalibClicked(ofxDatGuiButtonEvent e)
{
	if (calibrationStep == 2) {
		guiCalib1->setVisible(true);
		guiCalib2->setVisible(false);
	}
	calibrationStep -= 1;
}

void ofApp::onNextCalibClicked(ofxDatGuiButtonEvent e)
{
	if (calibrationStep == 1) {
		guiCalib1->setVisible(false);
		guiCalib2->setVisible(true);
		setBackgroundDepth();
	}
	calibrationStep += 1;
}

void ofApp::onPlayClicked(ofxDatGuiButtonEvent e)
{
	calibrating = 0;
	calibrationStep = 1;
	if(oscInputAddress->getText() != "") oscSender.setup(oscInputAddress->getText(), DEF_SEND_PORT);
	guiPlay->setVisible(true);
	guiCalib2->setVisible(false);
}

void ofApp::onRecalibrateClicked(ofxDatGuiButtonEvent e)
{
	calibrating = 1;
	calibrationStep = 2;
	guiPlay->setVisible(false);
	guiCalib2->setVisible(true);
}

void ofApp::onBoxMapColorPicked(ofxDatGuiColorPickerEvent e)
{
	//cout << e.target->getLabel() << " was changed!" << endl;
}

void ofApp::onWaveScaleChanged(ofxDatGuiSliderEvent e)
{
	//cout << "Wave Scale: " << ofToString(waveScale, 0) << endl;
	oscMessage.setAddress("/RippleWaveScale");
	oscMessage.addFloatArg(waveScale);
	oscSender.sendMessage(oscMessage);
	oscMessage.clear();
}

void ofApp::onWaveStrengthChanged(ofxDatGuiSliderEvent e)
{
	//cout << "Wave Strength: " << ofToString(waveStrength, 0) << endl;
	oscMessage.setAddress("/RippleWaveStrength");
	oscMessage.addFloatArg(waveStrength);
	oscSender.sendMessage(oscMessage);
	oscMessage.clear();
}

void ofApp::onFishWaveScaleChanged(ofxDatGuiSliderEvent e)
{
	oscMessage.setAddress("/RippleFishWaveScale");
	oscMessage.addFloatArg(fishWaveScale);
	oscSender.sendMessage(oscMessage);
	oscMessage.clear();
}

void ofApp::onFishWaveStrengthChanged(ofxDatGuiSliderEvent e)
{
	oscMessage.setAddress("/RippleFishWaveStrength");
	oscMessage.addFloatArg(fishWaveStrength);
	oscSender.sendMessage(oscMessage);
	oscMessage.clear();
}

void ofApp::setBackgroundDepth() {
	backgroundDepth = kinect.getRawDepthPixels();
	for (int Y = 0; Y < OUT_HEIGHT; Y++) {
		for (int X = 0; X < OUT_WIDTH; X++) {
			float a = float(X) / OUT_WIDTH;
			float b = float(Y) / OUT_HEIGHT;
			float c = a * b;
			warpPoints[X][Y] =
				(1 - b - a + c) * corners[0]
				+ (a - c) * corners[1]
				+ c * corners[2]
				+ (b - c) * corners[3];
		}
	}
}

void ofApp::onSaveCalibrationSettingsClicked(ofxDatGuiButtonEvent e)
{
	ofSerialize(settings, calib_configs);
	settings.save("calib_configs.xml");
	settings.clear();
}

void ofApp::onSaveUnitySettingsClicked(ofxDatGuiButtonEvent e)
{
	ofSerialize(settings, unity_configs);
	settings.save("unity_configs.xml");
	settings.clear();
}

void ofApp::onLoadCalibrationSettingsClicked(ofxDatGuiButtonEvent e)
{
	settings.load("calib_configs.xml");
	ofDeserialize(settings, calib_configs);
	settings.clear();
}

void ofApp::onLoadUnitySettingsClicked(ofxDatGuiButtonEvent e)
{
	settings.load("unity_configs.xml");
	ofDeserialize(settings, unity_configs);
	settings.clear();
}

void ofApp::onExitUnityClicked(ofxDatGuiButtonEvent e)
{
	oscMessage.setAddress("/AppController");
	oscMessage.addIntArg(0);
	oscSender.sendMessage(oscMessage);
	oscMessage.clear();
}

void ofApp::guiSetup() {

	guiWidth = 300;
	guiCalib1 = new ofxDatGui(margin_left, margin_top);
	guiCalib1->setTheme(new CbusGuiTheme());
	guiCalib1->setWidth(guiWidth);
	guiCalib1->addHeader("::  CALIBRATION 1 ::", false);

	boxMapColor = new ofxDatGuiColorPicker("Box Color", ofColor::fromHex(0xFF0000));
	boxMapColor->setTheme(new CbusGuiTheme());
	boxMapColor->setPosition(margin_left, margin_top + guiCalib1->getHeight());
	boxMapColor->setWidth(guiWidth, 100);
	boxMapColor->onColorPickerEvent(this, &ofApp::onBoxMapColorPicked);
	boxMapColor->setStripeVisible(false);

	resetCornerBtn = new ofxDatGuiButton("Reset Dot");
	resetCornerBtn->setTheme(new CbusRedButtonDisabled());
	resetCornerBtn->onButtonEvent(this, &ofApp::onResetCornerClicked);
	resetCornerBtn->setPosition(margin_left, margin_top + boxMapColor->getY() + boxMapColor->getHeight() + 10);


	minDistance.set("Min Distance(mm)", 30, 0, 100);
	maxDistance.set("Max Distance(mm)", 150, 0, 2000);
	minBlobArea.set("Min Blob Area", 5, 1, 10);
	maxBlobArea.set("Max Blob Area", 4, 1, 10);
	
	calib_configs.add(minDistance);
	calib_configs.add(maxDistance);
	calib_configs.add(minBlobArea);
	calib_configs.add(maxBlobArea);

	guiCalib2 = new ofxDatGui(margin_left, margin_top);
	guiCalib2->setTheme(new CbusGuiTheme());
	guiCalib2->addHeader("::  CALIBRATION 2 ::", false);
	guiCalib2->addSlider(minDistance);
	guiCalib2->addSlider(maxDistance);//->onSliderEvent(this, &ofApp::onMaxDistanceEvent);
	guiCalib2->addSlider(minBlobArea);
	guiCalib2->addSlider(maxBlobArea);
	guiCalib2->setWidth(guiWidth, 140);
	guiCalib2->setVisible(false);
	guiCalib2->addBreak()->setHeight(10.0f);

	oscInputAddress = new ofxDatGuiTextInput("OSC Receiver IP: ", oscSender.getHost());
	oscInputAddress->setTheme(new CbusGuiTheme());
	oscInputAddress->setWidth(guiWidth, 140);
	oscInputAddress->setPosition(margin_left, guiCalib2->getHeight() + margin_top);

	loadConfigBtn = new ofxDatGuiButton("Load");
	loadConfigBtn->setTheme(new CbusYellowButton());
	loadConfigBtn->setWidth(80);
	loadConfigBtn->setPosition(margin_left, oscInputAddress->getY() + oscInputAddress->getHeight() + 20);
	loadConfigBtn->onButtonEvent(this, &ofApp::onLoadCalibrationSettingsClicked);

	saveConfigBtn = new ofxDatGuiButton("Save");
	saveConfigBtn->setTheme(new CbusSkyblueButton());
	saveConfigBtn->setWidth(80);
	saveConfigBtn->setPosition(margin_left + loadConfigBtn->getWidth() + 10, loadConfigBtn->getY());
	saveConfigBtn->onButtonEvent(this, &ofApp::onSaveCalibrationSettingsClicked);

	//cout << "gui width: " << ofToString(gui->getWidth(), 0) << endl;

	prevCalibBtn = new ofxDatGuiButton("Prev");
	prevCalibBtn->setTheme(new CbusBlueButton());
	prevCalibBtn->onButtonEvent(this, &ofApp::onPrevCalibClicked);
	prevCalibBtn->setPosition(margin_left, ofGetHeight() - margin_top - 25);

	nextCalibBtn = new ofxDatGuiButton("Next");
	nextCalibBtn->setTheme(new CbusBlueButtonDisabled());
	nextCalibBtn->onButtonEvent(this, &ofApp::onNextCalibClicked);
	nextCalibBtn->setPosition(margin_left + prevCalibBtn->getWidth() + 20, ofGetHeight() - margin_top - 25);
	//nextCalibBtn->setWidth(140);

	playBtn = new ofxDatGuiButton("Play");
	playBtn->setTheme(new CbusGreenButton());
	playBtn->onButtonEvent(this, &ofApp::onPlayClicked);
	playBtn->setPosition(margin_left + prevCalibBtn->getWidth() + 20, ofGetHeight() - margin_top - 25);
	playBtn->setWidth(140);
	playBtn->setStripeVisible(false);

	waveScale.set("Wave Scale", 0.05f, 0.0, 1.0);
	waveStrength.set("Wave Strength", 0.1f, 0.0, 1.0);
	fishWaveScale.set("Fish Wave Scale", 0.03f, 0.0, 1.0);
	fishWaveStrength.set("Fish Wave Strength", 0.09f, 0.0, 1.0);
	unity_configs.add(waveScale);
	unity_configs.add(waveStrength);
	unity_configs.add(fishWaveScale);
	unity_configs.add(fishWaveStrength);

	guiPlay = new ofxDatGui(margin_left, margin_top);
	guiPlay->setTheme(new CbusGuiTheme());
	guiPlay->addHeader("::   LIVE GAME   ::", false);
	guiPlay->addLabel("::   WATER");
	guiPlay->addSlider(waveScale)->onSliderEvent(this, &ofApp::onWaveScaleChanged);
	guiPlay->addSlider(waveStrength)->onSliderEvent(this, &ofApp::onWaveStrengthChanged);
	guiPlay->addBreak()->setHeight(10.0f);
	guiPlay->addLabel("::   FISH");
	guiPlay->addSlider(fishWaveScale)->onSliderEvent(this, &ofApp::onFishWaveScaleChanged);
	guiPlay->addSlider(fishWaveStrength)->onSliderEvent(this, &ofApp::onFishWaveStrengthChanged);
	guiPlay->setWidth(guiWidth, 140);
	guiPlay->setVisible(false);
	guiPlay->addBreak()->setHeight(10.0f);

	loadUnityBtn = new ofxDatGuiButton("Load");
	loadUnityBtn->setTheme(new CbusYellowButton());
	loadUnityBtn->setWidth(80);
	loadUnityBtn->setPosition(margin_left, margin_top + guiPlay->getHeight() + 20);
	loadUnityBtn->onButtonEvent(this, &ofApp::onLoadUnitySettingsClicked);

	saveUnityBtn = new ofxDatGuiButton("Save");
	saveUnityBtn->setTheme(new CbusSkyblueButton());
	saveUnityBtn->setWidth(80);
	saveUnityBtn->setPosition(margin_left + loadUnityBtn->getWidth() + 10, loadUnityBtn->getY());
	saveUnityBtn->onButtonEvent(this, &ofApp::onSaveUnitySettingsClicked);

	recalibrateBtn = new ofxDatGuiButton("Re-Calibrate");
	recalibrateBtn->setTheme(new CbusRedButton());
	recalibrateBtn->onButtonEvent(this, &ofApp::onRecalibrateClicked);
	recalibrateBtn->setPosition(margin_left, ofGetHeight() - margin_top - 25);

	exitUnityBtn = new ofxDatGuiButton("Close Unity");
	exitUnityBtn->setTheme(new CbusRedButton());
	exitUnityBtn->onButtonEvent(this, &ofApp::onExitUnityClicked);
	exitUnityBtn->setPosition(ofGetWidth() - (exitUnityBtn->getWidth() + margin_left), recalibrateBtn->getY());

	settings.load("calib_configs.xml");
	ofDeserialize(settings, calib_configs);
	settings.clear();
	settings.load("unity_configs.xml");
	ofDeserialize(settings, unity_configs);
	settings.clear();
}