#define pi 3.14159265
#define DEG(a) ((a)*pi/180.0)
#define RAD(a) ((a)*180.0/pi)

// all x: len(x) == 1
//#define NORM

struct ang{
	float x, y;
};

float len(struct ang a){
	return(sqrtf(a.x * a.x + a.y * a.y));
}

struct ang norm(struct ang a){
	struct ang ret;
	ret.x = a.x / len(a);
	ret.y = a.y / len(a);
	return(ret);
}

struct ang new_ang(float a){
	struct ang ret;
	ret.x = cosf(a);
	ret.y = sinf(a);
	return(ret);
}

struct ang new_ang2(float a,float b){
	struct ang ret;
	ret.x = a;
	ret.y = b;
	return(norm(ret));
}

struct ang diff(struct ang a, struct ang b){
	struct ang ret;
	ret.x = b.x - a.x;
	ret.y = b.y - a.y;
	return(ret);
}

struct ang mid(struct ang a, struct ang b){
	struct ang ret;
	ret.x = (b.x + a.x) / 2.0;
	ret.y = (b.y + a.y) / 2.0;
	return(ret);
}

#ifdef NORM
float ang_sin2(struct ang a, struct ang b){
	return(a.x * b.y - a.y * b.x);
}

float ang_cos2(struct ang a, struct ang b){
	return(a.x * b.x + a.y * b.y);
}

#else
float ang_sin2(struct ang a, struct ang b){
	return((a.x * b.y - a.y * b.x) / (len(a) * len(b)));
}

float ang_cos2(struct ang a, struct ang b){
	return((a.x * b.x + a.y * b.y) / (len(a) * len(b)));
}
#endif

float ang_sin(struct ang a){
	return(ang_sin2(new_ang(0), a));
}

float ang_cos(struct ang a, struct ang b){
	return(ang_cos2(new_ang(0), a));
}

float rad(struct ang a){
	return(atan2f(a.y, a.x));
}

struct ang rot(struct ang a, float s, float c){
	struct ang ret;

	ret.x = c * a.x - s * a.y;
	ret.y = s * a.x + c * a.y;

	return(ret);
}

struct ang plus(struct ang a, struct ang b){
	struct ang n;
	n.x = 1;
	n.y = 0;

	float s,c;
	s = ang_sin2(n, b);
	c = ang_cos2(n, b);

	return(rot(a, s, c));
}

struct ang minus(struct ang a, struct ang b){
	struct ang n;
	n.x = 1;
	n.y = 0;

	float s,c;
	s = ang_sin2(b, a);
	c = ang_cos2(b, a);

	return(rot(n, s, c));
}

struct ang mul(struct ang a, unsigned int b){
	struct ang ret = new_ang(DEG(0));

	for(int i = 0; i < b; i++){
		ret = plus(ret, a);
	}

	return(ret);
}