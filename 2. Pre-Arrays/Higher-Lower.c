#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int check(int res, int n);

int main()
{
	int flag = 2, n, res, cnt = 1;
	srand((unsigned)time(NULL));
	res = rand() % 1000;
	printf("Now,guess the number:\n");
	scanf("%d", &n);
	
	while (cnt < 11 && flag != 0) {
		flag = check(res, n);
		if (flag == 1) {
			printf("This is your %d time.\nIt is larger than result.\nPlease enter a number again:\n", cnt);
			scanf("%d", &n);
		}	
		if (flag==-1) {
			printf("This is your %d time.\nIt is smaller than result.\nPlease enter a number again:\n", cnt);
			scanf("%d", &n);
		}
		cnt++;
	}

	if (flag == 0) {
        printf("Yes! The result is %d!You got it!\n", res);
    } 
	else {
        printf("Sorry,you lose the game.The result is %d", res);
    } 

	return 0;
}

int check(int res, int n)
{
	if (res == n){
        return 0;
    } 
	else if (res < n) {
        return 1;
    }
	else {
        return -1;
    } 
    
	return 0;
}