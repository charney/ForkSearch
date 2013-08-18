/*
 * forksearch.c
 *
 *  	Created on: Apr 6, 2013
 *      Author: Chad Harney
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void findWord(char* word, FILE* in, FILE* out, int buf, int offset);

int main(int argc, char* argv[]){
	if(argc != 5){
		return -1;
	}

	char* word = argv[1];
	FILE* in = fopen(argv[2],"r");
	FILE* out = fopen(argv[3],"w");
	if(in == NULL){
		return -1;
	}
	if(out == NULL){
		return -1;
	}
	int buf = atoi(argv[4]);
	int i;
	fseek(in,0L,SEEK_END);
	int size = ftell(in);
	fseek(in, 0, SEEK_SET);

	int buffer = size/buf;
	if(size%buf != 0){
		buffer++;
	}

	for (i = 0; i < buffer; i++)
	{
		pid_t pID = fork();
		if(pID == 0){
			findWord(word, in, out, buf, i*buf);
			return 0;

		}

	}
	fseek(out, 0, 0);
	fclose(out);

	out = fopen(argv[3],"r");
	int num;
	fscanf(out,"%d",&num);
	fclose(out);
	if(num != 1){
		out = fopen(argv[3],"w");
		fprintf(out,"%d\n",0);
		fclose(out);
	}else if(num == 1){
		out = fopen(argv[3],"w");
		fprintf(out,"%d\n",1);
	}
	return 0;
}

/* findWord function
 * -----------------
 *
 * Runs through the text file and searchs for the given word
 *
 * @param word: pointer to the word being searched for
 * @param in: pointer to the file being accessed
 * @param out: pointer to the file being written
 * @param buf: the buffer sized being used to seperate the file into parts
 * @param offset: how much of the file has already been read (e.g. buf is 256, this is the 3rd time being read so offset is 512)
 */
void findWord(char* word, FILE* in, FILE* out, int buf, int offset){

	char buffer[buf];
	fseek(in, offset, 0);
	int counter = 0;
	fscanf(in,"%s",buffer);
	int length = strlen(buffer);
	counter = length;
	int noMatch = strcmp(buffer,word);
	while(noMatch && counter < buf){
		fscanf(in,"%s",buffer);
		length = strlen(buffer);
		counter += length + 1;
		noMatch = strcmp(buffer,word);
	}

	if(noMatch == 0){
		fprintf(out,"%d\n",1);
	}


}
