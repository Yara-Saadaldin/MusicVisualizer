#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){

	//Screens
		AudioVisualizer = true;

	//Window shape
		ofSetVerticalSync(true);
		ofSetWindowShape(1280, 900);

	//Song
		m_soundPlayer.load("Before Chill.mp3");
		m_soundPlayer.setLoop(true);
		m_soundPlayer.play();
		m_audioAnalyser.init(&m_soundPlayer, 20); 

	//Loads in images
		m_Background.load("Background.png");
		m_BuildingLights.load("BuildingLights.png");
		m_LightBulb.load("LightBulb.png");
		m_Stars.load("Stars.png");
		m_RightSound.load("RightSound.png");
		m_LeftSound.load("LeftSound.png");
}



//--------------------------------------------------------------
void ofApp::update() {
	m_audioAnalyser.update();
}



//--------------------------------------------------------------
void ofApp::draw() {

	//Audio Information screen
		if (AudioInfo) {

			//Background color
				ofBackground(ofColor::black);


			//Text color
				ofSetColor(255);


			// Volume
				ofSetColor(ofColor::white);
				ofDrawBitmapString("Volume Level", 140, 50);

				ofDrawCircle(100, 100, m_audioAnalyser.getLeftLevel()  * 100.0f); 
				ofDrawCircle(200, 100, m_audioAnalyser.getRightLevel() * 100.0f); 
				ofDrawCircle(300, 100, m_audioAnalyser.getMixLevel()   * 100.0f); 
	
				ofDrawBitmapString("Left", 80, 170);
				ofDrawBitmapString("Right", 180, 170);
				ofDrawBitmapString("Mix", 290, 170);


			// Frequency / FFT information
				//Waveform
					m_audioAnalyser.drawWaveform(40, 300, 1200, 90); 
				//Spectrum (frequency bins)
					m_audioAnalyser.drawSpectrum(40, 460, 1200, 128); 
				//Linear average
					m_audioAnalyser.drawLinearAverages(40, 650, 1200, 128); 


			//Instruments
				float Tone_1 = m_audioAnalyser.getLinearAverage(0);
				float Tone_2 = m_audioAnalyser.getLinearAverage(4);
				float Tone_3 = m_audioAnalyser.getLinearAverage(15);
		
				m_circlesBrightness_1 = ofMap(Tone_1, 220.0f, 450.0f, 0.0f, 1.0f, true);
				m_circlesBrightness_3 = ofMap(Tone_3, 10.0f, 50.0f, 0.0f, 1.0f, true);
				m_circlesBrightness_4 = ofMap(Tone_2, 33.0f, 130.0f, 0.0f, 1.0f, true);

				ofSetColor(ofFloatColor(m_circlesBrightness_1, 0.0f, 0.0f));
					ofDrawCircle(650, 100, 50);

				ofSetColor(ofFloatColor(0.0f, 0.0f, m_circlesBrightness_3));
					ofDrawCircle(800, 100, 50);

				ofSetColor(ofFloatColor(m_circlesBrightness_4, m_circlesBrightness_4, m_circlesBrightness_4));
					ofDrawCircle(950, 100, 50);

				ofSetColor(ofColor::white);


			// Song Time
				float songTimeSeconds = m_soundPlayer.getPositionMS() / 1000.0f;
				ofDrawBitmapString("Song time: " + ofToString(songTimeSeconds), 40, 250);
		}


	//Audio Visualizer screen
		if (AudioVisualizer) {
		
			//Background Image
				ofPushMatrix();
					m_Background.draw(0.0f, 0.0f, ofGetWindowWidth(), ofGetWindowHeight());
				ofPopMatrix();

			//React to Frequency
				//Tone 1
					float Tone_1 = m_audioAnalyser.getLinearAverage(0);
					m_circlesBrightness_1 = ofMap(Tone_1, 220.0f, 450.0f, 0.0f, 1.0f, true);
						if (m_circlesBrightness_1 == true) {
							ofPushMatrix();
								ofScale(0.25f, 0.25f);
								m_BuildingLights.draw(20, 1735);
							ofPopMatrix();
						}

				//Tone 2
					float Tone_2 = m_audioAnalyser.getLinearAverage(4);
					m_circlesBrightness_4 = ofMap(Tone_2, 33.0f, 130.0f, 0.0f, 1.0f, true);
						if (m_circlesBrightness_4 == true) {
							ofPushMatrix();
								ofScale(0.2f, 0.2f);
								m_Stars.draw(900, 50);
							ofPopMatrix();
						}

				//Tone 3
					float Tone_3 = m_audioAnalyser.getLinearAverage(15);
					m_circlesBrightness_3 = ofMap(Tone_3, 10.0f, 50.0f, 0.0f, 1.0f, true); //between ? and ? , range
						if (m_circlesBrightness_3 == true) {
							ofPushMatrix();
								ofScale(0.24f, 0.24f);
								m_LightBulb.draw(3855, 755);
							ofPopMatrix();
						}


			//React to Volume
				//Reacts to the Right sound
					ofPushMatrix();
						ofScale(0.27f, 0.27f);
						m_RightSound.draw(20, 1, m_audioAnalyser.getLeftLevel()  * 100.0f);
					ofPopMatrix();
				//Reacts to the Left sound
					ofPushMatrix();
						ofScale(0.27f, 0.27f);
						m_LeftSound.draw(2380, 20, m_audioAnalyser.getLeftLevel()  * 100.0f);
					ofPopMatrix();	
		}
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//AudioInformation Toggled by spacebar
		if (key == 32) {  
			AudioVisualizer = false;
			AudioInfo = true;
		}

	//AudioVizualizer toggled by 'b'
		else {
			if (key == 'b') {
				AudioVisualizer = true;
				AudioInfo = false;
			}
		}
}
