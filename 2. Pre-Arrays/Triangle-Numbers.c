#include<stdio.h>

int triangleNum(int n);

int main(void)
{
	int n, i;
	printf("Please enter an iteration times:\n");		
	scanf("%d", &n);
	printf("The triangle numbers is:\n");	
	for(i = 1; i <= n; ++i) {
		printf("%d ", triangleNum(i));		
		if (i % 10 == 0) {
            printf("\n"); 
        } 	
	}
	return 0;
}

int triangleNum(int n)
{
	if (n == 1 || n == 2 || n == 3) {
        return (n * (n + 1) / 2);
    } 
	else{
        return (triangleNum(n - 1) + n);
    } 
	
	return 0;	
}