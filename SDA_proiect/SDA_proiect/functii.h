#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define clear system("cls");
#define err printf("a fost o eroare");

enum tip_bani {lei,euro,rubla};

typedef struct {
	int iban;
	int bani;
	char tip[3];
	int e;

}conturi;

typedef struct userul {
	int bani;
	char nume[20];
	char prenume[20];
	int cnp;
	int nr_conturi;
	conturi cont[5];
	FILE* extras;
	FILE* conturi;
	char parola[20];

	struct userul* urm;
} userul;

userul* head = NULL;
userul* aux;

void inceput(FILE* user) {
	char nume[20];
	char prenume[20];
	int cnp;
	int nr;
	char parola[20];
	
	user = fopen("clienti.txt", "r");
	while (fscanf(user, "%s %d %s %d %s", nume, &cnp, prenume,&nr,parola) == 5) {
		aux = (userul*)malloc(sizeof(userul));

		strcpy(aux->nume, nume);
		aux->cnp = cnp;
		strcpy(aux->prenume, prenume);
		aux->urm = NULL;
		aux->nr_conturi = nr;
		strcpy(aux->parola, parola);

		if (head == NULL || strcmp(aux->nume, head->nume) < 0) {
			aux->urm = head;
			head = aux;
		}
		else {
			
			userul* current = head;
			while (current->urm != NULL && strcmp(aux->nume, current->urm->nume) > 0) {
				current = current->urm;
			}
			aux->urm = current->urm;
			current->urm = aux;
		}

	}
	fclose(user);
	userul* auxul = head;
	char file[20];
	FILE* us;
	while (auxul) {
		if (auxul->nr_conturi > 0) {
			strcpy(file, auxul->nume);
			strcat(file, ".txt");
			us=fopen(file, "r");
			for (int i = 0; i < auxul->nr_conturi; i++) {
				auxul->cont[i].e = 1;
				fscanf(us,"%d %d", &auxul->cont[i].iban, &auxul->cont[i].bani);
				
			}
			fclose(us);
			for (int i = auxul->nr_conturi; i < 5; i++) {
				auxul->cont[i].e = 0;
			}
		}
		auxul = auxul->urm;
	}

	
}

void Afisare_cont() {

	userul* current = head;
	while (current != NULL) {
		if (current->nr_conturi > 0) {
			printf("%s: ", current->nume);
			for (int i = 0; i < current->nr_conturi; i++) {
				printf("IBAN:%d BANI:%d\n",current->cont[i].iban,current->cont[i].bani);
			}
		}

		current = current->urm;
	}
}

void Afisare() {

	userul* current = head;
	while (current != NULL) {
		printf("Nume: %s, Prenume: %s, CNP: %d Conturi:%d Parola:%s\n", current->nume, current->prenume, current->cnp,current->nr_conturi,current->parola);
		current = current->urm;
	}
}

int verificare_parola_admin(FILE* admin) {
	int verif;
	char parola[20];
	char parola_verif[20];
	admin = fopen("admin.txt","r");

	if (admin == NULL) {
		err;
		exit(0);
	}

	fscanf(admin, "%d", &verif);
	fclose(admin);

	if (verif == 0) {
		printf("adaugati o parola de Admin:");
		scanf("%s", parola);
		admin = fopen("admin.txt", "w");
		fprintf(admin, "1;%s",parola);
		fclose(admin);
		return 1;
	}
	if (verif == 1) {
		printf("Parola:");
		scanf("%s", parola);
		admin = fopen("admin.txt", "r");
		fscanf(admin, "%*d;%21[^\n]", parola_verif);

		if (strcmp(parola, parola_verif) == 0) {
			return 1;

		}
		else {
			return 0;
		}
	}

}

void adaugare_utiliz(FILE* user){
	clear;
	char nume[20];
	int cnp;
	char prenume[20];
	printf("dati numele:");
	scanf("%s", nume);
	clear;
	printf("dati prenumele:");
	scanf("%s", prenume);
	clear;
	printf("dati CNP:");
	scanf("%d",&cnp);
	int nr = 0;
	char parola[20];
	clear;
	printf("dati parola:");
	scanf("%s", parola);


	user = fopen("clienti.txt", "a");
	if (user == NULL) {
		err;
		exit(0);
	}
	fprintf(user, "%s %d %s %d %s\n", nume, cnp, prenume,nr,parola);
	fclose(user);
	
	aux= (userul*)malloc(sizeof(userul));
	
	strcpy(aux->nume, nume);
	aux->cnp = cnp;
	strcpy(aux->prenume, prenume);
	aux->nr_conturi = 0;
	aux->urm = NULL;
	strcpy(aux->parola, parola);

	if (head == NULL || strcmp(aux->nume, head->nume) < 0) {
		aux->urm = head;
		head = aux;
	}
	else {

		userul* current = head;
		while (current->urm != NULL && strcmp(aux->nume, current->urm->nume) > 0) {
			current = current->urm;
		}
		aux->urm = current->urm;
		current->urm = aux;
	}
}

void schimbare_nume(FILE *user) {

	clear;
	int cnp;
	printf("Dati un CNP: ");
	scanf("%d", &cnp);
	userul* aux=head;
	int ok = 0;
	while (aux) {
		if(aux->cnp==cnp){
			
			clear;
			printf("Utilizatorul:%s %s\n", aux->nume, aux->prenume);
			printf("Nume nou: \n");
			scanf("%s", aux->nume);
			printf("Prenume nou: \n");
			scanf("%s", aux->prenume);
			ok = 1;
		}
		aux = aux->urm;
	}
	clear;

	if (ok==0) {
		printf("CNP-ul este incorect");
		getchar();
		return;
	}
	
	
	user = fopen("clienti.txt","w");
	aux = head;
	while (aux) {
		fprintf(user,"%s %d %s %d %s\n",aux->nume ,aux->cnp,aux->prenume,aux->nr_conturi,aux->parola);
		aux = aux->urm;
	}
	fclose(user);
}

void adaugare_cont(FILE* user){
	clear;
	int cnp;
	printf("la ce user doriti sa ii adaugati un cont(CNP):");
	scanf("%d", &cnp);
	userul* aux = head;
	int iban;
	char file[20];


	while (aux) {
		if (aux->cnp == cnp) {
			if (aux->nr_conturi == 5) {
				clear;
				printf("ai depasit limita de conturi");
				getchar();
				getchar();
				
				return;
			}

			if (aux->nr_conturi == 0) {
				for (int i = 0; i < 5; i++) {
					aux->cont[i].e = 0;
				}
			}
			aux->nr_conturi += 1;

			user = fopen("clienti.txt", "w");
			 userul *auxil = head;
			while (auxil) {
				fprintf(user, "%s %d %s %d %s\n", auxil->nume, auxil->cnp, auxil->prenume, auxil->nr_conturi,auxil->parola);
				auxil = auxil->urm;
			}
			fclose(user);

			printf("adaugati un cont userului %s %s", aux->nume, aux->prenume);
			printf("\n\n IBAN:");
			scanf("%d", &iban);

			for (int i = 0; i < aux->nr_conturi; i++) {

				if (aux->cont[i].e == 0) {

					aux->cont[i].iban = iban;
					aux->cont[i].bani = 0;
					aux->cont[i].e = 1;
					break;
				}
			}
			strcpy(file, aux->nume);
			strcat(file, ".txt");
			aux->conturi = fopen(file,"a");
			
				
					fprintf(aux->conturi, "%d %d\n", aux->cont[aux->nr_conturi-1].iban, aux->cont[aux->nr_conturi-1].bani);
			
			fclose(aux->conturi);
			clear;
			break;
			
		}
		aux = aux->urm;
	}
	
}

void stergere_iban(FILE *user) {
	clear;
	int cnp;
	printf("la ce persoana doriti sa ii stergeti un cont ?\n");
	printf("dati CNP:");
	scanf("%d", &cnp);
	int iban;

	userul* aux = head;
	
	while (aux) {
		if (aux->cnp == cnp) {
			clear;
			printf("Utilizatorul:%s %s\n", aux->nume, aux->prenume);
			
			if (aux->nr_conturi == 0) {
				printf("acest utilizator nu are conturi");
				return;
			}
			
			printf("ce cont doriti sa stergeti (IBAN):");
			scanf("%d", &iban);

			for (int i = 0; i < aux->nr_conturi-1; i++) {

				if (iban == aux->cont[i].iban) {
					
					aux->cont[i] = aux->cont[i + 1];
				}
			}
			aux->nr_conturi -= 1;
			user = fopen("clienti.txt", "w");

			userul *auxul = head;
			while (auxul) {
				fprintf(user, "%s %d %s %d %s\n", auxul->nume, auxul->cnp, auxul->prenume, auxul->nr_conturi, auxul->parola);
				auxul = auxul->urm;
			}
			fclose(user);

			char file[20];
			strcpy(file, aux->nume);
			strcat(file, ".txt");
			aux->conturi = fopen(file, "w");
			for (int i = 0; i < aux->nr_conturi; i++) {
				if (aux->cont->e == 1) {
					fprintf(aux->conturi, "%d %d\n", aux->cont[i].iban, aux->cont[i].bani);
				}
			}
			fclose(aux->conturi);
			clear;
			return;

		}
		aux = aux->urm;
	}

}

void schimbare_parola_admin() {
	clear;
	char parola[20];
	int unu = 1;
	printf("Parola noua:");
	scanf("%s", parola);
	FILE* admin;
	admin = fopen("admin.txt", "w");
	fprintf(admin, "%d;%s", unu, parola);
	fclose(admin);
}

void ecran_admin(FILE* user) {
	
	while (1) {
		clear;
		printf("Adaugare utilizator(1)\n");
		printf("Adaugare IBAN(2)\n");
		printf("Afisare utilizatori(3)\n");
		printf("Stergere IBAN(4)\n");
		printf("Editare user(5)\n");
		printf("Schimbare parola(6)\n");
		printf("Delogare(7)\n");

		int opt;
		scanf("%d", &opt);

		switch (opt) {

		case 1:adaugare_utiliz(user);
			break;
			clear;

		case 2:adaugare_cont(user);
			break;
			clear;

		case 3:
			Afisare();
			getchar();
			getchar();
			break;

		case 4:stergere_iban(user);
			break;
			clear;

		case 5:schimbare_nume(user);
			break;
			clear;

		case 6:schimbare_parola_admin();
			break;
			clear;

		case 7:
			exit(0);
			break;

		}

	}

}

void depozit(userul *aux) {
	clear;
	int iban;
	char nume[20];
	int ok=0;
	int iban_s;
	int bani_s;
	strcpy(nume, aux->nume);
	strcat(nume, ".txt");
	printf("%s:\n", aux->nume);
	printf("Dati IBANUl contuli in care doriti sa adaugati suma:");
	scanf("%d", &iban);
	int bani;
	int i;
	for ( i = 0; i < aux->nr_conturi; i++) {

		if (aux->cont[i].iban == iban) {
			printf("ce suma doriti sa depuneti:");
			scanf("%d", &bani);
			aux->cont[i].bani += bani;
			ok = 1;
			break;
		}
	}
	
	if (ok) {
		aux->conturi = fopen(nume, "w");
		for (int j = 0; j < aux->nr_conturi; j++) {
			
				fprintf(aux->conturi, "%d %d\n", aux->cont[j].iban, aux->cont[j].bani);
		}
		fclose(aux->conturi);
	}
	clear;
	char extras[40];
	strcpy(extras, aux->nume);
	strcat(extras, "_Extras.txt");
	aux->extras = fopen(extras, "a");
	fprintf(aux->extras,"%s a depus %d in contul %d\n", aux->nume, bani, aux->cont[i].iban);
	fclose(aux->extras);
}

void retragere(userul* aux) {
	clear;
	int iban;
	char nume[20];
	int ok = 0;
	int iban_s;
	int bani_s;
	strcpy(nume, aux->nume);
	strcat(nume, ".txt");
	printf("%s:\n", aux->nume);
	printf("Dati IBANUl contuli in care doriti sa retrageti suma:");
	scanf("%d", &iban);
	int bani;
	int i;

	for (i = 0; i < aux->nr_conturi; i++) {

		if (aux->cont[i].iban == iban) {
			printf("ce suma doriti sa retrageti:");
			scanf("%d", &bani);
			aux->cont[i].bani -= bani;
			ok = 1;
			break;
		}
	}

	if (ok) {
		aux->conturi = fopen(nume, "w");
		for (int i = 0; i < aux->nr_conturi; i++) {

			fprintf(aux->conturi, "%d %d\n", aux->cont[i].iban, aux->cont[i].bani);
		}
		fclose(aux->conturi);
	}
	clear;
	char extras[40];
	strcpy(extras, aux->nume);
	strcat(extras, "_Extras.txt");
	aux->extras = fopen(extras, "a");
	fprintf(aux->extras, "%s a retras %d din contul %d\n", aux->nume, bani, aux->cont[i].iban);
	fclose(aux->extras);
}

void afisare_cont_user(userul *aux) {
	for (int i = 0; i < aux->nr_conturi; i++) {
		printf("IBAN:%d BANI:%d\n", aux->cont[i].iban, aux->cont[i].bani);
	}
	getchar();
	getchar();
}

void transfer(userul *aux) {
	clear;
	int iban;
	int i;
	int contulll=0;
	printf("Din ce cont vrei sa transferi bani (IBAN): ");
	scanf("%d", &iban);
	for ( i = 0; i < aux->nr_conturi; i++) {
		if (iban == aux->cont[i].iban) {
			
			break;
		}
	}
	
	int bani;
	printf("\ncati bani:");
	scanf("%d", &bani);
	int cnp;
	printf("cui vrei sa ii transferi bani (CNP): ");
	scanf("%d", &cnp);
	userul* auxul = head;
	char nume[20];
	char numele[20];
	
	clear;
	int j;
	
	while (auxul) {
		if (auxul->cnp == cnp) {
			
			strcpy(nume, auxul->nume);
			strcat(nume, ".txt");
			auxul->conturi = fopen(nume,"w");

			strcpy(numele, aux->nume);
			strcat(numele, ".txt");
			aux->conturi = fopen(numele, "w");

			printf("in ce cont a lui %s %s (IBAN):", auxul->nume, auxul->prenume);
			scanf("%d", &iban);
			for ( j = 0; j < auxul->nr_conturi; j++) {
				if (iban == auxul->cont[j].iban) {
					contulll = auxul->cont[j].iban;
					auxul->cont[j].bani += bani;
					aux->cont[i].bani -= bani;
					for (int k = 0; k < auxul->nr_conturi; k++) {
						fprintf(auxul->conturi, "%d %d\n", auxul->cont[k].iban, auxul->cont[k].bani);
						
					}
					fclose(auxul->conturi);

					for (int k = 0; k < aux->nr_conturi; k++) {
						fprintf(aux->conturi, "%d %d\n", aux->cont[k].iban, aux->cont[k].bani);

					}
					fclose(aux->conturi);

				}
			}
		}
		auxul = auxul->urm;
	}
	clear;
	char extras[40];
	strcpy(extras, aux->nume);
	strcat(extras, "_Extras.txt");
	aux->extras = fopen(extras, "a");
	fprintf(aux->extras, "%s a transferat %d din contul %d in contul %d\n", aux->nume, bani, aux->cont[i].iban,contulll);
	fclose(aux->extras);
	
}

void afiasre_extras(userul *aux) {
	char line[100];
	char extras[40];
	strcpy(extras, aux->nume);
	strcat(extras, "_Extras.txt");
	aux->extras = fopen(extras, "r");
	while (fgets(line, sizeof(line), aux->extras)) {
		printf("%s", line);
	}
	fclose(aux->extras);
	getchar();
	getchar();
}

void schimbare_parola_user(userul *aux) {
	clear;
	printf("Parola noua:");
	scanf("%s", aux->parola);
	FILE* user;
	
	user = fopen("clienti.txt", "w");
	userul* auxil = head;
	while (auxil) {
		fprintf(user, "%s %d %s %d %s\n", auxil->nume, auxil->cnp, auxil->prenume, auxil->nr_conturi, auxil->parola);
		auxil = auxil->urm;
	}
	fclose(user);
}

void ecran_user(FILE *user) {
	clear;
	char parola[20];
	int opt;
	printf("Dati parola:");
	scanf("%s", parola);
	userul* auxul = head;
	while (auxul) {
		if (strcmp(auxul->parola,parola)==0){

			while(1){
				clear;
				printf("Utilizator: %s %s\n\n", auxul->nume, auxul->prenume);
				printf("Depozit(1)\n");
				printf("Retragere(2)\n");
				printf("Transfer(3)\n");
				printf("Afisare contrui(4)\n");
				printf("Afisare extrase(5)\n");
				printf("Schimbare parola(6)\n");
				printf("Deloggare(7)\n");

				scanf("%d", &opt);

				switch (opt) {

				case 1:
					depozit(auxul);
					break;

				case 2:
					retragere(auxul);
					break;

				case 3:
					transfer(auxul);
					break;

				case 4:
					afisare_cont_user(auxul);
					break;

				case 5:
					afiasre_extras(auxul);
					break;

				case 6:
					schimbare_parola_user(auxul);
					break;

				case 7:
					exit(0);

				}

			}
			
		}
		auxul = auxul->urm;
	}

}