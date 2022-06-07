#include <stdio.h>
#include <iostream>
#include <vector>
#include<math.h>
#include<cmath>

using namespace std;

void search(double& b, double& c, double& d, int n, vector<double>x, vector<double>y);
void search_d(double& b, double& c, double& d, int n, vector<double>x, vector<double>y, double a);
void search_c(double& b, double& c, double d, int n, vector<double>x, vector<double>y, double a, double ave_x, double ave_y);
void search_b(double& b, double c, double d, int n, vector<double>x, vector<double>y, double a);
void lsm(double& b, double& c, double d, int n, vector<double>x, vector<double>y);
void gosa_cal(double& gosa, double b, double c, double d, int n, vector<double>x, vector<double>y);
void get_data(vector<double>& x, vector<double>& y);

int main() {
	printf("hellow\n");

	vector<double> x; //電流
	vector<double> y; //関節角度

	double b = 0.0, c = 0.0, d = 35;
	int n = 0;

	get_data(x, y);
	n = x.size();

	printf("first %d\n", n);

	///*for (int i = 0; i < n; i++){
	//	printf("%lf %lf\n", x[i], y[i]);
	//}*/

	search(b, c, d, n, x, y);

	printf("finish  %lf  %lf  %lf \n", b, c, d);

	double E = d + 1 / b + 1 / c;

	printf("E is %lf", E);

	return 0;
}



void search(double& b, double& c, double& d, int n, vector<double>x, vector<double>y) {

	double a = 1.0;
	search_d(b, c, d, n, x, y, a);

	printf("\n\n\n\n\nd finish \n\n\n\n\n");

	double ly = 0.0;
	double sum_x = 0.0, sum_y = 0.0;
	double ave_x = 0.0, ave_y = 0.0;
	for (int i = 0; i < n; i++) {
		if (y[i] == 0) {
			y[i] = 1 / pow(10, 10);
		}
		sum_x += x[i];
		ly = abs((d - y[i]) / (y[i]));
		sum_y += log(ly);
	}
	ave_x = sum_x / n;
	ave_y = sum_y / n;

	a = 1.0;
	search_c(b, c, d, n, x, y, a, ave_x, ave_y);

	printf("\n\n\n\n\nc finish\n\n\n\n\n");

	a = 1.0;
	search_b(b, c, d, n, x, y, a);



	return;
}


void search_d(double& b, double& c, double& d, int n, vector<double>x, vector<double>y, double a) {

	double now = 0.0, next = 0.0;
	lsm(b, c, d, n, x, y);

	gosa_cal(now, b, c, d, n, x, y);
	printf("%lf d %lf   now gosa %lf\n", a, d, now);

	double k, l, m;
	k = d - a;

	//if (k < 0) {
	//	return;
	//}

	lsm(l, m, k, n, x, y);

	gosa_cal(next, l, m, k, n, x, y);
	printf("         k %lf   next gosa %lf \n",k, next);

	if (next < now) {
		d = k;
		printf("\n\nCHANGE!!\n\n");
		search_d(b, c, d, n, x, y, a);		
	}

	printf("\nNEXT CHANCE\n");

	if (a == 1.0) {
		a = 0.1;
		search_d(b, c, d, n, x, y, a);
	}
	if (a == 0.1) {
		a = 0.01;
		search_d(b, c, d, n, x, y, a);
	}

	printf("\n\nNO CHANGE!!  %lf\n\n", a);

    double aa = 0;
	if (a == 1.0) {
		aa = -0.1;
		search_d(b, c, d, n, x, y, aa);

		aa = -0.01;
		search_d(b, c, d, n, x, y, aa);
	}
	else if (a == 0.1) {
		aa = -0.01;
		search_d(b, c, d, n, x, y, aa);
	}	
	
	printf("\n\nprogram finish!! %lf\n", d);

	return;
}


void search_c(double& b, double& c, double d, int n, vector<double>x, vector<double>y, double a, double ave_x, double ave_y) {
	//printf("hellow2\n\n");
	double now = 0.0, next = 0.0;

	double sub1 = 0.0, sub2 = 0.0;

	sub1 = c - ave_x;
	if (sub1 == 0) {
		sub1 = 1 / pow(10, 10);
	}
	b = ave_y / sub1;

	gosa_cal(now, b, c, d, n, x, y);
	printf("%lf c %lf   now gosa %lf\n", a, c, now);

	double l, m, k;
	k = d;
	m = c - a;

	//if (m < 0) {
	//	return;
	//}

	sub2 = c - ave_x;
	if (sub2 == 0) {
		sub2 = 1 / pow(10, 10);
	}
	l = ave_y / sub2;

	gosa_cal(next, l, m, k, n, x, y);
	printf("         m %lf   next gosa %lf \n", m, next);


	if (next < now) {
		c = m;
		printf("\n\nCHANGE2!!\n\n");
		search_c(b, c, d, n, x, y, a, ave_x, ave_y);
	}


	printf("\nNEXT CHANCE 2\n");


	if (a == 1.0) {
		a = 0.1;
		search_c(b, c, d, n, x, y, a, ave_x, ave_y);
	}
	if (a == 0.1) {
		a = 0.01;
		search_c(b, c, d, n, x, y, a, ave_x, ave_y);
	}

	printf("\n\nNO CHANGE2!!  %lf\n\n", a);

	double aa = 0;
	if (a == 1.0) {
		aa = -0.1;
		search_c(b, c, d, n, x, y, aa, ave_x, ave_y);

		aa = -0.01;
		search_c(b, c, d, n, x, y, aa, ave_x, ave_y);
	}
	else if (a == 0.1) {
		aa = -0.01;
		search_c(b, c, d, n, x, y, aa, ave_x, ave_y);
	}

	printf("\n\nprogram finish!!  c  %lf\n", c);

	return;

}


void search_b(double& b, double c, double d, int n, vector<double>x, vector<double>y, double a) {
	//printf("hellow2\n\n");
	double now = 0.0, next = 0.0;

	gosa_cal(now, b, c, d, n, x, y);
	printf("%lf b %lf   now gosa %lf\n", a, b, now);

	double l, m, k;
	k = d;
	m = c;
	l = b - a;

	//if (l < 0) {
	//	return;
	//}

	gosa_cal(next, l, m, k, n, x, y);
	printf("         l %lf   next gosa %lf \n", l, next);


	if (next < now) {
		b = l;
		printf("\n\nCHANGE3!!\n\n");
		search_b(b, c, d, n, x, y, a);
	}

	printf("\nNEXT CHANCE 3\n");

	if (a == 1.0) {
		a = 0.1;
		search_b(b, c, d, n, x, y, a);
	}
	if (a == 0.1) {
		a = 0.01;
		search_b(b, c, d, n, x, y, a);
	}


	printf("\n\nNO CHANGE3!!  %lf\n\n", a);

	double aa = 0;
	if (a == 1.0) {
		aa = -0.1;
		search_b(b, c, d, n, x, y, aa);

		aa = -0.01;
		search_b(b, c, d, n, x, y, aa);
	}
	else if (a == 0.1) {
		aa = -0.01;
		search_b(b, c, d, n, x, y, aa);
	}

	printf("\n\nprogram finish!! b  %lf\n", b);

	return;

}


void lsm(double& b, double& c, double d, int n, vector<double>x, vector<double>y) {
	double ave_x = 0.0, ave_y = 0.0;
	double sum_x = 0.0, sum_x2 = 0.0, sum_y = 0.0;
	double cov = 0.0;
	double a = 0.0;

	for (int i = 0; i < n; i++) {
		if (y[i] == 0) {
			//printf("yes");
			y[i] = 1 / pow(10, 10);
		}
		sum_x += x[i];
		sum_x2 += x[i] * x[i];
		a = abs((d - y[i]) / (y[i]));
		//printf("%lf %lf %lf\n", x[i], a, log(a));
		sum_y += log(a);
		cov += x[i] * log(a);

	}


	ave_x = sum_x / n;
	ave_y = sum_y / n;
	double A = 0.0, B = 0.0, C = 0.0;
	A = (n * cov - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
	B = (sum_x2 * sum_y - cov * sum_x) / (n * sum_x2 - sum_x * sum_x);
	b = -1 * A;

	
	C = ave_y - A * ave_x;
	c = C / b;
	printf("A B C %lf %lf %lf\n\n", b, c, d);

	return;
}


void gosa_cal(double& gosa ,double b, double c, double d, int n, vector<double>x, vector<double>y) {
	gosa = 0;
	double y_2;
	double a = 0;

	for (int i = 0; i < n; i++) {
		y_2 = d / (1 + exp(b * (c - x[i])));

		a = abs((y[i] - y_2));
		//printf("gosa %lf\n\n", a*a);
		gosa += a * a;
	}

	return;
}


void get_data(vector<double>& x, vector<double>& y) {

	FILE* fp; // FILE型構造体
	char fname[] = "16°56'21.txt";

	fp = fopen(fname, "r"); // ファイルを開く。失敗するとNULLを返す。
	if (fp == NULL) {
		printf("%s file not open!\n", fname);
		return;
	}
	else {
		printf("%s file opened!\n", fname);
	}

	double f1, f2;

	while (fscanf(fp, "%lf %lf", &f1, &f2) != EOF) {
		x.push_back(f1);
		y.push_back(f2);
	}

	fclose(fp); // ファイルを閉じる

	return;
}



