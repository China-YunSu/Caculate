#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "./boolean.h"

#define NUMBER_STATUS_BEGIN 	0
#define NUMBER_STATUS_DOT 		1
#define NUMBER_STATUS_INTEGER 	2
#define NUMBER_STATUS_SIGN 		3
#define NUMBER_STATUS_DECIMAL 	4
#define NUMBER_STATUS_END 		5

typedef struct ARG {
	boolean ok;
	boolean finished;
	int status;
	double value;
	double pw;
	int i;
	int sign;
}ARG;

static void progressNumberInteger(int ch, ARG *arg);
static void progressNumberDecimal(int ch, ARG *arg);
static void dealNumberStatusInteger(int ch, ARG *arg);
static void dealNumberStatusBegin(int ch, ARG *arg);
static void dealNumberStatusDot(int ch, ARG *arg);
boolean getNumber(const char *str, double *result, int *count);
static void dealNumberStatusDecimal(int ch, ARG *arg);
static void dealNumberStatusSign(int ch, ARG *arg);



#endif