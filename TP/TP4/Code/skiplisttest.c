#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "skiplist.h"


/**
 *	@defgroup SkipListTest Test program for SkipList Implantation
 *  @brief Definition of the SkipList type and operators
 *  @{
 */

/** Print a usage message for this program.
 
 @code{.unparsed}
 $skiplisttest -id num
 where id is :
 c : construct and print the skiplist with data read from file ../Test/test_files/construct_num.txt
 s : construct the skiplist with data read from file ../Test/test_files/construct_num.txt and search elements from file test_files/search_num..txt
 Print statistics about the searches.
 i : construct the skiplist with data read from file ../Test/test_files/construct_num.txt and search, using an iterator, elements read from file test_files/search_num.txt
 Print statistics about the searches.
 r : construct the skiplist with data read from file test_files/construct_num.txt, remove values read from file test_files/remove_num.txt and print the list in reverse order
 
 and num is the file number for input.
 @endcode
 */
void usage(const char *command) {
	printf("usage : %s -id num\n", command);
	printf("where id is :\n");
	printf("\tc : construct and print the skiplist with data read from file test_files/construct_num.txt\n");
	printf("\ts : construct the skiplist with data read from file test_files/construct_num.txt and search elements from file test_files/search_num..txt\n\t\tPrint statistics about the searches.\n");
	printf("\ti : construct the skiplist with data read from file test_files/construct_num.txt and search, using an iterator, elements read from file test_files/search_num.txt\n\t\tPrint statistics about the searches.\n");
	printf("\tr : construct the skiplist with data read from file test_files/construct_num.txt, remove values read from file test_files/remove_num.txt and print the list in reverse order\n");
	printf("and num is the file number for input\n");
}

/** Return the filename associated with the action to perform and the number of a test.
 
 @param action Describe the action associated with the file to load. Must be one of "construct" "search" or "remove"
 @param filenumber Number of the file to load. Corresponds to the number of the test to perform.
 @return fileame  in a relative form
 
 @code{.c}
 char *filename = gettestfilename("construct", 2);
 @endcode
 will set filename to @code{.c}"../Test/test_files/construct_2.txt"@endcode
 
 */
char *gettestfilename(const char *action, int numtest) {
	const char *testdir ="../Test/test_files/";
	int l = strlen(testdir) + strlen(action) + strlen("_xxx.txt");
	char *name = malloc(l*sizeof(char));
	sprintf(name, "%s%s_%d.txt", testdir, action, numtest);
	return name;
}

/** Build a list corresponding to the fiven file number.
 */
SkipList buildlist(int num) {
	SkipList d;
	FILE *input;
	unsigned int level;
	unsigned int nb_values;
	int value;
	
	char *constructfromfile = gettestfilename("construct", num);
	input = fopen(constructfromfile, "r");
	if (input!=NULL) {
		
		if (fscanf(input, "%u", &level) == -1) {
			perror("Bad file descriptor");
			exit(EXIT_FAILURE);
		}
		d = skiplist_create(level);
		
		if (fscanf(input, "%u", &nb_values) == -1) {
			perror("Bad file descriptor");
			exit(EXIT_FAILURE);
		}
		for (unsigned int i=0;i< nb_values; ++i) {
			if (fscanf(input, "%d", &value) == -1) {
				perror("Bad file descriptor");
				exit(EXIT_FAILURE);
			}
			d = skiplist_insert(d, value);
		}
	} else {
		printf("Unable to open file %s\n", constructfromfile);
		free(constructfromfile);
		exit (1);
	}
	free(constructfromfile);
	fclose(input);
	return d;
}

/* Affiche un entier. L'environnement n'est pas utilisé */
void print_value(int v, void* env) {
	if (!env) printf("%d ", v);
}

/*----------------------------------------------------------------------------------------------*/

/** Exercice 1.
 	Programming and test of skiplist construction.
 */
void test_construction(int num){
	SkipList d = buildlist(num);
	printf("Skiplist (%d)\n", skiplist_size(d));
	skiplist_map(d, print_value, NULL);
	printf("\n");
	skiplist_delete(d);
}

/** Exercice 2.
 Programming and test of skiplist search operator.
 */
void test_search(int num){
	SkipList d = buildlist(num);
	FILE *input;
	unsigned int nb_values;
	unsigned int nb_operations_local = 0, nb_operations_total = 0;
	unsigned int nb_operations_min = UINT_MAX, nb_operations_max = 0;;
	unsigned int values_found = 0, values_not_found = 0;
	int value;
	
	char *constructfromfile = gettestfilename("search", num);
	input = fopen(constructfromfile, "r");
	if (input!=NULL) {
		
		if (fscanf(input, "%u", &nb_values) == -1) {
			perror("Bad file descriptor");
			exit(EXIT_FAILURE);
		}
		for (unsigned int i=0;i< nb_values; ++i) {
			if (fscanf(input, "%d", &value) == -1) {
				perror("Bad file descriptor");
				exit(EXIT_FAILURE);
			}
			
			if (skiplist_search(d, value, &nb_operations_local)) {
				++values_found;
				printf("%d -> true\n", value);
			} else {
				++values_not_found;
				printf("%d -> false\n", value);
			}

			/* Calcul du nombre total d'opérations */
			nb_operations_total += nb_operations_local;

			/* Calcul du nombre minimal et maximal d'opérations */
			if (nb_operations_local > nb_operations_max) {
				nb_operations_max = nb_operations_local;
			}
			if (nb_operations_local < nb_operations_min) {
				nb_operations_min = nb_operations_local;
			}
		}
	} else {
		printf("Unable to open file %s\n", constructfromfile);
		free(constructfromfile);
		exit (1);
	}
	free(constructfromfile);
	fclose(input);

	/* Affichage des statistiques */
	printf("Statistics :\n\tSize of the list : %d\nSearch %d values :\n", skiplist_size(d), nb_values);
	printf("\tFound %d\n", values_found);
	printf("\tNot found %d\n", values_not_found);
	printf("\tMin number of operations : %u\n", nb_operations_min);
	printf("\tMax number of operations : %u\n", nb_operations_max);
	printf("\tMean number of operations : %u\n", nb_operations_total / nb_values);

	skiplist_delete(d);
}

/** Exercice 3.
 Programming and test of naïve search operator using iterators.
 */
void test_search_iterator(int num){
	SkipList d = buildlist(num);
	SkipListIterator e = skiplist_iterator_create(d, FORWARD_ITERATOR);
	FILE *input;
	unsigned int nb_values;
	unsigned int nb_operations_local = 0, nb_operations_total = 0;
	unsigned int nb_operations_min = UINT_MAX, nb_operations_max = 0;;
	unsigned int values_found = 0, values_not_found = 0;
	int value;
	
	char *constructfromfile = gettestfilename("search", num);
	input = fopen(constructfromfile, "r");
	if (input!=NULL) {
		
		if (fscanf(input, "%u", &nb_values) == -1) {
			perror("Bad file descriptor");
			exit(EXIT_FAILURE);
		}
		for (unsigned int i=0;i< nb_values; ++i) {
			if (fscanf(input, "%d", &value) == -1) {
				perror("Bad file descriptor");
				exit(EXIT_FAILURE);
			}
			
			nb_operations_local = 0;

			for (e = skiplist_iterator_begin(e); !skiplist_iterator_end(e); e = skiplist_iterator_next(e)) {
				++nb_operations_local;
				if (skiplist_iterator_value(e) == value) {
					++values_found;
					printf("%d -> true\n", value);
					break;
				}
			}

			if (skiplist_iterator_end(e)) {
				++values_not_found;
				printf("%d -> false\n", value);
			}

			/* Calcul du nombre total d'opérations */
			nb_operations_total += nb_operations_local;

			/* Calcul du nombre minimal et maximal d'opérations */
			if (nb_operations_local > nb_operations_max) {
				nb_operations_max = nb_operations_local;
			}
			if (nb_operations_local < nb_operations_min) {
				nb_operations_min = nb_operations_local;
			}
		}
	} else {
		printf("Unable to open file %s\n", constructfromfile);
		free(constructfromfile);
		exit (1);
	}
	free(constructfromfile);
	fclose(input);

	/* Affichage des statistiques */
	printf("Statistics :\n\tSize of the list : %d\nSearch %d values :\n", skiplist_size(d), nb_values);
	printf("\tFound %d\n", values_found);
	printf("\tNot found %d\n", values_not_found);
	printf("\tMin number of operations : %u\n", nb_operations_min);
	printf("\tMax number of operations : %u\n", nb_operations_max);
	printf("\tMean number of operations : %u\n", nb_operations_total / nb_values);

	skiplist_delete(d);
	skiplist_iterator_delete(e);
}

/** Exercice 4.
 Programming and test of skiplist remove operator.
 */
void test_remove(int num){
	SkipList d = buildlist(num);
	FILE *input;
	unsigned int nb_values;
	int value;
	
	char *constructfromfile = gettestfilename("remove", num);
	input = fopen(constructfromfile, "r");
	if (input!=NULL) {
		
		if (fscanf(input, "%u", &nb_values) == -1) {
			perror("Bad file descriptor");
			exit(EXIT_FAILURE);
		}
		for (unsigned int i=0;i< nb_values; ++i) {
			if (fscanf(input, "%d", &value) == -1) {
				perror("Bad file descriptor");
				exit(EXIT_FAILURE);
			}
			d = skiplist_remove(d, value);
		}
	} else {
		printf("Unable to open file %s\n", constructfromfile);
		free(constructfromfile);
		exit (1);
	}
	free(constructfromfile);
	fclose(input);

	/* Affichage dans l'ordre décroissant des valeurs en utilisant un itérateur (fonctionne avec une fonction map)*/
	printf("Skiplist (%d)\n", skiplist_size(d));
	SkipListIterator e = skiplist_iterator_create(d, BACKWARD_ITERATOR);
	for(e=skiplist_iterator_begin(e); !skiplist_iterator_end(e); e=skiplist_iterator_next(e))
		printf("%d ", skiplist_iterator_value(e));
	printf("\n");
	
	skiplist_delete(d);
	skiplist_iterator_delete(e);
}

/** Function you ca use to generate dataset for testing.
 */
void generate(int nbvalues);


int main(int argc, const char *argv[]){
	if (argc < 3) {
		usage(argv[0]);
		return 1;
	}
	switch (argv[1][1]) {
		case 'c' :
			test_construction(atoi(argv[2]));
			break;
		case 's' :
			test_search(atoi(argv[2]));
			break;
		case 'i' :
			test_search_iterator(atoi(argv[2]));
			break;
		case 'r' :
			test_remove(atoi(argv[2]));
			break;
		case 'g' :
			generate(atoi(argv[2]));
			break;
		default :
			usage(argv[0]);
			return 1;
	}
	return 0;
}


/* Generates a set of test files for a given number of value. */

void generate(int nbvalues){
	FILE *output;
	int depth;
	int maxvalue;
	output = fopen("construct.txt", "w");
	srand(nbvalues);
	depth = rand()%16;
	maxvalue = rand()%10 * nbvalues;
	fprintf(output, "%d\n%d\n", depth, nbvalues);
	for (int i=0; i< nbvalues; ++i) {
		fprintf(output, "%d\n", rand()%maxvalue);
	}
	fclose(output);
	output = fopen("search.txt", "w");
	srand(rand());
	nbvalues *= depth/4;
	fprintf(output, "%d\n", nbvalues);
	for (int i=0; i< nbvalues; ++i) {
		fprintf(output, "%d\n", rand()%maxvalue);
	}
	fclose(output);
}


/** @} */

/**
 @mainpage
 @section TP45 Implantation du TAD Listes à raccourci.
 @subsection SkipLists SkipLists
 Les listes à raccourcis, nommées SkipLists, sont une alternative aux arbres de
 recherche équilibrés que nous verrons dans la suite du cours.
 
 Inventées par William Pugh en 1902, elles reposent sur une structure de données
 linéaire construite de manière probabiliste.
 Les opérations de dictionnaire définies sur les listes à raccourcis sont simples à
 programmer, élégantes, et l’aspect probabiliste permet de démontrer une complexité
 moyenne en O(log(N)) au lieu des O(N) inhérents aux structures linéaires.
 
 Toutefois, l’aspect probabiliste de la structure de données ne permet pas d’assurer
 la complexité en pire cas, comme on pourra le faire sur les arbres équilibrés, mais
 donne, en pratique, une structure extrêmement efficace (la probabilité de construire
 une structure non efficace étant très faible). Nous ne ferons pas de démonstration
 ici, et il ne vous est pas demandé d’en effectuer une, les étudiants curieux
 pourront se rapporter à leur cours de complexité et à l’analyse fournie dans
 l’article de William Pugh, accessible sur Moodle et sur la <a href="https://dl.acm.org/citation.cfm?id=78977"> bibliothèque ACM</a>.
 
 @note \anchor WP-skiplist-1990 William Pugh, <a href="https://dl.acm.org/citation.cfm?id=78977">Skip lists : a probabilistic alternative to balanced trees</a> in Communications of the ACM, June 1990, 33(6) 668-676
 
 @subsection Objectifs Objectifs du TP
 - Implantation du TAD SkipList
 - Implantation d'un itérateur sur une SkipList
 - Comparaison empirique des opérations de dictionnaires sur les SkipList et en utilisant un itérateur.
 
 */
