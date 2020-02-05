#include<stdio.h>
#include<math.h>

#define ALPHANUM 26
#define UNVOWELNUM 21
#define VOWELNUM 5

void createLetter(char *letter);
void createUnvowel(char *unvowel, char *vowel);
int sub(char c);
int isPrime(int k);

int main(void)
{
    char unvowel[UNVOWELNUM], vowel[VOWELNUM] = {'a', 'e', 'i', 'o', 'u'};
	char letter[ALPHANUM];
	int i, j, cnt = 1;

	createLetter(letter);
    createUnvowel(unvowel, vowel);

    for (i = 0; i < UNVOWELNUM; ++i) {
        for(j = 0; j < VOWELNUM; ++j) {
			if (isPrime(sub(unvowel[i]) * 2 + sub(vowel[j]))) {
				printf("%d %c%c%c\n", cnt, unvowel[i], vowel[j], unvowel[i]);
				cnt++;
			}		
        }
    }

	for (i = 0; i < UNVOWELNUM; ++i) {
        for (j = 0; j < VOWELNUM; ++j) {
			if (isPrime(sub(unvowel[i]) + sub(vowel[j] * 2))) {
				printf("%d %c%c%c\n", cnt, vowel[j], unvowel[i], vowel[j]);
				cnt++;
			}		
        }
    }
	
	return 0;
}

void createLetter(char *letter)
{
	int i;

	for (i = 0; i < ALPHANUM; ++i) {
		letter[i] = 'a' + i;
	}
}

void createUnvowel(char *unvowel, char *vowel)
{
    int i, j = 0, k = 0;
    char temp;

    for (i = 0; i < ALPHANUM; ++i) {
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

int sub(char c)
{
	return (c - 'a' + 1);
}

int isPrime(int k)
{
	int temp;
    
	for (temp = 2; temp < k; ++temp) {
		if (k % temp == 0){
            return 0;
        } 
	}

	return 1;
}