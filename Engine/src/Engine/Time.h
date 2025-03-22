#pragma once
namespace ENGINE
{
	class Time
	{
	public:
		Time(float t) :time(t) {}
		float GetSecond()const { return time; }
		float GetMillSecond()const { return time * 1000; }
		static float GetTime();
	private:
		float time;
	};
}