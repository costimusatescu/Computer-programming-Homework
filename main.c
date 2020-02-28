#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fabrica{
	char tip[100];
	int nr;
	char oras[100];
	char dir;
};


struct cadou{
    char tip[100];
    int nr;
    char oras[100];
};

// Functie colectare cadouri
void colecteaza(struct fabrica *f, struct cadou *sac, int *nrcadou){
    // Verifica daca mai sunt cadouri disponibile
	if(f->nr == 0){
        return;
    }
	
    struct cadou c;
    strcpy(c.tip,f->tip);   // trece tipul cadoului din f in c
    strcpy(c.oras, f->oras);    // trece orasul din f in c
    c.nr = 1;   // setare numar cadou cu 1
	
    sac[*nrcadou] = c;  // trecerea din structura de tip cadou c in sac
    (*nrcadou)++;   // incrementare numar cadou
    f->nr--;    // decrementare numar cadouri disponibile
}

// Functie plimbare sanie
void plimba(struct fabrica *harta, int *ls, int *cs, int c){
    if(harta[*ls * c + *cs].dir == 'U'){
            (*ls)--;
            return;
        }
        if(harta[*ls * c + *cs].dir == 'D'){
            (*ls)++;
            return;
        }
        if(harta[*ls * c + *cs].dir == 'R'){
            (*cs)++;
            return;
        }
        if(harta[*ls * c + *cs].dir == 'L'){
            (*cs)--;
            return;
        }

}

// Functie cautare cadouri in sac
int cauta_cadou(struct cadou *sac, int nr_cad, char *tip_cadou) {
    int i;
	
    for(i=0; i<nr_cad; i++){
        if(strcmp(tip_cadou, sac[i].tip) == 0)
            return i;
    }
	
    return -1;
}

// Functie sortarea cadourilor din sac
void sortare_cadouri(struct cadou *sac, int nr_cad) {
    int gv = sac[0].nr, gi = 0, gf=0, i, j, k;
    struct cadou tmp;

    for(i = 0; i<nr_cad; i++) {
        if(gv != sac[i].nr) {

            if(gf != gi) {
                for(j=gi; j<gf; j++)
					for(k=j+1; k <=gf; k++)
						if(strcmp(sac[j].tip, sac[k].tip) > 0) {
							tmp = sac[j];
							sac[j] = sac[k];
							sac[k] = tmp;
						}
				}

            gv = sac[i].nr;
            gi = i;
            gf=i;
        }
        else
        {
            gf = i;
        }
    }
	
    if(gf != gi) {
        for(j=gi; j<gf; j++)
			for(k=j+1; k <=gf; k++)
				if(strcmp(sac[j].tip, sac[k].tip) > 0) {
					tmp = sac[j];
					sac[j] = sac[k];
					sac[k] = tmp;
				}
    }

}

int main()
{
    int capacitate = 2;
    int nrcadou = 0;
    int l, c, p, i, j, k;
    int ls, cs;
    int nr;
    char dir;
    struct fabrica *harta;
    struct cadou *sac;
    char oras_tmp[100];

    char **orase;
    int nrorase = 0;

    int nrcadtmp, pozcad;
    struct cadou *sac_temp, cadou_temp;

    sac = malloc(2 * sizeof(struct cadou));

    FILE *fin = fopen("D:/input.txt", "r");
    FILE *fout = fopen("D:/output.txt", "w");
	
    fscanf(fin, "%d", &l);  // numarul de linii
    fscanf(fin, "%d", &c);  // numarul de coloane
    harta = malloc(l * c * sizeof(struct fabrica)); // alocare memorie pentru harta
    fscanf(fin, "%d", &ls); // linia de start
    fscanf(fin, "%d", &cs); // coloana de start
    fscanf(fin, "%d", &p);  // numarul de pasi
	
    for(i = 0; i < l * c; i ++){    // se citeste structura hartii, organizata ca un tablou unidimensional
        fscanf(fin, "%s", harta[i].oras);
        fscanf(fin, "%s", harta[i].tip);
        fscanf(fin, "%d", &harta[i].nr);
        fscanf(fin, " %c", &harta[i].dir);
    }

    for(i = 0; i < p; i++){
		// Colecteaza folosind linia de start, coloana si coloana de start care se actualizeaza la fiecare apel al functiei
        colecteaza(&harta[ls * c + cs], sac, &nrcadou); 
        
		// Daca se ajunge la capacitatea, se realoca memorie dubla pentru sac
		if( nrcadou == capacitate){ 
            sac = realloc(sac, 2 * capacitate * sizeof(struct cadou));
            capacitate *= 2;
        }
		
		// Se schimba pozitiei pe harta
        plimba(harta, &ls, &cs, c);

		// Se verifica daca noua pozitie se incadreaza in harta
        if((cs == c) || (cs < 0) || (ls == l) || (ls < 0)){ 
                fprintf(fout, "TOO MUCH SNOW !\n");
                break;
            }
    }
    fprintf(fout, "%d\n", i);   // numarul de pasi parcursi
    fprintf(fout, "%d\n", nrcadou); // numarul de cadouri adunate

	// Memorie pentru un vector de pointeri
    orase = malloc(nrcadou * sizeof(char*));    
    
	// Memorie pentru o linie pentru 100 de caractere
	for(i=0; i < nrcadou; i++)
        orase[i] = malloc(100 * sizeof(char));
	
    for(i = 0; i < nrcadou; i++) {
        for(j = 0; j < nrorase; j++)    
			// Verifica daca orasul se repeta
            if(strcmp(sac[i].oras, orase[j]) == 0)
                break;
        if(j == nrorase) {
            strcpy(orase[nrorase], sac[i].oras);    // se trec orasele din sac in tabloul orase
            nrorase++;  // se incrementeaza numarul de orase diferite
        }
    }
	
	// Ordoneaza alfabetic orasele
    for(i = 0; i < nrorase - 1; i++)
        for(j = i + 1; j < nrorase; j++){
            if(strcmp(orase[i], orase[j]) > 0) {
                strcpy(oras_tmp, orase[i]);
                strcpy(orase[i], orase[j]);
                strcpy(orase[j], oras_tmp);
            }

        }
	
	// Memorie pentru structura de tip cadou sac_temp
    sac_temp = malloc(nrcadou * sizeof(struct cadou));  
	
	for(i = 0 ; i < nrorase; i++) {     // pentru un anumit oras
        nrcadtmp = 0;
        for(j = 0; j < nrcadou; j++)
            if(strcmp(sac[j].oras, orase[i]) == 0) {    // verifica daca este acelasi oras
                pozcad = cauta_cadou(sac_temp, nrcadtmp, sac[j].tip);
                if(pozcad >= 0)
                    sac_temp[pozcad].nr++;  // se incrementeaza numarul tipului de cadou de pe pozcad
                else
                    sac_temp[nrcadtmp++] = sac[j];  // se trece la alt tip de cadou
            }
        for(j = 0; j < nrcadtmp - 1; j++) // se ordoneaza descrescator cadourile dintr-un oras
            for(k=j+1; k<nrcadtmp; k++)
                if(sac_temp[j].nr < sac_temp[k].nr) {
                    cadou_temp = sac_temp[j];
                    sac_temp[j] = sac_temp[k];
                    sac_temp[k] = cadou_temp;
                }
        sortare_cadouri(sac_temp, nrcadtmp); // se ordoneaza alfabetic cadourile dintr-un oras deja ordonate descrescator
        fprintf(fout, "%s:\n", orase[i]);   // se afiseaza orasul
        for(j=0; j<nrcadtmp; j++)
            fprintf(fout, "  %d %s\n", sac_temp[j].nr, sac_temp[j].tip);    // se afiseaza numarul si tipul de cadou
    }

    fclose(fin);    // se inchide fisierul fin
    fclose(fout);   //se inchide fisierul fout
    free(sac);  //free la toate alocarile de memorie
    free(harta);
    free(sac_temp);
    for(i = 0; i < nrcadou; i++)
        free(orase[i]);
    free(orase);
    return 0;
}
