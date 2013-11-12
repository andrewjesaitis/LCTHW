#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

//define our Person struct
struct Person
{
	char *name;
	int age;
	int height;
	int weight;
};

//defines a function that creates a Person; similar to a constructor in OOP
struct Person *Person_create(char *name, int age, int height, int weight)
{
	//allocate enough memory for a Person
	struct Person *who = malloc(sizeof(struct Person));
	assert(who != NULL);

	//set the attributes of the new Person for which we just allocated memory
	//first we use the strdup function to allocate memory and copy the name into that memory
	who->name = strdup(name);
	who->age = age;
	who->height = height;
	who->weight = weight;

	//return a pointer to our person
	return who;
}

void Person_destroy(struct Person *who)
{
	assert(who != NULL);
	//free the memory that has been allocated to both the char array for the name
	//and the Person struct itself
	free(who->name);
	free(who);
}

void Person_print(struct Person *who)
{
	//print out the attributes of our Person
	printf("Name: %s\n", who->name);
	printf("\tAge: %d\n", who->age);
	printf("\tHeight: %d\n", who->height);
	printf("\tWeight: %d\n", who->weight);
}

int main(int argc, char *argv[])
{
	// make a couple of people
	struct Person *joe = Person_create("Joe Alex", 32, 64, 140);
	struct Person *frank = Person_create("Frank Black", 20, 72, 180);

	//print out memory addresses
	printf("Joe is at memory address: \t%p\n", joe);
	Person_print(joe);

	printf("Frank is at memory address: \t%p\n", frank);
	Person_print(frank);

	//make 'em 20 again
	joe->age = 20;
	joe->height -= 2;
	joe->weight += 40;
	Person_print(joe);

	frank->age = 20;
	frank->weight += 20;
	Person_print(frank);

	//destroy them
	Person_destroy(joe);
	Person_destroy(frank);

	return 0;
}
