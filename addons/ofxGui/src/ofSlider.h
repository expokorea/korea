#pragma once

#include "ofPanel.h"
#include "ofParameter.h"

template<typename Type>
class ofSlider : public ofBaseGui{
	friend class ofPanel;
	
public:	
	ofSlider* setup(string sliderName, ofParameter<Type> _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight){
		name = sliderName;
		value = _val;
		min = _min;
		max = _max;
		b.x = 0;
		b.y = 0;
		b.width = width;
		b.height = height;
		currentFrame = 0;			
		bGuiActive = false;
		return this;
	}
	
	virtual void mouseMoved(ofMouseEventArgs & args){
	}
	
	virtual void mousePressed(ofMouseEventArgs & args){
		setValue(args.x, args.y, true);
	}
	
	virtual void mouseDragged(ofMouseEventArgs & args){
		setValue(args.x, args.y, false);
	}
	
	virtual void mouseReleased(ofMouseEventArgs & args){
		bGuiActive = false;		
	}	
	
	virtual void saveToXml(ofxXmlSettings& xml) {
		string xmlName = name;
		ofStringReplace(xmlName," ","_");
		xml.setValue(xmlName, value);
	}
	
	virtual void loadFromXml(ofxXmlSettings& xml) {
		string xmlName = name;
		ofStringReplace(xmlName," ","_");
		value = xml.getValue(xmlName, value);
	}

	template<class ListenerClass>
	void addListener(ListenerClass * listener, void ( ListenerClass::*method )(Type&)){
		ofAddListener(value.changedE,listener,method);
	}

	template<class ListenerClass>
	void removeListener(ListenerClass * listener, void ( ListenerClass::*method )(Type&)){
		ofRemoveListener(value.changedE,listener,method);
	}


	double operator=(Type v){
		value = v;
		return v;
	}

	operator Type & (){
		return value;
	}

	void draw(){
		ofPushStyle();
		ofPushMatrix();
	
		currentFrame = ofGetFrameNum();
		ofFill();
		ofSetColor(backgroundColor);
		ofRect(b);
		
		ofTranslate(b.x, b.y);		
		float valAsPct = ofMap( value, min, max, 0, b.width-2, true );
		ofEnableAlphaBlending();
		ofSetColor(fillColor);		
		ofRect(1, 1, valAsPct, b.height-2);
		
		ofTranslate(0, b.height / 2 + 4);
		ofSetColor(textColor);
		ofDrawBitmapString(name, textPadding, 0);
		string valStr = ofToString(value.getValue());
		ofDrawBitmapString(valStr, b.width - textPadding - valStr.length() * 8, 0);
		
		ofPopMatrix();
		ofPopStyle();
	}
	
	ofParameter<Type> value;

protected:
	Type min, max;
	
	void setValue(float mx, float my, bool bCheck){
		if( ofGetFrameNum() - currentFrame > 1 ){
			bGuiActive = false;
			return; 
		}
		if( bCheck ){
			if( b.inside(mx, my) ){
				bGuiActive = true;
			}else{
				bGuiActive = false;
			}
		}
		if( bGuiActive ){
			value = ofMap(mx, b.x, b.x + b.width, min, max, true);
		}
	}
};

typedef ofSlider<float> ofFloatSlider;
typedef ofSlider<int> ofIntSlider;
