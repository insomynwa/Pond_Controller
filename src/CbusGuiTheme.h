#pragma once
#include "ofxDatGuiTheme.h"


class CbusGuiTheme : public ofxDatGuiTheme {

public:

	CbusGuiTheme()
	{
		stripe.visible = false;
		font.size = 9;
		layout.upperCaseLabels = false;
		color.label = hex(0xF8F3F0);
		color.guiBackground = hex(0x2C3137);
		color.background = hex(0x343B41);
		color.slider.fill = hex(0x60B9ED);
		color.slider.text = hex(0xFFFFFF);
		color.inputAreaBackground = hex(0x434A50);
		color.textInput.text = hex(0xFFFFFF);
		color.textInput.highlight = hex(0x434A50);
		color.textInput.backgroundOnActive = hex(0x2C3137);
		color.backgroundOnMouseOver = hex(0x434A50);
		color.backgroundOnMouseDown = hex(0x2C3137);
		color.matrix.hover.button = hex(0x60B9ED);
		color.matrix.selected.button = hex(0x2C3137);
		init();
	}
};

class CbusBlueButton : public ofxDatGuiTheme {

public:

	CbusBlueButton()
	{
		stripe.visible = false;
		font.size = 9;
		layout.width = 140;
		layout.upperCaseLabels = false;
		color.label = hex(0x007BFF);
		color.background = hex(0x23272B);
		color.backgroundOnMouseOver = hex(0x343A40);
		color.backgroundOnMouseDown = hex(0x343A40);
		border.color = hex(0x007BFF);
		border.width = 1;
		border.visible = true;
		init();
	}
};

class CbusBlueButtonDisabled : public ofxDatGuiTheme {

public:

	CbusBlueButtonDisabled()
	{
		stripe.visible = false;
		font.size = 9;
		layout.width = 140;
		layout.upperCaseLabels = false;
		color.label = hex(0x6C757D);
		color.background = hex(0x23272B);
		color.backgroundOnMouseOver = hex(0x343A40);
		color.backgroundOnMouseDown = hex(0x343A40);
		border.color = hex(0x7BA9D0);
		border.width = 1;
		border.visible = true;
		init();
	}
};

class CbusSkyblueButton : public ofxDatGuiTheme {

public:

	CbusSkyblueButton()
	{
		stripe.visible = false;
		font.size = 9;
		layout.width = 140;
		layout.upperCaseLabels = false;
		color.label = hex(0x17A2B8);
		color.background = hex(0x23272B);
		color.backgroundOnMouseOver = hex(0x343A40);
		color.backgroundOnMouseDown = hex(0x343A40);
		border.color = hex(0x17A2B8);
		border.width = 1;
		border.visible = true;
		init();
	}
};

class CbusSkyblueButtonDisabled : public ofxDatGuiTheme {

public:

	CbusSkyblueButtonDisabled()
	{
		stripe.visible = false;
		font.size = 9;
		layout.width = 140;
		layout.upperCaseLabels = false;
		color.label = hex(0x6C757D);
		color.background = hex(0x23272B);
		color.backgroundOnMouseOver = hex(0x343A40);
		color.backgroundOnMouseDown = hex(0x343A40);
		border.color = hex(0x95D6EA);
		border.width = 1;
		border.visible = true;
		init();
	}
};

class CbusYellowButton : public ofxDatGuiTheme {

public:

	CbusYellowButton()
	{
		stripe.visible = false;
		font.size = 9;
		layout.width = 140;
		layout.upperCaseLabels = false;
		color.label = hex(0xFFC107);
		color.background = hex(0x23272B);
		color.backgroundOnMouseOver = hex(0x343A40);
		color.backgroundOnMouseDown = hex(0x343A40);
		border.color = hex(0xFFC107);
		border.width = 1;
		border.visible = true;
		init();
	}
};

class CbusYellowButtonDisabled : public ofxDatGuiTheme {

public:

	CbusYellowButtonDisabled()
	{
		stripe.visible = false;
		font.size = 9;
		layout.width = 140;
		layout.upperCaseLabels = false;
		color.label = hex(0x6C757D);
		color.background = hex(0x23272B);
		color.backgroundOnMouseOver = hex(0x343A40);
		color.backgroundOnMouseDown = hex(0x343A40);
		border.color = hex(0xF5CA8C);
		border.width = 1;
		border.visible = true;
		init();
	}
};

class CbusGreenButton : public ofxDatGuiTheme {

public:

	CbusGreenButton()
	{
		stripe.visible = false;
		font.size = 9;
		layout.width = 140;
		layout.upperCaseLabels = false;
		color.label = hex(0x28A745);
		color.background = hex(0x23272B);
		color.backgroundOnMouseOver = hex(0x343A40);
		color.backgroundOnMouseDown = hex(0x343A40);
		border.color = hex(0x28A745);
		border.width = 1;
		border.visible = true;
		init();
	}
};

class CbusGreenButtonDisabled : public ofxDatGuiTheme {

public:

	CbusGreenButtonDisabled()
	{
		stripe.visible = false;
		font.size = 9;
		layout.width = 140;
		layout.upperCaseLabels = false;
		color.label = hex(0x6C757D);
		color.background = hex(0x23272B);
		color.backgroundOnMouseOver = hex(0x343A40);
		color.backgroundOnMouseDown = hex(0x343A40);
		border.color = hex(0x96D196);
		border.width = 1;
		border.visible = true;
		init();
	}
};

class CbusRedButton : public ofxDatGuiTheme {

public:

	CbusRedButton()
	{
		stripe.visible = false;
		font.size = 9;
		layout.width = 140;
		layout.upperCaseLabels = false;
		color.label = hex(0xDC3545);
		color.background = hex(0x23272B);
		color.backgroundOnMouseOver = hex(0x343A40);
		color.backgroundOnMouseDown = hex(0x343A40);
		border.color = hex(0xDC3545);
		border.width = 1;
		border.visible = true;
		init();
	}
};

class CbusRedButtonDisabled : public ofxDatGuiTheme {

public:

	CbusRedButtonDisabled()
	{
		stripe.visible = false;
		font.size = 9;
		layout.width = 140;
		layout.upperCaseLabels = false;
		color.label = hex(0x6C757D);
		color.background = hex(0x23272B);
		color.backgroundOnMouseOver = hex(0x343A40);
		color.backgroundOnMouseDown = hex(0x343A40);
		border.color = hex(0xE6908D);
		border.width = 1;
		border.visible = true;
		init();
	}
};