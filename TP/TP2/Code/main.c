// Need this to use the getline C function on Linux. Works without this on MacOs. Not tested on Windows.
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "token.h"
#include "queue.h"
#include "stack.h"

#define BUFSIZE 32
#define MALLOC_ERROR 99


/* ========================================================================================= */

bool isSymbol(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')';
}

/* ========================================================================================= */

bool isNumValue(char c) {
	return 48 <= c && c <= 57;
}

/* ========================================================================================= */

Queue* stringToTokenQueue(const char* expression) {
	Queue* queue = createQueue();
	Token* token;
	const char* curpos = expression;
	const char *numberPtr;
	int valueLength;

	while (*curpos != '\0') {
		if (!isSymbol(*curpos) && !isNumValue(*curpos)) curpos++;
		else {
			if (isSymbol(*curpos)) {
				token = createTokenFromString(curpos, 1);
			} else {
				valueLength = 0;
				numberPtr = curpos;
				while (isNumValue(*numberPtr) || *numberPtr == '.') {
					valueLength++;
					numberPtr++;
				}
				token = createTokenFromString(curpos, valueLength);
			}
			queue = queuePush(queue, token);
			curpos++;
		}
	}
	return queue;
}

/* ========================================================================================= */

void printToken(FILE *f, const void* e) {tokenDump(f, (Token*)e);}

/* ========================================================================================= */

Token* getTopAndPop(Stack* s) {
	Token* t = (Token*)stackTop(s);
	s = stackPop(s);
	return t;
}

/* ========================================================================================= */

void freeTokenQueue(ptrQueue* q) {
	Token* t;
	while(!queueEmpty(*q)) {
		t = (Token*)queueTop(*q);
		deleteToken(&t);
		*q = queuePop(*q);
	}
	free(*q);
	*q = NULL;
}

/* ========================================================================================= */

Queue* shuntingYard(Queue* infix) {
	Queue* postfix = createQueue();
	Stack* operatorStack = createStack(queueSize(infix));

	while (!queueEmpty(infix)) {
		Token* token = (Token*)queueTop(infix);
		if (tokenIsNumber(token)) {
			postfix = queuePush(postfix, token);
		}
		if (tokenIsOperator(token)) {
			while (!stackEmpty(operatorStack) 
				&& (tokenGetOperatorPriority((Token*)stackTop(operatorStack)) > tokenGetOperatorPriority(token)
				|| (tokenGetOperatorPriority((Token*)stackTop(operatorStack)) == tokenGetOperatorPriority(token) 
				&& tokenOperatorIsLeftAssociative(token) 
				&& (!tokenIsParenthesis((Token*)stackTop(operatorStack)) 
				|| tokenGetParenthesisSymbol((Token*)stackTop(operatorStack)) != '('))) )
			{
				postfix = queuePush(postfix, getTopAndPop(operatorStack));
			}
			operatorStack = stackPush(operatorStack, token);
		}
		if (tokenIsParenthesis(token) && tokenGetParenthesisSymbol(token) == '(') {
			operatorStack = stackPush(operatorStack, token);
		} 
		if (tokenIsParenthesis(token) && tokenGetParenthesisSymbol(token) == ')') {
			while (!stackEmpty(operatorStack) && (!tokenIsParenthesis((Token*)stackTop(operatorStack)) || (tokenGetParenthesisSymbol((Token*)stackTop(operatorStack)) != '(')))
				postfix = queuePush(postfix, getTopAndPop(operatorStack));
			if (!stackEmpty(operatorStack)) {
				Token* toDelete = (Token*)stackTop(operatorStack);
				operatorStack = stackPop(operatorStack);
				deleteToken(&toDelete);
				deleteToken(&token);
			}
		}
		infix = queuePop(infix);
	}

	if (queueEmpty(infix))
		while (!stackEmpty(operatorStack) && tokenIsOperator((Token*)stackTop(operatorStack)))
			postfix = queuePush(postfix, getTopAndPop(operatorStack));

	deleteStack(&operatorStack);
	// deleteQueue(&infix);

	return postfix;
}

/* ========================================================================================= */

Token* evaluateOperator(Token* arg1, Token* op, Token* arg2) {
	float operand1 = tokenGetValue(arg1);
	float operand2 = tokenGetValue(arg2);
	switch(tokenGetOperatorSymbol(op)) {
		case '+': return createTokenFromValue(operand1 + operand2);
		case '-': return createTokenFromValue(operand1 - operand2);
		case '*': return createTokenFromValue(operand1 * operand2);
		case '/': return createTokenFromValue(operand1 / operand2);
		case '^': return createTokenFromValue(pow(operand1, operand2));
		default: return NULL;
	}
}

/* ========================================================================================= */

float evaluateExpression(Queue* postfix) {
	Stack* s = createStack(queueSize(postfix));
	while (!queueEmpty(postfix)) {
		Token* token = (Token*)queueTop(postfix);
		if (tokenIsOperator(token)) {
			Token* operand2 = getTopAndPop(s);
			Token* operand1 = getTopAndPop(s);
			Token* result = evaluateOperator(operand1, token, operand2);
			s = stackPush(s, result);
			deleteToken(&operand2);
			deleteToken(&operand1);
			deleteToken(&token);
		}
		else {
			s = stackPush(s, token);
		}
		postfix = queuePop(postfix);
		
	}
	Token* lastToken = (Token*)stackTop(s);
	float evalValue = tokenGetValue(lastToken);
	deleteToken(&lastToken);
	deleteStack(&s);
	//deleteQueue(&postfix);
	return evalValue;
}

/* ========================================================================================= */

void computeExpressions(FILE* input) {
	char* buffer;
	size_t bufsize = (size_t)BUFSIZE;
	size_t string;
	Queue* queueInfix;
	Queue* queuePostfix;
	float evaluationValue;

	buffer = (char *)malloc(bufsize * sizeof(char));
	if (buffer == NULL) {
		perror("Allocation for buffer has failed.");
		exit(MALLOC_ERROR);
	}

	string = getline(&buffer, &bufsize, input);

	while (!feof(input)) {
		if (string > 1) {
			printf("Input\t : %s", buffer);

			queueInfix = stringToTokenQueue(buffer);
			printf("Infix\t : ");
			queueDump(stdout, queueInfix, printToken);
			printf("\n");

			queuePostfix = shuntingYard(queueInfix);
			printf("Postfix\t : ");
			queueDump(stdout, queuePostfix, printToken);
			printf("\n");

			evaluationValue = evaluateExpression(queuePostfix);
			printf("Evaluate : %f", evaluationValue);
			printf("\n\n");

			deleteQueue(&queueInfix);
			deleteQueue(&queuePostfix);
			// freeTokenQueue(&queueInfix);
			// freeTokenQueue(&queuePostfix);
		}
		string = getline(&buffer, &bufsize, input);
	}

	free(buffer);
}

/* ========================================================================================= */

/** Main function for testing.
 * The main function expects one parameter that is the file where expressions to translate are
 * to be read.
 *
 * This file must contain a valid expression on each line
 *
 */
int main(int argc, char **argv){
	if (argc<2) {
		fprintf(stderr,"usage : %s filename\n", argv[0]);
		return 1;
	}
	
	FILE *input = fopen(argv[1], "r");

	if ( !input ) {
		perror(argv[1]);
		return 1;
	}

	computeExpressions(input);

	fclose(input);
	return 0;
}
 