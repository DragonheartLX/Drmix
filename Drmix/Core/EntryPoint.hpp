#pragma once
#include "Core/Mixture.hpp"

extern Drmix::Mixture* Drmix::createMixture(int argc, char** argv);

int main(int argc, char** argv)
{
	Drmix::Mixture* mixture = Drmix::createMixture(argc, argv);
	
	mixture->init();
	mixture->run();
	mixture->release();

	delete mixture;
    return 0;
}