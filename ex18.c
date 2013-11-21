#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void die(const char *msg)
{
	if(errno) {
		perror(msg);
	} else {
		printf("ERROR: %s\n", msg);
	}

	exit(1);
}

void print_list(int *list, int count){
	int i = 0;
	for(i = 0; i < count; i++){
		printf("%d ", list[i]);
	}
	printf("\n");
}

// a typedef creates a fake type, in this case for a function pointer
typedef int (*compare_cb)(int a, int b);

// Bubble my sort
int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
	int temp = 0;
	int i = 0;
	int j = 0;
	int *target = malloc(count * sizeof(int));

	if(!target) die("Memory error.");

	memcpy(target, numbers, count * sizeof(int));

	for(i = 0; i < count; i++){
		for(j=0; j < count - 1; j++){
			if(cmp(target[j], target[j+1]) > 0){
				temp = target[j+1];
				target[j+1] = target[j];
				target[j] = temp;
			}
		}
	}
	return target;
}

void swap(int *list, int a, int b)
{
	// printf("list[a]: %d :: %d\n", a, list[a]);
	// printf("list[b]: %d :: %d\n", b, list[b]);
	int temp = list[a];
	list[a] = list[b];
	list[b] = temp;
	// printf("list[a]: %d :: %d\n", a, list[a]);
	// printf("list[b]: %d :: %d\n", b, list[b]);
}

void quick_sort(int *list, int start, int end, compare_cb cmp)
{
	//base case: indicies have reversed
	if (end < start) return;

	int pivot = list[start];
	int i = start + 1;
	int j = end;
	while(i <= j){
		while(i <= end && cmp(list[i], pivot) <= 0) {
			i++;
		}
		while(j >= start && cmp(list[j], pivot) > 0) {
			j--;
		}
		if(i < j){
			//swap 'em since list[j] < pivot < list[i]
			swap(list, i, j);
		}
	}
	//we need to swap the pivot into place since it is just the first element in the arrary right now
	swap(list, start, j);
    quick_sort(list, start, j-1, cmp);
    quick_sort(list, j+1, end, cmp);
}

int sorted_order(int a, int b)
{
	return a - b;
}

int reverse_order(int a, int b)
{
	return b - a;
}

int starage_order(int a, int b)
{
	if(a == 0 || b == 0){
		return 0;
	} else {
		return a % b;
	}
}

void test_sorting(int *numbers, int count, compare_cb cmp)
{
	int *sorted = bubble_sort(numbers, count, cmp);

	if(!sorted) die("Failed to sort as requested.");

	print_list(sorted, count);

	free(sorted);
}

int main(int argc, char *argv[])
{
	if(argc < 2) die("USAGE: ex18 4 3 1 6 5");

	int count = argc -1;
	int i = 0;
	char **inputs = argv + 1;

	int *numbers = malloc(count * sizeof(int));
	if(!numbers) die("Memory error.");

	for(i = 0; i < count; i++){
		numbers[i] = atoi(inputs[i]);
	}

	test_sorting(numbers, count, sorted_order);
	test_sorting(numbers, count, reverse_order);
	test_sorting(numbers, count, starage_order);

	// since this sorts in place we have to make some copies
	int *list_copy1 = malloc(count * sizeof(int));
	memcpy(list_copy1, numbers, count * sizeof(int));
	int *list_copy2 = malloc(count * sizeof(int));
	memcpy(list_copy2, numbers, count * sizeof(int));
	int *list_copy3 = malloc(count * sizeof(int));
	memcpy(list_copy3, numbers, count * sizeof(int));

	printf("Use quicksort, like a boss.\n");
	quick_sort(list_copy1, 0, count-1, sorted_order);
	print_list(list_copy1, count);
	quick_sort(list_copy2, 0, count-1, reverse_order);
	print_list(list_copy2, count);
	quick_sort(list_copy3, 0, count-1, starage_order);
	print_list(list_copy3, count);

	free(numbers);
	free(list_copy1);
	free(list_copy3);
	free(list_copy2);

	return 0;

}
