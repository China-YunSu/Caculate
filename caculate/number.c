#include <stdio.h>
#include <ctype.h>


#include "./include/boolean.h"
#include "./include/number.h"

extern const char *errNo;

static void progressNumberInteger(int ch, ARG *arg) {
	 arg->value = arg->value * 10 + (ch - '0');
}

static void progressNumberDecimal(int ch, ARG *arg) {
	if (arg->pw < 1e-6) {
		arg->finished = TRUE;
		return;
	}

	arg->value = (ch - '0') * arg->pw + arg->value;
	arg->pw /= 10.0;
}

static void dealNumberStatusInteger(int ch, ARG *arg) {
	if ('.' == ch) {
		arg->status = NUMBER_STATUS_DOT;
		arg->i++;
	} else if (isdigit(ch)) {
		progressNumberInteger(ch, arg);
		arg->status = NUMBER_STATUS_INTEGER;
		arg->i++;
	} else {
		arg->status = NUMBER_STATUS_END;
	}
}

static void dealNumberStatusDot(int ch, ARG *arg) {
	if (isdigit(ch)) {
		progressNumberDecimal(ch, arg);
		arg->status = NUMBER_STATUS_DECIMAL;
		arg->i++;
	} else {
		printf("小数点后缺少数字"); 
		arg->ok = FALSE;
	}

}

static void dealNumberStatusSign(int ch, ARG *arg) {
	if ('.' == ch) {
		arg->status = NUMBER_STATUS_DOT;
		arg->i++;
	} else if (isdigit(ch)) {
		progressNumberInteger(ch, arg);
		arg->status = NUMBER_STATUS_INTEGER;
		arg->i++;
	} else {
		printf("缺少数字");
		arg->ok = FALSE;
	}
}

static void dealNumberStatusDecimal(int ch, ARG *arg) {
	if (isdigit(ch)) {
		progressNumberDecimal(ch, arg);
		arg->status = NUMBER_STATUS_DECIMAL;
		arg->i++;
	} else {
		arg->status = NUMBER_STATUS_END;
	}
}

static void dealNumberStatusBegin(int ch, ARG *arg) {

	if (isdigit(ch)) {
		progressNumberInteger(ch, arg);
		arg->status = NUMBER_STATUS_INTEGER;
		arg->i++;
	} else if ('+' == ch || '-' == ch){
		if('-' == ch) {
			arg->sign = -1;
		}

		arg->i++;
		arg->status = NUMBER_STATUS_SIGN;
	} else if ('.' == ch) {
		arg->status = NUMBER_STATUS_DOT;
		arg->i++;
	} else {
		printf("出师未捷身先死");
		arg->ok = FALSE;
	}
}

boolean getNumber(const char *str, double *result, int *count) {
	ARG arg = {
		TRUE,			// 	boolean ok;
		FALSE,			// boolean finished;
		NUMBER_STATUS_BEGIN,			// int status;
		0.0,					// double value;
		0.1,			// double pw;
		0,			// int i;
		1            // int sign
	};

	while (arg.ok && !arg.finished) {
		if (arg.status == NUMBER_STATUS_BEGIN) {
			dealNumberStatusBegin(str[arg.i], &arg);
		} else if (arg.status == NUMBER_STATUS_DOT) {
			dealNumberStatusDot(str[arg.i], &arg);
		} else if (arg.status == NUMBER_STATUS_INTEGER) {
			dealNumberStatusInteger(str[arg.i], &arg);
		} else if (arg.status == NUMBER_STATUS_SIGN) {
			dealNumberStatusSign(str[arg.i], &arg);
		} else if (arg.status == NUMBER_STATUS_DECIMAL) {
			dealNumberStatusDecimal(str[arg.i], &arg);
		} else {
			*result = arg.sign == 1 ? arg.value : -arg.value;
			*count = arg.i;
			arg.finished = TRUE;
		}
	}

	return arg.ok;
}