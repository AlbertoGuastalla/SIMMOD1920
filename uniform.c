#include "rngs_MR.h"
#include "rngs_MR.c"

double Uniform(double a, double b) {
	return (a + (b - a) * Random());
}
