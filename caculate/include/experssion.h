#ifndef _EXPERSSION_H_
#define _EXPERSSION_H_

#include "boolean.h"
#include "myStack.h"

typedef struct sentence{
	double value;
	int status;
	boolean ok;
	boolean finished;
	int index;
	int bracket;
	MY_STACK *operatorStack;
	MY_STACK *operandStack;
}ARG_EXP;

#define OPERATORCOUNT 					5
#define HIGH							1
#define LOW								0

#define EXPERSSION_STATUS_BEGIN			1
#define EXPERSSION_STATUS_OPERAND		2
//#define EXPERSSION_STATUS_OPERTOR		3
#define EXPERSSION_STATUS_END      		4

static boolean isNumber(int ch);
static boolean isOperatorStackEmpty(MY_STACK *operatorStack);
static boolean isOperator(int ch);
static boolean compute(double *left, double *right,char *operator);
static boolean progress(MY_STACK *operatorStack, MY_STACK *operandStack);
static int indexof(char *str, int ch);
static boolean compare(int cur, int per);
static void dealExperssionStatusBegin(char *srt, int ch, ARG_EXP *arg);
//static void dealExperssionStatusOperater(char *srt, int ch, ARG_EXP *arg);
static void dealExperssionStatusOperand(char *srt, int ch, ARG_EXP *arg);
boolean experssionAnalysis(char *str, double *value);

#endif
