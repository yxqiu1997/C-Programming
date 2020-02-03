#include<stdio.h>
#include<math.h>
#include <time.h>

#define TARGET_MAX 3.141592654
#define TARGET_MIN 3.141592645

double pi(int cnt, double res);
int check(double res);

int main(void)
{
	clock_t startTime, endTime;      
	double res = 0.0;
    unsigned int cnt = 0;
    startTime = clock();
	
	while (!check(res)) {
		res = pi(cnt, res);
		cnt++;
	}

	printf("\n\nThere are %d iterations to get p correctly approximated to 9 digits.(%.10f)\n", cnt, res);
    endTime = clock();
    printf("Total time is:%.10f", (double)(endTime - startTime) / CLOCKS_PER_SEC);
    
    return 0;
}

int check(double res)
{
	return (res <= TARGET_MAX && res >= TARGET_MIN);	
}

double  pi(int cnt, double res)
{
	res = res + 4 * pow((-1), (double)cnt) / (2 * cnt + 1);
	return res;	
}
