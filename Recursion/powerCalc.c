#include <stdio.h>
double power(double x, int n);//전방 선언
int main() {
	double x;//x는 소수도 포함하게 할 것이므로 int형이 아닌 double형으로 x 선언.
	int n;
	while (1) {
		printf(">> ");
		scanf("%lf %d", &x, &n);//x의 n제곱을 구하기 위해 x와 n을 입력받음.
		if (n == 0)//종료조건, n이 0이면
			break;//프로그램 실행 종료
		else printf("%.1f ^ %d = %.3f\n", x, n, power(x, n));//n이 0이 아니라면 power 함수의 최종 반환값을 출력
	}
	return 0;
}
void indent(int);//전방 선언
int tab = 0;//공백 0으로 초기화
double power(double x, int n) {//x의 n제곱을 구하는 power 함수
	double result;
	if (n == 0)//n이 0이면
		return 1;//순환 종료
	indent(tab);//print문 앞에 공백 출력
	printf("power(%.3f, %d)\n", x, n);//공백 한 칸씩 늘어나면서 print문 출력
	++tab;//공백 한 칸 늘리기
	if (n % 2 == 0) {//n이 짝수이면
		result = power(x * x, n / 2);//result는 함수 power(x * x, n / 2)의 리턴값
	}
	else {//n이 홀수이면
		result = x * power(x * x, (n - 1) / 2);//result는 x * 함수 power(x * x, (n - 1)/ 2)의 리턴값
	}//(n - 1)을 한 이유는 홀수를 2로 나누게 되면 소수점 부분이 잘리기 때문. (n - 1)에 대한 보정으로 앞에 x를 곱해줌. 
	--tab;//공백 한 칸 줄이기
	if (n > 1) {//n = 1일때는 밑의 프린트문이 출력되지 않게 n > 1이라는 조건 걸어줌.
		indent(tab);
		printf("power(%.3f, %d)= %.3f\n", x, n, result);//공백 한 칸씩 줄어들면서 프린트문 출력
	}
	return result;//최종 결과값을 반환
}
void indent(int tab) {//공백을 출력하는 함수
	int i;
	for (i = 0; i < tab; i++)//tab이 늘어나거나 줄어든 만큼
		printf("    ");//공백 출력
}
