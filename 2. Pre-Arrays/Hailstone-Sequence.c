#include<stdio.h>

typedef struct {
	int longest;
	int longestNum;
	int max;
	int maxNum;
} Record;

int hailstone(int temp);

int main(void)
{
	int i, n, cnt, temp;
	Record record;
	printf("Please enter a range:");
	scanf("%d", &n);
	record.max = 0;
	record.longest = 0;
	record.maxNum = 0;
	record.longestNum = 0;

	for (i = 1; i <= n; ++i) {
		temp = i;
		cnt = 1;
		if (temp > record.max) {
            record.max = temp;
            record.maxNum = i;
        }
		while (temp != 1) {
			temp = hailstone(temp);
			cnt++;
			if (temp > record.max) {
				record.max = temp;
				record.maxNum = i;
			}		
		}
		if(cnt > record.longest) {
			record.longest = cnt;
			record.longestNum = i;
		}		
	}
	
	printf("\n\nThe initial number (less than %d) creating the longest hailstone sequence (%d numbers) is %d.\n", n, record.longest, record.longestNum);
	printf("\nThe initial number (less than %d) creating the largest hailstone sequence (%d) is %d.\n", n, record.max, record.maxNum);

	return 0;
}

int hailstone(int temp)
{
	if (temp % 2 == 0) {
		return (temp / 2);
		
	}
    else {
		return (3 * temp + 1);
	}
}