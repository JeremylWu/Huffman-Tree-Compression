/*Huffman Tree Compression*/
/*Author : Jeremy Wu*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <limits.h>
/*Struct of the program*/
struct node{
	int count;
	unsigned char ch;
	int parent,lchild,rchild;
	char bits[256];
}header[512],temp;

void print_command();
void init();
long huffmancoding(char inputfile[]);
void transfer_to_file(long sum,char inputfile[],char sto[]);
void uncompress(char sto[],char filename[]);
void print();
void print_history();
int print_info();

long n;
/*Quick sort*/
int cmp(const void *a,const void *b){
	long *pa = (long*)a,*pb = (long*)b;
	return (int)((*pb)-(*pa));
}

int main(){
	int i;
	char input_file[128],info[128],result_file[128];
	long length;
	char ch;
	int flag_file = 0;
	FILE *coding_history;
	char coding_[128];
	init();
	while(1){
		scanf("%c*%c",&ch);
		if(ch == '0'){
			printf("See you next time.\n");
			exit(1);
		}
		else if(ch == '1'){
			flag_file = 1;
			print();
			strcpy(input_file, "in");
			//printf("Please scan the file name to be compressed.\n");
			//scanf("%s",input_file);
			printf("Please scan the file name after compressed.\n");
			scanf("%s*%c",info);
			printf("\n");
			/*Huffman Encoding*/
			length = huffmancoding(input_file);
			/*File compression*/
			transfer_to_file(length,input_file,info);
            printf("****************************************\n");
			scanf("%c*%c",&ch);
		}
		else if(ch =='2'){
			if(!print_info())	scanf("%c*%c",&ch);
			else{
				printf("Please scan the file name wait to be uncompressed.\n");
				scanf("%s",info);
				printf("Please scan the file name after uncompressed.\n");
				scanf("%s",result_file);
				/*File Uncompression*/
				uncompress(info,result_file);
				scanf("%c*%c",&ch);
			}
		}
		else if(ch == '3'){
			print_history();
			scanf("%c*%c",&ch);
		}
		else if(ch == '4'){
			if(flag_file==1){
				coding_history = fopen("coding.txt","r");
				if(coding_history == NULL){
					printf("File open failed.\n");
					exit(1);
				}
				while(!feof(coding_history)){
					fscanf(coding_history,"%s",coding_);
					printf("%s\n",coding_);
				}
				fclose(coding_history);
				scanf("%c*%c",&ch);
				flag_file = 0;
			}
			else if(flag_file == 0){
				printf("Yet to be encoded, please choose this function after encoded.\n");
				scanf("%c*%c",&ch);
			}

		}
		else if(ch == '5'){
			print_command();
			scanf("%c*%c",&ch);
		}
		else{
			printf("Command not found, please scan again.\n");
			scanf("%c*%c",&ch);
		}
		system("cls");
	}
	return 0;
}

/*Show Command function*/
void print_command(){
	printf(" 1 : Compressed text file.\n");
	printf(" 2 : Uncompress text file.\n");
	printf(" 3 : View history.\n");
	printf(" 4 : Show the result of Hullman Tree encoding.\n");
	printf(" 5 : Show all the commands\n");
	printf(" 0 : EXIT the program.\n");
}

/*Init function*/
void init(){
    printf("/");
    for(int i = 0; i < 55; ++i) printf("*");
    printf("\\\n");
    printf("|  Welcome to file Compressor powered by Huffman Tree:");
    printf("  |\n");
	printf("|  Version : V1.0 Author : Jeremy Wu                    |\n|  Class : CS1303 School : ZJSU\t\t\t\t|\n|\t\t\t\t\t\t\t|\n");
	printf("|  Warning : the input file must be named as in|\n");
    printf("|  Please choose functions as follow:\t\t\t|\n");
    printf("\\");
    for(int i = 0; i < 55; ++i) printf("*");
    printf("/\n");
	print_command();
}

/*Compresiong program*/
long huffmancoding(char inputfile[]){
	FILE *coding_result;
	unsigned char ch;
	int i,j;
	long m,pt,prelength,ioflength,tmp;
	long min;
	FILE *input_file;

	input_file = fopen(inputfile,"rb");
	if(input_file==NULL){
		printf("File open failed.\n");
		exit(1);
	}
	prelength = 0;

	while(!feof(input_file)){
		fread(&ch,1,1,input_file);
		header[ch].count++;
		prelength++;
	}

	prelength--;
	ioflength = prelength;
	header[ch].count--;

	for(i=0;i<512;i++){
		if(header[i].count!=0) header[i].ch=(unsigned char)i;
		else header[i].ch=0;
		header[i].parent=-1;header[i].lchild=header[i].rchild=-1;
	}
	qsort(header,512,sizeof(struct node),cmp);

	for(i = 0; i < 256; i++) if(header[i].count==0) break;
	n = i;
	m = 2 * n - 1;
	for(i = n; i < m; i++){
		min=32767;
		for(j=0;j<i;j++){
			if(header[j].parent!=-1) continue;
			if(min>header[j].count){
				pt=j;
				min=header[j].count;
				continue;
			}
		}
		header[i].count=header[pt].count;
		header[pt].parent=i;
		header[i].lchild=pt;
		min=32767;
		for(j=0;j<i;j++){
			if(header[j].parent!=-1) continue;
			if(min>header[j].count){
				pt=j;
				min=header[j].count;
				continue;
			}
		}
		header[i].count+=header[pt].count;
		header[i].rchild=pt;
		header[pt].parent=i;
	}
	for(i=0;i<n;i++){
		tmp=i;
		header[i].bits[0]=0;
		while(header[tmp].parent!=-1){
			j=tmp;
			tmp=header[tmp].parent;
			if(header[tmp].lchild==j){
				j=strlen(header[i].bits);
				memmove(header[i].bits+1,header[i].bits,j+1);
				header[i].bits[0]='0';
			}
			else{
				j=strlen(header[i].bits);
				memmove(header[i].bits+1,header[i].bits,j+1);
				header[i].bits[0]='1';
			}
		}
	}

	coding_result = fopen("coding.txt","w");
	if(coding_result==NULL){
		printf("  File created failed.\n");
		exit(1);
	}
	for(i=0;i<n;i++){
		fprintf(coding_result,"%s",header[i].bits);
		fprintf(coding_result,"\n");
	}
	printf("File encoded succeed, result is saved in the file coding.txt.\n");
	fclose(coding_result);
	fclose(input_file);
	return prelength;
}
/*Transfer Program*/
void transfer_to_file(long sum,char inputfile[],char sto[]){
	double div;
	long prelength=sum,after_length,f,pt;
	FILE *in_file,*out_file;
	int i,k;
	char buf[512];
	unsigned char ch;
	FILE *history,*info;
	time_t now;
	struct tm  *timenow;

	in_file = fopen(inputfile,"rb");
	if(in_file == NULL){
		printf("  File open failed..\n");
		exit(1);
	}

	out_file = fopen(sto,"wb");
	if(out_file == NULL){
		printf("  File created failed.\n");
		exit(1);
	}

	fwrite(&prelength,sizeof(int),1,out_file);
	fseek(out_file,8,SEEK_SET);
	buf[0]=0;f=0;pt=8;
	while(!feof(in_file)){
		ch = fgetc(in_file);
		f++;
		for(i=0;i<n;i++){
			if(ch==header[i].ch)break;
		}
		strcat(buf,header[i].bits);
		k = strlen(buf);
		ch = 0;
		while(k>=8){
			for(i=0;i<8;i++){
				if(buf[i]=='1') ch=(ch<<1)|1;
				else ch=ch<<1;
			}
			fwrite(&ch,1,1,out_file);
			pt++;
			strcpy(buf,buf+8);
			k=strlen(buf);
		}
		if(f==prelength)break;
	}
	if(k>0){
		strcat(buf,"00000000");
		for(i=0;i<8;i++){
			if(buf[i]=='1') ch=(ch<<1)|1;
			else ch=ch<<1;
		}
		fwrite(&ch,1,1,out_file);
		pt++;
	}

	fseek(out_file,4,SEEK_SET);
	fwrite(&pt,sizeof(long),1,out_file);
	fseek(out_file,pt,SEEK_SET);
	fwrite(&n,sizeof(long),1,out_file);
	for(i=0;i<n;i++){
		fwrite(&(header[i].ch),1,1,out_file);
		ch=strlen(header[i].bits);
		fwrite(&ch,1,1,out_file);
		k=strlen(header[i].bits);
		if(k % 8 != 0){
			for(f=k%8;f<8;f++)
				strcat(header[i].bits,"0");
		}
		while(header[i].bits[0]!=0){
			ch=0;
			for(k=0;k<8;k++){
				if(header[i].bits[k]=='1') ch=(ch<<1)|1;
				else ch=ch<<1;
			}
			strcpy(header[i].bits,header[i].bits+8);
			fwrite(&ch,1,1,out_file);
		}
	}
	after_length=pt--;
	div = (double)1.0*after_length/sum;
	printf("The length of file the former:\t%ld\n",sum);
	printf("The length of file the later:\t%ld\n",after_length);
	printf("Conversion rate:\t%lf%%\n",div * 100 );
	time(&now);
	timenow = localtime(&now);


	info = fopen("Uncompressed_available.txt","a");
	if(info == NULL){
		printf("  File open failed.\n");
		exit(1);
	}
	fseek(info,0L,SEEK_END);
	fprintf(info,"%s",sto);
	
	//fprintf(info,"\n");
	fclose(info);

	history = fopen("history.dat","a");
	if(history == NULL){
		printf("history file can not loaded.\n");
		exit(1);
	}

	fseek(history,0L,SEEK_END);
	//	printf("%ld\n",ftell(history));
	fprintf(history,"%s","****************************************\n");
	fprintf(history,"%s\t","File compression time:");
	fprintf(history,"%s",asctime(timenow));
	fprintf(history,"\n");
	fprintf(history,"%s\t%s","The source file name of Zip file:",inputfile);
	fprintf(history,"\n");
	fprintf(history,"%s\t%s","The target file name of Zip file:",sto);
	fprintf(history,"\n");
	fprintf(history,"The length of file the former:");
	fprintf(history,"\t%ld",sum);
	fprintf(history,"\n");
	fprintf(history,"%s\t%ld","The length of file the later:",after_length);
	fprintf(history,"\n");
	fprintf(history,"%s\t%lf%%","Conversion rate:",div * 100 );
	fprintf(history,"\n");
	fprintf(history,"%s","****************************************\n");

	fclose(history);
	fclose(in_file);
	fclose(out_file);

}

/*Uncomress Program*/
void uncompress(char sto[],char filename[]){
	char buf[255],bx[255];
	unsigned char ch;
	long i,j,m,n,f,p,l;
	long flength;
	FILE *ifp,*ofp;
	FILE *history,*info;
	time_t now;
	struct tm  *timenow;
	ifp=fopen(sto,"rb");
	if(ifp==NULL){
		printf("\nFile open failed!\n");
		return;
	}

	ofp = fopen(filename,"wb");
	if(ofp==NULL){
		printf("\nUncompress failed!\n");
		return;
	}
	fread(&flength,sizeof(long),1,ifp);
	fread(&f,sizeof(long),1,ifp);
	fseek(ifp,f,SEEK_SET);
	fread(&n,sizeof(long),1,ifp);
	for(i=0;i<n;i++){
		fread(&header[i].ch,1,1,ifp);
		fread(&ch,1,1,ifp);
		p=(long)ch;
		header[i].count=p;
		header[i].bits[0]=0;
		if(p%8>0) m=p/8+1;
		else m=p/8;
		for(j=0;j<m;j++){
			fread(&ch,1,1,ifp);
			f=ch;
			itoa(f,buf,2);
			f=strlen(buf);
			for(l=8;l>f;l--)	strcat(header[i].bits,"0");
			strcat(header[i].bits,buf);
		}
		header[i].bits[p]=0;
	}

	for(i=0;i<n;i++){
		for(j=i+1;j<n;j++){
			if(strlen(header[i].bits)>strlen(header[j].bits)){
				temp=header[i];
				header[i]=header[j];
				header[j]=temp;
			}
		}
	}

	p=strlen(header[n-1].bits);
	fseek(ifp,8,SEEK_SET);
	m=0;
	bx[0]=0;
	while(1){
		while(strlen(bx)<(unsigned int)p){
			fread(&ch,1,1,ifp);
			f=ch;
			itoa(f,buf,2);
			f=strlen(buf);
			for(l=8;l>f;l--)	strcat(bx,"0");
			strcat(bx,buf);
		}
		for(i=0;i<n;i++)
			if(memcmp(header[i].bits,bx,header[i].count)==0) break;
		strcpy(bx,bx+header[i].count);
		ch=header[i].ch;
		fwrite(&ch,1,1,ofp);
		m++;
		if(m==flength) break;
	}

	time(&now);
	timenow = localtime(&now);
	history = fopen("history.dat","a");
	if(history == NULL){
		printf("File open failed.\n");
		exit(1);
	}

	info = fopen("After_uncompressed_file.txt","a");
	if(info == NULL){
		printf("File open failed.\n");
		exit(1);
	}
	fseek(info,0L,SEEK_END);
	fprintf(info,"%s",filename);
	fprintf(info,"\n");
	fclose(info);

	//fprintf(history,"%s","****************************************\n");
	fprintf(history,"%s\t","File compression time:");
	fprintf(history,"%s",asctime(timenow));
	fprintf(history,"\n");
	fprintf(history,"Source file:");
	fprintf(history,"\t%s",sto);
	fprintf(history,"\n");
	fprintf(history,"%s","Target file:");
	fprintf(history,"\t%s",filename);
	fprintf(history,"\n");

	fprintf(history,"%s","****************************************\n");

	fclose(history);
	fclose(ifp);
	fclose(ofp);
	printf("\ncompression succeed!\n");

	for(i=0;i<512;i++){
		header[i].lchild = header[i].lchild = header[i].parent=-1;
		header[i].count=0;
		header[i].ch = 0;
		strcpy(header[i].bits,"\0");
	}
	return;
}

/*Print Function*/
void print(){
	FILE *info;
	char str[128];
	info = fopen("in","r");
	if(info == NULL){
		printf("Input file open failed, you may not rename the input file as in .\n");
		exit(1);
	}
	printf("\n\nThe file can be compressed now.\n");
	while(!feof(info)){
		fscanf(info,"%s",str);
		printf("%s\n",str);
	}
	printf("\n\n");
	fclose(info);
}

int print_info(){
	FILE *fp;
	char str[128];
	int number;
	fp = fopen("Uncompressed_available.txt","r");
	if(fp == NULL){
		printf("File open failed.\n");
		exit(1);
	}
	fscanf(fp,"%d",&number);
	if(number == 0){
		printf("Compressed file not found.\n");
		printf("Please choose other functions.\n");
		return 0;
	}
	else{
		printf("\n\nThe file can be pressed now:\n");
		while(!feof(fp)){
			fscanf(fp,"%s",str);
			printf("%s\n",str);
		}
	}
	printf("\n\n");
	fclose(fp);
	return 1;
}

void print_history(){
	char ch;
	FILE *fp;
	fp = fopen("history.dat","r");
	if(fp==NULL)	exit(1);
	while(!feof(fp)){
		ch = fgetc(fp);
		printf("%c",ch);
	}
	printf("\n");
	fclose(fp);
}
