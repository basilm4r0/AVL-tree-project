// COMP2421 Project 2 by Basil Mari     Student ID: 1191027     Date: 27/5/2022      Section: 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

//define tree node data structure
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

//Frees tree T from memory
struct Node* MakeEmpty(struct Node* T){
	if(T != NULL){
		MakeEmpty(T->left);
		MakeEmpty(T->right);
		free(T);
	}
	return NULL;
}

//returns height of node T
int getHeight(struct Node* T) {
	if(T == NULL)
		return 0;
	return T->Height;
}

//returns max of two ints
int getMax(int a, int b) {
	return (a > b)? a : b;
}

//returns deepest left node in tree branch
struct Node* getMin(struct Node* T)  {
	if(T == NULL)
		return NULL;
	else if(T->left == NULL)
		return T;
	else
		return getMin(T->left);
}

//implements strcasecmp on 64 byte strings
int CompareName(char key[64], char key2[64]){
	return strcasecmp(key, key2);
}

//Returns course with key name[] in tree T
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

//Returns parent of node with key name[] in tree T
struct Node* FindParent(char name[64],struct Node* T){
	if(T == NULL)
		return NULL;
	else if(T->left != NULL) {
		if(CompareName(name, T->left->name) == 0)
			return T;
	}
	if(T->right != NULL) {
		if(CompareName(name, T->right->name) == 0)
			return T;
	}
	if(CompareName(name, T->name) < 0)
		return FindParent(name,T->left);
	if(CompareName(name, T->name) > 0)
		return FindParent(name,T->right);
	else
		return NULL;
};

//Checks if course with key name[] exists in tree T
int IsRepeated(struct Node* T,char name[64]){
	struct Node* N = Find(name,T);
	if(N != NULL && strcasecmp(name,N->name)==0)
		return 1;
	else
		return 0;
}

//Updates course node data
struct Node* Update(struct Node* T, char name[64], int credits, char code[16], char department[64], char topics[256]){
	strncpy(T->name, name, 64);
	T->credits = credits;
	strncpy(T->code, code, 16);
	strncpy(T->department, department, 64);
	strncpy(T->topics, topics, 256);
	return T;
}

//creates a new course node
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

//Rotates tree T to the right
struct Node* Rotateright(struct Node* T){
	struct Node* l = T->left;
	struct Node* r = l->right;

	l->right = T;
	T->left = r;

	T->Height = getMax(getHeight(T->left),getHeight(T->right))+1;
	l->Height = getMax(getHeight(l->left),getHeight(l->right))+1;
	return l;
}

//Rotates tree T to the left
struct Node* Rotateleft(struct Node* T){
	struct Node* r = T->right;
	struct Node* l = r->left;

	r->left = T;
	T->right = l;

	T->Height = getMax(getHeight(T->left),getHeight(T->right))+1;
	r->Height = getMax(getHeight(r->left),getHeight(r->right))+1;
	return r;
}

//Returns banance factor for node N
int getBalance(struct Node* N){
	if (N == NULL)
		return 0;
	return getHeight(N->left) - getHeight(N->right);
}

//Inserts node in tree T and balances it
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
	if(Balance < -1 && CompareName(name, T->right->name) < 0) {
		T->right = Rotateright(T->right);
		return Rotateleft(T);
	}
	return T;
}

//Deletes node with key name[] from tree with root node root
struct Node* DeleteNode(struct Node* root, struct Node* T,char name[64]){
	if(T == NULL)
		return T;
	if(CompareName(name, T->name) < 0)
		T->left = DeleteNode(root, T->left,name);
	else if(CompareName(name, T->name) > 0)
		T->right = DeleteNode(root, T->right,name);
	else{
		if(T->left == NULL || T->right == NULL){
			struct Node* temp = T->left ? T->left : T->right;

			if(temp == NULL){
				temp = T;
				T = NULL;
				struct Node* parent = FindParent(temp->name, root);
				if(parent != NULL) {
					if(parent->left != NULL) {
						if(CompareName(name, parent->left->name) == 0)
							parent->left = NULL;
					}
					if(parent->right != NULL) {
						if(CompareName(name, parent->right->name) == 0)
							parent->right = NULL;
					}
				}
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

			T->right = DeleteNode(root, T->right,temp->name);
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

//Deletes courses with names that start with the specified letter from tree T
void DeleteLetterCourses(char letter, struct Node* T, struct Node* root){
	if(T != NULL){
		DeleteLetterCourses(letter, T->left, root);
		DeleteLetterCourses(letter, T->right, root);
		if (letter == T->name[0])
			DeleteNode(root, root, T->name);
	}
}

//Deletes all courses that belong to a department from tree T
void DeleteDepartmentCourses(char department[64], struct Node* T, struct Node* root){
	if(T != NULL){
		DeleteDepartmentCourses(department, T->left, root);
		DeleteDepartmentCourses(department, T->right, root);
		if (strcmp(department, T->department) == 0)
			DeleteNode(root, root, T->name);
	}
}

//Prints course information of a single node
void PrintData(struct Node* node){
	if(node != NULL)
		printf("%d  %s  %d  %s  %s  %s\n", node->Height, node->name, node->credits, node->code, node->department, node->topics);
	else
		printf("Not Found!\n");
}

//Prints course infromation for tree T In Order
void InOrder(struct Node* T){
	if(T != NULL){
		InOrder(T->left);
		printf("%d  %s  %d  %s  %s  %s\n", T->Height, T->name, T->credits, T->code, T->department, T->topics);
		InOrder(T->right);
	}
}

//Prints course information for all courses in a department
void DepartmentCourses(char department[64], struct Node* T){
	if(T != NULL){
		DepartmentCourses(department, T->left);
		if (strcmp(department, T->department) == 0)
			printf("%d  %s  %d  %s  %s  %s\n", T->Height, T->name, T->credits, T->code, T->department, T->topics);
		DepartmentCourses(department, T->right);
	}
}

//Loads information from courses.txt into tree T
struct Node* LoadData(struct Node* T){
	FILE *in;
	char name[64];
	int credits;
	char code[16];
	char department[64];
	char topics[256];
	char line[500];
	in = fopen("courses.txt","r");
	while(fgets(line,499,in) != 0){	//iterate file lines
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

//Prints passenger information from tree T to file out
void SaveData(FILE* out,struct Node* T){
	if(T != NULL){
		SaveData(out,T->left);
		fprintf(out, "%s:%d#%s/%s\n", T->name, T->credits, T->code, T->topics);
		SaveData(out,T->right);
	}
}

//trims newline character at end of name[] string
int TrimNewline(char name[]) {
	if ((strlen(name) > 0) && (name[strlen (name) - 1] == '\n'))
		name[strlen (name) - 1] = '\0';
	return 1;
}

//Main body of program contains user interface
int main() {
	int option;
	char name[64];
	char newname[64];
	int credits;
	char code[16];
	char department[64];
	char topics[256];
	char input[256];
	char letter;
	struct Node* T = NULL;
	struct Node* temp = NULL;
	FILE* out;
	out = fopen("offered_courses.txt","w");

	printf("\nEnter a number to perform one of the following operations:\n1. Load course list from busses.txt into a tree.\n2. Add new course.\n3. Update the information of a course.\n4. Print courses and their information in lexicographic order.\n5. Print topics associated with a course.\n6. List all courses in a department in lexicographic order.\n7. Delete a course.\n8. Delete all courses that start with a specific letter.\n9. Delete all courses that belong to a department.\n10. Save all information in a file named offered_courses.txt.\n11. Exit program.\n");
	do {
		printf("Option: ");		//Printing prompt and reading user input
		fgets(input, 63, stdin);
		sscanf(input, "%d", &option);
		switch (option) {

			case (1):			//Option 1: Load courses from courses.txt into a tree
				T = LoadData(T);
				break;

			case (2):			//Option 2: Add a new course to tree
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

			case (3):			//Option 3: Update course information for a node
				printf("Enter the information of the course to be updated. Course name: ");
				fgets(input, 63, stdin);
				TrimNewline(input);
				strcpy(name, input);
				printf("New course name: ");
				fgets(input, 63, stdin);
				TrimNewline(input);
				strcpy(newname, input);
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
					if (CompareName(newname, name) == 0)
						Update(temp, name, credits, code, department, topics);
					else {
						Insert(T, newname, credits, code, department, topics);
						DeleteNode(T, temp, name);
					}
				}
				break;

			case (4):			//Option 4: Print all course information in lexicographic order
				if (T != NULL) {
					printf("\nCourses in tree:\n");
					InOrder(T);
				}
				break;

			case (5):			// Option 5: Print course information of a specific course
				printf("Course name: ");
				fgets(input, 63, stdin);
				TrimNewline(input);
				strcpy(name, input);
				if ((temp = Find(name, T)) == NULL)
					printf("Course does not exit!\n");
				else
					printf("Course topics:\n%s\n", temp->topics);
				break;

			case (6):			// Option 6: Print course information of all courses within a department
				printf("Department name: ");
				fgets(input, 63, stdin);
				TrimNewline(input);
				strcpy(department, input);
				if (T != NULL) {
					printf("\nCourses in department:\n");
					DepartmentCourses(department, T);
				}
				break;

			case (7):			// Option 7: Delete a course from the tree
				printf("Course name: ");
				fgets(input, 63, stdin);
				TrimNewline(input);
				strcpy(name, input);
				if ((temp = Find(name, T)) == NULL)
					printf("Course does not exit!\n");
				else
					DeleteNode(T, T, name);

				break;

			case (8):			// Option 8: Delete all courses that start with a specific letter
				printf("Letter: ");
				fgets(input, 5, stdin);
				TrimNewline(input);
				sscanf(input, "%c", &letter);
				if (T != NULL) {
					DeleteLetterCourses(letter, T, T);
				}
				break;

			case (9):			// Option 9: Delete all courses that belong to a specific department
				printf("Department name: ");
				fgets(input, 63, stdin);
				TrimNewline(input);
				strcpy(department, input);
				if (T != NULL) {
					DeleteDepartmentCourses(department, T, T);
				}
				break;

			case (10):			// Option 10: Save course information to output file offered_courses.txt
				SaveData(out, T);
				break;

			case (11):			// Option 11: Exit program
				printf("Exiting program...\n");
				break;

			default:			// Default option: print error message and prompt user for valid input
				printf("Option invalid. Please enter a valid option.\n");
				break;
		}
	} while(option != 11); //check if exit option has been picked

	MakeEmpty(T);
	return 0;
}
