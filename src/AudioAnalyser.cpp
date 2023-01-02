#include "AudioAnalyser.h"

AudioAnalyser::AudioAnalyser()
	: m_pSoundPlayer(NULL)
	, m_sampleRate(0.0f)
	, m_linearAverages(NULL)
	, m_numLinearAverages(0)
	, m_numBinsPerLinearAverage(0)
	, m_spectrumMix(NULL)
	, m_numSpectrumBins(NULL)
	, m_waveformLeft(NULL)
	, m_waveformRight(NULL)
	, m_waveformMix(NULL)
	, m_waveformNumSamples(0)
	, m_leftLevel(0.0f)
	, m_rightLevel(0.0f)
	, m_mixLevel(0.0f)
{
}

AudioAnalyser::~AudioAnalyser()
{
	m_pSoundPlayer = NULL;

	delete[] m_linearAverages;
	m_linearAverages = NULL;

	delete[] m_waveformLeft;
	m_waveformLeft = NULL;

	delete[] m_waveformRight;
	m_waveformRight = NULL;

	delete[] m_waveformMix;
	m_waveformMix = NULL;
}

void AudioAnalyser::init(ofFmodSoundPlayer * _fmodSoundPlayer, int _numBinsPerAverage)
{
	m_pSoundPlayer = _fmodSoundPlayer;

	m_sampleRate = 44100.0f;

	m_numBinsPerLinearAverage = _numBinsPerAverage;
	m_numLinearAverages = floorf(513.0f / (float)m_numBinsPerLinearAverage);
	if (513 % m_numBinsPerLinearAverage != 0)
	{
		++m_numLinearAverages;
	}

	m_linearAverages = new float[m_numLinearAverages];
	memset(m_linearAverages, 0, m_numLinearAverages * sizeof(m_linearAverages[0]));

	m_waveformNumSamples = 1024;
	m_waveformLeft = new float[m_waveformNumSamples];
	m_waveformRight = new float[m_waveformNumSamples];
	m_waveformMix = new float[m_waveformNumSamples];

	memset(m_waveformLeft, 0, m_waveformNumSamples * sizeof(m_waveformLeft[0]));
	memset(m_waveformRight, 0, m_waveformNumSamples * sizeof(m_waveformRight[0]));
	memset(m_waveformMix, 0, m_waveformNumSamples * sizeof(m_waveformMix[0]));


	m_numSpectrumBins = 1024;

	m_spectrumMix = new float[m_numSpectrumBins];
	memset(m_spectrumMix, 0, m_numSpectrumBins * sizeof(m_spectrumMix[0]));

	m_fft.setup(1024, OF_FFT_WINDOW_HAMMING);

}

void AudioAnalyser::drawWaveform(int _x, int _y, int _width, int _height)
{
	float sampleStepX = (float)_width / m_waveformNumSamples;
	ofPushMatrix();
	ofTranslate(_x, _y);

	ofNoFill();
	ofSetColor(ofColor::white);
	ofDrawRectangle(0, 0, _width, _height);

	ofTranslate(0, _height);

	ofNoFill();

	float halfHeight = _height * 0.5f;

	ofBeginShape();
	for (int i = 0; i < m_waveformNumSamples; i++)
	{
		ofVertex(sampleStepX * i, -m_waveformMix[i] * halfHeight - halfHeight);
	}
	ofEndShape();

	ofFill();
	ofDrawBitmapString("Waveform", 10, 25);

	ofPopMatrix();
}

void AudioAnalyser::drawSpectrum(int _x, int _y, int _width, int _height)
{
	int binWidth = (float)_width / (float)((m_numSpectrumBins - 1) / 2);
	float halfHeight = _height * 0.5f;

	ofPushMatrix();
	ofTranslate(_x, _y);

	ofNoFill();
	ofSetColor(ofColor::white);
	ofDrawRectangle(0, 0, _width, _height);

	ofTranslate(0, _height);

	ofFill();

	for (int i = 0; i < (m_numSpectrumBins - 1) / 2; i++)
	{
		if (i % 2 == 0)
		{
			ofSetColor(255, 0, 0);
		}
		else
		{
			ofSetColor(150, 10, 40);
		}

		float value = std::min((float)_height, (m_spectrumMix[i] * 150.0f) / (_height * 0.5f) * _height);
		ofDrawRectangle(i * binWidth, 0, binWidth, -value);
	}

	ofSetColor(ofColor::white);
	ofDrawBitmapString("FFT Spectrum", 10, 25);

	ofPopMatrix();

	float mouseY = ofGetMouseY();
	if (mouseY > _y && mouseY < _y + _height)
	{
		float mouseX = ofGetMouseX();
		if (mouseX > _x && mouseX < _x + _width)
		{
			float mousePos = _y + _height - mouseY;
			int index = ((mouseX - _x) / (float)_width) * (float)((m_numSpectrumBins - 1) / 2);
			float freqStep = (m_sampleRate / 2.0f) / m_fft.getBinSize();

			float val = (mousePos * halfHeight) / (_height) / 150.0f;

			ofDrawLine(_x, mouseY, _x + _width, mouseY);
			ofDrawBitmapString("(" + ofToString(index) + ") Freq: " + ofToString(index * freqStep) + " to " + ofToString((index + 1) * freqStep) + ",\nValue: " + ofToString(val * 1000.0f), mouseX + 8, mouseY - 30);
		}
	}
}

void AudioAnalyser::drawLinearAverages(int _x, int _y, int _width, int _height)
{
	int binWidth = (float)_width / (float)m_numLinearAverages;
	float halfHeight = _height * 0.5f;

	ofPushMatrix();
	ofTranslate(_x, _y);

	ofNoFill();
	ofSetColor(ofColor::white);
	ofDrawRectangle(0, 0, _width, _height);

	ofTranslate(0, _height);

	ofFill();

	for (int i = 0; i < m_numLinearAverages; i++)
	{
		if (i % 2 == 0)
		{
			ofSetColor(255, 0, 0);
		}
		else
		{
			ofSetColor(150, 10, 40);
		}

		ofDrawRectangle(i * binWidth, 0, binWidth, ((m_linearAverages[i] * -150.0f) / halfHeight) * _height);

		ofSetColor(ofColor::white);
		ofDrawBitmapString(ofToString(i), i * binWidth + binWidth / 2.0f - 7, 30);
	}

	ofSetColor(ofColor::white);
	ofDrawBitmapString("Linear Averages", 10, 60);

	ofPopMatrix();

	float mouseY = ofGetMouseY();
	if (mouseY > _y && mouseY < _y + _height)
	{
		float mouseX = ofGetMouseX();
		if (mouseX > _x && mouseX < _x + _width)
		{
			float mousePos = _y + _height - mouseY;
			int index = ((mouseX - _x) / (float)_width) * (m_numLinearAverages);

			float val = (mousePos * halfHeight) / (_height) / 150.0f;

			ofDrawLine(_x, mouseY, _x + _width, mouseY);
			ofDrawBitmapString("(" + ofToString(index) + "): " + ofToString(val * 1000.0f), mouseX + 8, mouseY - 10);
		}
	}
}

float AudioAnalyser::calcAverage(float _lowFreq, float _hiFreq)
{
	int lowBound = freqToIndex(_lowFreq);
	int hiBound = freqToIndex(_hiFreq);

	float avg = 0.0f;

	for (int i = lowBound; i <= hiBound; ++i)
	{
		avg += std::abs(m_spectrumMix[i]);
	}

	avg /= (hiBound - lowBound + 1);
	return avg;
}

void AudioAnalyser::update()
{
	FMOD_Channel_GetWaveData(m_pSoundPlayer->channel, m_waveformLeft, m_waveformNumSamples, 0);
	FMOD_Channel_GetWaveData(m_pSoundPlayer->channel, m_waveformRight, m_waveformNumSamples, 1);

	calcVolume();

	// perform FFT on the stereo mix (left and right speaker mixed)
	m_fft.setSignal(m_waveformMix);

	// get the result of the FFT
	float * currFft = m_fft.getAmplitude();

	memcpy(m_spectrumMix, currFft, sizeof(float) * m_fft.getBinSize());
	m_numSpectrumBins = m_fft.getBinSize();

	calcLinearAverages();
}

void AudioAnalyser::calcVolume()
{
	float leftVol = 0.0f;
	float rightVol = 0.0f;

	// go through each sample and calculate the root mean square - a rough way to calculate volume	
	for (int i = 0; i < m_waveformNumSamples; i++)
	{
		float left = m_waveformLeft[i];
		float right = m_waveformRight[i];
		m_waveformMix[i] = (left + right) * 0.5f;

		leftVol += left * left;
		rightVol += right * right;
	}

	// get the mean of rms 
	float mixVol = (leftVol + rightVol) / ((float)m_waveformNumSamples * 2.0f);
	leftVol /= (float)m_waveformNumSamples;
	rightVol /= (float)m_waveformNumSamples;

	// root of rms (this is our rough volume)
	m_leftLevel = sqrt(leftVol);
	m_rightLevel = sqrt(rightVol);
	m_mixLevel = sqrt(mixVol);
}

void AudioAnalyser::calcLinearAverages()
{
	float freqStep = (m_sampleRate / 2.0f) / m_fft.getBinSize() * m_numBinsPerLinearAverage;
	float f = 0.0f;

	for (int i = 0; i < m_numLinearAverages; ++i)
	{
		m_linearAverages[i] = calcAverage(f, f + freqStep) * 0.5f + m_linearAverages[i] * 0.5f;
		f += freqStep;
	}
}

int AudioAnalyser::freqToIndex(float _freq)
{
	float bandWidth = (2.0f / (float)1024.0f) * (m_sampleRate / 2.0f);

	// special case: freq is lower than the bandwidth of spectrum[0]
	if (_freq < bandWidth / 2)
	{
		return 0;
	}

	// special case: freq is within the bandwidth of spectrum[spectrum.length - 1]
	if (_freq > m_sampleRate / 2 - bandWidth / 2)
	{
		return m_numSpectrumBins - 1;
	}

	// all other cases
	float fraction = _freq / (m_sampleRate / 2.0f);
	int i = roundf(bandWidth * fraction);

	return i;
}
