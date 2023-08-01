#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAZE_SIZE 20
#define WALL '1'
#define ROAD '0'
#define GOAL 'G'
#define VISITED '.'
#define PATH '#'

typedef struct {
	int x;
	int y;
}element;

typedef struct {
	element* data;
	int capacity;
	int top;
}StackType;

int neighbor(element next, element here);
int maze_size = 0;
element entry = { 0, 0 };
char maze[MAZE_SIZE][MAZE_SIZE];

void init_stack(StackType* s) {
	s->top = -1;
	s->capacity = 100;
	s->data = (element*)malloc(s->capacity * sizeof(element));
}

int is_empty(StackType* s) {
	return (s->top == -1);
}

int is_full(StackType* s) {
	return (s->top == (s->capacity - 1));
}

void push(StackType* s, element item) {
	if (is_full(s)) {
		s->capacity *= 2;
		s->data = (element*)realloc(s->data, s->capacity * sizeof(element));
	}
	s->data[++(s->top)] = item;
}

element pop(StackType* s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택이 비었습니다\n");
		exit(1);
	}
	else return s->data[(s->top)--];
}

// 위치를 스택에 삽입
// 미로 내의 좌표이고 벽이나 이미 방문한 경로가 아니면
// 그것을 element를 만들어서 push
void push_loc(StackType* s, int x, int y)
{
	if (x < 0 || y < 0) return;
	if (x >= maze_size || y >= maze_size) return;
	if (maze[x][y] != WALL && maze[x][y] != PATH) {
		element tmp;
		tmp.x = x;
		tmp.y = y;
		push(s, tmp);
	}
}

// 미로를 화면에 출력한다. 
void maze_print()
{
	printf("\n");
	for (int x = 0; x < maze_size; x++) {
		printf("%s\n", maze[x]);  // 한 행씩 문자열처럼 출력
	}
}

void init_maze(element* entry)
{
	printf("maze size = ");
	scanf_s("%d", &maze_size);
	for (int x = 0; x < maze_size; x++) {
		printf("%d행: ", x);  // 한 행씩 문자열처럼 입력
		scanf_s("%s", maze[x], MAZE_SIZE);
	}
}

void print_stack(StackType* s) {
	for (int i = 0; i <= s->top; i++) {
		printf("%d,%d   ", s->data[i].x, s->data[i].y);
	}
	printf("\n");
}

int main(void)
{
	int x, y;
	char buf[10];
	element here;
	element next;
	StackType s;
	StackType path;
	init_maze(&entry);
	here = entry;  // 구조체 지정 (x, y) 값 복사
	init_stack(&s);
	init_stack(&path);
	push_loc(&path, here.x, here.y);
	while (maze[here.x][here.y] != GOAL) {
		maze[here.x][here.y] = PATH; // 현재 위치를 '#'으로 표시
		gets_s(buf, 10);
		maze_print(maze);
		push_loc(&s, here.x - 1, here.y);  // 상
		push_loc(&s, here.x + 1, here.y);  // 하
		push_loc(&s, here.x, here.y - 1);  // 좌
		push_loc(&s, here.x, here.y + 1); // 우.. 갈수 있는 곳을 모두 push
		if (is_empty(&s)) {  // 더 이상 갈 곳이 없으면 길이 없다는 뜻
			printf("실패\n");
			return 1;
		}
		next = pop(&s);
		while (neighbor(next, here) != 1) { // 현재 위치와 다음에 가야할 곳이 인접해 있지 않다면
			maze[here.x][here.y] = VISITED; // "."을 찍고
			here = pop(&path); // path 스택을 팝하여 이전의 위치로 돌아간다.
		}
		here = next;
		push_loc(&path, here.x, here.y);
	}
	printf("성공\n");
	return 0;
}

int neighbor(element next, element here) {
		if (((here.x == next.x - 1) && (here.y == next.y)) || ((here.x == next.x + 1) && (here.y == next.y)) || ((here.x == next.x) && (here.y == next.y + 1)) || ((here.x == next.x) && (here.y == next.y - 1)))
			return 1; // 다음에 가야할 곳과 현재 위치가 인접해 있다면 1 반환
		else return 0; // 인접하지 않으면 0 반환
}