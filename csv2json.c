#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int ColumnSize(FILE *pFile);


int main(int argc, char *argv[]){
	
	int i;
	char charRead;
	int columnNumber = 1;
	int numberOfNewLine = 1;		
	int lineOffset = 1;
	int columnIndex = 0;
	bool isNewLine = false;
	int numberOfColumns;
	
	int columnCharCounter = 0;	
	char comma = ',';
	char colon = ':';
	char quotationMark = '"';
	char curlyOpening = '{';
	char curlyClosed = '}';
	char bracketOpening = '[';
	char bracketClosed = ']';
	char newLine = '\n';
	
	FILE *pCSV;
	
	if((pCSV = fopen(argv[1], "r")) == NULL){
		
		printf("CSV file cannot be read...");
		exit(1);
	}
		
	while((charRead = getc(pCSV)) != newLine) {
		
		columnCharCounter++;		
	}
	
	char columnNames[columnCharCounter];
	
	rewind(pCSV);
	
	fread(columnNames, columnCharCounter, 1, pCSV);
	
	FILE *pJSON;
	
	if((pJSON = fopen(argv[2], "w")) == NULL){
		
		printf("JSON file cannot be read...");
		exit(1);
	}
	
	numberOfColumns = ColumnSize(pCSV);
			
	fprintf(pJSON, "%c", bracketOpening);
	fprintf(pJSON, "\n  %c", curlyOpening);	
	
	while(1) {
				
		if(!strcmp(argv[3], "header=ON")){
		
			fprintf(pJSON, "\n    %c", quotationMark);
			
			for(i = columnIndex; i < sizeof(columnNames); i++) {
				
				if(columnNames[i] != comma) {
					
					fprintf(pJSON, "%c", columnNames[i]);
							
				} else {
					
					fprintf(pJSON, "%c%c %c", quotationMark, colon, quotationMark);
					columnIndex = i + 1;
					break;
				}	
					
				columnIndex = i;	
				if((columnIndex + 1) == sizeof(columnNames)) {
					
					columnIndex = 0;		
					fprintf(pJSON, "%c%c %c", quotationMark, colon, quotationMark);
					break;
				}	
			}
			
			fseek(pCSV, columnCharCounter + numberOfNewLine + lineOffset, SEEK_SET);
		
		} else if(!strcmp(argv[3], "header=OFF")){
			
			columnCharCounter = 0;
			
			fprintf(pJSON, "\n    %c%s%d%c%c %c", quotationMark, "Column", columnNumber, quotationMark, colon, quotationMark);
			
			columnNumber++;
			
			if(columnNumber == (numberOfColumns)) {
				
				columnNumber = 1;
			}
			
			fseek(pCSV, columnCharCounter + numberOfNewLine + lineOffset - 2, SEEK_SET);
	
		} else {
			
			printf("Error: Wrong syntax...\n");	
			exit(1);	
		}
						
		while((charRead = getc(pCSV)) != comma) {
			
			if((charRead == EOF)){
				
				fprintf(pJSON, "%c\n  %c\n%c", quotationMark, curlyClosed, bracketClosed);
				
				return 0;
			}
					
			if(charRead == newLine) {
							
				numberOfNewLine++;
				isNewLine = true;
				break;
			}
						
			fprintf(pJSON, "%c", charRead);
			lineOffset++;		
		}
		
		if(!isNewLine) {
			
			fprintf(pJSON, "%c%c", quotationMark, comma);
		}
		
		if(isNewLine) {
			
			fprintf(pJSON, "%c\n  %c%c\n\n  %c", quotationMark, curlyClosed, comma, curlyOpening);
			isNewLine = false;
		}
		
		lineOffset++;	
	}						
}


int ColumnSize(FILE *pFile) {
	
	rewind(pFile);
	
	char ch;
	int commaCounter = 1;
		
	while((ch = getc(pFile)) != EOF){
		
		if(ch == ',') {
			
			commaCounter++;
		
		} else if(ch == '\n') {
			
			break;
		}
	}
	
	return commaCounter + 1;
}
