#include "si3_AnimeCounter.h"
#include <assert.h>

si3::AniCounter::AniCounter()
{
	counter = 0;
	frame_ = 1;
}
void si3::AniCounter::config(int frame, int resistor)
{
	this->frame_ = abs(frame) * abs(resistor);
	assert(this->frame_ > 0);
	this->resistor_ = abs(resistor);

	counter %= this->frame_;
}
void si3::AniCounter::frame(int v)
{
	config(v, resistor_);
}
void si3::AniCounter::resistor(int v)
{
	config(frame_, v);
}
void si3::AniCounter::set(int No)
{
	if (No < 0)
	{
		No %= frame_;
		No = frame_ + No;
	}
	counter = No%frame_;
}
void si3::AniCounter::move(int num)
{
	set(counter + num);
}
int si3::AniCounter::now() const
{
	return counter / resistor_;
}
int si3::AniCounter::now_No() const
{
	return counter;
}
int si3::AniCounter::max_No() const
{
	return frame_ - 1;
}
