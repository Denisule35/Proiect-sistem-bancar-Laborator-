#include <stdio.h>
#include <stdlib.h>
#include "functii.h"
enum utilizator{user=1,admin};
#define clear system("cls");

FILE* admin_f;
FILE* user_f;

int main() {
	inceput(user_f);
	
	printf("doresti sa intri ca admin sau ca utilizator:\n\n");
	printf("Utilizator(1)\n");
	printf("Admin(2)\n");

	int opt;
	scanf("%d", &opt);
	switch (opt) {
	case user:
		ecran_user(user_f);
		clear;
		
		break;
	case admin:
		clear;
		if (verificare_parola_admin(admin_f)==0) {
			int ok = 0;
			while (ok == 0) {
				
				clear;
				if (verificare_parola_admin(admin_f)==1) {
					ok = 1;
				}
			}
		}
		clear;
		ecran_admin(user_f);
		break;
	}
	Afisare();
	return 0;
}