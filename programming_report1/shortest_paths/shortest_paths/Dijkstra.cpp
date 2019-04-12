//1605020 박소현
//Dijkstra algorithm 구현

#include <stdio.h>
#include <string.h>

#define FILE_NAME "graph4.txt"	//불러올 파일의 이름을 이곳에 작성해주세요!
#define MAX 1000
#define SIZE 100
#define MAX_INT 2147483647

int graph[SIZE][SIZE]; //입력 파일에서 받아오는 값을 저장
int vertex[SIZE];	//선택된 노드와 선택되지 않은 노드에 대한 구분을 해주는 변수
int length[SIZE];	//정점에서 다른 정점까지의 최단 경로를 저장
int index = 0;	//dijkstra 함수 호출 횟수를 조절
int sum = 0;

FILE *fp;

//필요한 변수들의 값을 초기화해주는 함수
void initialize(int node, int start) {
	//입력 파일에서 둘째 줄부터 끝 줄까지 읽어서 graph배열에 값 저장
	for (int i = 0; i < node; i++)	
		for (int j = 0; j < node; j++)
			fscanf(fp, "%d", &graph[i][j]);
	//노드의 개수만큼 반복하며 미리 0으로 초기화(0 : 아직 선택되지 않은 상태)
	for (int i = 0; i < node; i++)	
		vertex[i] = 0;
	//시작 정점의 행을 따로 다른 변수에 저장
	for (int i = 0; i < node; i++)	
		length[i] = graph[start][i];
}

//실제 다익스트라 알고리즘의 구현
void dijkstra(int node, int start, int old) {
	int min = MAX_INT;
	int newnode;	//선택되는 노드가 저장되는 변수

	//해당 노드가 0일 때
	if (vertex[start] == 0) {
		vertex[start] = 1;	//해당 노트가 선택되었으므로 1로 바꿔준다.
		for (int i = 0; i < node; i++) {
			//이전에 선택된 가중치와 새로 지정된 노드에서 특정 경로의 합이 이전 노드에서의 경로보다 작을 경우
			if ((sum + graph[start][i] < length[i]) && vertex[i] == 0 && graph[start][i] != MAX_INT)
				length[i] = graph[start][i] + sum;
			else if (graph[start][i] == MAX_INT)
				continue;
		} 
		for (int i = 0; i < node; i++) {
			if( min > length[i]) {
				if (vertex[i] == 1)	//선택된 노드는 건너뛴다.
					continue;
				else {
					min = length[i]; //최소 경로를 min에 저장
					sum = min;
					newnode = i;	//선택된 노드의 값을 newnode에 저장
				}
			}
		}
		//index의 값이 노드의 개수-1 보다 크면 더 이상 재귀 호출을 하지 않음.
		if(++index < node - 1)
			dijkstra(node, newnode, start);	//선택된 노드에 대해서 함수 재귀 호출
	}
}

int main(void) {
	int node;	//노드의 개수 저장
	int start;	//시작 정점의 값 저장

	fp = fopen(FILE_NAME, "r");	//입력받을 파일을 열어서 fp에 저장

	//파일이 열리지 않았을 경우 프로그램 종료
	if (fp == NULL) {
		printf("File did not open\n");
		return 0;
	}
	printf("입력파일 : %s\n\n", FILE_NAME);
	fscanf(fp, "%d %d", &node, &start);	//입력파일의 첫 줄을 먼저 읽어서 node와 start 변수 초기화
	initialize(node, start-1);	//필요한 변수들의 값을 초기화
	dijkstra(node, start-1, NULL);	//다익스트라 함수 호출
	printf("정점 %d(으)로부터 각 정점까지의 최단경로 = (", start);
	for (int i = 0; i < node - 1; i++)
		printf("%d, ", length[i]);
	printf("%d)\n", length[node - 1]);
}