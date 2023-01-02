// Filename: AudioAnalyser.h
// Author: James Acres
// Created: 11/01/2016
// Last Modified: 11/01/2016 at 1:44 PM
// Much of the ideas are based on the logarithmic averaging of FFTs found in the
// Processing minim audio library

#pragma once
#include "ofxFftBasic.h"

class AudioAnalyser
{
public:
	AudioAnalyser();
	~AudioAnalyser();

	void    init(ofFmodSoundPlayer * _fmodSoundPlayer, int _numBinsPerAverage);

	void    update();

	void    drawWaveform(int _x, int _y, int _width, int _height);
	void    drawSpectrum(int _x, int _y, int _width, int _height);
	void    drawLinearAverages(int _x, int _y, int _width, int _height);

	inline float   getLeftLevel() const { return m_leftLevel; }
	inline float   getRightLevel() const { return m_rightLevel; }
	inline float   getMixLevel() const { return m_mixLevel; }

	inline float   getLinearAverage(int _binIndex) const
	{
		if (_binIndex < 0 || _binIndex >= m_numLinearAverages)
		{
			return 0;
		}

		return m_linearAverages[_binIndex] * 1000.0f;
	}

	inline int     getNumAverages() const { return m_numLinearAverages; }

	inline float   getAvgForFreqRange(float _lowFreq, float _hiFreq) { return calcAverage(_lowFreq, _hiFreq) * 1000.0f; }


private:
	int     freqToIndex(float _freq);

	void    calcVolume();
	void    calcLinearAverages();
	float   calcAverage(float _lowFreq, float _hiFreq);

	ofFmodSoundPlayer     * m_pSoundPlayer;
	ofxFftBasic             m_fft;

	float           m_sampleRate;

	float         * m_linearAverages;
	int             m_numLinearAverages;
	int             m_numBinsPerLinearAverage;

	float         * m_spectrumMix;
	int             m_numSpectrumBins;

	float         * m_waveformLeft;
	float         * m_waveformRight;
	float         * m_waveformMix;
	int             m_waveformNumSamples;

	float           m_leftLevel;
	float           m_rightLevel;
	float           m_mixLevel;
};
