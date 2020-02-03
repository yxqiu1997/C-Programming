#include<stdio.h>

#define ALPHANUM 26
#define UNVOWELNUM 21
#define VOWELNUM 5

void createUnvowel(char *unvowel, char *vowel);

int main(void)
{
	char unvowel[UNVOWELNUM], vowel[VOWELNUM]={'a', 'e', 'i', 'o', 'u'};
	int i, j, k, cnt = 1;

	createUnvowel(unvowel, vowel);
	
	for (i = 0; i < UNVOWELNUM; ++i) {
		for (j = 0; j < VOWELNUM; ++j) {
			for (k = 0; k < 21; ++k) {
				printf("%d %c%c%c\n", cnt, unvowel[i], vowel[j], unvowel[k]);
				cnt++;
			}
		}
	}
	return 0;			
}

void createUnvowel(char *unvowel, char *vowel)
{
	int i, j = 0, k = 0;
	char temp;

	for (i = 0; i < ALPHANUM; ++i)
	{
		temp = 'a' + i;
		if (temp != vowel[j]) {
			unvowel[k] = temp;
			k++;
		}
		else {
			j++;
		}
	}
}	