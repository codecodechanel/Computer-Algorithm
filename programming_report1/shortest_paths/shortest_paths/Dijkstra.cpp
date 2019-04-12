//1605020 �ڼ���
//Dijkstra algorithm ����

#include <stdio.h>
#include <string.h>

#define FILE_NAME "graph4.txt"	//�ҷ��� ������ �̸��� �̰��� �ۼ����ּ���!
#define MAX 1000
#define SIZE 100
#define MAX_INT 2147483647

int graph[SIZE][SIZE]; //�Է� ���Ͽ��� �޾ƿ��� ���� ����
int vertex[SIZE];	//���õ� ���� ���õ��� ���� ��忡 ���� ������ ���ִ� ����
int length[SIZE];	//�������� �ٸ� ���������� �ִ� ��θ� ����
int index = 0;	//dijkstra �Լ� ȣ�� Ƚ���� ����
int sum = 0;

FILE *fp;

//�ʿ��� �������� ���� �ʱ�ȭ���ִ� �Լ�
void initialize(int node, int start) {
	//�Է� ���Ͽ��� ��° �ٺ��� �� �ٱ��� �о graph�迭�� �� ����
	for (int i = 0; i < node; i++)	
		for (int j = 0; j < node; j++)
			fscanf(fp, "%d", &graph[i][j]);
	//����� ������ŭ �ݺ��ϸ� �̸� 0���� �ʱ�ȭ(0 : ���� ���õ��� ���� ����)
	for (int i = 0; i < node; i++)	
		vertex[i] = 0;
	//���� ������ ���� ���� �ٸ� ������ ����
	for (int i = 0; i < node; i++)	
		length[i] = graph[start][i];
}

//���� ���ͽ�Ʈ�� �˰����� ����
void dijkstra(int node, int start, int old) {
	int min = MAX_INT;
	int newnode;	//���õǴ� ��尡 ����Ǵ� ����

	//�ش� ��尡 0�� ��
	if (vertex[start] == 0) {
		vertex[start] = 1;	//�ش� ��Ʈ�� ���õǾ����Ƿ� 1�� �ٲ��ش�.
		for (int i = 0; i < node; i++) {
			//������ ���õ� ����ġ�� ���� ������ ��忡�� Ư�� ����� ���� ���� ��忡���� ��κ��� ���� ���
			if ((sum + graph[start][i] < length[i]) && vertex[i] == 0 && graph[start][i] != MAX_INT)
				length[i] = graph[start][i] + sum;
			else if (graph[start][i] == MAX_INT)
				continue;
		} 
		for (int i = 0; i < node; i++) {
			if( min > length[i]) {
				if (vertex[i] == 1)	//���õ� ���� �ǳʶڴ�.
					continue;
				else {
					min = length[i]; //�ּ� ��θ� min�� ����
					sum = min;
					newnode = i;	//���õ� ����� ���� newnode�� ����
				}
			}
		}
		//index�� ���� ����� ����-1 ���� ũ�� �� �̻� ��� ȣ���� ���� ����.
		if(++index < node - 1)
			dijkstra(node, newnode, start);	//���õ� ��忡 ���ؼ� �Լ� ��� ȣ��
	}
}

int main(void) {
	int node;	//����� ���� ����
	int start;	//���� ������ �� ����

	fp = fopen(FILE_NAME, "r");	//�Է¹��� ������ ��� fp�� ����

	//������ ������ �ʾ��� ��� ���α׷� ����
	if (fp == NULL) {
		printf("File did not open\n");
		return 0;
	}
	printf("�Է����� : %s\n\n", FILE_NAME);
	fscanf(fp, "%d %d", &node, &start);	//�Է������� ù ���� ���� �о node�� start ���� �ʱ�ȭ
	initialize(node, start-1);	//�ʿ��� �������� ���� �ʱ�ȭ
	dijkstra(node, start-1, NULL);	//���ͽ�Ʈ�� �Լ� ȣ��
	printf("���� %d(��)�κ��� �� ���������� �ִܰ�� = (", start);
	for (int i = 0; i < node - 1; i++)
		printf("%d, ", length[i]);
	printf("%d)\n", length[node - 1]);
}