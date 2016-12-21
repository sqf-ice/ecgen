
#include <pari/pari.h>

#include <stdio.h>
#include <readline/readline.h>

#include "gp.h"

char *readHex(const char* prompt) {
    char *r = readline(prompt);
    size_t len = strlen(r);
    char *result = realloc(r, len + 3);
    memmove(result + 2, result, len);
    result[0] = '0';
    result[1] = 'x';
    result[len + 2] = 0;
    return result;
}

int main(int argc, char * argv[]) {
	GEN (*todo)(GEN,GEN,GEN,long) = &largest_prime;
	void (*print)(GEN) = &print_params;

	if (argc > 1) {
		if (!strcmp(argv[1], "-l")) {
		todo = &largest_prime;
		print = &print_params;
		} else if (!strcmp(argv[1], "-s")) {
		todo = &smallest_prime;
		print = &print_params;
		} else if (!strcmp(argv[1], "-p")) {
		todo = &small_pubkey;
		print = &print_params_pub;
		}
	}

	init_gp();
	pari_init( 1e9, 1e8 );
	
	//default0("datadir","./data");

	char *sp = readHex("p:");
	char *sa = readHex("a:");
	char *sb = readHex("b:");

	GEN p = strtoi(sp);
	GEN a = strtoi(sa);
	GEN b = strtoi(sb);
	free(sp);
	free(sa);
	free(sb);

	GEN result = (*todo)(p, a, b, 0);
	(*print)(result);
	pari_close();
	return 0;
}
