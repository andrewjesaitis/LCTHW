#include <stdio.h>

int main(int argc, char *argv[])
{
	// if(argc != 2) {
	// 	printf("ERROR: You need one argument.\n");
	// 	// this is how you abort a program
	// 	return 1;
	// }
	int j = 0;
	for(j = 1; j < argc; j++){
		printf("Word %d:\n", j);
		int i =  0;
		for(i = 0; argv[j][i] != '\0'; i++) {
			char letter = argv[j][i];
			letter = letter | 32;

			switch(letter) {
				case 'a':
					printf("%d: 'A'\n", i);
					break;
				case 'e':
					printf("%d: 'E'\n", i);
					break;
				case 'i':
					printf("%d: 'I'\n", i);
					break;
				case 'o':
					printf("%d: 'O'\n", i);
					break;
				case 'u':
					printf("%d: 'U'\n", i);
					break;
				case 'y':
					if(i > 2) {
						// it's only sometimes Y
						printf("%d: 'Y'\n", i);
					}
					break;
				default:
					printf("%d: %c is not a vowel\n", i, letter);
			}
		}

	}

	return 0;
}