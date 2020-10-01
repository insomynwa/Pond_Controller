#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxOsc.h"
#include "ofxDatGui.h"

#define DEF_SEND_PORT 12345
#define DEF_HOST "127.0.0.1"
#define OUT_WIDTH 640
#define OUT_HEIGHT 480

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		
		ofxKinect kinect;

		bool calibrating;
		int calibrationStep;
		ofPoint shift;
		vector<ofPoint> corners;

		ofShortPixels backgroundDepth;
		ofPoint warpPoints[OUT_WIDTH][OUT_HEIGHT];

		ofPixels valPixels,valCalibPixels;
		ofImage valImage,valCalibImage;

		ofxCv::ContourFinder contour;

		ofxOscSender oscSender;
		ofxOscMessage oscMessage;

		int margin_left, margin_top, guiWidth;
		ofParameterGroup calib_configs, unity_configs;
		ofParameter<int> minDistance;
		ofParameter<int> maxDistance;
		ofParameter<int> minBlobArea;
		ofParameter<int> maxBlobArea;
		ofParameter<float> waveScale;
		ofParameter<float> waveStrength;
		ofParameter<float> fishWaveScale;
		ofParameter<float> fishWaveStrength;

		ofXml settings;

		ofxDatGui* guiCalib1;
		ofxDatGui* guiCalib2;
		ofxDatGui* guiPlay;

		ofxDatGuiColorPicker* boxMapColor;
		ofxDatGuiColorPicker* lineColor;
		ofxDatGuiButton* resetCornerBtn;
		ofxDatGuiButton* saveConfigBtn;
		ofxDatGuiButton* loadConfigBtn;
		ofxDatGuiButton* saveUnityBtn;
		ofxDatGuiButton* loadUnityBtn;
		ofxDatGuiButton* prevCalibBtn;
		ofxDatGuiButton* nextCalibBtn;
		ofxDatGuiButton* playBtn;
		ofxDatGuiButton* recalibrateBtn;
		ofxDatGuiButton* exitUnityBtn;
		ofxDatGuiTextInput* oscInputAddress;

		void guiSetup();
		void setBackgroundDepth();
		void sendContourCentroid(ofVec2f centroid);
		void sendContourCircle(ofVec2f center, float radius);

		// EVENT
		void onResetCornerClicked(ofxDatGuiButtonEvent e);
		void onPrevCalibClicked(ofxDatGuiButtonEvent e);
		void onNextCalibClicked(ofxDatGuiButtonEvent e);
		void onPlayClicked(ofxDatGuiButtonEvent e);
		void onRecalibrateClicked(ofxDatGuiButtonEvent e);
		void onSaveCalibrationSettingsClicked(ofxDatGuiButtonEvent e);
		void onSaveUnitySettingsClicked(ofxDatGuiButtonEvent e);
		void onLoadCalibrationSettingsClicked(ofxDatGuiButtonEvent e);
		void onLoadUnitySettingsClicked(ofxDatGuiButtonEvent e);
		void onExitUnityClicked(ofxDatGuiButtonEvent e);
		void onMaxDistanceChanged(ofxDatGuiSliderEvent e);
		void onWaveScaleChanged(ofxDatGuiSliderEvent e);
		void onWaveStrengthChanged(ofxDatGuiSliderEvent e);
		void onFishWaveScaleChanged(ofxDatGuiSliderEvent e);
		void onFishWaveStrengthChanged(ofxDatGuiSliderEvent e);
		void onBoxMapColorPicked(ofxDatGuiColorPickerEvent e);
};
