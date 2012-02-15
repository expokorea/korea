#pragma once

#include "ofEvents.h"
#include "ofTypes.h"

template<typename ParameterType>
class ofParameter{
public:
	ofParameter();
	ofParameter(ParameterType v);
	ofParameter(string name, ParameterType v);
	virtual ~ofParameter(){};

	ParameterType operator=(ParameterType v);
	operator ParameterType & ();

	virtual ParameterType getValue();
	virtual void setValue(ParameterType v);

	void setName(string name);
	string getName();

	ofEvent<ParameterType> changedE;
private:
	class Value{
	public:
		Value(){};

		Value(ParameterType v)
		:value(v){};

		Value(string name, ParameterType v)
		:name(name)
		,value(v){};

		ParameterType value;
		string name;
	};
	ofPtr<Value> obj;
};

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(){
	obj = ofPtr<Value>(new Value);
}

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(ParameterType v)
:obj(ofPtr<Value>(new Value(v))){}

template<typename ParameterType>
ofParameter<ParameterType>::ofParameter(string name, ParameterType v)
:obj(ofPtr<Value>(new Value(name, v))){}

template<typename ParameterType>
ParameterType ofParameter<ParameterType>::operator=(ParameterType v){
	setValue(v);
	return obj->value;
}

template<typename ParameterType>
ParameterType ofParameter<ParameterType>::getValue(){
	return obj->value;
}

template<typename ParameterType>
void ofParameter<ParameterType>::setValue(ParameterType v){
	obj->value = v;
	ofNotifyEvent(changedE,obj->value);
}

template<typename ParameterType>
ofParameter<ParameterType>::operator ParameterType & (){
	return obj->value;
}
