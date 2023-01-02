#pragma once
#include "ofMain.h"
#include "AudioAnalyser.h"



class ofApp : public ofBaseApp{

	//----------------------------------------------------Variables----------------------------------------------------
		//Audio
			ofFmodSoundPlayer   m_soundPlayer;          
			ofSoundPlayer       m_soundPlayer_OF;        
			AudioAnalyser       m_audioAnalyser;        
		//Images
			ofImage             m_Background;
			ofImage             m_BuildingLights;
			ofImage             m_Buildings;
			ofImage             m_Light;
			ofImage             m_Moon;
			ofImage             m_LeftSound;
			ofImage             m_LightBulb;
			ofImage             m_RightSound;
			ofImage             m_Stars;

	//----------------------------------------------------Functions----------------------------------------------------
	public:
		void    setup();
		void    update();
		void    draw();

		void    keyPressed(int key);

		bool    AudioInfo;
		bool    AudioVisualizer;

		float   m_circlesBrightness_1;
		float   m_circlesBrightness_2;
		float   m_circlesBrightness_3;
		float   m_circlesBrightness_4;	
};
 