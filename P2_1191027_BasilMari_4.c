#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

struct Node{
	char name[64];
	int credits;
	char code[16];
	char department[64];
	char topics[256];
	struct Node* right;
	struct Node* left;
	int Height;
};

struct Node* MakeEmpty(struct Node* T){
	if(T != NULL){
		MakeEmpty(T->left);
		MakeEmpty(T->right);
		free(T);
	}
	return NULL;
}

int getHeight(struct Node* T) {
	if(T == NULL)
		return 0;
	return T->Height;
}

int getMax(int a, int b) {
	return (a > b)? a : b;
}

struct Node* getMin(struct Node* T)  {
	if(T == NULL)
		return NULL;
	else if(T->left == NULL)
		return T;
	else
		return getMin(T->left);
}

int CompareName(char key[64], char key2[64]){
	return strcmp(key2,key);
}

struct Node* Find(char name[64],struct Node* T){
	if(T == NULL)
		return NULL;
	else if(CompareName(name,T->name)==-1)
		return Find(name,T->left);
	else if(CompareName(name,T->name)==1)
		return Find(name,T->right);
	else
		return T;
};

int IsRepeated(struct Node* T,char name[64]){
	struct Node* N = Find(name,T);
	if(N != NULL && strcasecmp(N->name,name)==0)
		return 1;
	else
		return 0;
}

unsigned long convert(char str[20]){
	char result[100] = "";
	char* token;
	token = strtok(str,",");
	if(token == NULL)
		return 0;
	while(token != NULL) {
		strcat(result, token);
		token = strtok(NULL,",");
	}
	unsigned long n = atoi(result);

	return n;
}

void Reconvert(char* Subs){
	char *ptr, *optr;
	int i, length, commas;
	for ( ptr = Subs; *ptr; ptr++ );
	length = ptr - Subs;
	commas = ( length - 1 ) / 3;
	optr = Subs + length + commas;

	*optr-- = *ptr--;
	for(i=1;ptr>=Subs;i++){
		*optr-- = *ptr--;
		if((i%3)==0)
			*optr-- = ',';
	}
}

struct Node* Update(struct Node* T, char name[64], int credits, char code[16], char department[64], char topics[256]){
	strncpy(T->name, name, 64);
	T->credits = credits;
	strncpy(T->code, code, 16);
	strncpy(T->department, department, 64);
	strncpy(T->topics, topics, 256);
	return T;
}

struct Node* NewNode(char name[64], int credits, char code[16], char department[64], char topics[256]){
	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
	strncpy(node->name,name, 64);
	node->credits = credits;
	strncpy(node->code, code, 16);
	strncpy(node->department, department, 64);
	strncpy(node->topics, topics, 256);
	node->right = NULL;
	node->left = NULL;
	node->Height = 1;
	return node;
}

struct Node* Rotateright(struct Node* T){
	struct Node* l = T->left;
	struct Node* r = l->right;

	l->right = T;
	T->left = r;

	T->Height = getMax(getHeight(T->left),getHeight(T->right))+1;
	l->Height = getMax(getHeight(l->left),getHeight(l->right))+1;
	return l;
}

struct Node* Rotateleft(struct Node* T){
	struct Node* r = T->right;
	struct Node* l = r->left;

	r->left = T;
	T->right = l;

	T->Height = getMax(getHeight(T->left),getHeight(T->right))+1;
	r->Height = getMax(getHeight(r->left),getHeight(r->right))+1;
	return r;
}

int getBalance(struct Node* N){
	if (N == NULL)
		return 0;
	return getHeight(N->left) - getHeight(N->right);
}

struct Node* Insert(struct Node* T,char name[64], int credits, char code[16], char department[64], char topics[256]){
	if(T == NULL)
		return NewNode(name, credits, code, department, topics);

	else if(IsRepeated(T,name)){
		printf("Channel Already Exists!Data Updated!\n");
		struct Node* R = Find(name,T);
		R = Update(R, name, credits, code, department, topics);
		return R;
	}

	if(CompareName(name,T->name) == -1)
		T->left = Insert(T->left, name, credits, code, department, topics);
	else if(CompareName(name,T->name) == 1)
		T->right = Insert(T->left, name, credits, code, department, topics);
	else
		return T;

	T->Height = getMax(getHeight(T->left),getHeight(T->right))+1;
	int Balance = getBalance(T);
	//LL
	if(Balance > 1 && CompareName(name,T->left->name)==-1)
		return Rotateright(T);
	//RR
	if(Balance < -1 && CompareName(name,T->right->name))
		return Rotateleft(T);
	//LR
	if(Balance > 1 && CompareName(name,T->left->name)==1){
		T->left =  Rotateleft(T->left);
		return Rotateright(T);
	}
	//RL
	if(Balance < -1 && CompareName(name,T->right->name)==1){
		T->right = Rotateright(T->right);
		return Rotateleft(T);
	}
	return T;
}

struct Node* DeleteNode(struct Node* T,char name[64]){
	if(T == NULL)
		return T;
	if(CompareName(name,T->name)==-1)
		T->left = DeleteNode(T->left,name);
	else if(CompareName(name,T->name)==1)
		T->right = DeleteNode(T->right,name);
	else{
		if(T->left == NULL || T->right == NULL){
			struct Node* temp = T->left ? T->left : T->right;

			if(temp == NULL){
				temp = T;
				T = NULL;
			}
			else
				*T = *temp;
			free(temp);
		}
		else{
			struct Node* temp = getMin(T->right);
			strcpy(T->name,temp->name);
			T->credits = temp ->credits;
			strcpy(T->code,temp->code);
			strcpy(T->department,temp->department);
			strcpy(T->topics,temp->topics);
			T->Height = temp->Height;

			T->right = DeleteNode(T->right,temp->name);
		}
	}
	if(T == NULL)
		return T;

	T->Height = getMax(getHeight(T->left),getHeight(T->right))+1;

	int Balance = getBalance(T);

	//LL
	if (Balance > 1 && getBalance(T->left) >= 0)
		return Rotateright(T);

	//LR
	if (Balance > 1 && getBalance(T->left) < 0){
		T->left = Rotateleft(T->left);
		return Rotateright(T);
	}

	//RR
	if (Balance < -1 && getBalance(T->right) <= 0)
		return Rotateleft(T);

	//RL
	if (Balance < -1 && getBalance(T->right) > 0){
		T->right = Rotateright(T->right);
		return Rotateleft(T);
	}
	return T;
}

void PrintData(struct Node* node){
	if(node != NULL)
		printf("%d  %s  %d  %s  %s  %s\n", node->Height, node->name, node->credits, node->code, node->department, node->topics);
	else
		printf("Not Found!\n");
}

void PreOrder(struct Node* T){
	if(T != NULL){
		PreOrder(T->left);
		printf("%d  %s  %d  %s  %s  %s\n", T->Height, T->name, T->credits, T->code, T->department, T->topics);
		PreOrder(T->right);
	}
}

struct Node* LoadData(struct Node* T){
	struct Node* temp = T;
	FILE *in;
	char name[64];
	int credits;
	char code[16];
	char department[64];
	char topics[256];
	char line[1000];
	in = fopen("courses.txt","r");
	while(fgets(line,1000,in) != 0){
		if(feof(in))
			break;
		else {
			strncpy(name, strtok(line,":"), 64);
			credits = atoi(strtok(NULL,"#"));
			strcpy(code, strtok(NULL,"#"));
			strcpy(department, strtok(NULL,"/"));
			strcpy(topics, strtok(NULL,"\n"));
			temp = Insert(temp, name, credits, code, department, topics);
		}
	}
	fclose(in);
	return T;
}

void SaveData(FILE *out,struct Node* T){
	if(T != NULL){
		fprintf(out,"%s	%d	%s", T->name, T->credits,T->topics);
		SaveData(out,T->left);
		SaveData(out,T->right);
	}
}

int main() {
	struct Node* T = NULL;
	T = LoadData(T);
	PreOrder(T);
	MakeEmpty(T);
	return 0;
}
