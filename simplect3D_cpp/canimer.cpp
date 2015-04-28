#include "canimer.h"
#include <assert.h>

si3::canimer::canimer()
{
	counter = 0;
	nFrame = 1;
}
void si3::canimer::config(int frame, int resistor)
{
	this->nFrame = abs(frame) * abs(resistor);
	assert(this->nFrame > 0);
	this->nResistor = abs(resistor);

	counter %= this->nFrame;
}
void si3::canimer::frame(int v)
{
	config(v, nResistor);
}
void si3::canimer::resistor(int v)
{
	config(nFrame, v);
}
void si3::canimer::set(int No)
{
	if (No < 0)
	{
		No %= nFrame;
		No = nFrame + No;
	}
	counter = No%nFrame;
}
void si3::canimer::move(int num)
{
	set(counter + num);
}
int si3::canimer::now() const
{
	return counter / nResistor;
}
int si3::canimer::nowNo() const
{
	return counter;
}
int si3::canimer::maxNo() const
{
	return nFrame - 1;
}
