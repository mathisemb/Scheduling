#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "olist.h"
#include "bstree.h"
#include "instance.h"
#include "schedule.h"

int main(int argv, char *argc[]) {
	Instance I = readInstance("./data/exemple");
	viewInstance(I);
	reorderInstance(I, EBST, LPT);
	viewInstance(I);

	Schedule *SOL = newSchedule(OL, 1);
	computeSchedule(SOL, I);
	viewSchedule(SOL);
	saveSchedule(SOL, "./data/output_OL");
	printf("Makespan=%ld\n", makespan(SOL));
	printf("SumWjCj=%ld\n", SumWjCj(SOL));
	printf("SumWjFj=%ld\n", SumWjFj(SOL));
	printf("SumWjTj=%ld\n", SumWjTj(SOL));
	freeSchedule(SOL);

	Schedule *SBST = newSchedule(BST, 1);
	computeSchedule(SBST, I);
	viewSchedule(SBST);
	saveSchedule(SBST, "./data/output_BST");
	printf("Makespan=%ld\n", makespan(SBST));
	printf("SumWjCj=%ld\n", SumWjCj(SBST));
	printf("SumWjFj=%ld\n", SumWjFj(SBST));
	printf("SumWjTj=%ld\n", SumWjTj(SBST));
	freeSchedule(SBST);

	Schedule *SEBST = newSchedule(EBST, 1);
	computeSchedule(SEBST, I);
	viewSchedule(SEBST);
	saveSchedule(SEBST, "./data/output_EBST");
	printf("Makespan=%ld\n", makespan(SEBST));
	printf("SumWjCj=%ld\n", SumWjCj(SEBST));
	printf("SumWjFj=%ld\n", SumWjFj(SEBST));
	printf("SumWjTj=%ld\n", SumWjTj(SEBST));
	freeSchedule(SEBST);

	freeInstance(I, 1);

	return EXIT_SUCCESS;
}