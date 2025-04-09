#pragma once
#include"miniaudio.h"

#include<thread>
#include<memory>
namespace ENGINE
{
	class AudioPlayer
	{
	private:
		ma_engine engine;
		ma_sound sound;
		bool isPlaying = false;
		bool shouldStop = false;
		bool soundInitialized = false;
		std::unique_ptr<std::thread> audioThread;  // 播放音频的线程
		void playbackThread();
	public:
		AudioPlayer();
		~AudioPlayer();
		void Play();
		void Stop();
		bool loadAudio(const std::string& filePath);
		void setVolume(float volume);

	};
}