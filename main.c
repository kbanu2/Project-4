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

StudentNode* createStudent(char* name, char* id, double gpa, int creditHours);
void insertNameInOrder(StudentNode**, StudentNode*);
void insertIDInOrder(StudentNode** pHead, StudentNode* student);
void insertGPAInOrder(StudentNode**, StudentNode*);
void insertStudent(Database* db, char* name, char* id, double gpa, int creditHours);
int deleteStudentFromDB(Database*, char* ID);
StudentNode* findAndRemoveStudentID(StudentNode**, char*);
void removeStudentByAddress(StudentNode** pHead, StudentNode* studentToRemove);

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
	insertIDInOrder(&(db->pIDList), createStudent(name, id, gpa, creditHours));

	if (gpa >= 3.5)
		insertGPAInOrder(&(db->pHonorRollList), createStudent(name, id, gpa, creditHours));
	else if (gpa < 2.0)
		insertGPAInOrder(&(db->pAcademicProbationList), createStudent(name, id, gpa, creditHours));
	
	if (creditHours >= 90)
		insertNameInOrder(&(db->pSeniorList), createStudent(name, id, gpa, creditHours));
	else if (creditHours >= 60)
		insertNameInOrder(&(db->pJuniorList), createStudent(name, id, gpa, creditHours));
	else if (creditHours >= 30)
		insertNameInOrder(&(db->pSophomoreList), createStudent(name, id, gpa, creditHours));
	else
		insertNameInOrder(&(db->pFreshmanList), createStudent(name, id, gpa, creditHours));
}

/// @brief This function will remove a StudentNode from a list and will return a pointer to the StudentNode removed
/// @param pHead passed by reference pointer to head of list to remove node from
/// @param ID char array of StudentNode ID number to search and remove
/// @return pointer to StudentNode that was removed from the list
StudentNode* removeAndReturnByID(StudentNode** pHead, char* ID){
	StudentNode* curr = (*pHead);
	StudentNode* prev = NULL;

	while (curr != NULL){
		if (strcmp(curr->pStudent->id, ID) == 0){
			if (prev == NULL)
				(*pHead) = curr->pNext;
			else
				prev->pNext = curr->pNext;
			return curr;
		}
		prev = curr;
		curr = curr->pNext;
	}
	return NULL;
}

/// @brief This function will remove a StudentNode from a list and will free the node from memory
/// @param pHead passed by reference pointer to beginning of list to remove node from
/// @param ID char array of StudentNode ID to remove
void removeAndDeleteStudentByID(StudentNode** pHead, char* ID){
	StudentNode* curr = (*pHead);
	StudentNode* prev = NULL;
	while (curr != NULL){
		if (strcmp(curr->pStudent->id, ID) == 0){
			if (prev == NULL)
				(*pHead) = curr->pNext;
			else
				prev->pNext = curr->pNext;

			free(curr->pStudent->id);
			free(curr->pStudent->name);
			free(curr->pStudent);
			free(curr);
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
	StudentNode* studentToRemove = removeAndReturnByID(&(db->pIDList), ID);
	if (studentToRemove == NULL)
		return 0;

	if (studentToRemove->pStudent->gpa >= 3.5)
		removeAndDeleteStudentByID(&(db->pHonorRollList), ID);
	else if (studentToRemove->pStudent->gpa < 2.0)
		removeAndDeleteStudentByID(&(db->pAcademicProbationList), ID);
	
	if (studentToRemove->pStudent->creditHours >= 90)
		removeAndDeleteStudentByID(&(db->pSeniorList), ID);
	else if (studentToRemove->pStudent->creditHours >= 60)
		removeAndDeleteStudentByID(&(db->pJuniorList), ID);
	else if (studentToRemove->pStudent->creditHours >= 30)
		removeAndDeleteStudentByID(&(db->pSophomoreList), ID);
	else
		removeAndDeleteStudentByID(&(db->pFreshmanList), ID);

	free(studentToRemove->pStudent->id);
	free(studentToRemove->pStudent->name);
	free(studentToRemove->pStudent);
	free(studentToRemove);
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
StudentNode* createStudent(char* name, char* id, double gpa, int creditHours){
	StudentNode* node = (StudentNode*)malloc(sizeof(StudentNode));
	Student* student = (Student*)malloc(sizeof(Student));

	student->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	student->id = (char*)malloc(sizeof(char) * (strlen(id) + 1));
	strcpy(student->name, name);
	strcpy(student->id, id);
	student->gpa = gpa;
	student->creditHours = creditHours;

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

void printDatabase(Database* db){
	printf("ID List: ");
	printIDList(db->pIDList);
	printf("Honor List: ");
	printGPAList(db->pHonorRollList);
	printf("Probation List: ");
	printGPAList(db->pAcademicProbationList);
	printf("Senior List: ");
	printNameList(db->pSeniorList);
	printf("Junior List: ");
	printNameList(db->pJuniorList);
	printf("Sophomore List: ");
	printNameList(db->pSophomoreList);
	printf("Freshmen List: ");
	printNameList(db->pFreshmanList);
}


int main() {
	Database* t = (Database*)malloc(sizeof(Database));
	initializeDB(t);

	insertStudent(t, "krenar", "1", 3.5, 100);
	insertStudent(t, "adam", "100", 1.9, 30);
	insertStudent(t, "izzy", "2", 3, 70);
	insertStudent(t, "fiona", "8", 1.2, 120);
	insertStudent(t, "laine", "0", 4.0, 50);

	printDatabase(t);
	printf("\n\n");

	deleteStudentFromDB(t, "1");
	deleteStudentFromDB(t, "100");
	deleteStudentFromDB(t, "2");
	deleteStudentFromDB(t, "8");
	deleteStudentFromDB(t, "0");

	insertStudent(t, "krenar", "1", 3.5, 100);
	insertStudent(t, "fiona", "8", 1.2, 120);

	deleteStudentFromDB(t, "1");
	deleteStudentFromDB(t, "8");
	deleteStudentFromDB(t, "123");

	printDatabase(t);

	free(t);

	// printf("CS 211, Spring 2023\n");
	// printf("Program 4: Database of Students\n\n");

	// printf("Enter E to start with an empty database, \n");
	// printf("or F to start with a database that has information on students from a file.\n");
	// printf("Your choice --> ");
	// printf("Sorry, that input was invalid. Please try again.\n");
	
	// printf("Enter the name of the file you would like to use: ");

	// printf("\nEnter: \tC to create a new student and add them to the database,\n");
	// printf("\tR to read from the database,\n");
	// printf("\tD to delete a student from the database, or\n");
	// printf("\tX to exit the program.\n");
	// printf("Your choice --> ");
	// printf("Invalid option. Try again.\n");

	// printf("Enter the name of the new student: ");
	// printf("Enter the ID of the new student: ");
	// printf("Enter the GPA of the new student: ");
	// printf("Enter the credit hours of the new student: ");
	// printf("Successfully added the following student to the database!\n");

	// printf("Select one of the following: \n");
	// printf("\t1) Display the head (first 10 rows) of the database\n");
	// printf("\t2) Display students on the honor roll, in order of their GPA\n");
	// printf("\t3) Display students on academic probation, in order of their GPA\n");
	// printf("\t4) Display freshmen students, in order of their name\n");
	// printf("\t5) Display sophomore students, in order of their name\n");
	// printf("\t6) Display junior students, in order of their name\n");
	// printf("\t7) Display senior students, in order of their name\n");
	// printf("\t8) Display the information of a particular student\n");
	// printf("Your choice --> ");
	// printf("Sorry, that input was invalid. Please try again.\n");
	
	// printf("There are no students matching that criteria.\n\n");

	// printf("Enter the id of the student to find: ");
	// printf("Sorry, there is no student in the database with the id 6ABCDEFGH.\n");

	// printf("Thanks for playing!\n");
	// printf("Exiting...\n");
	
	return 0;
}