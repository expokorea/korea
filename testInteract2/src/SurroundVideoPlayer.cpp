/*
 * SurroundVideoPlayer.cpp
 *
 *  Created on: 12/04/2012
 *      Author: arturo
 */

#include "SurroundVideoPlayer.h"
#include "ofAppRunner.h"


#ifdef OF_SOUNDSTREAM_RTAUDIO
#include "RtAudio.h"
#elif defined(OF_SOUNDSTREAM_PORTAUDIO)
#include "portaudio.h"
#endif

// called when the appsink notifies us that there is a new buffer ready for
// processing

static GstFlowReturn on_new_buffer_from_source (GstAppSink * elt, void * data){
	GstBuffer *buffer = gst_app_sink_pull_buffer (GST_APP_SINK (elt));
	//cout << GST_BUFFER_SIZE(buffer)<< endl;
	((SurroundVideoPlayer*)data)->newBufferCB(buffer);
	return GST_FLOW_OK;//((SurroundVideoPlayer*)data)->buffer_cb(buffer);
}

static GstFlowReturn on_new_preroll_from_source (GstAppSink * elt, void * data){
	GstBuffer *buffer = gst_app_sink_pull_preroll(GST_APP_SINK (elt));
	((SurroundVideoPlayer*)data)->newBufferCB(buffer);
	return GST_FLOW_OK;//((SurroundVideoPlayer*)data)->preroll_cb(buffer);
}

static void on_eos_from_source (GstAppSink * elt, void * data){
	//((SurroundVideoPlayer*)data)->eos_cb();
}


SurroundVideoPlayer::SurroundVideoPlayer() {

}

SurroundVideoPlayer::~SurroundVideoPlayer() {
	// TODO Auto-generated destructor stub
}

int SurroundVideoPlayer::getDeviceIDByName(string name){
#ifdef OF_SOUNDSTREAM_RTAUDIO
	ofPtr<RtAudio> audioTemp;
	try {
		audioTemp = ofPtr<RtAudio>(new RtAudio());
	} catch (RtError &error) {
		error.printMessage();
		return -1;
	}
	int devices = audioTemp->getDeviceCount();
	RtAudio::DeviceInfo info;
	for (int i=0; i< devices; i++) {
		try {
			info = audioTemp->getDeviceInfo(i);
		} catch (RtError &error) {
			error.printMessage();
			break;
		}
		if(info.name==name) return i;
		/*std::cout << "device = " << i << " (" << info.name << ")\n";
		if (info.isDefaultInput) std::cout << "----* default ----* \n";
		std::cout << "maximum output channels = " << info.outputChannels << "\n";
		std::cout << "maximum input channels = " << info.inputChannels << "\n";
		std::cout << "-----------------------------------------\n";*/

	}
	return -1;
#elif defined(OF_SOUNDSTREAM_PORTAUDIO)
	int numDevices;

	numDevices = Pa_GetDeviceCount();
	if( numDevices < 0 )
	{
    	ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( numDevices ));
    	return -1;
	}
	const   PaDeviceInfo *deviceInfo;

	for( int i=0; i<numDevices; i++ )
	{
		deviceInfo = Pa_GetDeviceInfo( i );
		if( deviceInfo->name==name ) return i;
	}
	return -1;
#endif
}

void SurroundVideoPlayer::setDeviceFront(string name){
	nameFront = name;
}

void SurroundVideoPlayer::setDeviceRear(string name){
	nameRear = name;
}

void SurroundVideoPlayer::setDeviceSubLFE(string name){
	nameSubLFE = name;
}

bool SurroundVideoPlayer::loadMovie(string name){
	bufferFront.setup(8192,0);
	bufferRear.setup(8192,0);
	bufferLFESub.setup(8192,0);
	GstElement * gstPipeline = gst_element_factory_make("playbin2","player");
	g_object_set(G_OBJECT(gstPipeline), "uri", name.c_str(), (void*)NULL);

	// create the oF appsink for video rgb without sync to clock
	GstElement * gstSink = gst_element_factory_make("appsink", "app_sink");

	gst_base_sink_set_sync(GST_BASE_SINK(gstSink), true);
	gst_app_sink_set_max_buffers(GST_APP_SINK(gstSink), 8);
	gst_app_sink_set_drop (GST_APP_SINK(gstSink),true);
	gst_base_sink_set_max_lateness  (GST_BASE_SINK(gstSink), -1);

	int bpp = 24;
	string mime = "video/x-raw-rgb";


	GstCaps *caps = gst_caps_new_simple(mime.c_str(),
										"bpp", G_TYPE_INT, bpp,
										"depth", G_TYPE_INT, 24,
										"endianness",G_TYPE_INT,4321,
										"red_mask",G_TYPE_INT,0xff0000,
										"green_mask",G_TYPE_INT,0x00ff00,
										"blue_mask",G_TYPE_INT,0x0000ff,
										"alpha_mask",G_TYPE_INT,0x000000ff,


										NULL);
	gst_app_sink_set_caps(GST_APP_SINK(gstSink), caps);
	gst_caps_unref(caps);

	GstElement * appQueue = gst_element_factory_make("queue","appsink_queue");
	g_object_set(G_OBJECT(appQueue), "leaky", 0, "silent", 1, (void*)NULL);
	GstElement* appBin = gst_bin_new("app_bin");
	gst_bin_add(GST_BIN(appBin), appQueue);
	GstPad* appQueuePad = gst_element_get_static_pad(appQueue, "sink");
	GstPad* ghostPad = gst_ghost_pad_new("app_bin_sink", appQueuePad);
	gst_object_unref(appQueuePad);
	gst_element_add_pad(appBin, ghostPad);

	gst_bin_add_many(GST_BIN(appBin), gstSink, NULL);
	gst_element_link_many(appQueue, gstSink, NULL);


	g_object_set (G_OBJECT(gstPipeline),"video-sink",appBin,(void*)NULL);

	/*GstAudioChannelPosition channelPositions[] = {GST_AUDIO_CHANNEL_POSITION_FRONT_LEFT , GST_AUDIO_CHANNEL_POSITION_FRONT_RIGHT ,
			GST_AUDIO_CHANNEL_POSITION_REAR_LEFT , GST_AUDIO_CHANNEL_POSITION_REAR_RIGHT , GST_AUDIO_CHANNEL_POSITION_FRONT_CENTER , GST_AUDIO_CHANNEL_POSITION_LFE
	};*/
	caps = gst_caps_new_simple("audio/x-raw-float",
											"endianness",G_TYPE_INT,1234,
											"channels",G_TYPE_INT,6,
											"width",G_TYPE_INT,32,
											"rate",G_TYPE_INT,44100);
	GstElement * audioSink = gst_element_factory_make("appsink", "audio_sink");
	//g_object_set(audioSink,"buffer-time",0.011609977*GST_SECOND,(void*)NULL);
	//g_object_set(audioSink,"blocksize",512*6,(void*)NULL);
	//gst_base_sink_set_blocksize((GstBaseSink*)audioSink,512*6*4);
	gst_app_sink_set_caps(GST_APP_SINK(audioSink), caps);
	//gst_app_sink_set_max_buffers(GST_APP_SINK(audioSink), 8);
	//gst_app_sink_set_drop (GST_APP_SINK(audioSink),false);

	GstElement * audiorate = gst_element_factory_make("audiorate","audiorate");
	GstElement* audioBin = gst_bin_new("audio_bin");
	gst_bin_add(GST_BIN(audioBin), audiorate);
	GstPad* audioratePad = gst_element_get_static_pad(audiorate, "sink");
	GstPad* audioGhostPad = gst_ghost_pad_new("audiorate_sink", audioratePad);
	gst_object_unref(audioratePad);
	gst_element_add_pad(audioBin, audioGhostPad);

	gst_bin_add_many(GST_BIN(audioBin), audioSink, NULL);
	gst_element_link_many(audiorate, audioSink, NULL);



	g_object_set (G_OBJECT(gstPipeline),"audio-sink",audioBin,(void*)NULL);

	getGstVideoUtils()->setPipelineWithSink(gstPipeline,gstSink,false);

	// set the appsink to not emit signals, we are using callbacks instead
	// and frameByFrame to get buffers by polling instead of callback
	g_object_set (G_OBJECT (audioSink), "emit-signals", FALSE, "sync", true, (void*)NULL);

	GstAppSinkCallbacks gstCallbacks;
	gstCallbacks.eos = &on_eos_from_source;
	gstCallbacks.new_preroll = &on_new_preroll_from_source;
	gstCallbacks.new_buffer = &on_new_buffer_from_source;

	gst_app_sink_set_callbacks(GST_APP_SINK(audioSink), &gstCallbacks, this, NULL);

	//getGstVideoUtils()->play();

	//bIsStream = false;

	/*for(int i=0;i<4096;i++){
		bufferFront.write(0);
		bufferRear.write(0);
	}*/

	return allocate(bpp);
}

void SurroundVideoPlayer::play(){
    ofGstVideoPlayer::play();

    ofSleepMillis(10);
	frontOut.listDevices();
	deviceFront = getDeviceIDByName(nameFront);
	cout << "front" << deviceFront;
	frontOut.setDeviceID(deviceFront);
	frontOut.setOutput(this);
	frontOut.setup(2,0,44100,512,1);

	deviceRear = getDeviceIDByName(nameRear);
	cout << "rear" << deviceRear;
	rearOut.setDeviceID(deviceRear);
	rearOut.setOutput(this);
	rearOut.setup(2,0,44100,512,1);

	deviceLFESub = getDeviceIDByName(nameSubLFE);
	cout << "sub" << deviceLFESub;
	LFESubOut.setDeviceID(deviceLFESub);
	LFESubOut.setOutput(this);
	LFESubOut.setup(2,0,44100,512,1);
}


void SurroundVideoPlayer::newBufferCB(GstBuffer * buffer){
	/*mutex.lock();
	bufferQueue.push(buffer);
	mutex.unlock();*/
	float * bufferData = (float*)GST_BUFFER_DATA (buffer);
	int buffSize = GST_BUFFER_SIZE(buffer)/4/6;
	//cout << buffSize << endl;
	//while(bufferFront.is_full() || bufferLFESub.is_full() || bufferRear.is_full());

	for(int i=0;i<buffSize;i++){
		bufferFront.write(*bufferData++);
		bufferFront.write(*bufferData++);

		bufferLFESub.write(*bufferData++);
		bufferLFESub.write(*bufferData++);

		bufferRear.write(*bufferData++);
		bufferRear.write(*bufferData++);
	}

	gst_buffer_unref (buffer);
}

void SurroundVideoPlayer::audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount  ){
	if(deviceID==deviceFront){
		for(int i=0;i<bufferSize;i++){
			*output++ = bufferFront.read(0);
			*output++ = bufferFront.read(0);
		}
	}else if(deviceID==deviceRear){
		for(int i=0;i<bufferSize;i++){
			*output++ = bufferRear.read(0);
			*output++ = bufferRear.read(0);
		}
	}else if(deviceID==deviceLFESub){
		for(int i=0;i<bufferSize;i++){
			*output++ = bufferLFESub.read(0);
			*output++ = bufferLFESub.read(0);
		}
	}
}
