#include <stdio.h>
double power(double x, int n);//���� ����
int main() {
	double x;//x�� �Ҽ��� �����ϰ� �� ���̹Ƿ� int���� �ƴ� double������ x ����.
	int n;
	while (1) {
		printf(">> ");
		scanf("%lf %d", &x, &n);//x�� n������ ���ϱ� ���� x�� n�� �Է¹���.
		if (n == 0)//��������, n�� 0�̸�
			break;//���α׷� ���� ����
		else printf("%.1f ^ %d = %.3f\n", x, n, power(x, n));//n�� 0�� �ƴ϶�� power �Լ��� ���� ��ȯ���� ���
	}
	return 0;
}
void indent(int);//���� ����
int tab = 0;//���� 0���� �ʱ�ȭ
double power(double x, int n) {//x�� n������ ���ϴ� power �Լ�
	double result;
	if (n == 0)//n�� 0�̸�
		return 1;//��ȯ ����
	indent(tab);//print�� �տ� ���� ���
	printf("power(%.3f, %d)\n", x, n);//���� �� ĭ�� �þ�鼭 print�� ���
	++tab;//���� �� ĭ �ø���
	if (n % 2 == 0) {//n�� ¦���̸�
		result = power(x * x, n / 2);//result�� �Լ� power(x * x, n / 2)�� ���ϰ�
	}
	else {//n�� Ȧ���̸�
		result = x * power(x * x, (n - 1) / 2);//result�� x * �Լ� power(x * x, (n - 1)/ 2)�� ���ϰ�
	}//(n - 1)�� �� ������ Ȧ���� 2�� ������ �Ǹ� �Ҽ��� �κ��� �߸��� ����. (n - 1)�� ���� �������� �տ� x�� ������. 
	--tab;//���� �� ĭ ���̱�
	if (n > 1) {//n = 1�϶��� ���� ����Ʈ���� ��µ��� �ʰ� n > 1�̶�� ���� �ɾ���.
		indent(tab);
		printf("power(%.3f, %d)= %.3f\n", x, n, result);//���� �� ĭ�� �پ��鼭 ����Ʈ�� ���
	}
	return result;//���� ������� ��ȯ
}
void indent(int tab) {//������ ����ϴ� �Լ�
	int i;
	for (i = 0; i < tab; i++)//tab�� �þ�ų� �پ�� ��ŭ
		printf("    ");//���� ���
}
