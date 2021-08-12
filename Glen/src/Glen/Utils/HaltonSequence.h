#pragma once

#ifndef HALTON_H
#define HALTON_H

#include "glm/glm.hpp"

float CreateHaltonSequence(unsigned int index, int base)
{
	float f = 1;
	float r = 0;
	int current = index;
	do
	{
		f = f / base;
		r = r + f * (current % base);
		current = (int)glm::floor(current / base);
	} while (current > 0);

	return r;
}

#endif // !HALTON_H