#include <stdio.h>
#include <math.h>

#define pi 3.14159265

int main(){
	int steps = 1024;
	printf("float sint[] = {");

	for(int i = 0; i < steps; i++){
		printf("%f,", sin(i * 2.0 * pi / steps));
	}
	printf("};");

}