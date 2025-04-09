#include"pch.h"
#include"AudioPlayer.h"

namespace ENGINE
{
	void AudioPlayer::playbackThread()
	{
		if (!soundInitialized)return;
		ma_result result = ma_sound_start(&sound);
		if (result != MA_SUCCESS) {
			std::cerr << "Failed to play sound!" << std::endl;
		}

		while (!shouldStop) {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}
	void AudioPlayer::Play()
	{
		if (!soundInitialized) {
			std::cerr << "No audio file loaded, cannot play." << std::endl;
			return;
		}
		if (isPlaying) Stop();

		isPlaying = true;
		shouldStop = false;

		// 在后台线程中播放音频
		audioThread = std::make_unique<std::thread>(&AudioPlayer::playbackThread, this);
	}
	void AudioPlayer::Stop()
	{
		if (!soundInitialized)
		{
			std::cerr << "No audio file loaded, cannot stop." << std::endl;
			return;
		}
		if (isPlaying) {
			shouldStop = true;
			if (audioThread && audioThread->joinable()) {
				audioThread->join();
				ma_sound_stop(&sound);
			}
			isPlaying = false;
		}
	}
	bool AudioPlayer::loadAudio(const std::string& filePath)
	{
		if (soundInitialized) {
			Stop();
			ma_sound_uninit(&sound);
			soundInitialized = false;
		}

		ma_result result = ma_sound_init_from_file(&engine, filePath.c_str(), MA_SOUND_FLAG_DECODE, nullptr, nullptr, &sound);
		if (result != MA_SUCCESS) {
			std::cerr << "Failed to load sound from file: " << filePath << std::endl;
			return false;
		}
		soundInitialized = true;
		return true;
	}
	void AudioPlayer::setVolume(float volume)
	{
		if (!soundInitialized) return;
		ma_sound_set_volume(&sound, volume);
	}
	AudioPlayer::AudioPlayer()
	{
		ma_result result = ma_engine_init(nullptr, &engine);
		if (result != MA_SUCCESS) {
			std::cerr << "Failed to initialize audio engine." << std::endl;
		}
	}
	AudioPlayer::~AudioPlayer()
	{
		Stop();
		if (soundInitialized)
		{
			ma_sound_uninit(&sound);
		}
		ma_engine_uninit(&engine);
	}
}