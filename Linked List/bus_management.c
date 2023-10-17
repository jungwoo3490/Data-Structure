#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode { // 노드 타입
	char name;  // 승객명
	int arrive_time; // 도착시간
	int company;  // 동승인원
	struct ListNode* link;
} ListNode;

// 연결 리스트 헤더
typedef struct ListType { // 리스트 헤더 타입
	int size;		 // 현재 리스트의 노드 개수
	int num_aboard;  // 현재 리스트의 전체 인원
	char name[10];   // 리스트(그룹)의 이름
	ListNode* head;
	ListNode* tail;
	struct ListType* link;
} ListType;

void error(char* message) {
	fprintf(stderr, "%s\n", message);
	exit(1);
}

ListType* create(char* name);
void insert_last(ListType* plist, ListNode* p);
void delete_node(ListType* plist1, ListNode* p);
void poly_print(ListType* plist);

// plist에서 p를 제거
void delete_node(ListType* plist, ListNode* p);
// plist1과 plist2를 도착순서에 따라 plist3에 합친다
void merge_q(ListType* plist1, ListType* plist2, ListType* plist3);

// list3에서 capa만큼 태운 버스(리스트)를 리턴, list3에서는 삭제
ListType* next_bus(ListType* list3, int capa, char* name);

void generate_q(ListType* list, char name) {
	int n = 0;
	ListNode node;   // 새로 만들 노드의 값을 전달하는 객체 지역변수
	while (1) {
		scanf_s("%d %d", &node.company, &node.arrive_time);
		if (node.company == 0) break;
		node.name = name++;
		insert_last(list, &node);  // 함수에서 생성해서 복사
	}
}

int main(void) {
	// 기본 단계
	ListType* list1, * list2, * list3;

	// 연결 리스트 헤더 생성 - 이름 추가할 수 있게 create 확장
	list1 = create("list1"); generate_q(list1, 'A');
	list2 = create("list2"); generate_q(list2, 'a');
	list3 = create("merged");

	poly_print(list1);  // 고객수와 인원을 출력
	poly_print(list2);
	merge_q(list1, list2, list3);  // 대기줄을 하나로 합침
	poly_print(list3);

	printf("\n\n=== 도전 단계 ===\n\n");
	char busname[5] = "ABus";

	// 각 버스에는 승차한 고객리스트, bushead는 버스들의 연결리스트 헤더
	ListType* bus = NULL, * bushead = NULL;
	while (list3->size > 0) {
		bus = next_bus(list3, 5, busname); // 5명까지 승차
		busname[0]++;
		bus->link = bushead;
		bushead = bus;
	}
	bus = bushead;
	while (bus != NULL) {
		poly_print(bus);
		bus = bus->link;
	}
}

ListType* create(char* name) {
	ListType* plist = (ListType*)malloc(sizeof(ListType));//리스트 하나 생성
	plist->size = 0;
	plist->num_aboard = 0;
	strcpy(plist->name, name); // 매개변수로 받은 이름을 strcpy로 리스트 이름에 복사
	plist->head = plist->tail = NULL; // 리스트가 비었을 때 head와 tail은 NULL
	return plist; // 만든 리스트 리턴
}

void insert_last(ListType* plist, ListNode* p) {
	ListNode* node = (ListNode*)malloc(sizeof(ListNode)); // 노드를 생성한다.
	node->arrive_time = p->arrive_time; // 생성한 노드의 도착시간에 p의 도착시간 대입
	node->company = p->company; // 생성한 노드의 동승인원에 p의 동승인원 대입
	node->name = p->name; // 생성한 노드의 이름에 p의 이름 대입
	node->link = NULL; // 생성한 노드의 링크를 NULL로 초기화
	if (plist->tail == NULL) { // tail이 NULL이면, 즉 리스트가 비어있으면
		plist->head = plist->tail = node; // head와 tail이 모두 노드를 가리키게 함.
	}
	else { // 비어있지 않으면
		plist->tail->link = node; // 노드를 맨 뒤에 추가
		plist->tail = node; // tail이 새로 추가한 노드를 가리키게 함
	}
	plist->size++; // 추가했으므로 노드 개수 1 증가
}

void delete_node(ListType* plist1, ListNode* p) {
	ListNode *pre = (ListNode*)malloc(sizeof(ListNode));//삭제할 노드 이전 노드를 가리킴
	if (p == plist1->head) { // p가 head를 가리키면, 즉 첫번째 노드를 삭제하고 싶으면
		plist1->head = p->link; // head가 삭제할 노드 다음 노드를 가리키게 하고
		free(p); // p free해주기
	}
	else { // 맨 앞 노드가 아니면
		pre = plist1->head; // pre가 맨 첫번째 노드 가리키게 한 다음
		for (int i = 0; pre->link != p; i++) { // pre의 다음이 p가 될때까지 
			pre = pre->link; // 계속 링크타고 이동
		}
		if (p == plist1->tail) { // 만약 p가 맨 끝 노드를 가리키면
			plist1->tail = pre; // tail이 pre가 가리키는 노드를 가리키게 한 후
			free(p); // p를 free 해준다
		}
		else {//맨 끝이 아니면
			pre->link = p->link; // pre의 다음이 p의 다음이 되게 한 후
			free(p); // p를 free해준다.
		}
	}
	plist1->size--; // 하나 삭제되었으므로 노드 개수 1 감소
}

// 출력 함수 
void poly_print(ListType* plist) {
	int count = 0; // 몇 팀이 있는지 세어주는 카운트. 5팀을 찍었는지 판별하는데도 쓰인다. 
	plist->num_aboard = 0; // 전체 인원을 0으로 초기화(next_bus 함수에서 num_abroad를 조작하는 부분이 있기 때문에 0이 아닐 수 있으므로 0으로 초기화)
	ListNode* p = plist->head; // p가 리스트의 head 노드를 가리키게 한다.
	printf("%s ", plist->name); // 리스트 이름을 먼저 출력한다.
	for (; p; p = p->link) { // p가 NULL이 아닐 때까지 p는 p의 링크를 타며 이동하면서 반복
		printf("%c/%d(%d) ", p->name, p->company, p->arrive_time); // 이름, 동승인원, 도착시간 순서로 출력
		count = count + 1; // 카운트를 하나 증가시켜준다.
		plist->num_aboard += p->company; // 리스트의 전체 인원은 모든 노드의 동승인원의 합
		if (count % 5 == 0) { // count가 5의 배수이면(5개 출력했으면)
			printf("\n      "); // 줄바꿈
		}
	}
	printf("  [%d팀/%d명]\n", count, plist->num_aboard); // 총 팀 수는 count이고, 총 인원은 리스트의 num_abroad이다.
}

void merge_q(ListType* plist1, ListType* plist2, ListType* plist3) {
	ListNode* a = plist1->head; // a는 첫번째 리스트의 첫번째 노드 가리킴
	ListNode* b = plist2->head; // a는 두번째 리스트의 첫번째 노드 가리킴

	while (a && b) { // a도 NULL이 아니고 b도 NULL이 아닐 동안
		if (a->arrive_time == b->arrive_time) { // a와 b가 동시에 도착했다면
			if (a->company <= b->company) { // a의 동승인원이 b의 동승인원보다 적거나 같으면
				insert_last(plist3, a); // a 먼저 넣고
				insert_last(plist3, b); // b 넣는다.
				a = a->link; // a를 다음으로 넘기고
				b = b->link; // b도 다음으로 넘긴다
			}
			else { // b의 동승인원이 a의 동승인원보다 적으면
				insert_last(plist3, b); // b 먼저 넣고
				insert_last(plist3, a); // a 넣는다.
				a = a->link; // a를 다음으로 넘기고
				b = b->link; // b도 다음으로 넘긴다
			}
		}
		else if (a->arrive_time < b->arrive_time) { // a가 b보다 먼저 왔으면 
			insert_last(plist3, a); // a를 넣고
			a = a->link; // a를 다음으로 넘긴다.
		}
		else { // b가 a보다 먼저 왔으면 
			insert_last(plist3, b); // b를 넣고
			b = b->link; // b를 다음으로 넘긴다.
		}
	}

	// 하나가 먼저 NULL이 되면 그 다른쪽에 남은 팀들 리스트에 다 넣기
	for (; a != NULL; a = a->link)
		insert_last(plist3, a);
	for (; b != NULL; b = b->link)
		insert_last(plist3, b);
}

ListType* next_bus(ListType* list3, int capa, char* name) {
	ListType* bus = create(name); // 매개변수로 받은 이름을 가진 bus 리스트 생성
	ListNode *p = (ListNode*)malloc(sizeof(ListNode));
	p = list3->head; // p는 전체 리스트의 맨 앞 노드를 가리키게 함
	while (bus->num_aboard < capa && p != NULL) { // 현재 버스 인원이 버스 한계 인원보다 적고, 아직 리스트가 비지 않았으면
		if (bus->num_aboard + p->company > capa) { // 현재 버스 인원에 p의 동승인원을 더한 것이 버스 한계인원을 넘으면
			while (1) {
				p = p->link; // p를 다음으로 계속 넘긴다. 
				if (p == NULL) { // p가 NULL이 되면 끝까지 다 검사했는데 만족하는 인원의 팀이 없는 것이므로 
					return bus; // 그대로 버스를 리턴한다.
				}
				if (bus->num_aboard + p->company <= capa) { // 더했을 때 한계인원을 넘지 않는 팀을 찾으면
					break; // p는 그 팀(노드)을 가리킨 채로 무한반복 루프를 벗어난다.
				}
			}
		}
		insert_last(bus, p); // 버스에 p를 넣는다.
		bus->num_aboard = bus->num_aboard + p->company; // 현재 버스 인원에 방금 추가한 p의 인원수를 더한다.
		delete_node(list3, p); // p를 전체 리스트에서 제거하고
		p = list3->head; // p는 전체 리스트의 맨 앞 노드를 가리키게 한다.
	}
	return bus; // 만든 버스 리턴
}