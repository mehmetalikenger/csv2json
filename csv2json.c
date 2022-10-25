#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[]){
	
	FILE *pOkunan;
	
	if((pOkunan = fopen(argv[1], "r")) == NULL){
		
		printf("CSV dosyasi acilamadi...");
		exit(1);
	}
		
	char okunanKarakter;
	int sutunKarakterSayac = 0;
	
	while((okunanKarakter = getc(pOkunan)) != '\n') {
		
		sutunKarakterSayac++;		
	}
	
	char sutunIsimleri[sutunKarakterSayac];
	
	sutunKarakterSayac = sizeof(sutunIsimleri);
	
	rewind(pOkunan);
	
	fread(sutunIsimleri, sutunKarakterSayac, 1, pOkunan);
	
	FILE *pYazilan;
	
	if((pYazilan = fopen(argv[2], "w")) == NULL){
		
		printf("JSON dosyasi acilamadi...");
		exit(1);
	}
		
	int sutunIndeks = 0;
	int satirOffset = 1;
	int i;
	int k = 1;
	char virgul = ',';
	int satirN = 1;
	bool newLine = false;	
	char ch;
	int comma = 1;
	
	rewind(pOkunan);
	while((ch = getc(pOkunan)) != EOF){
		
		if(ch == ',') {
			
			comma++;
		
		} else if(ch == '\n') {
			
			break;
		}
	}
			
	fprintf(pYazilan, "%c", '[');
	fprintf(pYazilan, "\n  %c", '{');	
	
	while(true) {
				
		if(!strcmp(argv[3], "header=ON")){
		
			fprintf(pYazilan, "\n    %c", '"');
			
			for(i = sutunIndeks; i < sizeof(sutunIsimleri); i++) {
				
				if(sutunIsimleri[i] != virgul) {
					
					fprintf(pYazilan, "%c", sutunIsimleri[i]);
							
				} else {
					
					fprintf(pYazilan, "%c%c %c", '"',':','"');
					sutunIndeks = i + 1;
					break;
				}	
					
				sutunIndeks = i;	
				if((sutunIndeks + 1) == sizeof(sutunIsimleri)) {
					
					sutunIndeks = 0;		
					fprintf(pYazilan, "%c%c %c", '"',':','"');
					break;
				}	
			}
			
			fseek(pOkunan, sutunKarakterSayac + satirOffset + satirN, SEEK_SET);
		
		} else if(!strcmp(argv[3], "header=OFF")){
			
			sutunKarakterSayac = 0;
			
			fprintf(pYazilan, "\n    %c%s%d%c%c %c", '"',"Column",k,'"',':','"');
			
			k++;
			
			if(k == (comma+1)) {
				
				k = 1;
			}
			
			fseek(pOkunan, sutunKarakterSayac + satirOffset + satirN - 2, SEEK_SET);
	
		} else {
			
			printf("Hatali bilgi girisi...\n");	
			exit(1);	
		}
						
		while((okunanKarakter = getc(pOkunan)) != virgul) {
			
			if((okunanKarakter == EOF)){
				
				fprintf(pYazilan, "%c\n  %c\n%c", '"','}',']');
				
				return 0;
			}
					
			if(okunanKarakter == '\n') {
							
				satirN++;
				newLine = true;
				break;
			}
						
			fprintf(pYazilan, "%c", okunanKarakter);
			satirOffset++;		
		}
		
		if(!newLine) {
			
			fprintf(pYazilan, "%c%c", '"',',');
		}
		
		if(newLine) {
			
			fprintf(pYazilan, "%c\n  %c%c\n\n  %c", '"', '}',',','{');
			newLine = false;
		}
		
		satirOffset++;	
	}						
}
