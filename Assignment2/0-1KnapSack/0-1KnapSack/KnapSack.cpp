#include <stdio.h>
#include <time.h>

#define MAX 100 //행의 개수의 최대
#define WMAX 300 //열의 개수의 최대(dynamic programming에서만 사용)
#define FILE_NAME "p2data5.txt"	//불러올 파일의 이름을 이곳에 작성해주세요!

typedef struct btable {
	float ppw;
	int index;
}TABLE;

int M; //knapsack의 maximum capacity
int num; //object의 개수

//dynamic programming
int dp[MAX]; //profit 배열
int dw[MAX]; //weight 배열
int X[MAX]; //object가 knapsack에 들어가는지에 대한 여부를 기록한 배열
int result; //최대 이윤

//backtracking
int bp[MAX]; //정렬된 profit 배열
int bw[MAX]; //정렬된 weight 배열
TABLE btable[MAX];
int bestset[MAX]; //최종적으로 들어가는 object들이 무엇인지에 대한 배열
int include[MAX]; //중간 과정에서 사용되는 배열
int maxprofit = 0; //최대 profit
int bresult[MAX]; //bestset[]의 값들이 정렬이 되지 않은 순서로 출력하기 위한 배열

FILE *fp;

void initialize() {
	printf("==========================================\n");
	fscanf(fp, "%d", &num); //object의 개수 초기화

	for (int i = 0; i < num; i++)
		fscanf(fp, "%d", &dp[i]);	//dynammic programming의 profit 배열 초기화
	for (int i = 0; i < num; i++)
		fscanf(fp, "%d", &dw[i]);	//dynamic programming의 weight 배열 초기화
	for (int i = 1; i <= num; i++)
		bp[i] = dp[i - 1];	//db를 bp에 복사
	for (int i = 1; i <= num; i++)
		bw[i] = dw[i - 1];	//dw를 bw에 복사
	for (int i = 1; i <= num; i++) {
		btable[i].ppw = (float)bp[i] / bw[i];	//(profit/weight)의 값을 저장
		btable[i].index = i;
		bestset[i] = 0;
		include[i] = 0;
	}

	//변수들의 값 출력
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
		X[i] = 0;	//X 행렬 모두 0으로 초기화(knapsack에 들어간 object가 아직 없음)
	printf("==========================================\n");
}

//두 수를 비교하여 두 큰 수를 반환해주는 함수
int max(int a, int b) {
	if (a > b)
		return a;
	else
		return b;
}

//btable 안의 원소들의 위치를 변경해주는 함수
void tswap(TABLE* a, TABLE* b){
	TABLE t = *a;
	*a = *b;
	*b = t;
}

//bp와 bw 안의 원소들의 위치 변경해주는 함수
void pwswap(int*a, int*b) {
	int t = *a;
	*a = *b;
	*b = t;
}

//btable을 내림차순으로 정렬해주는 함수(btable의 순서에 맞게 bp와 bw도 함께 정렬)
void quicksort(TABLE btable[], int low, int high) {
	// 원소가 1개인 경우
	if (low >= high)
		return;

	int pivot = low;
	int i = pivot + 1; // 왼쪽 출발 지점 
	int j = high; // 오른쪽 출발 지점

	while (i <= j) {
		// 포인터가 엇갈릴때까지 반복
		while (i <= high && btable[i].ppw >= btable[pivot].ppw) //pivot 값보다 작은 값을 찾을 때까지 i 증가
			i++;
		while (j > low && btable[j].ppw <= btable[pivot].ppw)	//pivot 값보다 큰 값을 찾을 때까지 i 증가
			j--;

		// i와 j가 엇갈림
		if (i > j) {
			tswap(&btable[j], &btable[pivot]);	//pivot과 j swap
			pwswap(&bp[j], &bp[pivot]);
			pwswap(&bw[j], &bw[pivot]);
		}
		else {
			tswap(&btable[i], &btable[j]);	// i번째와 j번째를 swap
			pwswap(&bp[i], &bp[j]);
			pwswap(&bw[i], &bw[j]);
		}
	}
	//분할 계산
	quicksort(btable, low, j - 1);
	quicksort(btable, j + 1, high);
}

//0-1 knapsack problem을 동적 계획법으로 구현한 함수
int dKnapSack(int M, int dw[], int dp[]) {
	int table[MAX][WMAX];	//중간 과정을 저장하기 위해 2차원 배열 생성

	for (int i = 0; i <= num; i++) {
		for (int j = 0; j <= M; j++) {
			if (i == 0 || j == 0)
				table[i][j] = 0;	//1행과 1열은 0으로 채움
			
			//object의 크기가 table의 열 인덱스의 크기(j 값)보다 작거나 같을 경우
			else if (dw[i - 1] <= j)
				table[i][j] = max(dp[i - 1] + table[i - 1][j - dw[i - 1]], table[i - 1][j]);	//object를 넣었을 때와 안 넣었을 때의 값을 비교하여 큰 값을 넣는다.

			//object의 크기가 table의 열 인덱스의 크기(j 값)보다 클 경우
			else
				table[i][j] = table[i - 1][j];	//해당 object는 넣지 않는다. 
		}
	}

	int result = table[num][M];	//최종 결과를 result에 저장
	int weight = M;

	//해벡터에 어떠한 object가 들어가는지 저장하기 위한 for문
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

//backtracking에서 문제의 조건을 체크해주는 함수
bool promising(int a, int profit, int weight) {
	int b, c;
	int totweight;
	float bound;	//fractional knapsack 문제로 값을 구함.

	if (weight >= M)	//무게의 합이 가방의 maximum capacity를 넘었을 경우 false 반환
		return false;
	else {
		b = a + 1; //들어간 노드 다음 노드의 index부터 고려
		bound = profit;
		totweight = weight;
		
		//한 object가 통째로 들어가는 경우만 먼저 고려하여 넣을 수 있을 만큼 넣음.
		while (b <= num && totweight + bw[b] <= M) {
			totweight += bw[b];
			bound += bp[b];
			b++;
		}
	}
	c = b;

	//아직 들어가지 않은 object가 남아있을 경우
	if (c <= num)
		bound += (M - totweight) * btable[c].ppw;	//fractional로 object를 넣음.
	return (bound > maxprofit);	//bound > maxprofit일 때만 true
}

//0-1 knapsack 문제를 backtracking으로 구현한 함수
void bKnapSack(int a, int profit, int weight) {

	//현재 solution이 이전의 best solution보다 나은지 확인(best update인지 확인)
	if (weight <= M && profit > maxprofit) {
		maxprofit = profit;
		for (int i = 1; i <= num; i++)
			bestset[i] = include[i];
	}

	//부모 노드가 promising하면 자식 노드를 생성
	if (promising(a, profit, weight)) {
		include[a + 1] = 1;	//object를 넣을 경우
		bKnapSack(a + 1, profit + bp[a + 1], weight + bw[a + 1]);	//넣은 상태로 재귀 호출
		include[a + 1] = 0;	//object를 넣지 않을 경우
		bKnapSack(a + 1, profit, weight);	//넣지 않은 상태로 재귀 호출
	}
}

int main(void) {
	clock_t dt, bt;

	fp = fopen(FILE_NAME, "r");	//입력받을 파일을 열어서 fp에 저장
						
	if (fp == NULL) {
		printf("File did not open\n");
		return 0;
	}
	printf("입력파일 : %s\n\n", FILE_NAME);
	initialize();	//변수 초기화 함수 호출

	dt = clock();
	printf("(1) Dynamic Programming\n");
	printf("The maximum profit is $%d.\n", dKnapSack(M, dw, dp));	//최종 값 출력

	//최종 해벡터 출력
	printf("The solution vector X = (");
	for (int i = 0; i < num; i++) {
		if (i < num - 1)
			printf("%d, ", X[i]);
		else
			printf("%d)\n", X[i]);
	}

	dt = clock() - dt;
	double dtime = ((double)dt) / CLOCKS_PER_SEC;
	printf("The execution time is %.2lf milliseconds\n", dtime * 1000);	//수행시간 출력

	bt = clock();
	printf("\n(2) Backtracking\n");
	quicksort(btable, 1, num);	//먼저 btable, bp, bw 배열을 내림차순으로 정렬
	bKnapSack(0, 0, 0);	
	printf("The maximum profit is $%d.\n", maxprofit);

	for (int i = 1; i <= num; i++)
		bresult[btable[i].index] = bestset[i];	//정렬되지 않은 순서로 bestset[]의 값을 복사

	//최종 해벡터 출력
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
	printf("The execution time is %.2lf milliseconds\n", btime * 1000);	//수행시간 출력
}