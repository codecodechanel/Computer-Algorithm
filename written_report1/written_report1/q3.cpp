#include <stdio.h>
#include <stdlib.h>

int find_element(int a[], int start, int end) {
	int found = 0;
	int mid = (end + start) / 2;
	while (sizeof(a) >= 1) {
		int *la = (int*)malloc((end-start+1) * sizeof(int));
		int *ra = (int*)malloc((end-start+1)* sizeof(int));
		if((a[mid]==mid) && sizeof(a) == 1) {
			printf("a[%d] = %d\n", mid, mid);
			found++;
		}
		for (int i = start; i <= mid; i++)
			la[i] = a[i];
		for (int i = mid+1; i <= end; i++)
			ra[i-(mid+1)] = a[i];
		find_element(la, start, mid);
		find_element(ra, mid, end);
	}
	if (found == 0)
		return -1; 
	else
		return found;
}

//a[1]<a[2]< ... <a[n]을 가정
int main(void) {
	int n;
	int found;
	scanf("%d", &n);
	int *a = (int*)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++)
		scanf("%d", &a[i]);
	found = find_element(a, 0, n-1);
	if (found == -1)
		printf("찾지 못했습니다\n");
	else
		printf("%d개 찾았습니다\n", found);
}