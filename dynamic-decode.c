/* Week 11 Workshop: Decoding ASCII bitstrings given an input model 
   containing symbols and respective codewords 
	
	Made by Emmanuel Macario 20/05/17

 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

#define DEBUG 0
#define MAXMODELSIZE    128
#define MAXCODEWORDSIZE 8

typedef char codeword_t[MAXCODEWORDSIZE+1];

typedef struct model {
	char **codewords;      /* An array of pointers to codewords */
	char *symbols;         /* An array of symbols */
	int currsize;          /* Number of symbols currently in the model */
	int maxsize;           /* Max number of symbols that can fit in model
	                          at a certain point in time */
} model_t;


/* Helper function prototypes */
model_t *new_model(void);
void update_model(model_t *model, int symbol, char codeword[]);
void print_model(model_t *model);
void decode_text(model_t *model);
void print_stdin(void);

int
main(int argc, char *argv[]) {
	int c;
	codeword_t codeword;
	model_t *model = new_model();

	/* Read in commandline arguments */
	if (argc < 2) {
		fprintf(stderr, "No filename specified\n");
		fprintf(stderr, " -->Usage: main.exe model.txt < encoded-text.txt\n");
		exit(EXIT_FAILURE);
	}

	FILE *fp = fopen(argv[1], "r");
	assert(fp);

	int i, symbol, newline_last = 1;
	while ((c=fgetc(fp)) && c!=EOF) {
		if (newline_last) {
			/* Get the symbol */
			symbol = c;

			/* Skip the colon character */
			c = fgetc(fp);

			/* Get the codeword */
			i = 0;
			while ((c=fgetc(fp)) && isdigit(c)) {
				codeword[i] = c;
				i++;
			}
			/* And don't forget to close the codeword string 
			   with a nullbyte! */
			codeword[i] = '\0';

			/* Now update the model */
			update_model(model, symbol, codeword);
			newline_last = 0;
		}
		if (c=='\n') {
			newline_last = 1;
		}
	}

	/* Close the handle to that file */
	fclose(fp);

	/* Print the model we have so far */
	if (DEBUG) {
		print_model(model);
	}
	
	/* Now decode the encoded ASCII text! */
	decode_text(model);

	/* Job done! */
	return 0;
}


model_t 
*new_model(void) {
	model_t *model;

	model = malloc(sizeof(*model));
	assert(model);

	model->codewords = malloc(sizeof(*model->codewords));
	model->symbols   = malloc(sizeof(*model->symbols));
	assert(model->codewords && model->symbols);

	/* Currently we have zero symbols, but can fit one symbol in our model */
	model->currsize = 0;
	model->maxsize = 1;

	return model;
}

void 
update_model(model_t *model, int symbol, char codeword[]) {
	/* If we have reached the maximum capacity for symbols, double
	   the capacity size of the model */
	if (model->currsize == model->maxsize) {
		model->maxsize *= 2;
		model->symbols = realloc(model->symbols, 
			                    model->maxsize * sizeof(*model->symbols));
		model->codewords = realloc(model->codewords, 
			                    model->maxsize * sizeof(*model->codewords));
		assert(model->codewords && model->symbols);
	}

	/* Now we know we have room for a new symbol and codeword */
	model->symbols[model->currsize] = symbol;
	model->codewords[model->currsize] = malloc((1+strlen(codeword))*sizeof(char));
	assert(model->codewords[model->currsize]);
	strcpy(model->codewords[model->currsize], codeword);

	/* Don't forget to increment the current model size */
	model->currsize += 1;
}


void print_model(model_t *model) {
	printf("Number of symbols: %d\n", model->currsize);
	printf("Max capacity     : %d\n", model->maxsize);
	int i;
	for (i=0; i<model->currsize; i++) {
		printf("Symbol  : %c\n", model->symbols[i]);
		printf("Codeword: %s\n", model->codewords[i]);
	}
	return;
}

/* Prints a symbol for a codeword and returns true if that codeword was a 
   codeword in the model, otherwise returns false */
bool try_codeword(model_t *model, char codeword[]) {
	int i;

	for (i=0; i<model->currsize; i++) {
		if (strcmp(codeword, model->codewords[i]) == 0) {
			printf("%c", model->symbols[i]);
			return true;
		}
	}
	return false;
}


void decode_text(model_t *model) {
	int i=0, c;
	codeword_t codeword;

	while ((c=fgetc(stdin)) != EOF) {
		/* Enumerate a possible codeword */
		codeword[i] = c;
		codeword[i+1] = '\0';
		i++;

		/* Try that codeword to see if it represents a symbol
		   in our model */
		if (try_codeword(model, codeword)) {
			i = 0;
		}
	}
}