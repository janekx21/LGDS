#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HELPMSG "help\n\tget some"
#define null 0
#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

char* commands[] = {
	"Hallo Tom.",
	"Tom sieht $."
};

char* cpyFT(char* text,int f,int t){
	char* tmp = malloc(sizeof(char)*(t-f+1));
	for(int i=0;i<(t-f);i++)tmp[i] = text[i+f];
	tmp[t-f+1] = '\0';
	return tmp;
}

#define MAXLITERAL 32
char** testPattern(char* pattern,char* string){
	int index = 0;
	int sindex = 0;
	char* literals[];
	while(pattern[index]){
		if(string[sindex]=='\0')return 0;
		if(pattern[index] == '$'){
			char target = pattern[index+1];
			printf("$ found target: %c\n",target);
			for(int y=0;y<MAXLITERAL;y++){
				char c = string[sindex+y];
				if(c == target){
					//found literal
					printf("found %d-%d\n",sindex,sindex+y);
					char* literal = cpyFT(string,sindex,sindex+y);
					printf("found literal:%s\n",literal);
					sindex += y;
					index++;
					break;
				}
			}
		}
		if(pattern[index] != string[sindex])
			return 0;

		sindex++;
		index++;
	}
	return 1;
}

void patternMatch(char* line,char* com[]){
	printf(">>>%s\n",line);
	for(int i = 0;com[i];i++){
		printf("try pattern:%s\n",com[i]);
		printf("result:%d\n",testPattern(com[i],line));
	}
}

void compile(char* code){
	int index = 0;
	while(code[index]){
		int lineindex = 0;
		while(code[lineindex+index]!='\n')lineindex++;
		char* line = malloc(sizeof(char)*lineindex);
		line[lineindex]='\0';//replacing \n with \0
		for(int i=0;i<lineindex;i++){
			char c = code[i+index];
			//replacing possible here
			line[i] = c;
		}
		//pattern matching here
		patternMatch(line,commands);
		index+=lineindex;
		index++;
	}
}

int main(int argc, char *argv[]){
	if(argc > 1){
		FILE* f = fopen(argv[1],"r");	
		if(!f){
			printf("file %s not found",argv[1]);
			return 1;
		}
		fseek(f,null,SEEK_END);
		long size = ftell(f);
		rewind(f);
		char* code = (char*)malloc(sizeof(char)*size);
		fread(code,size,sizeof(char),f);
		printf("code-länge %ld-\n%s\n----------\n",size,code);
		compile(code);
		fclose(f);
	}else{
		printf(HELPMSG);
	}
}
