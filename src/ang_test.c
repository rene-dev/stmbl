#include <stdio.h>
#include <math.h>
#include "ang.h"


int main(){
	struct ang a;
	struct ang b;

	b = new_ang(DEG(0));

	for(int i = 0; i < 400; i += 10){
		a = new_ang(DEG(i));
		printf("sin(%d°) = %f ang_sin(a) = %f cos(a) = %f ang_cos(a) = %f\n", i, sin(DEG(i)), ang_sin2(b, a), cos(DEG(i)), ang_cos2(a, b));
	}
	for(int i = 0; i < 400; i += 10){
		a = new_ang(DEG(i));
		printf("b = %d° => %f°\n", i, RAD(rad(new_ang(DEG(i)))));
	}

	b = new_ang(DEG(24));
	for(int i = 0; i < 400; i += 10){
		a = new_ang(DEG(i));
		printf("b = 24° + %d° => %f°\n", i, RAD(rad(plus(b, new_ang(DEG(i))))));
	}

	for(int i = 0; i < 400; i += 10){
		a = new_ang(DEG(i));
		printf("b = 24° - %d° => %f°\n", i, RAD(rad(minus(b, new_ang(DEG(i))))));
	}

	for(int i = 0; i < 400; i += 10){
		a = new_ang(DEG(i));
		printf("b = 24° rot %d° => %f°\n", i, RAD(rad(rot(b, sinf(DEG(i)), cosf(DEG(i))))));
	}
}