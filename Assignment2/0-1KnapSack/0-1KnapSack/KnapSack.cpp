#include <stdio.h>
#include <time.h>

#define MAX 100 //���� ������ �ִ�
#define WMAX 300 //���� ������ �ִ�(dynamic programming������ ���)
#define FILE_NAME "p2data5.txt"	//�ҷ��� ������ �̸��� �̰��� �ۼ����ּ���!

typedef struct btable {
	float ppw;
	int index;
}TABLE;

int M; //knapsack�� maximum capacity
int num; //object�� ����

//dynamic programming
int dp[MAX]; //profit �迭
int dw[MAX]; //weight �迭
int X[MAX]; //object�� knapsack�� �������� ���� ���θ� ����� �迭
int result; //�ִ� ����

//backtracking
int bp[MAX]; //���ĵ� profit �迭
int bw[MAX]; //���ĵ� weight �迭
TABLE btable[MAX];
int bestset[MAX]; //���������� ���� object���� ���������� ���� �迭
int include[MAX]; //�߰� �������� ���Ǵ� �迭
int maxprofit = 0; //�ִ� profit
int bresult[MAX]; //bestset[]�� ������ ������ ���� ���� ������ ����ϱ� ���� �迭

FILE *fp;

void initialize() {
	printf("==========================================\n");
	fscanf(fp, "%d", &num); //object�� ���� �ʱ�ȭ

	for (int i = 0; i < num; i++)
		fscanf(fp, "%d", &dp[i]);	//dynammic programming�� profit �迭 �ʱ�ȭ
	for (int i = 0; i < num; i++)
		fscanf(fp, "%d", &dw[i]);	//dynamic programming�� weight �迭 �ʱ�ȭ
	for (int i = 1; i <= num; i++)
		bp[i] = dp[i - 1];	//db�� bp�� ����
	for (int i = 1; i <= num; i++)
		bw[i] = dw[i - 1];	//dw�� bw�� ����
	for (int i = 1; i <= num; i++) {
		btable[i].ppw = (float)bp[i] / bw[i];	//(profit/weight)�� ���� ����
		btable[i].index = i;
		bestset[i] = 0;
		include[i] = 0;
	}

	//�������� �� ���
	printf("n = %d\n", num);
	printf("pi = ");
	for (int i = 0; i < num; i++)
		printf("%d ", dp[i]);
	printf("\nwi = ");
	for (int i = 0; i < num; i++)
		printf("%d ", dw[i]);
	printf("\npi/wi = ");
	for (int i = 0; i < num; i++) {
		printf("%.1f ", (float)dp[i] / dw[i]);
	}
	fscanf(fp, "%d", &M);
	printf("\nM = %d\n", M);
	for (int i = 0; i < num; i++)
		X[i] = 0;	//X ��� ��� 0���� �ʱ�ȭ(knapsack�� �� object�� ���� ����)
	printf("==========================================\n");
}

//�� ���� ���Ͽ� �� ū ���� ��ȯ���ִ� �Լ�
int max(int a, int b) {
	if (a > b)
		return a;
	else
		return b;
}

//btable ���� ���ҵ��� ��ġ�� �������ִ� �Լ�
void tswap(TABLE* a, TABLE* b){
	TABLE t = *a;
	*a = *b;
	*b = t;
}

//bp�� bw ���� ���ҵ��� ��ġ �������ִ� �Լ�
void pwswap(int*a, int*b) {
	int t = *a;
	*a = *b;
	*b = t;
}

//btable�� ������������ �������ִ� �Լ�(btable�� ������ �°� bp�� bw�� �Բ� ����)
void quicksort(TABLE btable[], int low, int high) {
	// ���Ұ� 1���� ���
	if (low >= high)
		return;

	int pivot = low;
	int i = pivot + 1; // ���� ��� ���� 
	int j = high; // ������ ��� ����

	while (i <= j) {
		// �����Ͱ� ������������ �ݺ�
		while (i <= high && btable[i].ppw >= btable[pivot].ppw) //pivot ������ ���� ���� ã�� ������ i ����
			i++;
		while (j > low && btable[j].ppw <= btable[pivot].ppw)	//pivot ������ ū ���� ã�� ������ i ����
			j--;

		// i�� j�� ������
		if (i > j) {
			tswap(&btable[j], &btable[pivot]);	//pivot�� j swap
			pwswap(&bp[j], &bp[pivot]);
			pwswap(&bw[j], &bw[pivot]);
		}
		else {
			tswap(&btable[i], &btable[j]);	// i��°�� j��°�� swap
			pwswap(&bp[i], &bp[j]);
			pwswap(&bw[i], &bw[j]);
		}
	}
	//���� ���
	quicksort(btable, low, j - 1);
	quicksort(btable, j + 1, high);
}

//0-1 knapsack problem�� ���� ��ȹ������ ������ �Լ�
int dKnapSack(int M, int dw[], int dp[]) {
	int table[MAX][WMAX];	//�߰� ������ �����ϱ� ���� 2���� �迭 ����

	for (int i = 0; i <= num; i++) {
		for (int j = 0; j <= M; j++) {
			if (i == 0 || j == 0)
				table[i][j] = 0;	//1��� 1���� 0���� ä��
			
			//object�� ũ�Ⱑ table�� �� �ε����� ũ��(j ��)���� �۰ų� ���� ���
			else if (dw[i - 1] <= j)
				table[i][j] = max(dp[i - 1] + table[i - 1][j - dw[i - 1]], table[i - 1][j]);	//object�� �־��� ���� �� �־��� ���� ���� ���Ͽ� ū ���� �ִ´�.

			//object�� ũ�Ⱑ table�� �� �ε����� ũ��(j ��)���� Ŭ ���
			else
				table[i][j] = table[i - 1][j];	//�ش� object�� ���� �ʴ´�. 
		}
	}

	int result = table[num][M];	//���� ����� result�� ����
	int weight = M;

	//�غ��Ϳ� ��� object�� ������ �����ϱ� ���� for��
	for (int i = num; i > 0 && result > 0; i--) {
		if (result == table[i - 1][weight])
			continue;
		else {
			result -= dp[i - 1];
			weight = weight - dw[i - 1];
			X[i - 1] = 1;
		}
	}

	return table[num][M];
}

//backtracking���� ������ ������ üũ���ִ� �Լ�
bool promising(int a, int profit, int weight) {
	int b, c;
	int totweight;
	float bound;	//fractional knapsack ������ ���� ����.

	if (weight >= M)	//������ ���� ������ maximum capacity�� �Ѿ��� ��� false ��ȯ
		return false;
	else {
		b = a + 1; //�� ��� ���� ����� index���� ���
		bound = profit;
		totweight = weight;
		
		//�� object�� ��°�� ���� ��츸 ���� ����Ͽ� ���� �� ���� ��ŭ ����.
		while (b <= num && totweight + bw[b] <= M) {
			totweight += bw[b];
			bound += bp[b];
			b++;
		}
	}
	c = b;

	//���� ���� ���� object�� �������� ���
	if (c <= num)
		bound += (M - totweight) * btable[c].ppw;	//fractional�� object�� ����.
	return (bound > maxprofit);	//bound > maxprofit�� ���� true
}

//0-1 knapsack ������ backtracking���� ������ �Լ�
void bKnapSack(int a, int profit, int weight) {

	//���� solution�� ������ best solution���� ������ Ȯ��(best update���� Ȯ��)
	if (weight <= M && profit > maxprofit) {
		maxprofit = profit;
		for (int i = 1; i <= num; i++)
			bestset[i] = include[i];
	}

	//�θ� ��尡 promising�ϸ� �ڽ� ��带 ����
	if (promising(a, profit, weight)) {
		include[a + 1] = 1;	//object�� ���� ���
		bKnapSack(a + 1, profit + bp[a + 1], weight + bw[a + 1]);	//���� ���·� ��� ȣ��
		include[a + 1] = 0;	//object�� ���� ���� ���
		bKnapSack(a + 1, profit, weight);	//���� ���� ���·� ��� ȣ��
	}
}

int main(void) {
	clock_t dt, bt;

	fp = fopen(FILE_NAME, "r");	//�Է¹��� ������ ��� fp�� ����
						
	if (fp == NULL) {
		printf("File did not open\n");
		return 0;
	}
	printf("�Է����� : %s\n\n", FILE_NAME);
	initialize();	//���� �ʱ�ȭ �Լ� ȣ��

	dt = clock();
	printf("(1) Dynamic Programming\n");
	printf("The maximum profit is $%d.\n", dKnapSack(M, dw, dp));	//���� �� ���

	//���� �غ��� ���
	printf("The solution vector X = (");
	for (int i = 0; i < num; i++) {
		if (i < num - 1)
			printf("%d, ", X[i]);
		else
			printf("%d)\n", X[i]);
	}

	dt = clock() - dt;
	double dtime = ((double)dt) / CLOCKS_PER_SEC;
	printf("The execution time is %.2lf milliseconds\n", dtime * 1000);	//����ð� ���

	bt = clock();
	printf("\n(2) Backtracking\n");
	quicksort(btable, 1, num);	//���� btable, bp, bw �迭�� ������������ ����
	bKnapSack(0, 0, 0);	
	printf("The maximum profit is $%d.\n", maxprofit);

	for (int i = 1; i <= num; i++)
		bresult[btable[i].index] = bestset[i];	//���ĵ��� ���� ������ bestset[]�� ���� ����

	//���� �غ��� ���
	printf("The solution vector X = (");
	for (int i = 1; i <= num; i++) {
		if (i < num)
			printf("%d, ", bresult[i]);
		else
			printf("%d)", bresult[i]);
	}
	printf("\n");

	bt = clock() - bt;
	double btime = ((double)bt) / CLOCKS_PER_SEC;
	printf("The execution time is %.2lf milliseconds\n", btime * 1000);	//����ð� ���
}