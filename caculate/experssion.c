#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "./include/boolean.h"
#include "./include/number.h"
#include "./include/experssion.h"		
#include "./include/myStack.h"

extern const char *errorMess;
const char *OPERATORLIST = "+-*/^";

void freeData(MY_STACK *stack) {
	void *tmp;

	while(NULL != (tmp = pop(stack))) {
		free(tmp);
	} 
}

const compareList [OPERATORCOUNT][OPERATORCOUNT] = {
		//	+		-		*		/		^
/* + */ 	HIGH,	HIGH,	LOW,	LOW,	LOW,
/* - */		HIGH,	HIGH,	LOW,	LOW,	LOW,
/* * */		HIGH,	HIGH,	HIGH,	HIGH,	LOW,
/* / */		HIGH,	HIGH,	HIGH,	HIGH,	LOW,
/* ^ */		HIGH,	HIGH,	HIGH,	HIGH,	HIGH,
};

static int indexOf(const char *str, int ch) {
	int index;

	for(index = 0; str[index] && ch != str[index]; index++)
		;
	return index;
}

static boolean isOperatorStackEmpty(MY_STACK *operatorStack) {
	return isStackEmpty(operatorStack);
}

static boolean compare(int cur, int per) {
	cur = indexOf(OPERATORLIST, cur);
	per = indexOf(OPERATORLIST, per);

	return compareList[cur][per];
}

static boolean compute(double *left, double *right,char *operator) {
	switch (*operator) {
		case '+' :
		*left += *right;
		break;
		case '-' :
		*left -= *right;
		break;
		case '*' :
		*left *= *right;
		break;
		case '/' :
		if(*right < 1e-6) {
			printf("����Ϊ�㣡");
			*left = 1;
			return FALSE;
		}
		*left /= *right;
		break;
		case '^' :
		*left = pow(*left,*right);
		break;
	}
	return TRUE;
}
static boolean progress(MY_STACK *operatorStack, MY_STACK *operandStack) {

	boolean yes = TRUE;
	while(!isOperatorStackEmpty(operatorStack) && yes) {
		double *rightnum = (double *)pop(operandStack);
		double *leftnum = (double *)pop(operandStack);
		char *operator = (char *)pop(operatorStack);

		if('(' == *(char *)leftnum) {
			free(leftnum);
			push(operandStack,(void *)rightnum);
			push(operatorStack,(void *)operator);
			break;
		}
			yes = compute(leftnum,rightnum,operator);
			free(rightnum);
			free(operator);
	 		push(operandStack,(void *)leftnum);		
	}

	 return yes;
}

static boolean isNumber(int ch) {
	return isdigit(ch) || '+' == ch ||
			'-' == ch || '.' == ch;
}

static boolean isOperator(int ch) {
	int index;

	if(ch == '\0') {
		return FALSE;
	}

	for(index = 0; OPERATORLIST[index] && ch != OPERATORLIST[index]; index++)
		;
	return ch == OPERATORLIST[index];
}

static void dealExperssionStatusBegin(char *str, int ch, ARG_EXP *arg) {
	if (isNumber(ch)) {
		
		int count = 0;
		double result = 0;
		double *value;

		if(!(arg->ok = getNumber(str + arg->index, &result, &count))) {
			return;
		}

		//��������
		value = (double *) calloc(sizeof(double),1);
		*value = result;
		push(arg->operandStack, (void *) value);

		arg->status = EXPERSSION_STATUS_OPERAND;
		arg->index += count;
	} else if('(' == ch) {
		// TODO ����'(';

		//'��'��ջ
		char *bracket = (char *) calloc(sizeof(char),1);
		*bracket = ch;
		push(arg->operandStack,(void *) bracket);

		arg->bracket++;
		arg->index++;
		arg->status = EXPERSSION_STATUS_BEGIN;
	} else {
		errorMess = "������ʽ��";
		arg->ok = FALSE;
		arg->status = EXPERSSION_STATUS_END;
	}
}

static void dealExperssionStatusOperand(char *srt, int ch, ARG_EXP *arg) {

	if(isOperator(ch)) {
		//TODO������ ��������
		//��ջ��ǿ�ջ��������������ջ
		if(isOperatorStackEmpty(arg->operatorStack)) {
			char *operator = (char *) calloc(sizeof(char),1);			
			*operator = ch;   
			push(arg->operatorStack,(void *)operator);
		} else { 
			char pre;
			pre = *((char *)readTop(arg->operatorStack));

			//������s�Ƚ�
			if(compare(ch,pre)) {
				char *operator = (char *) calloc(sizeof(char),1);
				*operator = ch;   
				push(arg->operatorStack,(void *)operator);
			} else {
				//���㴦��
				char *operator;
				if(FALSE == progress(arg->operatorStack,arg->operandStack)) {
					arg->ok = FALSE;
					return;
				}
				//��������ջ
				operator = (char *) calloc(sizeof(char),1);
				*operator = ch;   
				push(arg->operatorStack,(void *)operator);				
			}
		}
		arg->status = EXPERSSION_STATUS_BEGIN;
		arg->index++;
	} else if (')' == ch) {
			//TODO ')'
			if(--arg->bracket < 0) {
				arg->ok = FALSE;
				errorMess = "\')\'��ƥ��!";
				return;
			}
			if(FALSE == progress(arg->operatorStack,arg->operandStack)) {
					arg->ok = FALSE;
					return;
			}
			arg->index++;
			arg->status = EXPERSSION_STATUS_OPERAND;
	} else {
		arg->status = EXPERSSION_STATUS_END;	
	}
}

boolean experssionAnalysis(char *str, double *value) {
	ARG_EXP arg = {
		0.0,							// double value;
		EXPERSSION_STATUS_BEGIN,		// int status;
		TRUE,							// boolean ok;
		FALSE,							// boolean finished;
		0,								// int index;		
		0,								// int bracket;
		NULL,							//MY_STACK *operatorStack;
		NULL,							//MY_STACK *operandStack;
	};

	//��ջ�
	initStack(&arg.operandStack, strlen(str) / 2 + 1 );
	initStack(&arg.operatorStack, strlen(str) / 2);

	//����
	while(arg.ok && !arg.finished) {
		if(EXPERSSION_STATUS_BEGIN == arg.status) {
			dealExperssionStatusBegin(str, str[arg.index], &arg);
		} else if (EXPERSSION_STATUS_OPERAND == arg.status) {
			dealExperssionStatusOperand(str, str[arg.index], &arg);
		} else if (EXPERSSION_STATUS_END == arg.status) {
			arg.finished = str[arg.index] == '\0'? TRUE : FALSE;

			if(arg.bracket > 0) {
				arg.ok = FALSE;
				errorMess = "\'(\'��ƥ��!";
			} else if(arg.ok && arg.finished) {
				//���㴦��
				if(FALSE == progress(arg.operatorStack,arg.operandStack)) {
					arg.ok = FALSE;
				} else {
					//��������ջ
					double *result = (double *)pop(arg.operandStack);
					arg.value = *result;
					free(result);		
				}
				//end

			} else {
				errorMess = "������ʽ��";
				arg.ok = FALSE;
			}

		}
	}

	//�ͷſռ�
	freeData(arg.operatorStack);
	freeData(arg.operandStack);
	destoryStack(&arg.operatorStack);
	destoryStack(&arg.operandStack);
	
	//���
	*value = arg.value;
	return arg.ok;
} 
