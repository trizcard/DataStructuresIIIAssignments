#include "funcGiven.h"

void binScreen(char *nameBinFile){ 

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nameBinFile == NULL || !(fs = fopen(nameBinFile, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binScreen): não foi possível abrir o file que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scanQuoteString(char *str) {
	char R;

	while((R = getchar()) != EOF && isspace(R)); 

	if(R == 'N' || R == 'n') { 
		getchar(); getchar(); getchar(); 
		strcpy(str, ""); 
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { 
			strcpy(str, "");
		}
		getchar(); 
	} else if(R != EOF){ 
		str[0] = R;
		R = getchar();
		int i = 1;
		while(!isspace(R) && R != EOF){
			str[i] = R;
			R = getchar();
			i++;
		}
		str[i] = '\0';
	} else { // EOF
		strcpy(str, "");
	}
}