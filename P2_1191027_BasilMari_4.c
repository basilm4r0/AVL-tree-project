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
	return strcasecmp(key, key2);
}

struct Node* Find(char name[64],struct Node* T){
	if(T == NULL)
		return NULL;
	else if(CompareName(name, T->name) < 0)
		return Find(name,T->left);
	else if(CompareName(name, T->name) > 0)
		return Find(name,T->right);
	else
		return T;
};

int IsRepeated(struct Node* T,char name[64]){
	struct Node* N = Find(name,T);
	if(N != NULL && strcasecmp(name,N->name)==0)
		return 1;
	else
		return 0;
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
		printf("Course Already Exists. Data Updated.\n");
		struct Node* R = Find(name,T);
		R = Update(R, name, credits, code, department, topics);
		return R;
	}

	if(CompareName(name, T->name) < 0)
		T->left = Insert(T->left, name, credits, code, department, topics);
	else if(CompareName(name, T->name) > 0)
		T->right = Insert(T->right, name, credits, code, department, topics);
	else
		return T;

	T->Height = getMax(getHeight(T->left),getHeight(T->right))+1;
	int Balance = getBalance(T);
	//LL
	if(Balance > 1 && CompareName(name, T->left->name) < 0)
		return Rotateright(T);
	//RR
	if(Balance < -1 && CompareName(name, T->right->name) > 0)
		return Rotateleft(T);
	//LR
	if(Balance > 1 && CompareName(name, T->left->name) > 0) {
		T->left =  Rotateleft(T->left);
		return Rotateright(T);
	}
	//RL
	if(Balance < -1 && CompareName(name, T->right->name) > 0) {
		T->right = Rotateright(T->right);
		return Rotateleft(T);
	}
	return T;
}

struct Node* DeleteNode(struct Node* T,char name[64]){
	if(T == NULL)
		return T;
	if(CompareName(name, T->name) < 0)
		T->left = DeleteNode(T->left,name);
	else if(CompareName(name, T->name) > 0)
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

void InOrder(struct Node* T){
	if(T != NULL){
		InOrder(T->left);
		printf("%d  %s  %d  %s  %s  %s\n", T->Height, T->name, T->credits, T->code, T->department, T->topics);
		InOrder(T->right);
	}
}

struct Node* LoadData(struct Node* T){
	FILE *in;
	char name[64];
	int credits;
	char code[16];
	char department[64];
	char topics[256];
	char line[500];
	in = fopen("courses.txt","r");
	while(fgets(line,499,in) != 0){
		if(feof(in))
			break;
		else {
			strncpy(name, strtok(line,":"), 64);
			credits = atoi(strtok(NULL,"#"));
			strncpy(code, strtok(NULL,"#"), 16);
			strncpy(department, strtok(NULL,"/"), 64);
			strncpy(topics, strtok(NULL,"\n"), 256);
			T = Insert(T, name, credits, code, department, topics);
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

int TrimNewline(char name[]) {
	if ((strlen(name) > 0) && (name[strlen (name) - 1] == '\n'))
		name[strlen (name) - 1] = '\0';
	return 1;
}

int main() {
	int option;
	char name[64];
	int credits;
	char code[16];
	char department[64];
	char topics[256];
	char input[256];
	struct Node* T = NULL;
	struct Node* temp = NULL;

	printf("\nEnter a number to perform one of the following operations:\n1. Load course list from busses.txt into a tree.\n2. Add new course.\n3. Update the information of a course.\n4. Print courses and their information in lexicographic order.\n5. Print topics associated with a course.\n6. List all courses in a department in lexicographic order.\n7. Delete a course.\n8. Delete all courses that start with a specific letter.\n9. Delete all courses that belong to a department.\n10. Save all information in a file named offered_courses.txt.\n");
	do {
		printf("Option: ");		//Printing prompt and reading user input
		fgets(input, 63, stdin);
		sscanf(input, "%d", &option);
		switch (option) {

			case (1):			//Option 1: Load bus information from busses.txt
				T = LoadData(T);
				break;

			case (2):			//Option 2: Load passenger information from passengers.txt
				printf("Enter the information of the course to be added.\nCourse name: ");
				fgets(input, 63, stdin);
				TrimNewline(input);
				strcpy(name, input);
				printf("Credit hours: ");
				fgets(input, 63, stdin);
				sscanf(input, "%d", &credits);
				printf("Couse code: ");
				fgets(input, 15, stdin);
				sscanf(input, "%15s", code);
				printf("Department: ");
				fgets(input, 63, stdin);
				sscanf(input, "%63s", department);
				printf("Course topics: ");
				fgets(input, 255, stdin);
				TrimNewline(input);
				strcpy(topics, input);
				Insert(T, name, credits, code, department, topics);
				break;

			case (3):			//Option 3: Assign loaded passengers to loaded buses and print assignment information
				printf("Enter the information of the course to be updated. Course name: ");
				fgets(input, 63, stdin);
				TrimNewline(input);
				strcpy(name, input);
				printf("Credit hours: ");
				fgets(input, 63, stdin);
				sscanf(input, "%d", &credits);
				printf("Couse code: ");
				fgets(input, 15, stdin);
				sscanf(input, "%15s", code);
				printf("Department: ");
				fgets(input, 63, stdin);
				sscanf(input, "%63s", department);
				printf("Course topics: ");
				fgets(input, 255, stdin);
				TrimNewline(input);
				strcpy(topics, input);
				if ((temp = Find(name, T)) == NULL)
					printf("Course does not exist!\n");
				else {
					Update(temp, name, credits, code, department, topics);
				}
				break;

			case (4):			//Option 4: Print information of specified bus along with its passengers
				if (T != NULL) {
					printf("\nCourses in tree:\n");
					InOrder(T);
				}
				break;

			case (5):			// Option 5: Print unmatched passengers and their information
				printf("Course name: ");
				fgets(input, 63, stdin);
				TrimNewline(input);
				strcpy(name, input);
				if ((temp = Find(name, T)) == NULL)
					printf("Course does not exit!\n");
				else
					printf("Course topics:\n%s\n", temp->topics);
				break;

			case (6):			// Option 6: Add passenger to passengerList
				break;

			case (7):			// Option 7: Delete passenger from passengerList
				break;

			case (8):			// Option 8: Delete bus from busList
				break;

			case (9):			// Option 9: Exit program
				printf("Exiting program...\n");
				break;

			default:			// Default option: print error message and prompt user for valid input
				printf("Option invalid. Please enter a valid option.\n");
				break;
		}
	} while(option != 9);

	MakeEmpty(T);
	return 0;
}
