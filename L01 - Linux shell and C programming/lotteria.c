#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

// <campus><matricola>
// campus := 2 char
// matric := 10 char

char * ALLOWED_CAMPUS[] = { "BO", "CE", "FO", "RA" };
int ALL_CAM_SIZE = 5;

bool is_stud_ok(char * stud)
{
	int len, i;
	bool ret;

	ret = false;
	for (i = 0; i < ALL_CAM_SIZE; i++){
		ret |= (strncmp(stud, ALLOWED_CAMPUS[i], 2) == 0);
	}

	len = strlen(stud);
	ret &= (len == 13);

	return ret;
}

int main(int argc, char ** argv)
{
	int i, w, m, r, j;
	int * lott;
	bool not_equal;

	if (argc < 3){
		fprintf(stderr, "%s: inserire almeno uno studente e un W\n", argv[0]);
		exit(-1);
	}

	for (i = 1; i < argc-1; i++){
		if (!is_stud_ok(argv[i])){
			fprintf(stderr, "%s: studente malformato\n", argv[i]);
			exit(-2);
		}
	}

	m = argc - 2;
	if (isdigit(*argv[argc-1])){
		w = atoi(argv[argc-1]);
	} else {
		fprintf(stderr, "w malformato\n");
		exit(-3);
	}

	if (w <= 0 || w > m){
			fprintf(stderr, "w malformato\n");
			exit(-3);
	}

	lott = (int*) malloc(m * sizeof(int));
	for (i = 0; i < m; not_equal && i++){
		r = rand() % m + 1;
		not_equal = true;
		for (j = 0; j < i; j++){
			if (lott[j] == r){
				not_equal = false;
				break;
			}
		}
		lott[i] = r;
	}
	
	for (i = 0; i < m; i++){
		if (lott[i] == w)
			printf("%s vincitore", argv[1+i]);
	}

}
