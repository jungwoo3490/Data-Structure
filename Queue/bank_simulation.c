#include <stdio.h>
#include <stdlib.h>


// ================ 원형큐 정의부 시작 =================
typedef struct { // 요소 타입
	int id;
	int arrival_time;
	int start_time; // element 구조체 필드에 업무 시작 시간을 추가한다.
	int service_time;
} element;			// 교체!
// ================ 원형큐 정의부 종료 =================

// ===== 원형큐 코드 시작 ======
#define MAX_QUEUE_SIZE 60
typedef struct { // 큐 타입
	element data[MAX_QUEUE_SIZE];
	int front, rear;
} QueueType;

// 오류 함수
void error(char* message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}

// 공백 상태 검출 함수
void init_queue(QueueType* q)
{
	q->front = q->rear = 0;
}

// 공백 상태 검출 함수
int is_empty(QueueType* q)
{
	return (q->front == q->rear);
}

// 포화 상태 검출 함수
int is_full(QueueType* q)
{
	return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}

// 원형큐 출력 함수
void queue_print(QueueType* q)
{
	printf("QUEUE(front=%d rear=%d) = ", q->front, q->rear);
	if (!is_empty(q)) {
		int i = q->front;
		do {
			i = (i + 1) % (MAX_QUEUE_SIZE);
			printf("%d | ", q->data[i]);
			if (i == q->rear)
				break;
		} while (i != q->front);
	}
	printf("\n");
}

// 삽입 함수
void enqueue(QueueType* q, element item)
{
	if (is_full(q))
		error("큐가 포화상태입니다");
	q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
	q->data[q->rear] = item;
}

// 삭제 함수
element dequeue(QueueType* q)
{
	if (is_empty(q))
		error("큐가 공백상태입니다");
	q->front = (q->front + 1) % MAX_QUEUE_SIZE;
	return q->data[q->front];
}

// 삭제 함수
element peek(QueueType* q)
{
	if (is_empty(q))
		error("큐가 공백상태입니다");
	return q->data[(q->front + 1) % MAX_QUEUE_SIZE];
}
// ===== 원형큐 코드 끝 ======

// get_count 함수 전방선언
int get_count(QueueType* q);

int main(void) {
	int minutes = 60;
	int total_wait = 0;
	int total_customers = 0;
	int service_time = 0;
	int service_customer;
	QueueType queue;
	QueueType log; // 로그로 사용할 큐 생성
	init_queue(&queue);
	init_queue(&log); // log 큐 초기화

	srand(time(NULL));
	for (int clock = 0; clock < minutes; clock++) {
		// 대기자 수는 큐의 요소 개수를 반환해주는 get_count 함수를 호출해서 출력한다. queue에 들어있는 요소의 개수가 대기자 수이다.
		printf("현재시각=%d    [%d명 대기]\n", clock, get_count(&queue));
		if ((rand() % 10) < 3) {
			element customer;
			customer.id = total_customers++;
			customer.arrival_time = clock;
			customer.service_time = rand() % 3 + 1;
			enqueue(&queue, customer);
			printf("고객 %d이 %d분에 들어옵니다. 업무처리시간= %d분\n",
				customer.id, customer.arrival_time, customer.service_time);
		}
		if (service_time > 0) {
			printf("고객 %d 업무처리중입니다. \n", service_customer);
			service_time--;
		}
		else {
			if (!is_empty(&queue)) {
				element customer = dequeue(&queue);
				customer.start_time = clock; // 현재 업무를 시작하므로 현재시각을 업무시작시간에 넣어준다.
				enqueue(&log, customer); // 업무 시작 시간까지 포함한 customer의 정보를 log 큐에 enqueue한다.
				service_customer = customer.id;
				service_time = customer.service_time;
				printf("고객 %d이 %d분에 업무를 시작합니다. 대기시간은 %d분이었습니다.\n",
					customer.id, clock, clock - customer.arrival_time);
				total_wait += clock - customer.arrival_time;
			}
		}
	}
	printf("전체 대기 시간=%d분 \n", total_wait);
	while (1) { // 로그 기록을 보여주기 위한 while문
		if (is_empty(&log)) { // log 큐가 비었으면
			break; // while문에서 나간다.
		}
		// customer라는 객체에 log에서 dequeue한 데이터를 넣는다. 먼저 enqueue된 데이터부터 큐에 들어있으므로 나올때도 먼저 업무처리한 순서대로 나온다.
		element customer = dequeue(&log);
		printf("[%d]  %d분 도착, [%d분 대기] %d ~ %d분\n", customer.id, customer.arrival_time, customer.start_time - customer.arrival_time, customer.start_time, customer.start_time + customer.service_time);
	} // 대기시간은 업무 시작 시간에서 은행 도착시간을 뺀 값이다. 업무 종료 시간은 업무 시작 시간에 업무 처리 시간을 더해준 값이다.
	printf("대기고객 %d명\n", get_count(&queue)); // queue에 남아있는 요소의 개수를 출력한다. 60분이 끝나고도 업무처리를 못받은 사람은 queue에 남아있을 것이다.
	while (1) { // 업무처리를 받지 못한 사람의 정보를 출력하기 위한 while문
		if (is_empty(&queue)) { // queue 큐가 비었으면
			break; // while문에서 나간다.
		}
		element customer = dequeue(&queue); // customer라는 객체에 queue에서 dequeue한 데이터를 넣는다.
		printf("[%d]  %d분 도착, [%d분 대기] 작업시간 %d분\n", customer.id, customer.arrival_time, 60 - customer.arrival_time, customer.service_time);
	} // 60분에 은행업무가 종료되었으므로 대기 시간은 60 - 고객이 도착한 시간이다. 
	return 0;
}

// 큐의 요소 개수를 반환하는 함수
int get_count(QueueType* q) {
	int count;
	count = q->rear - q->front; // 요소 개수는 rear에서 front를 뺀 값이다.
	if (count < 0) { // 원형큐이기 때문에 rear보다 front가 클 수 있다. 만약 그렇게 되면 rear에서 front값을 뺀 값이 음수가 된다.
		count = count + MAX_QUEUE_SIZE; // 이런 경우에 요소의 개수는 그 음수 값에 큐의 size를 더해주면 요소 개수와 같아진다.
	}
	return count; // 개수 반환
}
