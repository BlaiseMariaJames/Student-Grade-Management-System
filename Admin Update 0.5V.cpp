#include<sstream>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<sqlite3.h>
using namespace std;

string excp = "";
bool roc = false;

class student{
	protected:
	int gradepoint[10];
	char grade[10], roll_number[20];
	float m1[10], a1[10], ms1[10], m2[10], a2[10], ms2[10], internal_marks[10], external_marks[10];
	float resultm1[10], resulta1[10], resultms1[10], resultm2[10], resulta2[10], resultms2[10], finalmse[10], final_marks[10], final_gradepoint[10];
};

class class_teacher : public student{
	protected:
	char class_teacher_id[6] = {"001"}; int teacher_password;
	public:
	void read(student &s, char* roll_number, int k);
	void calculate_student_marks(student &s);
	void calculate_student_cgpa(student &s, int j);
	void write(student &s, int j, char* roll_number);
	void view_subject_wise_marks();
	void view_overall_marks();
	void generate_cgpa(student* s, class_teacher &ct);
	void class_teacher_main_menu(student* s, class_teacher &ct);
};

class section : public class_teacher{
	char student_id[6], subject[20]; int student_password;
	public:
	void login_student();
	void student_main_menu(section &s);
	void view_marks();
};

class course_teacher : public class_teacher{
	protected:
	char teacher_id[6], subject[20]; int teacher_password;
	public:
	void login_teacher(course_teacher &t);
	void teacher_main_menu(course_teacher &t);
	void add_student_marks();
	void view_overall_marks();
};

class admin{
	string id, password;
	public:
	void login_master();
	void add_faculty();
	void update_faculty();
	void delete_faculty();
	void view_faculty();
	void master_faculty_menu(admin &a);
	void add_student();
	void update_student();
	void delete_student();
	void view_student();
    void master_student_menu(admin &a);
	void master_main_menu(admin &a);
};

bool check_exception(string exp){
	int len = exp.size();
	for(int i = 0; i < len; i++){
	int c = exp[i];
	if(c<48 || c>57)
	return true;
	}
	return false;
}

static int create_insert_table(void *NotUsed, int argc, char **argv, char **azColName) {
   for(int i = 0; i<argc; i++) {
   printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

static int select_table(void *data, int argc, char **argv, char **azColName){
   for(int i = 0; i<argc; i++){
   printf("%s ", argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

static int exist_table(void *NotUsed, int argc, char **argv, char **azColName){
   for(int i = 0; i<argc; i++){
   const char *data = argv[i] ? argv[i] : "NULL";
   int access = stoi(data);
   if(access == 0)
   return 0;
   }
   return 1;
}

void admin :: login_master(){
	int count = 3;
	a: cout << "WELCOME TO ADMIN LOGIN PAGE\n";
	cout << "\nEnter Master ID : ";
	cin >> id;
	cout << "Enter Password : ";
	cin >> password;
	if(id == "196612" && password == "1234")
	cout << "\nLogin Successful!!!\n";
	else{
	cout << "Login Unsuccessful!!!";
	if(count <= 0){
	cout << "\n\nLogin Failed... Max no. of Attempts Reached!!!\n";
	cout << "Exiting Program for Security Reasons...\n";
	exit(0);
	}
	count--;
	cout << "\n\nYou have '" << count + 1 << "' more chances!!!\n";
	system("PAUSE");
	system("CLS");
	goto a;
	}
	system("PAUSE");
	system("CLS");
	return;
}

void admin :: add_faculty(){
    sqlite3 *db;
    int rc, n, dept_id;
    sqlite3_open("SAMS.db", &db);
	char *zErrMsg, *sql, ch, name[40], qualification[30], designation[40], research_area[60];
	char dept[12][4] = {"CSE","CSM","CSN","CSO","IT ","ECE","EEE","ECI","CIV","MEC","EMH","ENG"};
	char dept_no[12][4] = {"CS","AI","CN","IN","IT","EC","EE","CI","CE","ME","MH","EN"};
	a: cout << "Enter the number of faculty(ies) : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	cout << "\nChoose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	system("PAUSE");
	system("CLS");
	goto a;
	}
	n = stoi(excp);
	cout << "\nEntering Details of " << n << " faculty(ies)..." << endl;
	for(int i = 0; i < n; i++){
	b : cout << "\nEntering Details of faculty " << i+1 << endl;
	cout << "\nSelect Branch \n\n";
	cout << "Type '1' ----> CSE\n";
	cout << "Type '2' ----> CSM\n";
	cout << "Type '3' ----> CSN\n";
	cout << "Type '4' ----> CSO\n";
	cout << "Type '5' ----> IT\n";
	cout << "Type '6' ----> ECE\n";
	cout << "Type '7' ----> EEE\n";
	cout << "Type '8' ----> ECI\n";
	cout << "Type '9' ----> CIV\n";
	cout << "Type '10' ----> ME\n";
	cout << "Type '11' ----> EMH\n";
	cout << "Type '12' ----> ENG\n";
	cout << "\nEnter Here : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	c: cout << "\nChoose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	system("PAUSE");
	system("CLS");
	cout << "Enter the number of faculty(ies) : " << n << endl;
	cout << "\nEntering Details of " << n << " faculty(ies)..." << endl;
	goto b;
	}
	dept_id = stoi(excp);
    if(dept_id>12 || dept_id<1){
    goto c;
	}
	int id = dept_id - 1;
	system("CLS");
	cout << "Enter the number of faculty(ies) : " << n << endl;
	cout << "\nEntering Details of " << n << " faculty(ies)..." << endl;
	cout << "\nEntering Details of faculty " << i+1 << endl;
	cout << "\nEnter Branch : " << dept[id] << endl;
	cout << "Enter Faculty Name : ";
	cin >> ch;
	int j=0;
	while (ch != '\n' && j<40){
    name[j] = ch;
    ch = cin.get();
    j++;
	}
	name[j] = '\0';
	int len = 0;
	len = strlen(name);
	while(len < 39){
	name[len] = ' ';
	len = len + 1;
	}
    name[len] = '\0';
	cout << "Enter Faculty Qualification : ";
	cin >> ch;
	j=0;
	while (ch != '\n' && j<30){
    qualification[j] = ch;
    ch = cin.get();
    j++;
	}
	qualification[j] = '\0';
	len = 0;
	len = strlen(qualification);
	while(len < 29){
	qualification[len] = ' ';
	len = len + 1;
	}
    qualification[len] = '\0';
	cout << "Enter Faculty Designation : ";
	cin >> ch;
	j=0;
	while (ch != '\n' && j<40){
    designation[j] = ch;
    ch = cin.get();
    j++;
	}
	designation[j] = '\0';
	len = 0;
	len = strlen(designation);
	while(len < 39){
	designation[len] = ' ';
	len = len + 1;
	}
    designation[len] = '\0';
	cout << "Enter Faculty Research Area : ";
	cin >> ch;
    j=0;
	while (ch != '\n' && j<60){
    research_area[j] = ch;
    ch = cin.get();
    j++;
	}
	research_area[j] = '\0';
	len = 0;
	len = strlen(research_area);
	while(len < 59){
	research_area[len] = ' ';
	len = len + 1;
	}
    research_area[len] = '\0';
    string faculty_id = dept[id];
    string faculty_name = name;
    string faculty_qualification = qualification;
    string faculty_designation = designation;
    string faculty_researcharea = research_area;
    string faculty_deptno = dept_no[id];
	string insert_faculty = "INSERT INTO FACULTY VALUES ('" + faculty_id + "', '" + faculty_name + "', '" + faculty_qualification + "', '" + faculty_designation + "', '" + faculty_researcharea + "', '" + faculty_deptno + "');";
    const char *line = insert_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "\nSQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    cout << "\nUnable to register requested details of faculty... Try Again with valid ID...\n" << endl;
    system("PAUSE");
    system("CLS");
	cout << "Enter the number of faculty(ies) : " << n << endl;
	cout << "\nEntering Details of " << n << " faculty(ies)..." << endl;
    goto b;
    }
    if(i == n-1)
    cout << "\nDetails of student " << i+1 << " registered successfully..." << endl;
    else{
    cout << "\nDetails of student " << i+1 << " registered successfully... \n" << endl;
    system("PAUSE");
    system("CLS");
	cout << "Enter the number of faculty(ies) : " << n << endl;
	cout << "\nEntering Details of " << n << " faculty(ies)..." << endl;
    }
	}
    sqlite3_close(db);
    cout << "\nDetails of " << n << " faculty(ies) registered successfully..." << endl;
    cout << endl;
	system("PAUSE");
	system("CLS");
	return;
}

int update_faculty_function(string &old_id){
    int rc = 1;
    sqlite3 *db;
	char *zErrMsg, *sql;
	sqlite3_open("SAMS.db", &db);
    cout << "\n\nID    NAME\t\t\t    COURSE\t\t\t  CODE" << endl;
    sql = strdup("SELECT * from FACULTY ORDER BY ID");
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    cout << "\n\nEnter the ID to be updated : ";
    cin >> old_id;
    string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE ID = '"+ old_id +"');";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    sqlite3_close(db);
	if(rc == 0){
    cout << "\nFaculty with requested ID doesn't exist..." << endl;
    cout << "\nUnable to update requested details of faculty... Try Again using valid ID...\n" << endl;
    system("PAUSE");
    system("CLS");
    return 0;
    }
    else
    return 1;
}

void admin :: update_faculty(){
    sqlite3 *db;
    string old_id;
    int option = 0, rc;
	char *zErrMsg, *sql;
    while(option !=5){
	a: cout << "\nSelect the field to be updated...\n\n\n";
	cout << "Type '1' ----> Update ID\n";
	cout << "Type '2' ----> Update Name\n";
	cout << "Type '3' ----> Update Course\n";
	cout << "Type '4' ----> Update Code\n";
	cout << "Type '5' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: cout << "\nChoose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	system("PAUSE");
	system("CLS");
	goto a;
	}
	option = stoi(excp);
	if(option>5 || option<1){
    goto b;
	}
	else if(option == 1){
    int rc;
	system("CLS");
	string new_id;
	rc = update_faculty_function(old_id);
	if(rc == 1){
    sqlite3_open("SAMS.db", &db);
    cout << "Enter the new ID : ";
    cin >> new_id;
	string update_faculty = "UPDATE FACULTY set ID = '"+ new_id +"' WHERE ID = '"+ old_id +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "\nSQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    cout << "\nUnable to update requested details of faculty... Try Again using valid ID...\n" << endl;
    system("PAUSE");
    system("CLS");
    goto a;
    }
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    system("PAUSE");
    system("CLS");
    return;
	}
	}
	else if(option == 2){
	int rc;
    system("CLS");
	char ch, name[30];
	rc = update_faculty_function(old_id);
	if(rc == 1){
    sqlite3_open("SAMS.db", &db);
    cout << "Enter the New Name : ";
	cin >> ch;
	int j=0;
	while (ch != '\n' && j<30){
    name[j] = ch;
    ch = cin.get();
    j++;
	}
	name[j] = '\0';
	int len = 0;
	len = strlen(name);
	while(len < 29){
	name[len] = ' ';
	len = len + 1;
	}
    name[len] = '\0';
    string new_name = name;
	string update_faculty = "UPDATE FACULTY set NAME = '"+ new_name +"' WHERE ID = '"+ old_id +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    system("PAUSE");
    system("CLS");
    return;
	}
	}
	else if(option == 3){
	int rc;
    system("CLS");
	char ch, course[30];
	rc = update_faculty_function(old_id);
	if(rc == 1){
    sqlite3_open("SAMS.db", &db);
    cout << "Enter the new Course : ";
    cin >> ch;
	int j=0;
	while (ch != '\n' && j<30){
    course[j] = ch;
    ch = cin.get();
    j++;
	}
	course[j] = '\0';
	int len = 0;
	len = strlen(course);
	while(len < 29){
	course[len] = ' ';
	len = len + 1;
	}
    course[len] = '\0';
    string new_course = course;
	string update_faculty = "UPDATE FACULTY set COURSE = '"+ new_course +"' WHERE ID = '"+ old_id +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    system("PAUSE");
    system("CLS");
    return;
	}
    }
	else if(option == 4){
	int rc;
    system("CLS");
	string new_code;
	rc = update_faculty_function(old_id);
	if(rc == 1){
    sqlite3_open("SAMS.db", &db);
    cout << "Enter the new Code : ";
    cin >> new_code;
	string update_faculty = "UPDATE FACULTY set CODE = '"+ new_code +"' WHERE ID = '"+ old_id +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    system("PAUSE");
    system("CLS");
    return;
	}
	}
	if(option == 5){
	cout << "Redirecting back...\n";
	system("PAUSE");
	system("CLS");
	return;
	}
}
}

void admin :: delete_faculty(){
    int rc = 1;
    string id;
    sqlite3 *db;
	char *zErrMsg, *sql;
    sqlite3_open("SAMS.db", &db);
    cout << "\n\nID    NAME\t\t\t    COURSE\t\t\t  CODE" << endl;
    sql = strdup("SELECT * from FACULTY ORDER BY ID");
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	cout << "\n\nEnter the id to be deleted : ";
	cin >> id;
	string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE ID = '"+ id +"');";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    sqlite3_close(db);
    if(rc == 0){
    cout << "\nFaculty with requested ID doesn't exist..." << endl;
    cout << "\nUnable to delete requested details of faculty... Try Again using valid ID...\n" << endl;
    system("PAUSE");
    system("CLS");
    return;
    }
	a: cout << "\n\nAre you sure to delete the record of faculty with id " + id + " ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
    b: sqlite3_open("SAMS.db", &db);
	cout << "\nChoose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	system("PAUSE");
	system("CLS");
    cout << "\n\nID    NAME\t\t\t    COURSE\t\t\t  CODE" << endl;
    sql = strdup("SELECT * from FACULTY ORDER BY ID");
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    cout << "\n\nEnter the id to be deleted : " << id << endl;
	goto a;
	}
    sqlite3_close(db);
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto b;
	}
	if(choice == 1){
    sqlite3_open("SAMS.db", &db);
    string delete_faculty = "DELETE from FACULTY where ID = '"+ id +"';";
    const char *line = delete_faculty.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested details deleted successfully..." << endl;
    cout << endl;
    system("PAUSE");
    system("CLS");
    return;
	}
	if(choice == 2){
    cout << "\nRequested details retained successfully..." << endl;
    cout << endl;
    system("PAUSE");
    system("CLS");
	return;
	}
}

void admin :: view_faculty(){
   int rc;
   sqlite3 *db;
   char *zErrMsg = 0, *sql;
   cout << "\nDisplaying Details of faculty(ies)..." << endl;
   cout << "\n\nID    NAME\t\t\t    COURSE\t\t\t  CODE" << endl;
   rc = sqlite3_open("SAMS.db", &db);
   sql = strdup("SELECT * from FACULTY ORDER BY ID");
   rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
   sqlite3_close(db);
   cout << "\nDetails of all faculty(ies) displayed successfully..." << endl;
   cout << endl;
   system("PAUSE");
   system("CLS");
   return;
}

void admin :: master_faculty_menu(admin &a){
	int option = 0;
	while(option !=5){
	a: cout << "\nAccessing Faculty Operations...\n\n\n";
	cout << "Type '1' ----> Add Faculty\n";
	cout << "Type '2' ----> Update Faculty\n";
	cout << "Type '3' ----> Delete Faculty\n";
	cout << "Type '4' ----> View Faculty\n";
	cout << "Type '5' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : cout << "\nChoose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	system("PAUSE");
	system("CLS");
	goto a;
	}
	option = stoi(excp);
	if(option>5 || option<1){
	goto b;
	}
	else if(option == 1){
	system("CLS");
	a.add_faculty();
	}
	else if(option == 2){
	system("CLS");
	a.update_faculty();
	}
	else if(option == 3){
	system("CLS");
	a.delete_faculty();
	}
	else if(option == 4){
	system("CLS");
	a.view_faculty();
	}
	}
	if(option == 5){
	cout << "Redirecting back to Admin Page\n";
	system("PAUSE");
	system("CLS");
	return;
	}
}

void admin :: add_student(){
    int rc, n;
    string id;
	sqlite3 *db;
    sqlite3_open("SAMS.db", &db);
	char *zErrMsg, *sql, ch, name[30];
	a: cout << "Enter the number of student(s) : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	cout << "\nChoose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	system("PAUSE");
	system("CLS");
	goto a;
	}
	n = stoi(excp);
	cout << "\nEntering Details of " << n << " student(s)..." << endl;
	for(int i = 0; i < n; i++){
	b: cout << "\nEntering Details of student " << i+1 << endl;
	cout << "\nEnter Student ID : ";
	cin >> id;
	cout << "Enter Student Name : ";
	cin >> ch;
	int j=0;
	while (ch != '\n' && j<30){
    name[j] = ch;
    ch = cin.get();
    j++;
	}
	name[j] = '\0';
	int len = 0;
	len = strlen(name);
	while(len < 29){
	name[len] = ' ';
	len = len + 1;
	}
    name[len] = '\0';
	string insert_student = "INSERT INTO STUDENT (ID,NAME) VALUES ('" + id + "', '" + name + "');";
    const char *line = insert_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "\nSQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    cout << "\nUnable to register requested details of student... Try Again with valid ID..." << endl;
    cout << endl;
    goto b;
    }
	}
    sqlite3_close(db);
	cout << "\nDetails of " << n << " student(s) registered successfully..." << endl;
	cout << endl;
	system("PAUSE");
	system("CLS");
	return;
}

int update_student_function(string &old_id){
    int rc = 1;
    sqlite3 *db;
	char *zErrMsg, *sql;
	sqlite3_open("SAMS.db", &db);
    cout << "\n\nID    NAME" << endl;
    sql = strdup("SELECT * from STUDENT ORDER BY ID");
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    cout << "\n\nEnter the ID to be updated : ";
    cin >> old_id;
    string search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE ID = '"+ old_id +"');";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    sqlite3_close(db);
	if(rc == 0){
    cout << "\nStudent with requested ID doesn't exist..." << endl;
    cout << "\nUnable to update requested details of student... Try Again using valid ID...\n" << endl;
    system("PAUSE");
    system("CLS");
    return 0;
    }
    else
    return 1;
}

void admin :: update_student(){
    sqlite3 *db;
    string old_id;
    int option = 0, rc;
	char *zErrMsg, *sql;
    while(option !=3){
	a: cout << "\nSelect the field to be updated...\n\n\n";
	cout << "Type '1' ----> Update ID\n";
	cout << "Type '2' ----> Update Name\n";
	cout << "Type '3' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: cout << "\nChoose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	system("PAUSE");
	system("CLS");
	goto a;
	}
	option = stoi(excp);
	if(option>3 || option<1){
    goto b;
	}
	else if(option == 1){
	int rc;
	system("CLS");
	string new_id;
    rc = update_student_function(old_id);
	if(rc == 1){
    sqlite3_open("SAMS.db", &db);
    cout << "Enter the new ID : ";
    cin >> new_id;
	string update_student = "UPDATE STUDENT set ID = '"+ new_id +"' WHERE ID = '"+ old_id +"';";
    const char *line = update_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "\nSQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    cout << "\nUnable to update requested details of student... Try Again with valid ID...\n" << endl;
    system("PAUSE");
    system("CLS");
    goto a;
    }
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    system("PAUSE");
    system("CLS");
    return;
	}
	}
	else if(option == 2){
	int rc;
	system("CLS");
	char ch, name[30];
	rc = update_student_function(old_id);
	if(rc == 1){
    sqlite3_open("SAMS.db", &db);
    cout << "Enter the New Name : ";
	cin >> ch;
	int j=0;
	while (ch != '\n' && j<30){
    name[j] = ch;
    ch = cin.get();
    j++;
	}
	name[j] = '\0';
	int len = 0;
	len = strlen(name);
	while(len < 29){
	name[len] = ' ';
	len = len + 1;
	}
    name[len] = '\0';
    string new_name = name;
	string update_student = "UPDATE STUDENT set NAME = '"+ new_name +"' WHERE ID = '"+ old_id +"';";
    const char *line = update_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    system("PAUSE");
    system("CLS");
    return;
	}
	}
	if(option == 3){
	cout << "Redirecting back...\n";
	system("PAUSE");
	system("CLS");
	return;
	}
}
}

void admin :: delete_student(){
	int rc = 1;
    string id;
    sqlite3 *db;
	char *zErrMsg, *sql;
    sqlite3_open("SAMS.db", &db);
    cout << "\n\nID    NAME" << endl;
    sql = strdup("SELECT * from STUDENT ORDER BY ID");
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	cout << "\n\nEnter the id to be deleted : ";
	cin >> id;
	string search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE ID = '"+ id +"');";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    sqlite3_close(db);
	if(rc == 0){
    cout << "\nStudent with requested ID doesn't exist..." << endl;
    cout << "\nUnable to delete requested details of student... Try Again using valid ID...\n" << endl;
    system("PAUSE");
    system("CLS");
    return;
    }
	a : cout << "\n\nAre you sure to delete the record of student with id " + id + " ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: sqlite3_open("SAMS.db", &db);
	cout << "\nChoose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	system("PAUSE");
	system("CLS");
    cout << "\n\nID    NAME" << endl;
    sql = strdup("SELECT * from STUDENT ORDER BY ID");
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    cout << "\n\nEnter the id to be deleted : " << id << endl;
	goto a;
	}
    sqlite3_close(db);
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
	goto b;
	}
	if(choice == 1){
    sqlite3_open("SAMS.db", &db);
    string delete_student = "DELETE from STUDENT where ID = '"+ id +"';";
    const char *line = delete_student.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested details deleted successfully..." << endl;
    cout << endl;
    system("PAUSE");
    system("CLS");
    return;
	}
	if(choice == 2){
    cout << "\nRequested details retained successfully..." << endl;
    cout << endl;
    system("PAUSE");
    system("CLS");
	return;
	}
}

void admin :: view_student(){
   int rc;
   sqlite3 *db;
   char *zErrMsg = 0, *sql;
   cout << "\nDisplaying Details of student(s)..." << endl;
   cout << "\n\nID    NAME" << endl;
   rc = sqlite3_open("SAMS.db", &db);
   sql = strdup("SELECT * from STUDENT ORDER BY ID");
   rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
   sqlite3_close(db);
   cout << "\nDetails of all student(s) displayed successfully..." << endl;
   cout << endl;
   system("PAUSE");
   system("CLS");
   return;
}

void admin :: master_student_menu(admin &a){
	int option = 0;
	while(option !=5){
	a: cout << "\nAccessing Student Operations...\n\n\n";
	cout << "Type '1' ----> Add Student\n";
	cout << "Type '2' ----> Update Student\n";
	cout << "Type '3' ----> Delete Student\n";
	cout << "Type '4' ----> View Student\n";
	cout << "Type '5' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
	    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : cout << "\nChoose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	system("PAUSE");
	system("CLS");
	goto a;
	}
	option = stoi(excp);
	if(option>5 || option<1){
	goto b;
	}
	else if(option == 1){
	system("CLS");
	a.add_student();
	}
	else if(option == 2){
	system("CLS");
	a.update_student();
	}
	else if(option == 3){
	system("CLS");
	a.delete_student();
	}
	else if(option == 4){
	system("CLS");
	a.view_student();
	}
	}
	if(option == 5){
	cout << "Redirecting back to Admin Page\n";
	system("PAUSE");
	system("CLS");
	return;
	}
}

void admin :: master_main_menu(admin &a){
	int option = 0;
	while(option !=3){
	a: cout << "\nWelcome Admin\n\n\n";
	cout << "Type '1' ----> Manage Faculty Data\n";
	cout << "Type '2' ----> Manage Student Data\n";
	cout << "Type '3' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : cout << "\nChoose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	system("PAUSE");
	system("CLS");
	goto a;
	}
	option = stoi(excp);
	if(option>3 || option<1){
	goto b;
	}
	else if(option == 1){
	system("CLS");
	a.master_faculty_menu(a);
	}
	else if(option == 2){
	system("CLS");
	a.master_student_menu(a);
	}
	}
	if(option == 3){
	cout << "Redirecting back to Main Menu\n";
	system("PAUSE");
	system("CLS");
	return;
	}
}

void main_menu(){
	int option = 0;
	while(option !=4){
	a: cout << "\nWelcome to Student Grade Management System\n\n\n";
	cout << "Type '1' ----> Admin Login\n";
	cout << "Type '2' ----> Faculty Login\n";
	cout << "Type '3' ----> Student Login\n";
	cout << "Type '4' ----> To Exit\n";
	cout << "\nEnter Here : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : cout << "\nChoose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	system("PAUSE");
	system("CLS");
	goto a;
	}
	option = stoi(excp);
	if(option>4 || option<1){
	goto b;
	}
	else if(option == 1){
	cout << "Redirecting to Admin Login Page\n";
	system("PAUSE");
	system("CLS");
	admin a;
	a.login_master();
	a.master_main_menu(a);
	}
	else if(option == 2){
	cout << "Redirecting to Faculty Login Page\n";
	system("PAUSE");
	system("CLS");
	course_teacher t;
	// t.login_teacher(t);
	}
	else if(option == 3){
	cout << "Redirecting to Student Login Page\n";
	system("PAUSE");
	system("CLS");
	section s;
	// s.login_student();
	// s.student_main_menu(s);
	}
	}
	if(option == 4){
	exit(0);
	}
}

void table_creation(sqlite3 *db){
   int rc;
   char *zErrMsg, *sql;
   sql = strdup("PRAGMA FOREIGNKEYS = ON;");
   rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
   sqlite3_free(zErrMsg);
   }
   sql = strdup("CREATE TABLE FACULTY(FACULTYID VARCHAR2(6) PRIMARY KEY, FACULTYNAME  VARCHAR2 (40) NOT NULL, QUALIFICATION VARCHAR2 (30), DESIGNATION VARCHAR2 (40), RESEARCHAREA VARCHAR2 (60), DEPTNO VARCHAR2 (3) NOT NULL REFERENCES BRANCH (BRANCHID));");
   rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
   sqlite3_free(zErrMsg);
   }
   sql = strdup("CREATE TABLE STUDENT(ID VARCHAR2 (8) PRIMARY KEY, NAME  VARCHAR2 (20) NOT NULL);");
   rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
   sqlite3_free(zErrMsg);
   }
   sql = strdup("CREATE TABLE BRANCH(BRANCHID VARCHAR2 (3) PRIMARY KEY, BRANCHNAME  VARCHAR2 (40) NOT NULL);");
   rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
   sqlite3_free(zErrMsg);
   }
   sql = strdup(
   		 "INSERT INTO BRANCH VALUES ('CS', 'Computer Science & Engineering'); \
          INSERT INTO BRANCH VALUES ('AI', 'Computer Science & Engineering (Artificial Intelligence & Machine Learning)'); \
          INSERT INTO BRANCH VALUES ('CN', 'Computer Science & Engineering (Networks)'); \
          INSERT INTO BRANCH VALUES ('IN', 'Computer Science & Engineering (Internet Of Things)'); \
          INSERT INTO BRANCH VALUES ('IT', 'Information Technology'); \
          INSERT INTO BRANCH VALUES ('EC', 'Electronics & Communication Engineering'); \
          INSERT INTO BRANCH VALUES ('EE', 'Electrical & Electronics Engineering'); \
          INSERT INTO BRANCH VALUES ('CI', 'Electronics Communication & Instrumentation'); \
          INSERT INTO BRANCH VALUES ('ME', 'Mechanical Engineering'); \
          INSERT INTO BRANCH VALUES ('CE', 'Civil Engineering'); \
          INSERT INTO BRANCH VALUES ('MH', 'Mathematics'); \
          INSERT INTO BRANCH VALUES ('EN', 'English');");
   rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
   sqlite3_free(zErrMsg);
   }
}

void database_creation(){
   sqlite3 *db;
   sqlite3_open("SAMS.db", &db);
   table_creation(db);
   sqlite3_close(db);
}

int main(){
    database_creation();
	main_menu();
}
