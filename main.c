#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* name;
	char* id;
	double gpa;
	int creditHours;
} Student;

typedef struct StudentNode{
	Student* pStudent;
	struct StudentNode* pNext;
} StudentNode;

typedef struct {
	StudentNode* pIDList; //insertIDInOrder
	StudentNode* pHonorRollList; //insertGPAInOrder
	StudentNode* pAcademicProbationList; //insertGPAInOrder
	StudentNode* pFreshmanList; //insertNameInOrder
	StudentNode* pSophomoreList; //insertNameInOrder
	StudentNode* pJuniorList; //insertNameInOrder
	StudentNode* pSeniorList; //insertNameInOrder
} Database;

Student* createStudent(char* name, char* id, double gpa, int creditHours);
StudentNode* createStudentNode(Student* student);
void insertNameInOrder(StudentNode**, StudentNode*);
void insertIDInOrder(StudentNode** pHead, StudentNode* student);
void insertGPAInOrder(StudentNode**, StudentNode*);
void insertStudent(Database* db, char* name, char* id, double gpa, int creditHours);
int deleteStudentFromDB(Database*, char* ID);
void deleteStudentNodeByID(StudentNode** pHead, char* ID);
Student* findStudentByID(StudentNode* pHead, char* ID);

/// @brief insert a StudentNode into a list in alphabetical order of the student's name
/// @param pHead pointer to head of list passed by reference
/// @param student StudentNode initialized with data and pNext as NULL
void insertNameInOrder(StudentNode** pHead, StudentNode* student){
	if (*pHead == NULL){ //Empty List
		student->pNext = NULL;
		*pHead = student;
		return;
	}
	StudentNode* prev = NULL;
	StudentNode* curr = *pHead;

	while (curr != NULL){
		if (strcmp(student->pStudent->name, curr->pStudent->name) <= 0){ //Insert before
			if (prev == NULL){ //Insert before first node
				student->pNext = (*pHead);
				(*pHead) = student;
				return;
			}
			else{
				student->pNext = curr;
				prev->pNext = student;
				return;
			}
		}
		else{
			prev = curr;
			curr = curr->pNext;
		}
		if (curr == NULL) //Append to end
			prev->pNext = student;
	}
}

/// @brief insert a StudentNode into a list in alphabetical order of the ID
/// @param pHead pointer to head of list passed by reference
/// @param student StudentNode initialized with data and pNext as NULL
void insertIDInOrder(StudentNode** pHead, StudentNode* student){
	if (*pHead == NULL){ //Empty List
		*pHead = student;
		return;
	}
	StudentNode* prev = NULL;
	StudentNode* curr = *pHead;

	while (curr != NULL){
		if (strcmp(student->pStudent->id, curr->pStudent->id) <= 0){ //Insert before
			if (prev == NULL){ //Insert before first node
				student->pNext = (*pHead);
				(*pHead) = student;
				return;
			}
			else{
				student->pNext = curr;
				prev->pNext = student;
				return;
			}
		}
		else{
			prev = curr;
			curr = curr->pNext;
		}
		if (curr == NULL) //Append to end
			prev->pNext = student;
	}
}

/// @brief insert a StudentNode into a list in alphabetical order of the student's GPA
/// @param pHead pointer to head of list passed by reference
/// @param student StudentNode initialized with data and pNext as NULL
void insertGPAInOrder(StudentNode** pHead, StudentNode* student){
	if (*pHead == NULL){ //Empty List
		student->pNext = NULL;
		*pHead = student;
		return;
	}
	StudentNode* prev = NULL;
	StudentNode* curr = *pHead;

	while (curr != NULL){
		if (student->pStudent->gpa <= curr->pStudent->gpa){ //Insert before
			if (prev == NULL){ //Insert before first node
				student->pNext = (*pHead);
				(*pHead) = student;
				return;
			}
			else{
				student->pNext = curr;
				prev->pNext = student;
				return;
			}
		}
		else{
			prev = curr;
			curr = curr->pNext;
		}
		if (curr == NULL) //Append to end
			prev->pNext = student;
	}
}

/// @brief insert StudentNode object into correct list based on their GPA and credit standing
/// @param db pointer to database
/// @param student pointer to StudentNode to insert.  pNext must be initialized to NULL.
void insertStudent(Database* db, char* name, char* id, double gpa, int creditHours){
	Student* student = createStudent(name, id, gpa, creditHours);

	insertIDInOrder(&(db->pIDList), createStudentNode(student));

	if (gpa >= 3.5)
		insertGPAInOrder(&(db->pHonorRollList), createStudentNode(student));
	else if (gpa < 2.0)
		insertGPAInOrder(&(db->pAcademicProbationList), createStudentNode(student));
	
	if (creditHours >= 90)
		insertNameInOrder(&(db->pSeniorList), createStudentNode(student));
	else if (creditHours >= 60)
		insertNameInOrder(&(db->pJuniorList), createStudentNode(student));
	else if (creditHours >= 30)
		insertNameInOrder(&(db->pSophomoreList), createStudentNode(student));
	else
		insertNameInOrder(&(db->pFreshmanList), createStudentNode(student));
}

Student* findStudentByID(StudentNode* pHead, char* ID){
	StudentNode* curr = pHead;
	while (curr != NULL){
		if (strcmp(curr->pStudent->id, ID) == 0){
			return curr->pStudent;
		}
		curr = curr->pNext;
	}
	return NULL;
}

void deleteStudentNodeByID(StudentNode** pHead, char* ID){
	StudentNode* curr = (*pHead);
	StudentNode* prev = NULL;

	while (curr != NULL){
		if (strcmp(curr->pStudent->id, ID) == 0){
			if (prev == NULL)
				(*pHead) = curr->pNext;
			else
				prev->pNext = curr->pNext;
			free (curr);
			return;
		}
		prev = curr;
		curr = curr->pNext;
	}
}

/// @brief delete StudentNode from all lists using the student's ID number.
/// @param db pointer to database to delete StudentNode from
/// @param ID string of Student ID to delete
/// @return int return 1 if student was found and deleted, return 0 if student was not found
int deleteStudentFromDB(Database* db, char* ID){
	Student* studentToDelete = findStudentByID(db->pIDList, ID);
	if (studentToDelete == NULL)
		return 0;

	deleteStudentNodeByID(&(db->pIDList), ID);

	if (studentToDelete->gpa >= 3.5)
		deleteStudentNodeByID(&(db->pHonorRollList), ID);
	else if (studentToDelete->gpa < 2.0)
		deleteStudentNodeByID(&(db->pAcademicProbationList), ID);
	
	if (studentToDelete->creditHours >= 90)
		deleteStudentNodeByID(&(db->pSeniorList), ID);
	else if (studentToDelete->creditHours >= 60)
		deleteStudentNodeByID(&(db->pJuniorList), ID);
	else if (studentToDelete->creditHours >= 30)
		deleteStudentNodeByID(&(db->pSophomoreList), ID);
	else
		deleteStudentNodeByID(&(db->pFreshmanList), ID);

	free(studentToDelete->id);
	free(studentToDelete->name);
	free(studentToDelete);
	return 1;
}

void printNameList(StudentNode* pHead){
	if (pHead == NULL)
		printf("empty list");
	while (pHead != NULL){
		printf("%s ", pHead->pStudent->name);
		pHead = pHead->pNext;
	}
	printf("\n");
}

void printIDList(StudentNode* pHead){
	if (pHead == NULL)
		printf("empty list");
	while (pHead != NULL){
		printf("%s ", pHead->pStudent->id);
		pHead = pHead->pNext;
	}
	printf("\n");
}

void printGPAList(StudentNode* pHead){
	if (pHead == NULL)
		printf("empty list");
	while (pHead != NULL){
		printf("%lf ", pHead->pStudent->gpa);
		pHead = pHead->pNext;
	}
	printf("\n");
}

void printCreditList(StudentNode* pHead){
	if (pHead == NULL)
		printf("empty list");
	while (pHead != NULL){
		printf("%d ", pHead->pStudent->creditHours);
		pHead = pHead->pNext;
	}
	printf("\n");
}

/// @brief Dyanamically allocate StudentNode and intialize with given data
/// @param name string to set Student name to
/// @param id string to set Student ID to 
/// @param gpa double to set Student GPA to
/// @param creditHours int to set Student creditHours to
/// @return pointer to dynamically allocated StudentNode with pNext intialized to NULL
Student* createStudent(char* name, char* id, double gpa, int creditHours){
	Student* student = (Student*)malloc(sizeof(Student));

	student->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	student->id = (char*)malloc(sizeof(char) * (strlen(id) + 1));
	strcpy(student->name, name);
	strcpy(student->id, id);
	student->gpa = gpa;
	student->creditHours = creditHours;

	return student;
}

StudentNode* createStudentNode(Student* student){
	StudentNode* node = (StudentNode*)malloc(sizeof(StudentNode));
	node->pStudent = student;
	node->pNext = NULL;

	return node;
}

/// @brief Initialize all database lists to null
/// @param db pointer to database to initialize
void initializeDB(Database* db){
	db->pIDList = NULL;
	db->pHonorRollList = NULL;
	db->pAcademicProbationList = NULL;
	db->pSeniorList = NULL;
	db->pJuniorList = NULL;
	db->pSophomoreList = NULL;
	db->pFreshmanList = NULL;
}

void displayHeadOfDB(StudentNode* pHead){
	int i = 0;
	while (pHead != NULL && i < 10){
		printf("%s:\n", pHead->pStudent->name);
		printf("\tID - %s\n", pHead->pStudent->id);
		printf("\tGPA - %0.2f\n", pHead->pStudent->gpa);
		printf("\tCredit Hours - %d\n", pHead->pStudent->creditHours);

		pHead = pHead->pNext;
		i++;
	}
}

void displayStudentsInList(StudentNode* pHead){
	if (pHead == NULL){
		printf("There are no students matching that criteria.\n\n");
		return;
	}

	while (pHead != NULL){
		printf("%s:\n", pHead->pStudent->name);
		printf("\tID - %s\n", pHead->pStudent->id);
		printf("\tGPA - %0.2f\n", pHead->pStudent->gpa);
		printf("\tCredit Hours - %d\n", pHead->pStudent->creditHours);

		pHead = pHead->pNext;
	}
}

int loadFile(Database* db, char fileName[81]){
	FILE* filePtr = fopen("small-list.csv", "r");

	if (filePtr == NULL){
		printf("Failed to open file %s\n", fileName);
		return 0;
	}

	char line[1024];
	char* splitLine;
	char* name = (char*)malloc(sizeof(char) * 81);
	char* id = (char*)malloc(sizeof(char) * 81);
	double gpa;
	int creditHours, column;
	fgets(line, 1024, filePtr); //Remove first line
	while (fgets(line, 1024, filePtr)){
		splitLine = strtok(line, ",");
		column = 0;
		while (splitLine != NULL){
			switch (column){
				case 0:
					strcpy(name, splitLine);
					break;
				case 1:
					strcpy(id, splitLine);
					break;
				case 2:
					sscanf(splitLine, "%lf", &gpa);
					break;
				case 3:
					sscanf(splitLine, "%d", &creditHours);
					break;
			}
			column++;
			splitLine = strtok(NULL, ", ");
		}
		insertStudent(db, name, id, gpa, creditHours);
	}

	free(name);
	free(id);
	fclose(filePtr);
	return 1;
}

void freeDataBase(Database** db){
	StudentNode* next = NULL;
	StudentNode* curr = (*db)->pIDList;
	while (curr != NULL){
		next = curr->pNext;
		deleteStudentFromDB(*db, curr->pStudent->id);

		curr = next;
	}

	free(*db);
	(*db) = NULL;
}

void printMainMenu(){
	printf("\nEnter: \tC to create a new student and add them to the database,\n");
	printf("\tR to read from the database,\n");
	printf("\tD to delete a student from the database, or\n");
	printf("\tX to exit the program.\n");
}

void createStudentMenu(Database* db){
	char name[81];
	char id[81];
	double gpa;
	int creditHours;
	fflush(stdin);

	printf("Enter the name of the new student: ");
	fgets(name, 81, stdin);
	name[strlen(name) - 1] = 0;
	printf("Enter the ID of the new student: ");
	scanf("%s", id);
	printf("Enter the GPA of the new student: ");
	scanf("%lf", &gpa);
	printf("Enter the credit hours of the new student: ");
	scanf("%d", &creditHours);

	insertStudent(db, name, id, gpa, creditHours);

	printf("Successfully added the following student to the database!\n");
	printf("%s\n", name);
	printf("\tID - %s\n", id);
	printf("\tGPA - %0.2f\n", gpa);
	printf("\tCredit Hours - %d\n\n", creditHours);
}

void printStudent(Student* student){
	printf("%s:\n", student->name);
	printf("\tID - %s\n", student->id);
	printf("\tGPA - %0.2f\n", student->gpa);
	printf("\tCredit Hours - %d\n\n", student->creditHours);
}

void readStudentsMenu(Database* db){
	int userChoice;
	char idToFind[81];
	Student* student;
	printf("Select one of the following: \n");
	printf("\t1) Display the head (first 10 rows) of the database\n");
	printf("\t2) Display students on the honor roll, in order of their GPA\n");
	printf("\t3) Display students on academic probation, in order of their GPA\n");
	printf("\t4) Display freshmen students, in order of their name\n");
	printf("\t5) Display sophomore students, in order of their name\n");
	printf("\t6) Display junior students, in order of their name\n");
	printf("\t7) Display senior students, in order of their name\n");
	printf("\t8) Display the information of a particular student\n");
	printf("Your choice --> ");
	scanf(" %d", &userChoice);

	while (1){
		switch (userChoice){
			case 1:
				displayHeadOfDB(db->pIDList);
				return;
			case 2:
				displayStudentsInList(db->pHonorRollList);
				return;
			case 3:
				displayStudentsInList(db->pAcademicProbationList);
				return;
			case 4:
				displayStudentsInList(db->pFreshmanList);
				return;
			case 5:
				displayStudentsInList(db->pSophomoreList);
				return;
			case 6:
				displayStudentsInList(db->pJuniorList);
				return;
			case 7:
				displayStudentsInList(db->pSeniorList);
				return;
			case 8:
				printf("Enter the id of the student to find: ");
				scanf(" %s", idToFind);
				student = findStudentByID(db->pIDList, idToFind);
				if (student == NULL)
					printf("Sorry, there is no student in the database with the id %s.\n\n", idToFind);
				else
					printStudent(student);
				return;
			default:
				printf("Sorry, that input was invalid. Please try again.\n");
				printf("Your choice --> ");
				scanf(" %d", &userChoice);
		}
	}
}

int main() {
	Database* database = (Database*)malloc(sizeof(Database));
	initializeDB(database);
	char userInput;
	char inputString[81];

	printf("CS 211, Spring 2023\n");
	printf("Program 4: Database of Students\n\n");

	printf("Enter E to start with an empty database, \n");
	printf("or F to start with a database that has information on students from a file.\n");
	printf("Your choice --> ");
	scanf(" %c", &userInput);

	while(userInput != 'E' && userInput != 'F'){
		printf("Sorry, that input was invalid. Please try again.\n");
		printf("Your choice --> ");
		scanf(" %c", &userInput);
	}

	if (userInput == 'F'){
		printf("Enter the name of the file you would like to use: ");
		scanf(" %s", inputString);
		loadFile(database, inputString);
	}

	while (userInput != 'X' && userInput != 'x'){
		printMainMenu();
		printf("Your choice --> ");
		scanf(" %c", &userInput);
		switch (userInput){
			case 'C':
				createStudentMenu(database);
				break;
			case 'R':
				readStudentsMenu(database);
				break;
			case 'D':
				printf("Enter the id of the student to remove: ");
				scanf(" %s", inputString);
				deleteStudentFromDB(database, inputString);
				break;
			case 'X':
				freeDataBase(&database);
				break;
			default:
				printf("Invalid option. Try again.\n");
				printf("Your choice --> ");
				scanf(" %c", &userInput);
		}
	}

	printf("Thanks for playing!\n");
	printf("Exiting...\n");
	
	return 0;
}