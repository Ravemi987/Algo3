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

/* Teste si un caractère est un symbole pouvant être évalué par le programme */
bool isSymbol(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')';
}

/* ========================================================================================= */

/* Teste si un caractère est un chiffre. */
bool isNumValue(char c) {
	return 48 <= c && c <= 57;
}

/* ========================================================================================= */

/* Transforme une chaîne de caractères en file de tokens en notation infixe. */
Queue* stringToTokenQueue(const char* expression) {
	Queue* queue = createQueue(); /* File de tokens qui contiendra le résultat de la transformation */ 
	Token* token;
	const char* curpos = expression;
	const char *numberPtr; /* pointeur temporaire permettant de délimiter des valeurs numériques */
	int valueLength; /* compteur du nombre de caractères composant une valeur numérique */

	while (*curpos != '\0') {
		if (!isSymbol(*curpos) && !isNumValue(*curpos)) curpos++;
		else {
			if (isSymbol(*curpos)) {
				token = createTokenFromString(curpos, 1); /* Le token ne contient qu'un seul caractère*/
			} else {
				/* curpos pointe sur une valeur numérique */
				valueLength = 0; /* Initisalisation du compteur à 0 */
				numberPtr = curpos; /* Recherche de la fin du nombre à partir du curseur */
				while (isNumValue(*numberPtr) || *numberPtr == '.') {
					/* Si le caractère est un chiffre ou un nombre flottant, on incrémente la longueur et on passe au suivant */
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

/* Affiche le token représenté par le pointeur non typé 'e' sur le flux de sortie 'f' */
void printToken(FILE *f, const void* e) {
	tokenDump(f, (Token*)e);
}

/* ========================================================================================= */

/* Supprime un élément d'une pile, et renvoie l'élément qui était en tête avant la suppression */
Token* getTopAndPop(Stack* s) {
	Token* t = (Token*)stackTop(s);
	s = stackPop(s);
	return t;
}

/* ========================================================================================= */

/* Libère les ressources (token) allouées à l'extérieur du gestionnaire de collection (la file). */
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

/*Teste si un token est une parenthèse gauche */
bool tokenIsLeftParenthesis(Token *t) {
	return tokenIsParenthesis(t) && (tokenGetParenthesisSymbol(t) == '(');
} 

/* ========================================================================================= */

/* Teste si un token est une parenthèse droite */
bool tokenIsRightParenthesis(Token *t) {
	return tokenIsParenthesis(t) && (tokenGetParenthesisSymbol(t) == ')');
}

/* ========================================================================================= */

/** Transforme une expression arithmétique représentée par une file de tokens en notation infixe, 
 * en une file de tokens représentant l’expression en notation postfixe.
 */
Queue* shuntingYard(Queue* infix) {
	Queue* postfix = createQueue(); /* File qui contiendra les tokens en notation postfixe */
	Stack* opStack = createStack(queueSize(infix)); /* Pile temporaire utiliée par l'algorithme */

	while (!queueEmpty(infix)) {
		Token* token = (Token*)queueTop(infix);
		if (tokenIsNumber(token)) {
			postfix = queuePush(postfix, token);
		}
		if (tokenIsOperator(token)) {
			while (!stackEmpty(opStack) 
				&& (tokenGetOperatorPriority((Token*)stackTop(opStack)) > tokenGetOperatorPriority(token)
				|| (tokenGetOperatorPriority((Token*)stackTop(opStack)) == tokenGetOperatorPriority(token) 
				&& tokenOperatorIsLeftAssociative(token) 
				&& !tokenIsLeftParenthesis((Token*)stackTop(opStack))))) 
			{
				postfix = queuePush(postfix, getTopAndPop(opStack));
			}
			opStack = stackPush(opStack, token);
		}
		if (tokenIsLeftParenthesis(token)) {
			opStack = stackPush(opStack, token);
		} 
		if (tokenIsRightParenthesis(token)) {
			while (!stackEmpty(opStack) && !tokenIsLeftParenthesis((Token*)stackTop(opStack))) {
				postfix = queuePush(postfix, getTopAndPop(opStack));
			}
			if (!stackEmpty(opStack)) {
				/* Si la pile n'est pas vide, le top contient une parenthèse gauche */
				Token* toDelete = (Token*)stackTop(opStack); /* Récupération de la parenthèse gauche à supprimer */
				opStack = stackPop(opStack);
				/* Libération des tokens (parenthèses droites et gauches) qui ne seront plus utilisés */
				deleteToken(&toDelete);
				deleteToken(&token);
			}
		}
		infix = queuePop(infix);
	}
	if (queueEmpty(infix)) {
		while (!stackEmpty(opStack) && tokenIsOperator((Token*)stackTop(opStack))) {
			postfix = queuePush(postfix, getTopAndPop(opStack));
		}
	}
	/* Libère la pile créée au début de la fonction et vidée par l'exécution de l'algorithme */
	deleteStack(&opStack);
	// deleteQueue(&infix);
	return postfix;
}

/* ========================================================================================= */

/* Evalue l'application d'un opérateur sur deux opérandes et retourne un nouveau token résultat de ce calcul. */
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

/** Renvoie la valeur de l'évaluation d'une expression arithmétique représentée par une file de tokens 
 * en notation postfixe (algorithme d'évaluation en notation Polonaise inversée).
 */
float evaluateExpression(Queue* postfix) {
	/* Création d'une pile temporaire de nombres, utilisée par l'algorithme */
	Stack* stack = createStack(queueSize(postfix));
	Token *token, *operand2, *operand1, *result;
	while (!queueEmpty(postfix)) {
		token = (Token*)queueTop(postfix);
		if (tokenIsOperator(token)) {
			/* Le token est un opérateur, on évalue les deux éléments en tête de pile */
			operand2 = getTopAndPop(stack);
			operand1 = getTopAndPop(stack);
			result = evaluateOperator(operand1, token, operand2);
			/* On push le résultat sur la pile (nouvelle opérande) */
			stack = stackPush(stack, result);
			/* Libération des tokens qui ne seront plus utilisés */
			deleteToken(&operand2);
			deleteToken(&operand1);
			deleteToken(&token);
		}
		else {
			/* On stocke toutes les opérandes dans la pile temporaire */
			stack = stackPush(stack, token);
		}
		postfix = queuePop(postfix);
		
	}
	/* Création d'un pointeur vers le resultat final (dernier élément de la pile) */
	Token* lastToken = (Token*)stackTop(stack);
	float evalValue = tokenGetValue(lastToken);
	/* On libère ce dernier élément ainsi que la pile temporaire (maintenant vide) */
	deleteToken(&lastToken);
	deleteStack(&stack);
	//deleteQueue(&postfix);
	return evalValue;
}

/* ========================================================================================= */

/** Lit dans le flux d'entrée ouvert 'input' des chaînes de caractères correspondant
 * à des expressions arithmétiques et affiche le résultat des appels aux différentes fonctions de traitement.
 */
void computeExpressions(FILE* input) {
	char* buffer;
	size_t bufsize = (size_t)BUFSIZE;
	size_t expression; /* Chaîne de caractères lue à partir d'une ligne d'un fichier */
	Queue* queueInfix; /* File de tokens en notation infixe */
	Queue* queuePostfix; /* File de tokens en notation postfixe */
	float evaluationValue; /* Valeur d'une expression arithmétique réprésentée par une chaîne de caractères */

	buffer = (char *)malloc(bufsize * sizeof(char));
	/* Traite le cas rare où le buffer ne peut pas accéder à l'espace mémoire demandé. */
	if (buffer == NULL) {
		perror("Allocation for buffer has failed.");
		exit(MALLOC_ERROR);
	}

	/* On stocke l'expression dans le buffer*/
	expression = getline(&buffer, &bufsize, input);

	while (!feof(input)) {
		/* On ne traite que les expressions arithmétiques contenant au moins 2 caractères */
		if (expression > 1) {
			printf("Input\t : %s", buffer);

			/* conversion de la chaîne en une file de tokens */
			queueInfix = stringToTokenQueue(buffer);
			printf("Infix\t : ");
			queueDump(stdout, queueInfix, printToken);
			printf("\n");

			/* conversion en notation postfixe avec l'algorithme de Shunting Yard */
			queuePostfix = shuntingYard(queueInfix);
			printf("Postfix\t : ");
			queueDump(stdout, queuePostfix, printToken);
			printf("\n");

			/* évaluation à l'aide de l'algorithme d'évaluation en notation Polonaise inversée */ 
			evaluationValue = evaluateExpression(queuePostfix);
			printf("Evaluate : %f", evaluationValue);
			printf("\n\n");

			/* On libère les files (maintenant vides) crées au début de la fonction */
			deleteQueue(&queueInfix);
			deleteQueue(&queuePostfix);
			// freeTokenQueue(&queueInfix);
			// freeTokenQueue(&queuePostfix);
		}
		expression = getline(&buffer, &bufsize, input);
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
 