/*********************************************************************************************************************
    Welcome to SQLITE3 BASED C++ PROJECT ON COLLEGE MANAGEMENT SYSTEM MODULE 1.0 (STUDENT GRADE MANAGEMENT SYSTEM)
**********************************************************************************************************************

    Project Meta Data:
    TOTAL NO. OF LINES : 4275
    TOTAL NO. OF EMPTY LINES : 100
    TOTAL NO. OF COMMENT LINES : 50
    TOTAL NO. OF ACTUAL CODE LINES : 4125
    TOTAL NO. OF HEADER FILES INCLUDED : 5
    TOTAL NO. OF CLASSES USED : 5
    TOTAL NO. OF ADMIN CLASS FUNCTIONS : 29
	TOTAL NO. OF CLASS TEACHER CLASS FUNCTIONS : 8
	TOTAL NO. OF COURSE TEACHER CLASS FUNCTIONS : 7
	TOTAL NO. OF STUDENT CLASS FUNCTIONS : 5
	TOTAL NO. OF CLASS FUNCTIONS : 49
    TOTAL NO. OF GLOBAL VARIABLES : 13
    TOTAL NO. OF GLOBAL FUNCTIONS : 36
    C++ (FRONT-END) OFFLINE IDE USED : CODE::BLOCKS v20.30
    RDBMS (BACK-END) OFFLINE LIBRARY USED :  SQLITE v3.38.0
    TOTAL SIZE OF "main.cpp" SOURCE CODE : 169KB
    TOTAL SIZE OF "SAMS.db" DATABASE FILE (at creation) : 52KB

*********************************************************************************************************************
*/

//header files
#include<ctime>
#include<iostream>
#include<string.h>
#include<algorithm>
#include<sqlite3.h>
using namespace std;

//global variables
bool roc = false;
int gbl_data = 0;
string excp = "";
int course_count = 0;
string gbl_info[100];
string course_id[10];
int student_count = 0;
int course_credits[10];
string course_type[10];
string gbl_password = "1234";
char semester[8][8] = {"FIRST","SECOND","THIRD","FOURTH","FIFTH","SIXTH","SEVENTH","EIGHTH"};
char dept_no[14][4] = {"CS","AI","CN","IN","IT","EC","EE","CI","CE","ME","MH","PS","EN","PE"};
char dept[14][4] = {"CSE","CSM","CSN","CSO","IT","ECE","EEE","ECI","CE","ME","EMH","SCI","ENG","PED"};

//classes
class section{
    protected:
	int gradepoint[10]; char grade[10];
	string roll_number[9], awarded[10];
	float m1[10], a1[10], ms1[10], m2[10], a2[10], ms2[10], internal_marks[10], external_marks[10];
	float resultm1[10], resulta1[10], resultms1[10], resultm2[10], resulta2[10], resultms2[10], finalmse[10], final_marks[10], final_gradepoint[10];
};

class class_teacher : public section{
	protected:
	string teacher_id, teacher_password;
	public:
	void read(section &s, string student_id, string section_id);
	void calculate_student_marks(section &s);
    void calculate_student_sgpa(section &s, int index);
    void write(section &s, int index, string student_id);
	void view_subject_wise_marks(string section_id);
	void view_overall_marks(string section_id);
	void generate_sgpa(section* s, class_teacher &ct, string section_id);
	void class_teacher_main_menu(class_teacher &ct, string clstchr_id, string section_id);
};

class student : public class_teacher{
    string student_id, student_password, subject;
	public:
	int login_student();
	void student_main_menu(student &s);
	void view_marks(string student_id);
	void view_account(string student_id);
	void update_stdpass(string student_id);
};

class course_teacher : public class_teacher{
	protected:
	string teacher_id, teacher_password, subject;
	public:
	int login_teacher(bool &cls, string &clstchr_id, string &section_id);
	void teacher_main_menu(course_teacher &t, string clstchr_id);
	void add_student_marks(string faculty_id);
	void update_student_marks(string faculty_id);
	void view_overall_marks(string faculty_id);
	void view_account(string faculty_id);
	void update_tchrpass(string faculty_id);
};

class admin{
	string id, password;
	public:
	void login_master();
	void add_faculty();
	void update_faculty();
	void delete_faculty();
	void view_faculty();
    void master_faculty_edit_menu(admin &a);
    void assign_course();
    void remove_course();
    void view_course();
    void master_assign_course_menu(admin &a);
	void assign_clstchr();
    void remove_clstchr();
    void view_clstchr();
    void master_assign_clstchr_menu(admin &a);
    void assign_head();
    void remove_head();
    void view_head();
    void master_assign_head_menu(admin &a);
    void master_faculty_assign_menu(admin &a);
	void master_faculty_menu(admin &a);
	void add_student();
	void update_student();
	void delete_student();
	void view_student();
    void master_student_menu(admin &a);
    void view_archived_faculty();
    void view_archived_student();
    void master_archives_menu(admin &a);
	void master_main_menu(admin &a);
};

//global functions

//CATEGORY 1: USED FOR FORMATTING OR EXTRACTING VALID INPUT
void clear_screen(){
	system("PAUSE");
	system("CLS");
}

void error_message(){
	cout << "\nChoose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	clear_screen();
	return;
}

bool check_exception(string exp){
	int len = exp.size();
	for(int i = 0; i < len; i++){
	int c = exp[i];
	if(c<48 || c>57)
	return true;
	}
	return false;
}

void padded_input_string(char str[], int num){
	int len = 0;
	len = strlen(str);
	while(len < num){
	str[len] = ' ';
	len = len + 1;
	}
    str[len] = '\0';
}

//CATEGORY 2: USED FOR TABLE RELATED QUERY
static int create_insert_table(void *NotUsed, int argc, char **argv, char **azColName) {
    for(int i = 0; i<argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

static int select_table(void *data, int argc, char **argv, char **azColName){
    for(int i = 0; i<argc; i++){
    printf("%s ", argv[i] ? argv[i] : "");
    }
    printf("\n");
    return 0;
}

static int search_table(void *NotUsed, int argc, char **argv, char **azColName){
    gbl_data = 0;
    string data = argv[0] ? argv[0] : "NULL";
    gbl_data = stoi(data);
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

void set_foreignkeys(sqlite3 *db){
    int rc;
    char *sql;
    sql = strdup("PRAGMA FOREIGN_KEYS = ON;");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
}

//CATEGORY 3: USED FOR SPECIFIC DATA EXTRACTION
static int view_course_details(void *NotUsed, int argc, char **argv, char **azColName){
	string gbl_input[100];
	for(int i = 0; i<argc; i++){
    gbl_input[i] = argv[i] ? argv[i] : "";
    }
    int len = gbl_input[0].length();
    char data[len+1];
    strcpy(data,gbl_input[0].c_str());
    padded_input_string(data,20);
    gbl_input[0] = data;
    for(int i=0; i<argc; i++){
    if(i%6==0){
    string data = gbl_input[i];
    if(data[8] == ' ')
    cout << " " << gbl_input[i] << gbl_input[i+1] << "\t\t " << gbl_input[i+2] << gbl_input[i+3] << gbl_input[i+4] << gbl_input[i+5] << endl;
    else
    cout << gbl_input[i] << " " << gbl_input[i+1] << "\t\t " << gbl_input[i+2] << gbl_input[i+3] << gbl_input[i+4] << gbl_input[i+5] << endl;
    }
    }
    return 0;
}

static int view_hod_details(void *NotUsed, int argc, char **argv, char **azColName){
	string gbl_input[100];
	for(int i = 0; i<argc; i++){
    gbl_input[i] = argv[i] ? argv[i] : "NULL";
    }
    cout << "\nFaculty with id " << gbl_input[0] << " is already assigned as HEAD of ";
    return 0;
}

static int view_clstchr_details(void *NotUsed, int argc, char **argv, char **azColName){
	string gbl_input[100];
	for(int i = 0; i<argc; i++){
    gbl_input[i] = argv[i] ? argv[i] : "NULL";
    }
    cout << "\nFaculty with id " << gbl_input[0] << " is already assigned as CLASSTEACHER of" << gbl_input[1];
    return 0;
}

static int view_theoretical_ungraded_details(void *NotUsed, int argc, char **argv, char **azColName){
	string gbl_input[100];
	for(int i = 0; i<argc; i++){
    gbl_input[i] = (argv[i] ? argv[i] : "");
    }
    for(int i=0; i<argc; i++){
    if(i%3==0)
    cout << gbl_input[i] << "        " << gbl_input[i+1] << gbl_input[i+2] << "\t\tNA\tNA\t NA\tNA\tNA\t NA\tNA\tNA" << endl;
    }
    return 0;
}

static int view_practical_ungraded_details(void *NotUsed, int argc, char **argv, char **azColName){
	string gbl_input[100];
	for(int i = 0; i<argc; i++){
    gbl_input[i] = (argv[i] ? argv[i] : "");
    }
    for(int i=0; i<argc; i++){
    if(i%3==0)
    cout << gbl_input[i] << "        " << gbl_input[i+1] << gbl_input[i+2] << "\t\tNA\tNA" << endl;
    }
    return 0;
}

static int view_graded_details(void *NotUsed, int argc, char **argv, char **azColName){
	string gbl_input[100];
	for(int i = 0; i<argc; i++){
    gbl_input[i] = (argv[i] ? argv[i] : "");
    }
    for(int i=0; i<argc; i++){
    if(i%11==0)
    cout << gbl_input[i] << "        " << gbl_input[i+1] << gbl_input[i+2] << "\t\t" << gbl_input[i+3] << "\t" << gbl_input[i+4] << "\t " << gbl_input[i+5] << "\t" << gbl_input[i+6] << "\t" << gbl_input[i+7] << "\t " << gbl_input[i+8] << "\t" << gbl_input[i+9] << "\t" << gbl_input[i+10] << endl;
    }
    return 0;
}

static int view_faculty_account(void *NotUsed, int argc, char **argv, char **azColName){
	string gbl_input[100];
	for(int i = 0; i<argc; i++){
    gbl_input[i] = argv[i] ? argv[i] : "NULL";
    }
    cout << "Name : " << gbl_input[0] << endl;
    cout << "Qualification : " << gbl_input[1] << endl;
    cout << "Designation : " << gbl_input[2] << endl;
    cout << "Research Area : " << gbl_input[3] << endl;
    return 0;
}

static int view_student_account(void *NotUsed, int argc, char **argv, char **azColName){
	string gbl_input[100];
	for(int i = 0; i<argc; i++){
    gbl_input[i] = argv[i] ? argv[i] : "NULL";
    }
    cout << "Name : " << gbl_input[0] << endl;
    cout << "Semester : " << gbl_input[1] << endl;
    cout << "Section :" << gbl_input[2] << endl;
    cout << "Contact : " << gbl_input[3] << endl;
    return 0;
}

static int extract_details(void *NotUsed, int argc, char **argv, char **azColName){
	for(int i = 0; i<argc; i++){
    gbl_info[i] = argv[i] ? argv[i] : "NULL";
    }
    return 0;
}

//CATEGORY 4: USED FOR DISPLAYING DATA
void faculty_select_branch_function(){
	cout << "\nSelect Branch \n\n";
	cout << "Type '1' -----> CSE\n";
	cout << "Type '2' -----> CSM\n";
	cout << "Type '3' -----> CSN\n";
	cout << "Type '4' -----> CSO\n";
	cout << "Type '5' ----->  IT\n";
	cout << "Type '6' -----> ECE\n";
	cout << "Type '7' -----> EEE\n";
	cout << "Type '8' -----> ECI\n";
	cout << "Type '9' -----> CIV\n";
	cout << "Type '10' ---->  ME\n";
	cout << "Type '11' ----> EMH\n";
	cout << "Type '12' ----> SCI\n";
    cout << "Type '13' ----> ENG\n";
    cout << "Type '14' ----> PED\n";
    cout << "\nEnter Here : ";
}

void student_select_branch_function(){
	cout << "\nSelect Branch \n\n";
	cout << "Type '1' -----> CSE\n";
	cout << "Type '2' -----> CSM\n";
	cout << "Type '3' -----> CSN\n";
	cout << "Type '4' -----> CSO\n";
	cout << "Type '5' ----->  IT\n";
	cout << "Type '6' -----> ECE\n";
	cout << "Type '7' -----> EEE\n";
	cout << "Type '8' -----> ECI\n";
	cout << "Type '9' -----> CIV\n";
	cout << "Type '10' ---->  ME\n";
	cout << "\nEnter Here : ";
}

void select_branch_insert_function(int i, int n, string str1, string str2, int &dept_id){
	a: if(str2 == "faculty") faculty_select_branch_function();
	else student_select_branch_function();
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	cout << "Enter the number of " << str1 << " : " << n << endl;
	cout << "\nEntering Details of " << n << " " << str1 << "..." << endl;
	cout << "\nEntering Details of " << str2 << " " << i+1 << endl;
	goto a;
	}
	dept_id = stoi(excp);
    if((str2 == "faculty") ? (dept_id>14 || dept_id<1) : (dept_id>10 || dept_id<1)){
    goto b;
	}
}

void select_branch_view_function(int &dept_id, string str){
	a: if(str == "faculty") faculty_select_branch_function();
	else student_select_branch_function();
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	goto a;
	}
	dept_id = stoi(excp);
    if((str == "faculty") ? (dept_id>14 || dept_id<1) : (dept_id>10 || dept_id<1)){
    goto b;
	}
}

void view_faculty_function(string faculty_deptno, bool archived){
    int rc = 0;
    sqlite3 *db;
	string str = "", search_faculty;
	char *zErrMsg, *sql;
	sqlite3_open("SAMS.db", &db);
	if(archived == false)
    search_faculty = "SELECT COUNT(*) FROM FACULTY WHERE DEPTNO = '" + faculty_deptno + "' AND WORKING = 'Y';";
    else
    search_faculty = "SELECT COUNT(*) FROM FACULTY WHERE DEPTNO = '" + faculty_deptno + "' AND WORKING = 'N';";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, search_table, 0, &zErrMsg);
    sqlite3_close(db);
}

void view_student_function(string student_deptno, bool archived){
    int rc = 0;
    sqlite3 *db;
	string str = "", search_student;
	char *zErrMsg, *sql;
	sqlite3_open("SAMS.db", &db);
	if(archived == false)
    search_student = "SELECT COUNT(*) FROM STUDENT WHERE DEPTNO = '" + student_deptno + "' AND STUDYING = 'Y';";
    else
    search_student = "SELECT COUNT(*) FROM STUDENT WHERE DEPTNO = '" + student_deptno + "' AND STUDYING = 'N';";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, search_table, 0, &zErrMsg);
    sqlite3_close(db);
}

void view_table(string str, string id, string id2){
	int rc = 0;
    sqlite3 *db;
	string view;
	char *zErrMsg, *sql;
	rc = sqlite3_open("SAMS.db", &db);
	if(str == "HOD")
    view = "SELECT B.BRANCHCODE,B.HOD,F.FACULTYNAME,F.QUALIFICATION,F.DESIGNATION,F.RESEARCHAREA FROM BRANCH B LEFT JOIN FACULTY F ON B.HOD = F.FACULTYID;";
	else if(str == "CLSTCHR")
    view = "SELECT S.SECTIONID,S.CLSTCHR,F.FACULTYNAME,F.QUALIFICATION,F.DESIGNATION,F.RESEARCHAREA FROM SECTION S LEFT JOIN FACULTY F ON S.CLSTCHR = F.FACULTYID WHERE SECTIONID LIKE '" + ("  " + id + "%") + "';";
	else if(str == "FACULTY")
    view = "SELECT FACULTYID,FACULTYNAME,QUALIFICATION,DESIGNATION,RESEARCHAREA from FACULTY WHERE DEPTNO = '" + id + "' AND WORKING = 'Y' ORDER BY " + (str + "ID");
    else if(str == "STUDENT")
    view = "SELECT STUDENTID,STUDENTNAME,YEARJOINED,SEMESTER,SECTION,CONTACTNUMBER from STUDENT WHERE DEPTNO = '" + id + "' AND STUDYING = 'Y' ORDER BY " + (str + "ID");
	else if(str == "A-FACULTY"){
    str = str.substr(2);
    view = "SELECT FACULTYID,FACULTYNAME,QUALIFICATION,DESIGNATION,RESEARCHAREA from FACULTY WHERE DEPTNO = '" + id + "' AND WORKING = 'N' ORDER BY " + (str + "ID");
	}
    else if(str == "A-STUDENT"){
    str = str.substr(2);
    view = "SELECT STUDENTID,STUDENTNAME,YEARJOINED,SEMESTER,SECTION,CONTACTNUMBER from STUDENT WHERE DEPTNO = '" + id + "' AND STUDYING = 'N' ORDER BY " + (str + "ID");
    }
    else if(str == "COURSE")
    view = "SELECT SECTION, COURSECODE from COURSE WHERE CRSTCHR = '"+ id +"';";
    const char *line = view.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
}

//class functions

//ADMIN CLASS FUNCTIONS
void admin :: login_master(){
	int count = 3;
	a: cout << "WELCOME TO ADMIN LOGIN PAGE\n";
	cout << "\nEnter Master ID : ";
	cin >> id;
	cout << "Enter Password : ";
	cin >> password;
	if(id == "196612" && password == gbl_password)
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
	clear_screen();
	goto a;
	}
	clear_screen();
	return;
}

//CATEGORY 1: FACULTY EDIT OPERATION FUNCTIONS

//global function
void add_faculty_function(string &faculty_id, string faculty_deptno){
    int rc = 0;
    sqlite3 *db;
	string str = "";
	char *zErrMsg, *sql;
	sqlite3_open("SAMS.db", &db);
    string search_faculty = "SELECT COUNT(*) FROM FACULTY WHERE DEPTNO = '" + faculty_deptno + "';";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, search_table, 0, &zErrMsg);
    sqlite3_close(db);
	if(gbl_data == 0){
    faculty_id = faculty_id + "001";
    system("CLS");
    }
    else{
    gbl_data++;
    if(gbl_data<10)
    str = "00" + to_string(gbl_data);
    else if(gbl_data<100)
    str = "0" + to_string(gbl_data);
    else if(gbl_data<1000)
    str = to_string(gbl_data);
    faculty_id = faculty_id + str;
    system("CLS");
    }
}

void admin :: add_faculty(){
    sqlite3 *db;
    sqlite3_open("SAMS.db", &db);
    int rc, n, dept_id;
	char *zErrMsg, *sql, ch, name[40], qualification[30], designation[40], research_area[60];
	a: cout << "Enter the number of faculty(ies) : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	error_message();
	goto a;
	}
	n = stoi(excp);
	cout << "\nEntering Details of " << n << " faculty(ies)..." << endl;
	for(int i = 0; i < n; i++){
	cout << "\nEntering Details of faculty " << i+1 << endl;
	b: select_branch_insert_function(i, n, "faculty(ies)", "faculty", dept_id);
	int id = dept_id - 1;
    string faculty_id = dept_no[id];
    string faculty_deptno = dept_no[id];
    add_faculty_function(faculty_id, faculty_deptno);
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
	padded_input_string(name, 39);
	cout << "Enter Faculty Qualification : ";
	cin >> ch;
	j=0;
	while (ch != '\n' && j<30){
    qualification[j] = ch;
    ch = cin.get();
    j++;
	}
	qualification[j] = '\0';
    padded_input_string(qualification, 29);
	cout << "Enter Faculty Designation : ";
	cin >> ch;
	j=0;
	while (ch != '\n' && j<40){
    designation[j] = ch;
    ch = cin.get();
    j++;
	}
	designation[j] = '\0';
	padded_input_string(designation, 39);
	cout << "Enter Faculty Research Area : ";
	cin >> ch;
    j=0;
	while (ch != '\n' && j<60){
    research_area[j] = ch;
    ch = cin.get();
    j++;
	}
	research_area[j] = '\0';
	padded_input_string(research_area, 59);
    string faculty_name = name;
    string faculty_qualification = qualification;
    string faculty_designation = designation;
    string faculty_researcharea = research_area;
    set_foreignkeys(db);
	string insert_faculty = "INSERT INTO FACULTY VALUES ('" + faculty_id + "', '" + faculty_name + "', '" + faculty_qualification + "', '" + faculty_designation + "', '" + faculty_researcharea + "', 'Y', '" + faculty_deptno + "', '" + gbl_password + "');";
    const char *line = insert_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "\nSQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    cout << "\nUnable to register requested details of faculty... Try Again with valid ID...\n" << endl;
    clear_screen();
	cout << "Enter the number of faculty(ies) : " << n << endl;
	cout << "\nEntering Details of " << n << " faculty(ies)..." << endl;
    goto b;
    }
    if(i == n-1)
    cout << "\nDetails of faculty " << i+1 << " registered successfully..." << endl;
    else{
    cout << "\nDetails of faculty " << i+1 << " registered successfully... \n" << endl;
    clear_screen();
	cout << "Enter the number of faculty(ies) : " << n << endl;
	cout << "\nEntering Details of " << n << " faculty(ies)..." << endl;
    }
	}
    sqlite3_close(db);
    cout << "\nDetails of " << n << " faculty(ies) registered successfully..." << endl;
    cout << endl;
	clear_screen();
	return;
}

//global function
int update_faculty_function(string &faculty_id, int id, string faculty_deptno){
    int rc = 1;
    sqlite3 *db;
	char *zErrMsg, *sql;
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno, "");
	rc = sqlite3_open("SAMS.db", &db);
    cout << "\n\nEnter the ID to be updated : ";
    cin >> faculty_id;
    string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ faculty_id +"' AND DEPTNO = '"+ faculty_deptno +"' AND WORKING = 'Y');";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    sqlite3_close(db);
	if(rc == 0){
    cout << "\nFaculty with requested ID doesn't exist..." << endl;
    cout << "\nUnable to update requested details of faculty... Try Again using valid ID...\n" << endl;
    clear_screen();
    return 0;
    }
    else
    return 1;
}

void admin :: update_faculty(){
    sqlite3 *db;
    string faculty_id;
	char *zErrMsg, *sql;
    int option = 0, rc = 0, dept_id = 0;
    while(option !=5){
	select_branch_view_function(dept_id, "faculty");
	int id = dept_id - 1;
	string faculty_deptno = dept_no[id];
    system("CLS");
	view_faculty_function(faculty_deptno, false);
	if(gbl_data == 0){
    cout << "\nERROR: No Faculty Details Found...." << endl;
	cout << endl;
	clear_screen();
	return;
	}
	a: cout << "Selected Branch : " << dept[id] << endl;
    cout << "\nSelect the field to be updated...\n\n\n";
	cout << "Type '1' ----> Update Name\n";
	cout << "Type '2' ----> Update Qualification\n";
	cout << "Type '3' ----> Update Designation\n";
	cout << "Type '4' ----> Update Research Area\n";
	cout << "Type '5' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>5 || option<1){
    goto b;
	}
	else if(option == 1){
    system("CLS");
	char ch, name[40];
    rc = update_faculty_function(faculty_id, id, faculty_deptno);
	if(rc == 0)
    return;
	else if(rc == 1){
    sqlite3_open("SAMS.db", &db);
    cout << "Enter the New Name : ";
	cin >> ch;
	int j=0;
	while (ch != '\n' && j<40){
    name[j] = ch;
    ch = cin.get();
    j++;
	}
	name[j] = '\0';
	padded_input_string(name, 39);
    string new_name = name;
	string update_faculty = "UPDATE FACULTY set FACULTYNAME = '"+ new_name +"' WHERE FACULTYID = '"+ faculty_id +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
	}
	else if(option == 2){
    system("CLS");
	char ch, qualification[30];
    rc = update_faculty_function(faculty_id, id, faculty_deptno);
	if(rc == 0)
    return;
	else if(rc == 1){
    sqlite3_open("SAMS.db", &db);
    cout << "Enter the new Qualification : ";
    cin >> ch;
	int j=0;
	while (ch != '\n' && j<30){
    qualification[j] = ch;
    ch = cin.get();
    j++;
	}
	qualification[j] = '\0';
	padded_input_string(qualification, 29);
    string new_qualification = qualification;
	string update_faculty = "UPDATE FACULTY set QUALIFICATION = '"+ new_qualification +"' WHERE FACULTYID = '"+ faculty_id +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
    }
	else if(option == 3){
    system("CLS");
	char designation[40], ch;
    rc = update_faculty_function(faculty_id, id, faculty_deptno);
	if(rc == 0)
    return;
	else if(rc == 1){
    sqlite3_open("SAMS.db", &db);
    cout << "Enter the new Designation : ";
    cin >> ch;
	int j=0;
	while (ch != '\n' && j<40){
    designation[j] = ch;
    ch = cin.get();
    j++;
	}
	designation[j] = '\0';
	padded_input_string(designation, 39);
    string new_designation = designation;
	string update_faculty = "UPDATE FACULTY set DESIGNATION = '"+ new_designation +"' WHERE FACULTYID = '"+ faculty_id +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
	}
    else if(option == 4){
    system("CLS");
	char research_area[60], ch;
    rc = update_faculty_function(faculty_id, id, faculty_deptno);
	if(rc == 0)
    return;
	else if(rc == 1){
    sqlite3_open("SAMS.db", &db);
    cout << "Enter the new Research Area : ";
    cin >> ch;
	int j=0;
	while (ch != '\n' && j<60){
    research_area[j] = ch;
    ch = cin.get();
    j++;
	}
	research_area[j] = '\0';
	padded_input_string(research_area, 59);
    string new_research_area = research_area;
	string update_faculty = "UPDATE FACULTY set RESEARCHAREA = '"+ new_research_area +"' WHERE FACULTYID = '"+ faculty_id +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
	}
	if(option == 5){
	cout << "Redirecting back...\n";
	clear_screen();
	return;
	}
}
}

void admin :: delete_faculty(){
    sqlite3 *db;
    string faculty_id;
	char *zErrMsg, *sql;
    int rc = 0, dept_id = 0;
    select_branch_view_function(dept_id, "faculty");
	int id = dept_id - 1;
	string faculty_deptno = dept_no[id];
    system("CLS");
	view_faculty_function(faculty_deptno, false);
	if(gbl_data == 0){
    cout << "\nERROR: No Faculty Details Found...." << endl;
	cout << endl;
	clear_screen();
	return;
	}
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno, "");
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be deleted : ";
	cin >> faculty_id;
	string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ faculty_id +"' AND DEPTNO = '"+ faculty_deptno +"'  AND WORKING = 'Y');";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    sqlite3_close(db);
    if(rc == 0){
    cout << "\nFaculty with requested ID doesn't exist..." << endl;
    cout << "\nUnable to delete requested details of faculty... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
	a: cout << "\n\nAre you sure to delete the record of faculty with id " + faculty_id + " ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
    b: error_message();
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno, "");
	rc = sqlite3_open("SAMS.db", &db);
    cout << "\n\nEnter the id to be deleted : " << faculty_id << endl;
	goto a;
	}
    sqlite3_close(db);
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto b;
	}
	if(choice == 1){
    sqlite3_open("SAMS.db", &db);
    string delete_faculty = "UPDATE FACULTY set WORKING = 'N' where FACULTYID = '"+ faculty_id + "' AND DEPTNO = '" + faculty_deptno + "';";
    const char *line = delete_faculty.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	string update_course = "UPDATE COURSE set CRSTCHR = NULL WHERE CRSTCHR = '"+ faculty_id +"';";
    line = update_course.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	string update_hod = "UPDATE BRANCH set HOD = NULL WHERE HOD = '"+ faculty_id +"';";
    line = update_hod.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	string update_clstchr = "UPDATE SECTION set CLSTCHR = NULL WHERE CLSTCHR = '"+ faculty_id +"';";
    line = update_clstchr.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested details deleted successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
	if(choice == 2){
    cout << "\nRequested details retained successfully..." << endl;
    cout << endl;
    clear_screen();
	return;
	}
}

void admin :: view_faculty(){
    sqlite3 *db;
    int rc, dept_id;
    char *zErrMsg = 0, *sql;
	select_branch_view_function(dept_id, "faculty");
	int id = dept_id - 1;
	string faculty_deptno = dept_no[id];
	system("CLS");
	view_faculty_function(faculty_deptno, false);
	if(gbl_data == 0){
    cout << "\nERROR: No Faculty Details Found...." << endl;
	cout << endl;
	}
	else{
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno, "");
    cout << "\nDetails of all faculty(ies) of branch " << dept[id] << " displayed successfully..." << endl;
    cout << endl;
	}
    clear_screen();
    return;
}

void admin :: master_faculty_edit_menu(admin &a){
    int option = 0;
	while(option !=5){
	a: cout << "\nAccessing Faculty EDIT Operations...\n\n\n";
	cout << "Type '1' ----> Add Faculty\n";
	cout << "Type '2' ----> Update Faculty\n";
	cout << "Type '3' ----> Delete Faculty\n";
	cout << "Type '4' ----> View Faculty\n";
	cout << "Type '5' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>6 || option<1){
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
	cout << "Redirecting back...\n";
	clear_screen();
	return;
	}
}

//CATEGORY 2: FACULTY ASSIGN OPERATION FUNCTIONS
void admin :: assign_course(){
    sqlite3 *db;
    char *zErrMsg = 0, *sql;
    int rc, cls, dept_id, sem;
    string faculty_id, section_id, course_id;
    sqlite3_open("SAMS.db", &db);
    a: cout << "Registering Course Details..." << endl;
    cout << "\nEnter Semester : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	goto a;
	}
	sem = stoi(excp);
	if(sem>8 || sem<1){
	goto b;
	}
	cout << "\nEnter Section : ";
	cin >> section_id;
	section_id = "  " + to_string(sem) + section_id;
    string search_section = "SELECT EXISTS(SELECT * from SECTION WHERE SECTIONID = '"+ section_id +"');";
    const char *line = search_section.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nSection with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Section... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    string search_course = "SELECT EXISTS(SELECT * from COURSE WHERE SECTION = '"+ section_id +"');";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nSection with requested ID doesn't have any courses available..." << endl;
    cout << "\nUnable to access requested details of Section... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    cout << "\nDisplaying Details of Course(s) of Section" << section_id << endl;
    cout << "\nCOURSE_ID CODE\t    FACULTY\t\t NAME\t\t\t\t        QUALIFICATION\t\t     DESIGNATION\t\t\t    RESEARCH AREA" << endl;
    search_course = "SELECT CRS.COURSEID || \" \" || CRS.COURSECODE, CRS.CRSTCHR, F.FACULTYNAME, F.QUALIFICATION, F.DESIGNATION, F.RESEARCHAREA from COURSE CRS LEFT JOIN FACULTY F ON CRS.CRSTCHR = F.FACULTYID WHERE CRS.SECTION = '"+ section_id +"';";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_course_details, 0, &zErrMsg);
    cout << endl;
    cout << "Enter the Course Code to be allotted : ";
    cin >> course_id;
    search_course = "SELECT EXISTS(SELECT * from COURSE WHERE COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"');";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nCourse with requested Code doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Section... Try Again using valid Code...\n" << endl;
    clear_screen();
    return;
    }
    string search_clstchr = "SELECT EXISTS(SELECT * FROM SECTION WHERE CLSTCHR = (SELECT CRSTCHR from COURSE WHERE CRSTCHR IS NOT NULL AND COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"'));";
    line = search_clstchr.c_str();
    sql = strdup(line);
    cls = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    string search_faculty = "SELECT EXISTS(SELECT * from COURSE WHERE CRSTCHR IS NULL AND COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"');";
    line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    c: if((cls != 0) ? (cout << "\nThe Faculty allotted to the Course with requested Code is CLASS TEACHER..." << endl) : (cout << "\nCourse with requested Code is already allotted to a faculty..." << endl));
    cout << "\nAre you sure to make changes to " + course_id << " of Section" + section_id + " ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
    d: error_message();
    cout << "\nDisplaying Details of Course(s) of Section" << section_id << endl;
    cout << "\nCOURSE_ID CODE\t    FACULTY\t\t NAME\t\t\t\t        QUALIFICATION\t\t     DESIGNATION\t\t\t    RESEARCH AREA" << endl;
    search_course = "SELECT CRS.COURSEID || \" \" || CRS.COURSECODE, CRS.CRSTCHR, F.FACULTYNAME, F.QUALIFICATION, F.DESIGNATION, F.RESEARCHAREA from COURSE CRS LEFT JOIN FACULTY F ON CRS.CRSTCHR = F.FACULTYID WHERE CRS.SECTION = '"+ section_id +"';";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_course_details, 0, &zErrMsg);
    cout << endl;
    cout << "Enter the Course Code to be allotted : " << course_id << endl;
	goto c;
	}
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto d;
	}
    if(choice == 2){
    cout << "\nNo changes were made....\n" << endl;
    clear_screen();
    return;
	}
    }
    system("CLS");
    cout << "Registering Faculty Details...." << endl;
	select_branch_view_function(dept_id, "faculty");
	int id = dept_id - 1;
	string faculty_deptno = dept_no[id];
	string branch_id = dept[id];
	system("CLS");
	view_faculty_function(faculty_deptno, false);
	if(gbl_data == 0){
    cout << "\nERROR: No Faculty Details Found...." << endl;
	cout << endl;
	}
	else{
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno, "");
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be made as Faculty of " + course_id << " of Section" + section_id << " : ";
	cin >> faculty_id;
	string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ faculty_id +"' AND DEPTNO = '"+ faculty_deptno +"'  AND WORKING = 'Y');";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nFaculty with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of faculty... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    e: cout << "\n\nAre you sure to make the requested faculty with id " + faculty_id + " as COURSE TEACHER of " + course_id << " of Section" + section_id + " ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
    f: error_message();
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept_id << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno, "");
	cout << "\n\nEnter the id to be made as Faculty of " + course_id << " of Section" + section_id << " : " << faculty_id << endl;
	goto e;
	}
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto f;
	}
    if(choice == 1){
    sqlite3_open("SAMS.db", &db);
    string update_faculty = "UPDATE COURSE set CRSTCHR = '"+ faculty_id +"' WHERE COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"';";
    line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    if(cls != 0){
    update_faculty = "UPDATE SECTION set CLSTCHR = NULL WHERE SECTIONID = '"+ section_id +"';";
    line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    }
	sqlite3_close(db);
    cout << "\nRequested faculty made as Course Teacher successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
    }
    if(choice == 2){
    cout << "\nNo changes were made....\n" << endl;
    clear_screen();
    return;
	}
	}
	clear_screen();
	return;
}

void admin :: remove_course(){
    sqlite3 *db;
    int rc, sem;
    char *zErrMsg = 0, *sql;
    string faculty_id, section_id, course_id;
    sqlite3_open("SAMS.db", &db);
    a: cout << "Enter Course Details..." << endl;
    cout << "\nEnter Semester : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	goto a;
	}
	sem = stoi(excp);
	if(sem>8 || sem<1){
	goto b;
	}
	cout << "\nEnter Section : ";
	cin >> section_id;
	section_id = "  " + to_string(sem) + section_id;
    string search_section = "SELECT EXISTS(SELECT * from SECTION WHERE SECTIONID = '"+ section_id +"');";
    const char *line = search_section.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nSection with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Section... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    string search_course = "SELECT EXISTS(SELECT * from COURSE WHERE SECTION = '"+ section_id +"');";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nSection with requested ID doesn't have any courses available..." << endl;
    cout << "\nUnable to access requested details of Section... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    cout << "\nDisplaying Details of Course(s) of Section" << section_id << endl;
    cout << "\nCOURSE_ID CODE\t    FACULTY\t\t NAME\t\t\t\t        QUALIFICATION\t\t     DESIGNATION\t\t\t    RESEARCH AREA" << endl;
    search_course = "SELECT CRS.COURSEID || \" \" || CRS.COURSECODE, CRS.CRSTCHR, F.FACULTYNAME, F.QUALIFICATION, F.DESIGNATION, F.RESEARCHAREA from COURSE CRS LEFT JOIN FACULTY F ON CRS.CRSTCHR = F.FACULTYID WHERE CRS.SECTION = '"+ section_id +"';";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_course_details, 0, &zErrMsg);
    cout << endl;
	cout << "Enter the Course Code of Faculty to be removed as Faculty of Section" + section_id << " : ";
	cin >> course_id;
	search_course = "SELECT EXISTS(SELECT * from COURSE WHERE COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"');";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nCourse with requested Code doesn't exist..." << endl;
    cout << "\nUnable to access requested details of faculty... Try Again using valid Code...\n" << endl;
    clear_screen();
    return;
    }
    string search_faculty = "SELECT EXISTS(SELECT * from COURSE WHERE CRSTCHR IS NOT NULL AND COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"');";
    line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nCourse with requested Code is not allotted to any faculty yet..." << endl;
    cout << "\nUnable to access requested details of faculty... Try Again using valid Code...\n" << endl;
    clear_screen();
    return;
    }
    c: cout << "\n\nAre you sure to remove the requested faculty from being COURSE TEACHER of " + course_id << " of Section" + section_id + " ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
    d: error_message();
	cout << "\n\nEnter the Course Code of Faculty to be removed as Faculty of Section" + section_id << " : " << course_id;
	goto c;
	}
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto d;
	}
    if(choice == 1){
    sqlite3_open("SAMS.db", &db);
    string update_faculty = "UPDATE COURSE set CRSTCHR = NULL WHERE COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"';";
    line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested faculty removed from being as Course Teacher successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
    }
    if(choice == 2){
    cout << "\nNo changes were made....\n" << endl;
    clear_screen();
    return;
	}
	clear_screen();
	return;
}

void admin :: view_course(){
    sqlite3 *db;
    int rc, dept_id, sem;
    char *zErrMsg = 0, *sql;
    string faculty_id, section_id, course_id;
    sqlite3_open("SAMS.db", &db);
    a: cout << "Enter Semester : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	goto a;
	}
	sem = stoi(excp);
	if(sem>8 || sem<1){
	goto b;
	}
	cout << "\nEnter Section : ";
	cin >> section_id;
	section_id = "  " + to_string(sem) + section_id;
    string search_section = "SELECT EXISTS(SELECT * from SECTION WHERE SECTIONID = '"+ section_id +"');";
    const char *line = search_section.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nSection with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Section... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    string search_course = "SELECT EXISTS(SELECT * from COURSE WHERE SECTION = '"+ section_id +"');";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nSection with requested ID doesn't have any courses available..." << endl;
    cout << "\nUnable to access requested details of Section... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    cout << "\nDisplaying Details of Course(s) of Section" << section_id << endl;
    cout << "\nCOURSE_ID CODE\t    FACULTY\t\t NAME\t\t\t\t        QUALIFICATION\t\t     DESIGNATION\t\t\t    RESEARCH AREA" << endl;
    search_course = "SELECT CRS.COURSEID || \" \" || CRS.COURSECODE, CRS.CRSTCHR, F.FACULTYNAME, F.QUALIFICATION, F.DESIGNATION, F.RESEARCHAREA from COURSE CRS LEFT JOIN FACULTY F ON CRS.CRSTCHR = F.FACULTYID WHERE CRS.SECTION = '"+ section_id +"';";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_course_details, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nDetails of Course(s) of Section" << section_id << " displayed successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
}

void admin :: master_assign_course_menu(admin &a){
    int option = 0;
	while(option !=4){
	a: cout << "\nAccessing Faculty ASSIGN COURSE Operations...\n\n\n";
	cout << "Type '1' ----> Add Course\n";
	cout << "Type '2' ----> Remove Course\n";
	cout << "Type '3' ----> View Course\n";
	cout << "Type '4' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>4 || option<1){
	goto b;
	}
	else if(option == 1){
	system("CLS");
	a.assign_course();
	}
	else if(option == 2){
	system("CLS");
	a.remove_course();
	}
	else if(option == 3){
	system("CLS");
	a.view_course();
	}
	}
	if(option == 4){
	cout << "Redirecting back...\n";
	clear_screen();
	return;
	}
}

//global function
void clstchr_confirmation(int &choice, string dept_id, string faculty_deptno, string faculty_id, string section_id){
	a: cout << "\n\nAre you sure to make the requested faculty with id " + faculty_id + " as CLASS TEACHER of" + section_id + " ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
    b: error_message();
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept_id << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno, "");
	cout << "\n\nEnter the id to be made Class Teacher : " << faculty_id << endl;
	cout << "Enter the Section: " << section_id << endl;
	goto a;
	}
	choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto b;
	}
	else
    return;
}

//global function
void check_clstchr(string dept_id, string faculty_deptno, string faculty_id, string section_id){
    sqlite3 *db;
    int rc, choice;
    char *zErrMsg = 0, *sql;
    sqlite3_open("SAMS.db", &db);
    string search_clstchr = "SELECT EXISTS(SELECT * FROM SECTION WHERE CLSTCHR IS NOT NULL AND SECTIONID = '"+  section_id +"');";
    const char *line = search_clstchr.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    search_clstchr = "SELECT EXISTS(SELECT * FROM SECTION WHERE CLSTCHR = '"+ faculty_id +"');";
    line = search_clstchr.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc==0){
    clstchr_confirmation(choice,dept_id,faculty_deptno,faculty_id,section_id);
    if(choice == 1){
    sqlite3_open("SAMS.db", &db);
    set_foreignkeys(db);
    string update_branch = "UPDATE BRANCH set HOD = NULL WHERE HOD = '"+ faculty_id +"';";
    line = update_branch.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    string update_section = "UPDATE SECTION set CLSTCHR = '"+ faculty_id +"' WHERE SECTIONID = '"+ section_id +"';";
    const char *line = update_section.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested faculty made as Class Teacher successfully..." << endl;
    return;
    }
    if(choice == 2){
    cout << "\nNo changes were made...." << endl;
    return;
	}
    }
    else{
    search_clstchr = "SELECT CLSTCHR,SECTIONID from SECTION WHERE CLSTCHR = '" + faculty_id + "';";
    line = search_clstchr.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_clstchr_details, 0, &zErrMsg);
    clstchr_confirmation(choice,dept_id,faculty_deptno,faculty_id,section_id);
    if(choice == 1){
    sqlite3_open("SAMS.db", &db);
    set_foreignkeys(db);
    string update_faculty = "UPDATE SECTION set CLSTCHR = NULL WHERE CLSTCHR = '"+ faculty_id +"';";
    line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    string update_section = "UPDATE SECTION set CLSTCHR = '"+ faculty_id +"' WHERE SECTIONID = '"+ section_id +"';";
    line = update_section.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested faculty made as Class Teacher successfully..." << endl;
    return;
    }
    if(choice == 2){
    cout << "\nNo changes were made...." << endl;
    return;
	}
    }
    }
    else{
    search_clstchr = "SELECT CLSTCHR,SECTIONID from SECTION WHERE SECTIONID = '" + section_id + "';";
    line = search_clstchr.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_clstchr_details, 0, &zErrMsg);
    clstchr_confirmation(choice,dept_id,faculty_deptno,faculty_id,section_id);
    if(choice == 1){
    search_clstchr = "SELECT EXISTS(SELECT * FROM SECTION WHERE CLSTCHR = '"+ faculty_id +"');";
    line = search_clstchr.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc==0){
    sqlite3_open("SAMS.db", &db);
    set_foreignkeys(db);
    string update_branch = "UPDATE BRANCH set HOD = NULL WHERE HOD = '"+ faculty_id +"';";
    line = update_branch.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    string update_section = "UPDATE SECTION set CLSTCHR = '"+ faculty_id +"' WHERE SECTIONID = '"+ section_id +"';";
    const char *line = update_section.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested faculty made as Class Teacher successfully..." << endl;
    return;
    }
    else{
    search_clstchr = "SELECT CLSTCHR,SECTIONID from SECTION WHERE CLSTCHR = '" + faculty_id + "';";
    line = search_clstchr.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_clstchr_details, 0, &zErrMsg);
    clstchr_confirmation(choice,dept_id,faculty_deptno,faculty_id,section_id);
    if(choice == 1){
    sqlite3_open("SAMS.db", &db);
    set_foreignkeys(db);
    string update_faculty = "UPDATE SECTION set CLSTCHR = NULL WHERE CLSTCHR = '"+ faculty_id +"';";
    line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    string update_section = "UPDATE SECTION set CLSTCHR = '"+ faculty_id +"' WHERE SECTIONID = '"+ section_id +"';";
    line = update_section.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested faculty made as Class Teacher successfully..." << endl;
    return;
    }
    if(choice == 2){
    cout << "\nNo changes were made...." << endl;
    return;
	}
    }
    }
    if(choice == 2){
    cout << "\nNo changes were made...." << endl;
    return;
	}
    }
}

void admin :: assign_clstchr(){
    sqlite3 *db;
    int rc, dept_id, sem;
    char *zErrMsg = 0, *sql;
    string faculty_id, section_id, course_id;
    sqlite3_open("SAMS.db", &db);
    a: cout << "Enter Semester : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	goto a;
	}
	sem = stoi(excp);
	if(sem>8 || sem<1){
	goto b;
	}
	cout << "\nEnter Section : ";
	cin >> section_id;
	section_id = "  " + to_string(sem) + section_id;
    string search_section = "SELECT EXISTS(SELECT * from SECTION WHERE SECTIONID = '"+ section_id +"');";
    const char *line = search_section.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nSection with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Section... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    string search_course = "SELECT EXISTS(SELECT * from COURSE WHERE SECTION = '"+ section_id +"');";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nSection with requested ID doesn't have any faculty allotted so far..." << endl;
    cout << "\nUnable to access requested details of Section... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    cout << "\nDisplaying Details of Course(s) of Section" << section_id << endl;
    cout << "\nCOURSE_ID CODE\t    FACULTY\t\t NAME\t\t\t\t        QUALIFICATION\t\t     DESIGNATION\t\t\t    RESEARCH AREA" << endl;
    search_course = "SELECT CRS.COURSEID || \" \" || CRS.COURSECODE, CRS.CRSTCHR, F.FACULTYNAME, F.QUALIFICATION, F.DESIGNATION, F.RESEARCHAREA from COURSE CRS LEFT JOIN FACULTY F ON CRS.CRSTCHR = F.FACULTYID WHERE CRS.SECTION = '"+ section_id +"';";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_course_details, 0, &zErrMsg);
    cout << endl;
    cout << "Enter the Course Code of Faculty to be made Class Teacher of Section" + section_id << " : ";
	cin >> course_id;
	search_course = "SELECT EXISTS(SELECT * from COURSE WHERE COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"');";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nCourse with requested Code doesn't exist..." << endl;
    cout << "\nUnable to access requested details of faculty... Try Again using valid Code...\n" << endl;
    clear_screen();
    return;
    }
    string search_faculty = "SELECT EXISTS(SELECT * from COURSE WHERE CRSTCHR IS NOT NULL AND COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"');";
    line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nCourse with requested Code is not allotted to any faculty yet..." << endl;
    cout << "\nUnable to access requested details of faculty... Try Again using valid Code...\n" << endl;
    clear_screen();
    return;
    }
    search_faculty = "SELECT FACULTYID, DEPTNO from FACULTY WHERE FACULTYID = (SELECT CRSTCHR from COURSE WHERE COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"');";
    line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    faculty_id = gbl_info[0];
    string faculty_deptno = gbl_info[1];
    int n = sizeof(dept_no)/sizeof(dept_no[0]);
    auto itr = find(dept_no, dept_no + n, faculty_deptno);
    string branch_id = dept[distance(dept_no, itr)];
	check_clstchr(branch_id,faculty_deptno,faculty_id,section_id);
    cout << endl;
    clear_screen();
    return;
}

void admin :: remove_clstchr(){
    sqlite3 *db;
    string faculty_id;
    int rc, semester;
    char *zErrMsg = 0, *sql;
    a: cout << "Enter Semester : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	goto a;
	}
	semester = stoi(excp);
	if(semester>8 || semester<1){
    goto b;
	}
    cout << "\nDisplaying Details of Class Teachers(s) of Semester " + to_string(semester) << endl;
    cout << "\n\nSECTION ID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("CLSTCHR", to_string(semester), "");
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be removed from being Class Teacher : ";
	cin >> faculty_id;
	string search_clstchr = "SELECT EXISTS(SELECT * from SECTION WHERE CLSTCHR = '"+ faculty_id +"' AND SECTIONID LIKE '" + ("  " + to_string(semester) + "%") + "');";
    const char *line = search_clstchr.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nClass Teacher with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Class Teacher... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    c: cout << "\nAre you sure to remove the requested faculty with id " + faculty_id + " as Class Teacher ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
    d: error_message();
    cout << "\nDisplaying Details of Class Teacher(s) of Semester " + to_string(semester) << endl;
    cout << "\n\nSECTION ID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("CLSTCHR", to_string(semester), "");
	cout << "\n\nEnter the id to be removed from being Class Teacher : " << faculty_id << endl;
	goto c;
	}
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto d;
	}
    if(choice == 1){
    sqlite3_open("SAMS.db", &db);
    set_foreignkeys(db);
    string update_branch = "UPDATE SECTION set CLSTCHR = NULL WHERE CLSTCHR = '"+ faculty_id +"';";
    line = update_branch.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested faculty is removed from being Class Teacher successfully..." << endl;
    }
    if(choice == 2){
    cout << "\nNo changes were made...." << endl;
	}
	cout << endl;
    clear_screen();
    return;
}

void admin :: view_clstchr(){
    sqlite3 *db;
    int rc, semester;
    char *zErrMsg = 0, *sql;
    a: cout << "Enter Semester : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	goto a;
	}
	semester = stoi(excp);
	if(semester>8 || semester<1){
    goto b;
	}
    cout << "\nDisplaying Details of Class Teacher(s) of Semester " + to_string(semester) << endl;
    cout << "\n\nSECTION ID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("CLSTCHR", to_string(semester), "");
    cout << "\nDetails of all Class Teacher(s) of Semester " + to_string(semester) + " displayed successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
}

void admin :: master_assign_clstchr_menu(admin &a){
    int option = 0;
	while(option !=4){
	a: cout << "\nAccessing Faculty ASSIGN CLASS TEACHER Operations...\n\n\n";
	cout << "Type '1' ----> Add Class Teacher\n";
	cout << "Type '2' ----> Remove Class Teacher\n";
	cout << "Type '3' ----> View Class Teacher\n";
	cout << "Type '4' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>4 || option<1){
	goto b;
	}
	else if(option == 1){
	system("CLS");
	a.assign_clstchr();
	}
	else if(option == 2){
	system("CLS");
	a.remove_clstchr();
	}
	else if(option == 3){
	system("CLS");
	a.view_clstchr();
	}
	}
	if(option == 4){
	cout << "Redirecting back...\n";
	clear_screen();
	return;
	}
}

//global function
void check_hod(string faculty_deptno, string branch_id){
    int rc;
    sqlite3 *db;
    char *zErrMsg = 0, *sql;
    sqlite3_open("SAMS.db", &db);
    string search_hod = "SELECT EXISTS(SELECT * FROM BRANCH WHERE HOD IS NOT NULL AND BRANCHID = '"+  faculty_deptno +"');";
    const char *line = search_hod.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    return;
    }
    else{
    search_hod = "SELECT HOD from BRANCH WHERE BRANCHID = '" + faculty_deptno + "';";
    line = search_hod.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_hod_details, 0, &zErrMsg);
    cout << branch_id << endl;
    sqlite3_close(db);
    return;
    }
}

void admin :: assign_head(){
	sqlite3 *db;
    int rc, dept_id;
    string faculty_id;
    char *zErrMsg = 0, *sql;
	select_branch_view_function(dept_id, "faculty");
	int id = dept_id - 1;
	string faculty_deptno = dept_no[id];
	string branch_id = dept[id];
	system("CLS");
	view_faculty_function(faculty_deptno, false);
	if(gbl_data == 0){
    cout << "\nERROR: No Faculty Details Found...." << endl;
	cout << endl;
	}
	else{
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno, "");
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be made Head of Department : ";
	cin >> faculty_id;
	string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ faculty_id +"' AND DEPTNO = '"+ faculty_deptno +"'  AND WORKING = 'Y');";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nFaculty with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of faculty... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    a: check_hod(faculty_deptno,branch_id);
    cout << "\nAre you sure to make the requested faculty with id " + faculty_id + " as HEAD of " + branch_id + " ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
    b: error_message();
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno, "");
	cout << "\n\nEnter the id to be made Head of Department : " << faculty_id << endl;
	goto a;
	}
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto b;
	}
    if(choice == 1){
    sqlite3_open("SAMS.db", &db);
    string update_course = "UPDATE COURSE set CRSTCHR = NULL WHERE CRSTCHR = '"+ faculty_id +"';";
    line = update_course.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    string update_clstchr = "UPDATE SECTION set CLSTCHR = NULL WHERE CLSTCHR = '"+ faculty_id +"';";
    const char *line = update_clstchr.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    string update_branch = "UPDATE BRANCH set HOD = '"+ faculty_id +"' WHERE BRANCHID = '"+ faculty_deptno +"';";
    line = update_branch.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested faculty made as Head of Department successfully..." << endl;
    }
    if(choice == 2){
    cout << "\nNo changes were made...." << endl;
	}
    cout << endl;
    clear_screen();
    return;
	}
    clear_screen();
    return;
}

void admin :: remove_head(){
    int rc;
    sqlite3 *db;
    string faculty_id;
    char *zErrMsg = 0, *sql;
    cout << "\nDisplaying Details of HOD(s)..." << endl;
    cout << "\n\nBRANCH ID    NAME\t\t\t\t     QUALIFICATION\t\t   DESIGNATION\t\t\t\t   RESEARCH AREA" << endl;
    view_table("HOD", "", "");
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be removed from being Head of Department : ";
	cin >> faculty_id;
	string search_hod = "SELECT EXISTS(SELECT * from BRANCH WHERE HOD = '"+ faculty_id +"');";
    const char *line = search_hod.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nHOD with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of HOD... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    a: cout << "\nAre you sure to remove the requested faculty with id " + faculty_id + " as HEAD ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
    b: error_message();
    cout << "\nDisplaying Details of HOD(s)..." << endl;
    cout << "\n\nBRANCH ID    NAME\t\t\t\t     QUALIFICATION\t\t   DESIGNATION\t\t\t\t   RESEARCH AREA" << endl;
    view_table("HOD", "", "");
	cout << "\n\nEnter the id to be removed from being Head of Department : " << faculty_id << endl;
	goto a;
	}
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto b;
	}
    if(choice == 1){
    sqlite3_open("SAMS.db", &db);
    set_foreignkeys(db);
    string update_branch = "UPDATE BRANCH set HOD = NULL WHERE HOD = '"+ faculty_id +"';";
    line = update_branch.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested faculty is removed from being Head of Department successfully..." << endl;
    }
    if(choice == 2){
    cout << "\nNo changes were made...." << endl;
	}
	cout << endl;
    clear_screen();
    return;
}

void admin :: view_head(){
    int rc;
    sqlite3 *db;
    char *zErrMsg = 0, *sql;
    cout << "\nDisplaying Details of HOD(s)..." << endl;
    cout << "\n\nBRANCH ID    NAME\t\t\t\t     QUALIFICATION\t\t   DESIGNATION\t\t\t\t   RESEARCH AREA" << endl;
    view_table("HOD", "", "");
    cout << "\nDetails of all HOD(s) displayed successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
}

void admin :: master_assign_head_menu(admin &a){
    int option = 0;
	while(option !=4){
	a: cout << "\nAccessing Faculty ASSIGN HEAD Operations...\n\n\n";
	cout << "Type '1' ----> Add HOD\n";
	cout << "Type '2' ----> Remove HOD\n";
	cout << "Type '3' ----> View HOD\n";
	cout << "Type '4' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>4 || option<1){
	goto b;
	}
	else if(option == 1){
	system("CLS");
	a.assign_head();
	}
	else if(option == 2){
	system("CLS");
	a.remove_head();
	}
	else if(option == 3){
	system("CLS");
	a.view_head();
	}
	}
	if(option == 4){
	cout << "Redirecting back...\n";
	clear_screen();
	return;
	}
}

void admin :: master_faculty_assign_menu(admin &a){
    int option = 0;
	while(option !=4){
	a: cout << "\nAccessing Faculty ASSIGN Operations...\n\n\n";
	cout << "Type '1' ----> Assign a Course\n";
	cout << "Type '2' ----> Assign as Class Teacher\n";
	cout << "Type '3' ----> Assign as Head of Department\n";
	cout << "Type '4' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>4 || option<1){
	goto b;
	}
	else if(option == 1){
	system("CLS");
	a.master_assign_course_menu(a);
	}
	else if(option == 2){
	system("CLS");
	a.master_assign_clstchr_menu(a);
	}
	else if(option == 3){
	system("CLS");
    a.master_assign_head_menu(a);
	}
	}
	if(option == 4){
	cout << "Redirecting back...\n";
	clear_screen();
	return;
	}
}

void admin :: master_faculty_menu(admin &a){
	int option = 0;
	while(option !=3){
	a: cout << "\nAccessing Faculty Operations...\n\n\n";
	cout << "Type '1' ----> Access EDIT Operations\n";
	cout << "Type '2' ----> Access ASSIGN Operations\n";
	cout << "Type '3' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>3 || option<1){
	goto b;
	}
	else if(option == 1){
	system("CLS");
	a.master_faculty_edit_menu(a);
	}
	else if(option == 2){
	system("CLS");
	a.master_faculty_assign_menu(a);
	}
	}
	if(option == 3){
	cout << "Redirecting back to Admin Page\n";
	clear_screen();
	return;
	}
}

//CATEGORY 3: STUDENT EDIT OPERATION FUNCTIONS

//global function
void add_student_function(string &student_id, string &section, string year, string semester){
    int rc=0;
    sqlite3 *db;
	string str = "";
	char *zErrMsg, *sql;
	sqlite3_open("SAMS.db", &db);
    string search_student = "SELECT COUNT(*) FROM STUDENT WHERE DEPTNO = '" + student_id + "' AND YEARJOINED = '"+ year +"' AND SEMESTER = '"+ semester +"';";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, search_table, 0, &zErrMsg);
    sqlite3_close(db);
	if(gbl_data == 0){
	section = section + "1";
    student_id = student_id + "001";
    }
    else{
    gbl_data++;
    if(gbl_data<10)
    str = "00" + to_string(gbl_data);
    else if(gbl_data<100)
    str = "0" + to_string(gbl_data);
    else if(gbl_data<1000)
    str = to_string(gbl_data);
    student_id = student_id + str;
    if(gbl_data<61)
    section = section + "1";
    else if(gbl_data<121)
    section = section + "2";
    else if(gbl_data<181)
    section = section + "3";
    }
}

void admin :: add_student(){
	sqlite3 *db;
    long long contact;
	sqlite3_open("SAMS.db", &db);
	char *zErrMsg, *sql, ch, name[40];
    int rc, n, dept_id, sem, yearjoined;
	a: cout << "Enter the number of student(s) : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	error_message();
	goto a;
	}
	n = stoi(excp);
	cout << "\nEntering Details of " << n << " student(s)..." << endl;
	for(int i = 0; i < n; i++){
	cout << "\nEntering Details of student " << i+1 << endl;
	b: select_branch_insert_function(i, n, "student(s)", "student", dept_id);
	int id = dept_id - 1;
    string branch_id = dept_no[id];
    string student_deptno = dept_no[id];
    string student_sec = dept[id];
	system("CLS");
    cout << "Enter the number of student(s) : " << n << endl;
	cout << "\nEntering Details of " << n << " student(s)..." << endl;
	cout << "\nEntering Details of student " << i+1 << endl;
	student_select_branch_function();
	cout << dept[id] << endl;
	c: cout << "Enter Year Joined (>=2015): ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	d: error_message();
    cout << "Enter the number of student(s) : " << n << endl;
	cout << "\nEntering Details of " << n << " student(s)..." << endl;
	cout << "\nEntering Details of student " << i+1 << endl;
	student_select_branch_function();
	cout << dept[id] << endl;
	goto c;
	}
	yearjoined = stoi(excp);
	time_t system_time = time(0);
    tm *year = localtime(&system_time);
    int current_year =  1900 + year->tm_year;
	if(yearjoined>current_year || yearjoined<2015){
    goto d;
	}
    string student_yearjoined = to_string(yearjoined);
    e: cout << "Enter Semester : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	f: error_message();
    cout << "Enter the number of student(s) : " << n << endl;
	cout << "\nEntering Details of " << n << " student(s)..." << endl;
	cout << "\nEntering Details of student " << i+1 << endl;
	student_select_branch_function();
	cout << dept[id] << endl;
	cout << "Enter Year Joined (>=2015): " << yearjoined << endl;
	goto e;
	}
	sem = stoi(excp);
    if(sem>8 || sem<1){
    goto f;
	}
	string student_sem = to_string(sem);
    add_student_function(branch_id,student_sec,student_yearjoined,student_sem);
    student_sec = "  " + student_sem + student_sec;
    cout << "Enter Student Name : ";
	cin >> ch;
	int j=0;
	while (ch != '\n' && j<30){
    name[j] = ch;
    ch = cin.get();
    j++;
	}
	name[j] = '\0';
	padded_input_string(name, 39);
	g: cout << "Enter Contact Number : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	h: error_message();
    cout << "Enter the number of student(s) : " << n << endl;
	cout << "\nEntering Details of " << n << " student(s)..." << endl;
	cout << "\nEntering Details of student " << i+1 << endl;
	student_select_branch_function();
	cout << dept[id] << endl;
	cout << "Enter Year Joined (>=2015): " << yearjoined << endl;
	cout << "Enter Semester : " << sem << endl;
	cout << "Enter Student Name : " << name << endl;
	goto g;
	}
	contact = stoll(excp);
    if(contact>9999999999 || contact<1000000000){
    goto h;
	}
    string student_name = name;
    string year_offset = student_yearjoined.substr(2,2);
	string student_id = "B" + year_offset + branch_id;
	string contact_number = to_string(contact);
    char char_student_yearjoined[student_yearjoined.size() + 1];
    strcpy(char_student_yearjoined, student_yearjoined.c_str());
	string string_student_academic_year = char_student_yearjoined;
    set_foreignkeys(db);
    string insert_student = "INSERT INTO STUDENT (STUDENTID,STUDENTNAME,YEARJOINED,SEMESTER,SECTION,STUDYING,CONTACTNUMBER,DEPTNO,STUDENTPASSWORD) VALUES ('" + student_id + "', '" + student_name + "', '" + string_student_academic_year + "', " + student_sem + ", '" + student_sec + "', 'Y', "+ contact_number +", '" + student_deptno + "', '" + gbl_password + "');";
	const char *line = insert_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    if(strcmp(zErrMsg,"FOREIGN KEY constraint failed")==0)
    cout << "\nERROR: Maximum number of students reached for Department " << dept[id] << endl;
    else
    fprintf(stderr, "\nSQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    cout << "\nUnable to register requested details of student... Try Again with valid ID...\n" << endl;
    clear_screen();
	cout << "Enter the number of student(s) : " << n << endl;
	cout << "\nEntering Details of " << n << " student(s)..." << endl;
    goto b;
    }
    if(i == n-1)
    cout << "\nDetails of student " << i+1 << " registered successfully..." << endl;
    else{
    cout << "\nDetails of student " << i+1 << " registered successfully... \n" << endl;
    clear_screen();
	cout << "Enter the number of student(s) : " << n << endl;
	cout << "\nEntering Details of " << n << " student(s)..." << endl;
    }
	}
    sqlite3_close(db);
	cout << "\nDetails of " << n << " student(s) registered successfully..." << endl;
	cout << endl;
	clear_screen();
	return;
}

//global function
int update_student_function(string &student_id, int id, string student_deptno){
    int rc = 1;
    sqlite3 *db;
	char *zErrMsg, *sql;
	cout << "\nDisplaying Details of student(s) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID\t NAME\t\t\t\t\t YEAR SEM SECTION CONTACT" << endl;
    view_table("STUDENT", student_deptno, "");
	rc = sqlite3_open("SAMS.db", &db);
    cout << "\n\nEnter the ID to be updated : ";
    cin >> student_id;
    string search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ student_id +"' AND DEPTNO = '"+ student_deptno +"' AND STUDYING = 'Y');";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    sqlite3_close(db);
	if(rc == 0){
    cout << "\nStudent with requested ID doesn't exist..." << endl;
    cout << "\nUnable to update requested details of student... Try Again using valid ID...\n" << endl;
    clear_screen();
    return 0;
    }
    else
    return 1;
}

void admin :: update_student(){
    sqlite3 *db;
    string student_id;
	char *zErrMsg, *sql;
    int option = 0, rc = 0, dept_id = 0;
    while(option !=3){
    select_branch_view_function(dept_id, "student");
	int id = dept_id - 1;
	string student_deptno = dept_no[id];
    system("CLS");
	view_student_function(student_deptno, false);
	if(gbl_data == 0){
    cout << "\nERROR: No Student Details Found...." << endl;
	cout << endl;
	clear_screen();
	return;
	}
	a: cout << "Selected Branch : " << dept[id] << endl;
    cout << "\nSelect the field to be updated...\n\n\n";
	cout << "Type '1' ----> Update Name\n";
	cout << "Type '2' ----> Update Contact Number\n";
	cout << "Type '3' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>3 || option<1){
    goto b;
	}
	else if(option == 1){
	system("CLS");
	char ch, name[40];
    rc = update_student_function(student_id, id, student_deptno);
	if(rc == 0)
    return;
	else if(rc == 1){
    sqlite3_open("SAMS.db", &db);
    cout << "Enter the New Name : ";
	cin >> ch;
	int j=0;
	while (ch != '\n' && j<40){
    name[j] = ch;
    ch = cin.get();
    j++;
	}
	name[j] = '\0';
	padded_input_string(name, 39);
    string new_name = name;
	string update_student = "UPDATE STUDENT set STUDENTNAME = '"+ new_name +"' WHERE STUDENTID = '"+ student_id +"';";
    const char *line = update_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
	}
    else if(option == 2){
	system("CLS");
	long long contact;
    rc = update_student_function(student_id, id, student_deptno);
	if(rc == 0)
    return;
	else if(rc == 1){
    sqlite3_open("SAMS.db", &db);
    c: cout << "Enter Contact Number : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	d: error_message();
	goto c;
	}
	contact = stoll(excp);
    if(contact>9999999999 || contact<1000000000){
    goto d;
    }
    string new_contact_number = to_string(contact);
	string update_student = "UPDATE STUDENT set CONTACTNUMBER = '"+ new_contact_number +"' WHERE STUDENTID = '"+ student_id +"';";
    const char *line = update_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
	}
	if(option == 3){
	cout << "Redirecting back...\n";
	clear_screen();
	return;
	}
}
}

void admin :: delete_student(){
    sqlite3 *db;
    string student_id;
	char *zErrMsg, *sql;
	int rc = 0, dept_id = 0;
    select_branch_view_function(dept_id, "student");
	int id = dept_id - 1;
	string student_deptno = dept_no[id];
    system("CLS");
	view_student_function(student_deptno, false);
	if(gbl_data == 0){
    cout << "\nERROR: No Student Details Found...." << endl;
	cout << endl;
	clear_screen();
	return;
	}
    cout << "\nDisplaying Details of student(s) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID\t NAME\t\t\t\t\t YEAR SEM SECTION CONTACT" << endl;
    view_table("STUDENT", student_deptno, "");
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be deleted : ";
	cin >> student_id;
	string search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ student_id +"' AND DEPTNO = '"+ student_deptno +"'  AND STUDYING = 'Y');";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
	if(rc == 0){
    cout << "\nStudent with requested ID doesn't exist..." << endl;
    cout << "\nUnable to delete requested details of student... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
	a : cout << "\n\nAre you sure to delete the record of student with id " + student_id + " ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
    cout << "\nDisplaying Details of student(s) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID\t NAME\t\t\t\t\t YEAR SEM SECTION CONTACT" << endl;
    view_table("STUDENT", student_deptno, "");
    cout << "\n\nEnter the id to be deleted : " << student_id << endl;
	goto a;
	}
    sqlite3_close(db);
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
	goto b;
	}
	if(choice == 1){
    sqlite3_open("SAMS.db", &db);
    set_foreignkeys(db);
    string delete_student = "UPDATE STUDENT set STUDYING = 'N' where STUDENTID = '"+ student_id + "' AND DEPTNO = '" + student_deptno + "';";
    const char *line = delete_student.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested details deleted successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
	if(choice == 2){
    cout << "\nRequested details retained successfully..." << endl;
    cout << endl;
    clear_screen();
	return;
	}
}

void admin :: view_student(){
    sqlite3 *db;
    int rc, dept_id;
    char *zErrMsg = 0, *sql;
    select_branch_view_function(dept_id, "student");
	int id = dept_id - 1;
	string student_deptno = dept_no[id];
	system("CLS");
	view_student_function(student_deptno, false);
	if(gbl_data == 0){
    cout << "\nERROR: No Student Details Found...." << endl;
	cout << endl;
	}
	else{
    cout << "\nDisplaying Details of student(s) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID\t NAME\t\t\t\t\t YEAR SEM SECTION CONTACT" << endl;
    view_table("STUDENT", student_deptno, "");
    cout << "\nDetails of all student(s) of branch " << dept[id] << " displayed successfully..." << endl;
    cout << endl;
	}
    clear_screen();
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
	b : error_message();
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
	clear_screen();
	return;
	}
}

//CATEGORY 4: ARCHIVES OPERATION FUNCTIONS
void admin :: view_archived_faculty(){
    sqlite3 *db;
    int rc, dept_id;
    char *zErrMsg = 0, *sql;
	select_branch_view_function(dept_id, "faculty");
	int id = dept_id - 1;
	string faculty_deptno = dept_no[id];
	system("CLS");
	view_faculty_function(faculty_deptno, true);
	if(gbl_data == 0){
    cout << "\nERROR: No Faculty Details Found...." << endl;
    cout << endl;
	}
	else{
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("A-FACULTY", faculty_deptno, "");
    cout << "\nDetails of all faculty(ies) of branch " << dept[id] << " displayed successfully..." << endl;
    cout << endl;
	}
    clear_screen();
    return;
}

void admin :: view_archived_student(){
    sqlite3 *db;
    int rc, dept_id;
    char *zErrMsg = 0, *sql;
    select_branch_view_function(dept_id, "student");
	int id = dept_id - 1;
	string student_deptno = dept_no[id];
	system("CLS");
	view_student_function(student_deptno, true);
	if(gbl_data == 0){
    cout << "\nERROR: No Student Details Found...." << endl;
    cout << endl;
	}
	else{
    cout << "\nDisplaying Details of student(s) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID\t NAME\t\t\t\t\t YEAR SEM SECTION CONTACT" << endl;
    view_table("A-STUDENT", student_deptno, "");
    cout << "\nDetails of all student(s) of branch " << dept[id] << " displayed successfully..." << endl;
    cout << endl;
	}
    clear_screen();
    return;
}

void admin :: master_archives_menu(admin &a){
    int option = 0;
	while(option !=3){
	a: cout << "\nAccessing Archive Operations...\n\n\n";
	cout << "Type '1' ----> View Faculty Data\n";
	cout << "Type '2' ----> View Student Data\n";
	cout << "Type '3' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>3 || option<1){
	goto b;
	}
	else if(option == 1){
	system("CLS");
	a.view_archived_faculty();
	}
	else if(option == 2){
	system("CLS");
	a.view_archived_student();
	}
	}
	if(option == 3){
	cout << "Redirecting back to Admin Page\n";
	clear_screen();
	return;
	}
}

void admin :: master_main_menu(admin &a){
	int option = 0;
	while(option !=4){
	a: cout << "\nWelcome Admin\n\n\n";
	cout << "Type '1' ----> Manage Faculty Data\n";
	cout << "Type '2' ----> Manage Student Data\n";
	cout << "Type '3' ----> Archives\n";
	cout << "Type '4' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>4 || option<1){
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
    else if(option == 3){
	system("CLS");
	a.master_archives_menu(a);
	}
	}
	if(option == 4){
	cout << "Redirecting back to Main Menu\n";
	clear_screen();
	return;
	}
}

//CLASS TEACHER CLASS FUNCTIONS
void class_teacher :: view_subject_wise_marks(string section_id){
    int rc;
    sqlite3 *db;
    char *zErrMsg = 0, *sql;
    string course_id, course_type;
    sqlite3_open("SAMS.db", &db);
    cout << "\nDisplaying Details of Course(s) of Section" << section_id << endl;
    cout << "\nCOURSE_ID CODE\t    FACULTY\t\t NAME\t\t\t\t        QUALIFICATION\t\t     DESIGNATION\t\t\t    RESEARCH AREA" << endl;
    string search_course = "SELECT CRS.COURSEID || \" \" || CRS.COURSECODE, CRS.CRSTCHR, F.FACULTYNAME, F.QUALIFICATION, F.DESIGNATION, F.RESEARCHAREA from COURSE CRS LEFT JOIN FACULTY F ON CRS.CRSTCHR = F.FACULTYID WHERE CRS.SECTION = '"+ section_id +"';";
    const char *line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_course_details, 0, &zErrMsg);
    cout << endl;
    cout << "Enter the Course Code whose marks are to be displayed : ";
	cin >> course_id;
	search_course = "SELECT EXISTS(SELECT * from COURSE WHERE COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"');";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nCourse with requested Code doesn't exist..." << endl;
    cout << "\nUnable to access requested details of course... Try Again using valid Code...\n" << endl;
    clear_screen();
    return;
    }
    string search_strength = "SELECT EXISTS(SELECT * from GRADEREPORT G INNER JOIN STUDENT S ON G.STUDENTID = S.STUDENTID WHERE G.SECTIONID = '"+ section_id +"' AND G.COURSEID = '" + course_id +"' AND S.STUDYING = 'Y');";
	line = search_strength.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nERROR: No Student Details Found.... Please wait till the corresponding faculty awards marks to students and Try Again..." << endl;
	cout << endl;
	clear_screen();
	return;
    }
    string search_type = "SELECT COURSETYPE FROM COURSE WHERE COURSECODE = '" + course_id +"' AND SECTION = '"+ section_id +"';";
    line = search_type.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course_type = gbl_info[0];
    system("CLS");
    cout << "\nDisplaying Details of student(s)..." << endl;
    if(course_type == "T"){
    cout << "\nROLL NUMBER\tNAME\t\t\t\t\t SECTION\tM1\tA1\tMSE1\tM2\tA2\tMSE2\tINT\tESE"<< endl;
    string search_grade = "SELECT G.STUDENTID, S.STUDENTNAME, G.SECTIONID, G.M1, G.A1, G.MSE1, G.M2, G.A2, G.MSE2, G.INTERNALS, G.EXTERNALS FROM GRADEREPORT G LEFT JOIN STUDENT S ON G.STUDENTID = S.STUDENTID WHERE G.COURSEID = '" + course_id + "' AND G.SECTIONID = '" + section_id + "' AND S.STUDYING = 'Y';";
	line = search_grade.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_graded_details, 0, &zErrMsg);
    search_grade = "SELECT S.STUDENTID, S.STUDENTNAME, S.SECTION FROM STUDENT S WHERE S.STUDENTID NOT IN (SELECT G.STUDENTID FROM GRADEREPORT G WHERE G.COURSEID = '"+ course_id +"' AND G.SECTIONID = '"+ section_id +"') AND S.SECTION = '"+ section_id +"' AND S.STUDYING = 'Y';";
    line = search_grade.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_theoretical_ungraded_details, 0, &zErrMsg);
    sqlite3_close(db);
    cout << endl;
    cout << "Note : NA ---> Not Awarded\n\nDetails of all student(s) displayed successfully..." << endl;
    cout << endl;
    }
    else{
    cout << "\nROLL NUMBER\tNAME\t\t\t\t\t SECTION\tINT\tESE"<< endl;
    string search_grade = "SELECT G.STUDENTID, S.STUDENTNAME, G.SECTIONID, G.INTERNALS, G.EXTERNALS FROM GRADEREPORT G LEFT JOIN STUDENT S ON G.STUDENTID = S.STUDENTID WHERE COURSEID = '" + course_id + "' AND SECTIONID = '" + section_id + "' AND S.STUDYING = 'Y';";
	line = search_grade.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_graded_details, 0, &zErrMsg);
    search_grade = "SELECT S.STUDENTID, S.STUDENTNAME, S.SECTION FROM STUDENT S WHERE S.STUDENTID NOT IN (SELECT G.STUDENTID FROM GRADEREPORT G WHERE G.COURSEID = '"+ course_id +"' AND G.SECTIONID = '"+ section_id +"') AND S.SECTION = '"+ section_id +"' AND S.STUDYING = 'Y';";
	line = search_grade.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_practical_ungraded_details, 0, &zErrMsg);
    cout << endl;
    sqlite3_close(db);
    cout << endl;
    cout << "Note : NA ---> Not Awarded\n\nDetails of all student(s) displayed successfully..." << endl;
    cout << endl;
    }
    clear_screen();
    return;
}

void class_teacher :: read(section &s, string student_id, string section_id){
    int rc;
    sqlite3 *db;
	char *zErrMsg, *sql;
    sqlite3_open("SAMS.db", &db);
    gbl_data = stoi(section_id.substr(2,1));
    for(int i=0; i<course_count; i++){
    for(int i=0; i<100; i++)
    gbl_info[i] = "";
    if(course_type[i] == "T"){
    string search_marks = "SELECT M1, A1, MSE1, M2, A2, MSE2, INTERNALS, EXTERNALS FROM GRADEREPORT WHERE STUDENTID = '"+ student_id +"' AND SECTIONID = '" + section_id +"' AND COURSEID = '"+ course_id[i] +"'";
    const char *line = search_marks.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    m1[i] = stof(gbl_info[0]);
    a1[i] = stof(gbl_info[1]);
    ms1[i] = stof(gbl_info[2]);
    m2[i] = stof(gbl_info[3]);
    a2[i] = stof(gbl_info[4]);
    ms2[i] = stof(gbl_info[5]);
    internal_marks[i] = stof(gbl_info[6]);
    external_marks[i] = stof(gbl_info[7]);
    }
    else{
    string search_marks = "SELECT INTERNALS, EXTERNALS FROM GRADEREPORT WHERE STUDENTID = '"+ student_id +"' AND SECTIONID = '" + section_id +"' AND COURSEID = '"+ course_id[i] +"'";
    const char *line = search_marks.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    internal_marks[i] = stof(gbl_info[0]);
    external_marks[i] = stof(gbl_info[1]);
    }
    }
    sqlite3_close(db);
}

void class_teacher :: calculate_student_marks(section &s){
    float ta_index = 4, max_index = 0.7, min_index = 0.3;
    for(int i=0; i<course_count; i++){
    for(int i=0 ; i<100; i++)
    gbl_info[i] = "";
    if(course_type[i] == "T"){
    resultm1[i] = m1[i] / ta_index;
	resulta1[i] = a1[i] / ta_index;
	resultm2[i] = m2[i] / ta_index;
	resulta2[i] = a2[i] / ta_index;
	if(ms1[i]>ms2[i]){
	resultms1[i] = max_index * ms1[i];
	resultms2[i] = min_index * ms2[i];
	}
	else{
	resultms1[i] = min_index * ms1[i];
	resultms2[i] = max_index * ms2[i];
	}
	finalmse[i] = resultms1[i] + resultms2[i] + external_marks[i];
	final_marks[i] = internal_marks[i] + external_marks[i];
	if(final_marks[i]>=90 && finalmse[i]>31 && external_marks[i]>21){
	gradepoint[i] = 10; grade[i] = 'S';
	}
	else if(final_marks[i]>=80 && finalmse[i]>31 && external_marks[i]>21){
	gradepoint[i] = 9; grade[i] = 'A';
	}
	else if(final_marks[i]>=70 && finalmse[i]>31 && external_marks[i]>21){
	gradepoint[i] = 8; grade[i] = 'B';
	}
	else if(final_marks[i]>=60 && finalmse[i]>31 && external_marks[i]>21){
	gradepoint[i] = 7; grade[i] = 'C';
	}
	else if(final_marks[i]>=45 && finalmse[i]>31 && external_marks[i]>21){
	gradepoint[i] = 6; grade[i] = 'D';
	}
	else if(final_marks[i]>=35 && finalmse[i]>31 && external_marks[i]>21){
	gradepoint[i] = 4; grade[i] = 'P';
	}
	else if(final_marks[i]<35 || finalmse[i]<31 || external_marks[i]<21){
	gradepoint[i] = 0; grade[i] = 'F';
	}
    }
    else{
    final_marks[i] = internal_marks[i] + external_marks[i];
	if(final_marks[i]>=90 && external_marks[i]>21){
	gradepoint[i] = 10; grade[i] = 'S';
	}
	else if(final_marks[i]>=80 && external_marks[i]>21){
	gradepoint[i] = 9; grade[i] = 'A';
	}
	else if(final_marks[i]>=70 && external_marks[i]>21){
	gradepoint[i] = 8; grade[i] = 'B';
	}
	else if(final_marks[i]>=60 && external_marks[i]>21){
	gradepoint[i] = 7; grade[i] = 'C';
	}
	else if(final_marks[i]>=45 && external_marks[i]>21){
	gradepoint[i] = 6; grade[i] = 'D';
	}
	else if(final_marks[i]>=35 && external_marks[i]>21){
	gradepoint[i] = 4; grade[i] = 'P';
	}
	else if(final_marks[i]<35 || external_marks[i]<21){
	gradepoint[i] = 0; grade[i] = 'F';
	}
    }
    }
}

void class_teacher :: calculate_student_sgpa(section &s, int index){
    float score_sum = 0, credit_sum = 0;
    for(int i=0;i<course_count;i++){
    score_sum = score_sum + (gradepoint[i] * course_credits[i]);
    credit_sum = credit_sum + course_credits[i];
    }
    final_gradepoint[index] = (score_sum/credit_sum);
}

void class_teacher :: write(section &s, int index, string student_id){
    int rc;
    sqlite3 *db;
	char *zErrMsg, *sql;
    sqlite3_open("SAMS.db", &db);
    for(int i=0; i<course_count; i++){
    char grade_char = grade[i];
    string grade(1,grade_char);
    string update_grade = "UPDATE GRADEREPORT set GRADE = '"+ grade +"' WHERE COURSEID = '"+ course_id[i] +"' AND STUDENTID = '"+ student_id +"';";
    const char *line = update_grade.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    string update_gradepoint= "UPDATE GRADEREPORT set GRADEPOINT = '"+ to_string(gradepoint[i]) +"' WHERE COURSEID = '"+ course_id[i] +"' AND STUDENTID = '"+ student_id +"';";
    line = update_gradepoint.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    }
    string sem_index = semester[gbl_data-1];
    string update_sgpa = "UPDATE STUDENT set "+ sem_index + "SEM = '"+ to_string(final_gradepoint[index]) +"' WHERE STUDENTID = '"+ student_id +"';";
    const char *line = update_sgpa.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    return;
}

void class_teacher :: generate_sgpa(section* s, class_teacher &ct, string section_id){
    sqlite3 *db;
	char *zErrMsg, *sql;
    sqlite3_open("SAMS.db", &db);
    int rc, generated = 0, already = 0, generated_count = 0, deleted_count = 0;
    string search_strength = "SELECT EXISTS(SELECT * from STUDENT WHERE SECTION = '"+ section_id +"' AND STUDYING = 'Y');";
    const char *line = search_strength.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nERROR: No Student Details Found...." << endl;
	cout << endl;
	clear_screen();
	return;
    }
    string search_count = "SELECT COUNT(*) FROM STUDENT WHERE SECTION = '"+ section_id +"';";
    line = search_count.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    student_count = stoi(gbl_info[0]);
    int  studying[student_count];
    int available[student_count];
    int sgpa_done[student_count];
    for(int i=0;i<10;i++)
    awarded[i] = "YES";
    for(int i=0;i<student_count;i++){
    studying[i] = 0;
    available[i] = 0;
    sgpa_done[i] = -1;
    }
    search_count = "SELECT COUNT(*) FROM COURSE WHERE SECTION = '"+ section_id +"';";
    line = search_count.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course_count = stoi(gbl_info[0]);
    for(int i = 0; i<course_count; i++){
    string search_type = "SELECT COURSECODE,COURSETYPE,CREDITS FROM COURSE WHERE SECTION = '"+ section_id +"' AND ROWID = '" +  to_string(i+1) +"'";
    line = search_type.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course_id[i] = gbl_info[0];
    course_type[i] = gbl_info[1];
    course_credits[i] = stoi(gbl_info[2]);
    }
    for(int i=0; i<student_count; i++){
    string search_student = "SELECT STUDENTID FROM STUDENT WHERE SECTION = '"+ section_id +"' AND STUDENTID LIKE '%"+ to_string(i+1) +"';";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    roll_number[i] = gbl_info[0];
    }
    for(int i=0;i<student_count; i++){
    string search_student = "SELECT EXISTS(SELECT STUDENTID FROM STUDENT WHERE SECTION = '"+ section_id +"' AND STUDENTID = '"+ roll_number[i] +"' AND STUDYING = 'Y');";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    studying[i] = -1;
    deleted_count++;
    }
    }
    cout << "\nDisplaying Status of Marks Updation of Section" + section_id + "..."<< endl;
    cout << "\nROLL NUMBER " << " ";
    for(int i=0; i<course_count; i++){
    int len = course_id[i].length();
    char data[len+1];
    strcpy(data,course_id[i].c_str());
    padded_input_string(data,10);
    cout << data << " ";
    }
    cout << endl;
    for(int i=0; i<student_count; i++){
    if(studying[i] != -1)
    cout << roll_number[i] << "     ";
    gbl_data = stoi(section_id.substr(2,1));
    string sem_index = semester[gbl_data-1];
    string search_already = "SELECT EXISTS(SELECT * FROM STUDENT WHERE STUDENTID = '"+ roll_number[i] +"' AND " + sem_index + "SEM IS NULL);";
    line = search_already.c_str();
    sql = strdup(line);
    already = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(already == 0 && studying[i] != -1){
    sgpa_done[i] = 0;
    generated_count++;
    }
    for(int j=0; j<course_count; j++){
    string search_student = "SELECT EXISTS(SELECT S.STUDENTID FROM STUDENT S WHERE S.SECTION = '"+ section_id +"' AND S.STUDENTID NOT IN (SELECT G.STUDENTID FROM GRADEREPORT G WHERE G.COURSEID = '"+ course_id[j] +"') AND S.STUDENTID = '"+ roll_number[i] +"');";
    line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc!=0){
    available[i] = -1;
    awarded[j] = "NO ";
    int len = awarded[j].length();
    char data[len+1];
    strcpy(data,awarded[j].c_str());
    padded_input_string(data,10);
    if(studying[i] != -1)
    cout << data << " ";
    }
    else{
    int len = awarded[j].length();
    char data[len+1];
    strcpy(data,awarded[j].c_str());
    padded_input_string(data,10);
    if(studying[i] != -1)
    cout << data << " ";
    }
    }
    if(studying[i] != -1)
    cout << endl;
    }
    for(int i=0; i<student_count; i++)
    if(available[i] != -1 && studying[i] != -1)
    generated++;
    cout << "\n\nTotal No. of Students : " << (student_count - deleted_count) << endl;
    cout << "\nNo of. Students with all marks available to generate SGPA : " << generated << endl;
    cout << "\nNo of. Students with SGPA already generated : " << generated_count << endl;
    if(generated - generated_count == 0){
    if((generated_count == 0) ? (cout << "\n\nNOTE: Cannot Generate SGPA. Since, No student has all marks available to generate SGPA...\n" << endl) : (cout << "\n\nNOTE: Cannot Generate SGPA. Since, SGPA is already generated to student(s) with all marks available...\n" << endl));
    clear_screen();
    return;
    }
    cout << "\nNo of. students whose SGPA can be generated : " << (generated - generated_count) << endl;
    a: cout << "\nWARNING: STUDENT MARKS CANNOT BE UPDATED, ONCE SGPA IS GENERATED...\n\nAre you sure to continue? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
    b: cout << "Choose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	goto a;
	}
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto b;
	}
    if(choice == 1){
    for(int i=0; i<student_count; i++){
    if(available[i] != -1 && sgpa_done[i] != 0 && studying[i] != -1){
    ct.read(s[i], roll_number[i], section_id);
	ct.calculate_student_marks(s[i]);
	ct.calculate_student_sgpa(s[i], i);
	ct.write(s[i], i, roll_number[i]);
    }
    }
    sqlite3_close(db);
    cout << "\nSGPA Generated Successfully for " << (generated - generated_count)  << " student(s) of Section" + section_id + "...\n"<< endl;
    }
    if(choice == 2)
    cout << "\nNo changes were made....\n" << endl;
    clear_screen();
    return;
}

void class_teacher :: view_overall_marks(string section_id){
    sqlite3 *db;
	char *zErrMsg, *sql;
    int rc, num, generated = 0;
    string student_id, course[10];
    sqlite3_open("SAMS.db", &db);
	cout << "Displaying details of Section" << section_id << endl;
    a: cout << "\nEnter the number of student(s) : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	cout << "Displaying details of Section" << section_id << endl;
	goto a;
	}
	num = stoi(excp);
    if(num>60 || num<1){
	goto b;
	}
    cout << "\nEntering Details of " << num << " student(s)..." << endl;
    for(int i = 0; i<num; i++){
    cout << "\nEnter Roll Number : ";
    cin >> student_id;
    string search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ student_id +"' AND SECTION = '"+ section_id +"' AND STUDYING = 'Y');";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nStudent with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Student... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    search_student = "SELECT EXISTS(SELECT STUDENTID FROM STUDENT WHERE FOURTHSEM IS NOT NULL AND STUDENTID = '"+ student_id +"');";
    line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nSGPA for the Student with requested ID is not yet generated..." << endl;
    cout << "\nUnable to access requested details of Student... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    string search_count = "SELECT COUNT(*) FROM COURSE WHERE SECTION = '"+ section_id +"';";
    line = search_count.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course_count = stoi(gbl_info[0]);
    for(int i = 0; i<course_count; i++){
    string search_type = "SELECT COURSECODE,COURSETYPE,CREDITS FROM COURSE WHERE SECTION = '"+ section_id +"' AND ROWID = '" +  to_string(i+1) +"'";
    line = search_type.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course_id[i] = gbl_info[0];
    course_type[i] = gbl_info[1];
    course_credits[i] = stoi(gbl_info[2]);
    }
    for(int i=0; i<course_count; i++){
    for(int i=0; i<100; i++)
    gbl_info[i] = "";
    if(course_type[i] == "T"){
    string search_marks = "SELECT COURSEID, M1, A1, MSE1, M2, A2, MSE2, INTERNALS, EXTERNALS, GRADE, GRADEPOINT FROM GRADEREPORT WHERE STUDENTID = '"+ student_id +"' AND SECTIONID = '" + section_id +"' AND COURSEID = '"+ course_id[i] +"'";
    const char *line = search_marks.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course[i] = gbl_info[0];
    m1[i] = stof(gbl_info[1]);
    a1[i] = stof(gbl_info[2]);
    ms1[i] = stof(gbl_info[3]);
    m2[i] = stof(gbl_info[4]);
    a2[i] = stof(gbl_info[5]);
    ms2[i] = stof(gbl_info[6]);
    internal_marks[i] = stof(gbl_info[7]);
    external_marks[i] = stof(gbl_info[8]);
    char grade_array[gbl_info[9].size() + 1];
    strcpy(grade_array, gbl_info[9].c_str());
    grade[i] = grade_array[0];
    gradepoint[i] = stof(gbl_info[10]);
    }
    else{
    string search_marks = "SELECT COURSEID, INTERNALS, EXTERNALS, GRADE, GRADEPOINT FROM GRADEREPORT WHERE STUDENTID = '"+ student_id +"' AND SECTIONID = '" + section_id +"' AND COURSEID = '"+ course_id[i] +"'";
    const char *line = search_marks.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course[i] = gbl_info[0],
    internal_marks[i] = stof(gbl_info[1]);
    external_marks[i] = stof(gbl_info[2]);
    char grade_array[gbl_info[3].size() + 1];
    strcpy(grade_array, gbl_info[3].c_str());
    grade[i] = grade_array[0];
    gradepoint[i] = stof(gbl_info[4]);
    }
    }
    cout << "\nDisplaying marks of " << student_id << "...\n\n" << endl;
    cout << "Course\t\tM1\tA1\tMse1\tM2\tA2\tMse2\tInternal\tExternal\tGrade\tGradepoint" << endl;
    for(int i=0; i<course_count; i++){
    int len = course[i].length();
    char data[len+1];
    strcpy(data,course[i].c_str());
    padded_input_string(data,16);
    course[i] = data;
    }
    for(int i=0; i<course_count; i++){
    if(course_type[i] == "T")
    cout << course[i] << m1[i] << "\t" << a1[i] << "\t " << ms1[i] << "\t" << m2[i] << "\t" << a2[i] << "\t " << ms2[i] << "\t   " << internal_marks[i] << "\t\t   " << external_marks[i] << "\t\t  " << grade[i] << "\t    " << gradepoint[i] << endl;
    else
    cout << course[i] << "--\t--\t --\t--\t--\t --\t   " << internal_marks[i] << "\t\t   " << external_marks[i] << "\t\t  " << grade[i] << "\t    " << gradepoint[i] << endl;
    }
    gbl_data = stoi(section_id.substr(2,1));
    string sem_index = semester[gbl_data-1];
    string search_sgpa = "SELECT " + sem_index + "SEM FROM STUDENT WHERE STUDENTID = '"+ student_id +"' AND SECTION = '" + section_id +"'";
    line = search_sgpa.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    float final_sgpa = stof(gbl_info[0]);
    cout << "\nFinal Gradepoint : " << final_sgpa;
    cout << endl;
    cout << "\nMarks of " << student_id << " displayed successfully..." << endl;
    cout << endl;
    clear_screen();
    if(i<num-1){
    cout << "Displaying details of Section" << section_id << endl;
    cout << "\nEnter the number of student(s) : "<< num << endl;
    cout << "\nEntering Details of " << num << " student(s)..." << endl;
    }
    }
    return;
}

void class_teacher :: class_teacher_main_menu(class_teacher &ct, string clstchr_id, string section_id){
    student s[60];
    course_teacher t;
    teacher_id = clstchr_id;
    int option = 0;
	while(option !=5){
    a: cout << "\nWelcome " << teacher_id << "\n\n";
	cout << "Logged in as Class Teacher\nYou are currently accessing operations of Section :" + section_id << endl;
	cout << "\nPersonal Info : " << endl;
	t.view_account(teacher_id);
    cout << "\nType '1' ----> Generate SGPA\n";
    cout << "Type '2' ----> View SGPA\n";
    cout << "Type '3' ----> View Subject-Wise Marks\n";
    cout << "Type '4' ----> View other allotted sections/Update Password\n";
    cout << "Type '5' ----> Back to Main Menu\n";
    cout << "\nEnter Here : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>5 || option<1){
    goto b;
	}
    else if(option == 1){
    cout << "\nProcessing Request to generate SGPA for Section" << section_id << "..." << endl;
    cout << "Generating Report on Status of Marks Updation... \nReport Generated Successfully..." << endl;
    cout << endl;
    clear_screen();
    generate_sgpa(s,ct,section_id);
    }
    else if(option == 2){
    system("CLS");
    view_overall_marks(section_id);
    }
    else if(option == 3){
    system("CLS");
    view_subject_wise_marks(section_id);
	}
	else if(option == 4){
    system("CLS");
    t.teacher_main_menu(t, teacher_id);
	}
	}
	if(option == 5){
	cout << "Redirecting back to Main Menu\n";
	clear_screen();
	return;
	}
}

//COURSE TEACHER CLASS FUNCTIONS

//COURSE TEACHER - CLASS TEACHER COMMON LOGIN FUNCTION
int course_teacher :: login_teacher(bool &cls, string &clstchr_id, string &section_id){
    sqlite3 *db;
    int rc, count = 3;
    string faculty_deptno;
    char *zErrMsg = 0, *sql;
    rc = sqlite3_open("SAMS.db", &db);
	string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE WORKING = 'Y');";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    sqlite3_close(db);
	if(rc == 0){
    cout << "\nERROR: No Faculty Details Found...." << endl;
	cout << endl;
	clear_screen();
	return -1;
    }
	a: cout << "WELCOME TO FACULTY PAGE\n";
	cout << "\nEnter Faculty ID : ";
	cin >> teacher_id;
    cout << "Enter Password : ";
	cin >> teacher_password;
	rc = sqlite3_open("SAMS.db", &db);
	search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ teacher_id +"' AND FACULTYPASSWORD = '"+ teacher_password +"'  AND WORKING = 'Y');";
    line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
	if(rc == 0){
    cout << "Login Unsuccessful!!!";
	if(count <= 0){
	cout << "\n\nLogin Failed... Max no. of Attempts Reached!!!\n";
	cout << "Exiting Program for Security Reasons...\n";
	exit(0);
	}
	count--;
	cout << "\n\nYou have '" << count + 1 << "' more chances!!!\n";
	clear_screen();
	goto a;
	}
	else{
	cout << "\nLogin Successful!!!\n";
	string search_cls = "SELECT EXISTS(SELECT * from SECTION WHERE CLSTCHR = '"+ teacher_id +"');";
	line = search_cls.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
	if(rc != 0){
    cls = true;
    clstchr_id = teacher_id;
    search_cls = "SELECT SECTIONID FROM SECTION WHERE CLSTCHR = '"+ teacher_id +"'";
    line = search_cls.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    sqlite3_close(db);
    section_id = gbl_info[0];
	}
	}
	clear_screen();
	return 0;
}

void course_teacher :: add_student_marks(string faculty_id){
    sqlite3 *db;
    int rc, num;
    float marks[7];
	char *zErrMsg, *sql;
    string student_id, section_id, course_id, course_type;
    sqlite3_open("SAMS.db", &db);
    string check_crstchr = "SELECT EXISTS(SELECT * from COURSE WHERE CRSTCHR = '"+ faculty_id +"');";
    const char *line = check_crstchr.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nYou are not allotted to any Section so far..." << endl;
    cout << "\nUnable to access requested details of Faculty... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
	cout << "\nDisplaying Details of Course(s) you are allotted to...\n" << endl;
	cout << "   SEC CODE" << endl;
    view_table("COURSE",faculty_id, "");
    cout << "\nRegistering the details of Section and Course whose marks are to be entered..." << endl;
    cout << "\nEnter Section : ";
	cin >> section_id;
	section_id = "  " + section_id;
    string search_section = "SELECT EXISTS(SELECT * from COURSE WHERE SECTION = '"+ section_id +"' AND CRSTCHR = '"+ faculty_id +"');";
    line = search_section.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nYou are not allotted to the Section with requested ID or Section with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Section... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
	string search_strength = "SELECT EXISTS(SELECT * from STUDENT WHERE SECTION = '"+ section_id +"' AND STUDYING = 'Y');";
    line = search_strength.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nERROR: No Student Details Found...." << endl;
	cout << endl;
	clear_screen();
	return;
    }
    cout << "\nEnter Course Code (of Section" + section_id << ") : ";
	cin >> course_id;
	string search_course = "SELECT EXISTS(SELECT * from COURSE WHERE COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"' AND CRSTCHR = '"+ faculty_id +"');";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nYou are not allotted to the Course with requested ID or Course with requested Code doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Course... Try Again using valid Code...\n" << endl;
    clear_screen();
    return;
    }
    string search_type = "SELECT COURSETYPE FROM COURSE WHERE COURSECODE = '" + course_id +"' AND SECTION = '"+ section_id +"';";
    line = search_type.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course_type = gbl_info[0];
    system("CLS");
    cout << "Entering details of Section" << section_id << endl;
    a: cout << "\nEnter the number of student(s) : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b : error_message();
	cout << "Entering details of Section" << section_id << endl;
	goto a;
	}
	num = stoi(excp);
	if(num>60 || num<1){
	goto b;
	}
    cout << "\nEntering Details of " << num << " student(s)..." << endl;
    for(int i = 0; i<num; i++){
    cout << "\nEnter Roll Number : ";
    cin >> student_id;
    string search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ student_id +"' AND SECTION = '"+ section_id +"' AND STUDYING = 'Y');";
    line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nStudent with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Student... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    search_student = "SELECT EXISTS(SELECT * from GRADEREPORT WHERE STUDENTID = '"+ student_id +"' AND COURSEID = '"+ course_id +"');";
    line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc != 0){
    cout << "\nStudent with requested ID is already assigned with marks, cannot update at this point of time..." << endl;
    cout << "\nUnable to access requested details of Student... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    cout << endl;
    if(course_type == "T"){
    c : cout << "Enter minor 1 marks of " << course_id << " : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	d: cout << "Choose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	goto c;
	}
	marks[0] = stoi(excp);
	if(marks[0]>10 || marks[0]<0){
    goto d;
	}
	e : cout << "Enter assignment 1 marks of " << course_id << " : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	f: cout << "Choose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	goto e;
	}
	marks[1] = stoi(excp);
	if(marks[1]>10 || marks[1]<0){
    goto f;
	}
	g : cout << "Enter mse 1 marks of " << course_id << " : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	h: cout << "Choose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	goto g;
	}
	marks[2] = stoi(excp);
	if(marks[2]>30 || marks[2]<0){
    goto h;
	}
	i : cout << "Enter minor 2 marks of " << course_id << " : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	j: cout << "Choose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	goto i;
	}
	marks[3] = stoi(excp);
	if(marks[3]>10 || marks[3]<0){
    goto j;
	}
	k : cout << "Enter assignment 2 marks of " << course_id << " : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	l: cout << "Choose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	goto k;
	}
	marks[4] = stoi(excp);
	if(marks[4]>10 || marks[4]<0){
    goto l;
	}
	m : cout << "Enter mse 2 marks of " << course_id << " : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	n: cout << "Choose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	goto m;
	}
	marks[5] = stoi(excp);
	if(marks[5]>30 || marks[5]<0){
    goto n;
	}
	o : cout << "Enter ese marks of " << course_id << " : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	p: cout << "Choose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	goto o;
	}
	marks[6] = stoi(excp);
	if(marks[6]>60 || marks[6]<0){
    goto p;
	}
	q: cout << "\n\nAre you sure that the entered marks are correct  ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
    r: cout << "Choose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	goto q;
	}
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto r;
	}
    if(choice == 1){
    float teacher_assessment = (marks[0] + marks[1] + marks[3] + marks[4])/4;
    float mid_average = (marks[2] > marks[5]) ? ((0.7 * marks[2]) + (0.3 * marks[5])) : ((0.7 * marks[5]) + (0.3 * marks[2]));
    float internal_marks = teacher_assessment + mid_average;
    string insert_marks = "INSERT INTO GRADEREPORT (COURSEID,STUDENTID,SECTIONID,M1,A1,MSE1,M2,A2,MSE2,INTERNALS,EXTERNALS) VALUES ('" + course_id + "', '" + student_id + "', '" + section_id + "', '" + to_string(marks[0]) + "', '" + to_string(marks[1])  + "', '" + to_string(marks[2])  + "', '" + to_string(marks[3])  + "', '" + to_string(marks[4])  + "', '" + to_string(marks[5])  + "', '" + to_string(internal_marks) + "', '" +  to_string(marks[6]) + "');";
    const char *line = insert_marks.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
    }
    if(choice == 2){
    cout << endl;
    goto c;
    }
    }
    else{
    s: cout << "Enter internal marks of " << course_id << " : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	t: cout << "Choose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	goto s;
	}
	marks[0] = stoi(excp);
	if(marks[0]>40 || marks[0]<0){
    goto t;
	}
	u : cout << "Enter external marks of " << course_id << " : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	v: cout << "Choose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	goto u;
	}
	marks[1] = stoi(excp);
	if(marks[1]>60 || marks[1]<0){
    goto v;
	}
	w: cout << "\n\nAre you sure that the entered marks are correct  ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
    x: cout << "Choose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	goto w;
	}
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto x;
	}
    if(choice == 1){
    string insert_marks = "INSERT INTO GRADEREPORT (COURSEID,STUDENTID,SECTIONID,INTERNALS,EXTERNALS) VALUES ('" + course_id + "', '" + student_id + "', '" + section_id + "', '" + to_string(marks[0]) + "', '" + to_string(marks[1])  + "');";
    const char *line = insert_marks.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
    }
    if(choice == 2){
    cout << endl;
    goto s;
    }
    }
    }
    sqlite3_close(db);
    cout << "\nDetails of " << num << " student(s) registered successfully..." << endl;
    cout << endl;
	clear_screen();
	return;
}

void course_teacher :: update_student_marks(string faculty_id){
    sqlite3 *db;
	char *zErrMsg, *sql;
    float marks[7], new_marks;
    int rc, num, option = 0, already = 0;
    string student_id, section_id, course_id, course_type, update_marks;
    sqlite3_open("SAMS.db", &db);
    string check_crstchr = "SELECT EXISTS(SELECT * from COURSE WHERE CRSTCHR = '"+ faculty_id +"');";
    const char *line = check_crstchr.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nYou are not allotted to any Section so far..." << endl;
    cout << "\nUnable to access requested details of Faculty... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
	cout << "\nDisplaying Details of Course(s) you are allotted to...\n" << endl;
	cout << "   SEC CODE" << endl;
    view_table("COURSE",faculty_id, "");
    cout << "\nRegistering the details of Section and Course whose marks are to be displayed..." << endl;
    cout << "\nEnter Section : ";
	cin >> section_id;
	section_id = "  " + section_id;
    string search_section = "SELECT EXISTS(SELECT * from COURSE WHERE SECTION = '"+ section_id +"' AND CRSTCHR = '"+ faculty_id +"');";
    line = search_section.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nYou are not allotted to the Section with requested ID or Section with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Section... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    string search_strength = "SELECT EXISTS(SELECT * from STUDENT WHERE SECTION = '"+ section_id +"' AND STUDYING = 'Y');";
    line = search_strength.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nERROR: No Student Details Found...." << endl;
	cout << endl;
	clear_screen();
	return;
    }
    cout << "\nEnter Course Code (of Section" + section_id << ") : ";
	cin >> course_id;
	string search_course = "SELECT EXISTS(SELECT * from COURSE WHERE COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"' AND CRSTCHR = '"+ faculty_id +"');";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nYou are not allotted to the Course with requested ID or Course with requested Code doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Course... Try Again using valid Code...\n" << endl;
    clear_screen();
    return;
    }
    search_strength = "SELECT EXISTS(SELECT * from GRADEREPORT G INNER JOIN STUDENT S ON G.STUDENTID = S.STUDENTID WHERE G.SECTIONID = '"+ section_id +"' AND G.COURSEID = '" + course_id +"' AND S.STUDYING = 'Y');";
    line = search_strength.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nERROR: No Student Details Found.... Please award marks to students and Try Again..." << endl;
	cout << endl;
	clear_screen();
	return;
    }
    string search_type = "SELECT COURSETYPE FROM COURSE WHERE COURSECODE = '" + course_id +"' AND SECTION = '"+ section_id +"';";
    line = search_type.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course_type = gbl_info[0];
    system("CLS");
    while(option != 8){
    if(course_type == "T"){
    a: cout << "Selected Section :" << section_id << "  Selected Course : " << course_id << endl;
    cout << "\nSelect the field to be updated...\n\n\n";
	cout << "Type '1' ----> Update Minor 1 Marks\n";
	cout << "Type '2' ----> Update Assignment 1 Marks\n";
	cout << "Type '3' ----> Update Mse 1 Marks\n";
	cout << "Type '4' ----> Update Minor 2 Marks\n";
	cout << "Type '5' ----> Update Assignment 2 Marks\n";
	cout << "Type '6' ----> Update Mse 2 Marks\n";
	cout << "Type '7' ----> Update Ese Marks\n";
	cout << "Type '8' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>8 || option<1){
    goto b;
	}
	if(option == 8){
	cout << "Redirecting back...\n";
	clear_screen();
	return;
	}
    }
	else{
    c: cout << "Selected Section :" << section_id << "  Selected Course : " << course_id << endl;
    cout << "\nSelect the field to be updated...\n\n\n";
	cout << "Type '1' ----> Update Internal Marks\n";
	cout << "Type '2' ----> Update External Marks\n";
	cout << "Type '3' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	d: error_message();
	goto c;
	}
	option = stoi(excp);
	if(option>3 || option<1){
    goto d;
	}
    if(option == 3){
	cout << "Redirecting back...\n";
	clear_screen();
	return;
	}
	}
    system("CLS");
    cout << "\nDisplaying Details of student(s)..." << endl;
    if(course_type == "T"){
    cout << "\nROLL NUMBER\tNAME\t\t\t\t\t SECTION\tM1\tA1\tMSE1\tM2\tA2\tMSE2\tINT\tESE"<< endl;
    string search_grade = "SELECT G.STUDENTID, S.STUDENTNAME, G.SECTIONID, G.M1, G.A1, G.MSE1, G.M2, G.A2, G.MSE2, G.INTERNALS, G.EXTERNALS FROM GRADEREPORT G LEFT JOIN STUDENT S ON G.STUDENTID = S.STUDENTID WHERE G.COURSEID = '" + course_id + "' AND G.SECTIONID = '" + section_id + "' AND S.STUDYING = 'Y';";
    line = search_grade.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_graded_details, 0, &zErrMsg);
    cout << endl;
    }
    else{
    cout << "\nROLL NUMBER\tNAME\t\t\t\t\t SECTION\tINT\tESE"<< endl;
    string search_grade = "SELECT G.STUDENTID, S.STUDENTNAME, G.SECTIONID, G.INTERNALS, G.EXTERNALS FROM GRADEREPORT G LEFT JOIN STUDENT S ON G.STUDENTID = S.STUDENTID WHERE COURSEID = '" + course_id + "' AND SECTIONID = '" + section_id + "' AND S.STUDYING = 'Y';";
    line = search_grade.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_graded_details, 0, &zErrMsg);
    cout << endl;
    }
    cout << "Enter the ID to be updated : ";
    cin >> student_id;
    string search_student = "SELECT EXISTS(SELECT * from GRADEREPORT G INNER JOIN STUDENT S ON G.STUDENTID = S.STUDENTID WHERE G.STUDENTID = '"+ student_id +"' AND G.SECTIONID = '"+ section_id +"' AND G.COURSEID = '" + course_id +"' AND S.STUDYING = 'Y');";
    line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nPlease award marks to students and Try Again... or Student with requested ID doesn't exist..." << endl;
    cout << "\nUnable to update requested details of student... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    gbl_data = stoi(section_id.substr(2,1));
    string sem_index = semester[gbl_data-1];
    string search_already = "SELECT EXISTS(SELECT * FROM STUDENT WHERE STUDENTID = '"+ student_id +"' AND " + sem_index + "SEM IS NULL);";
    line = search_already.c_str();
    sql = strdup(line);
    already = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(already == 0){
    cout << "\nSGPA of Student with requested ID is already generated..." << endl;
    cout << "\nUnable to update requested details of student... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    if(course_type == "T"){
    string search_type = "SELECT M1, A1, MSE1, M2, A2, MSE2 FROM GRADEREPORT WHERE COURSEID = '" + course_id +"' AND SECTIONID = '"+ section_id +"' AND STUDENTID = '"+ student_id +"';";
    line = search_type.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    for(int i=0; i<6; i++)
    marks[i] = stof(gbl_info[i]);
    e: cout << "Enter updated marks : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	f: error_message();
	cout << "\nDisplaying Details of student(s)..." << endl;
    cout << "\nROLL NUMBER\tNAME\t\t\t\t\t SECTION\tM1\tA1\tMSE1\tM2\tA2\tMSE2\tINT\tESE"<< endl;
    string search_grade = "SELECT G.STUDENTID, S.STUDENTNAME, G.SECTIONID, G.M1, G.A1, G.MSE1, G.M2, G.A2, G.MSE2, G.INTERNALS, G.EXTERNALS FROM GRADEREPORT G LEFT JOIN STUDENT S ON G.STUDENTID = S.STUDENTID WHERE COURSEID = '" + course_id + "' AND SECTIONID = '" + section_id + "'";
    line = search_grade.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_graded_details, 0, &zErrMsg);
    cout << endl;
    cout << "Enter the ID to be updated : " << student_id << endl;
	goto e;
	}
	new_marks = stoi(excp);
    if(option != 7){
    if(option == 1){
    if(new_marks>10 || new_marks<0)
    goto f;
    marks[0] = new_marks;
    update_marks = "UPDATE GRADEREPORT SET M1 = " + to_string(new_marks) + " WHERE COURSEID = '" + course_id +"' AND SECTIONID = '"+ section_id +"' AND STUDENTID = '"+ student_id +"';";
    }
    else if(option == 2){
    if(new_marks>10 || new_marks<0)
    goto f;
    marks[1] = new_marks;
    update_marks = "UPDATE GRADEREPORT SET A1 = " + to_string(new_marks) + " WHERE COURSEID = '" + course_id +"' AND SECTIONID = '"+ section_id +"' AND STUDENTID = '"+ student_id +"';";
    }
    else if(option == 3){
    if(new_marks>30 || new_marks<0)
    goto f;
    marks[2] = new_marks;
    update_marks = "UPDATE GRADEREPORT SET MSE1 = " + to_string(new_marks) + " WHERE COURSEID = '" + course_id +"' AND SECTIONID = '"+ section_id +"' AND STUDENTID = '"+ student_id +"';";
    }
    else if(option == 4){
    if(new_marks>10 || new_marks<0)
    goto f;
    marks[3] = new_marks;
    update_marks = "UPDATE GRADEREPORT SET M2 = " + to_string(new_marks) + " WHERE COURSEID = '" + course_id +"' AND SECTIONID = '"+ section_id +"' AND STUDENTID = '"+ student_id +"';";
    }
    else if(option == 5){
    if(new_marks>10 || new_marks<0)
    goto f;
    marks[4] = new_marks;
    update_marks = "UPDATE GRADEREPORT SET A2 = " + to_string(new_marks) + " WHERE COURSEID = '" + course_id +"' AND SECTIONID = '"+ section_id +"' AND STUDENTID = '"+ student_id +"';";
    }
    else if(option == 6){
    if(new_marks>30 || new_marks<0)
    goto f;
    marks[5] = new_marks;
    update_marks = "UPDATE GRADEREPORT SET MSE2 = " + to_string(new_marks) + " WHERE COURSEID = '" + course_id +"' AND SECTIONID = '"+ section_id +"' AND STUDENTID = '"+ student_id +"';";
    }
    const char *line = update_marks.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    float teacher_assessment = (marks[0] + marks[1] + marks[3] + marks[4])/4;
    float mid_average = (marks[2] > marks[5]) ? ((0.7 * marks[2]) + (0.3 * marks[5])) : ((0.7 * marks[5]) + (0.3 * marks[2]));
    float internal_marks = teacher_assessment + mid_average;
    update_marks = "UPDATE GRADEREPORT SET INTERNALS = " + to_string(internal_marks) + " WHERE COURSEID = '" + course_id +"' AND SECTIONID = '"+ section_id +"' AND STUDENTID = '"+ student_id +"';";
    line = update_marks.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully...\n" << endl;
    clear_screen();
    return;
    }
    else{
    if(new_marks>60 || new_marks<0)
    goto f;
    update_marks = "UPDATE GRADEREPORT SET EXTERNALS = " + to_string(new_marks) + " WHERE COURSEID = '" + course_id +"' AND SECTIONID = '"+ section_id +"' AND STUDENTID = '"+ student_id +"';";
    line = update_marks.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
    }
    }
    else{
    g: cout << "Enter updated marks : ";
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	h: error_message();
	cout << "\nDisplaying Details of student(s)..." << endl;
    cout << "\nROLL NUMBER\tNAME\t\t\t\t\t SECTION\tINT\tESE"<< endl;
    string search_grade = "SELECT G.STUDENTID, S.STUDENTNAME, G.SECTIONID, G.INTERNALS, G.EXTERNALS FROM GRADEREPORT G LEFT JOIN STUDENT S ON G.STUDENTID = S.STUDENTID WHERE COURSEID = '" + course_id + "' AND SECTIONID = '" + section_id + "'";
    line = search_grade.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_graded_details, 0, &zErrMsg);
    cout << endl;
    cout << "Enter the ID to be updated : " << student_id << endl;
	goto g;
	}
	new_marks = stoi(excp);
    if(option == 1){
    if(new_marks>40 || new_marks<0)
    goto h;
    update_marks = "UPDATE GRADEREPORT SET INTERNALS = " + to_string(new_marks) + " WHERE COURSEID = '" + course_id +"' AND SECTIONID = '"+ section_id +"' AND STUDENTID = '"+ student_id +"';";
    }
    else if(option == 2){
    if(new_marks>60 || new_marks<0)
    goto h;
    update_marks = "UPDATE GRADEREPORT SET EXTERNALS = " + to_string(new_marks) + " WHERE COURSEID = '" + course_id +"' AND SECTIONID = '"+ section_id +"' AND STUDENTID = '"+ student_id +"';";
    }
    const char *line = update_marks.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
    }
    }
    clear_screen();
    return;
}

void course_teacher :: view_overall_marks(string faculty_id){
    sqlite3 *db;
    int rc, num;
    float marks[7];
	char *zErrMsg, *sql;
    string student_id, section_id, course_id, course_type;
    sqlite3_open("SAMS.db", &db);
    string check_crstchr = "SELECT EXISTS(SELECT * from COURSE WHERE CRSTCHR = '"+ faculty_id +"');";
    const char *line = check_crstchr.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nYou are not allotted to any Section so far..." << endl;
    cout << "\nUnable to access requested details of Faculty... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
	cout << "\nDisplaying Details of Course(s) you are allotted to...\n" << endl;
	cout << "   SEC CODE" << endl;
    view_table("COURSE",faculty_id, "");
    cout << "\nRegistering the details of Section and Course whose marks are to be displayed..." << endl;
    cout << "\nEnter Section : ";
	cin >> section_id;
	section_id = "  " + section_id;
    string search_section = "SELECT EXISTS(SELECT * from COURSE WHERE SECTION = '"+ section_id +"' AND CRSTCHR = '"+ faculty_id +"');";
    line = search_section.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nYou are not allotted to the Section with requested ID or Section with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Section... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    string search_strength = "SELECT EXISTS(SELECT * from STUDENT WHERE SECTION = '"+ section_id +"' AND STUDYING = 'Y');";
    line = search_strength.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nERROR: No Student Details Found...." << endl;
	cout << endl;
	clear_screen();
	return;
    }
    cout << "\nEnter Course Code (of Section" + section_id << ") : ";
	cin >> course_id;
	string search_course = "SELECT EXISTS(SELECT * from COURSE WHERE COURSECODE = '"+ course_id +"' AND SECTION = '"+ section_id +"' AND CRSTCHR = '"+ faculty_id +"');";
    line = search_course.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nYou are not allotted to the Course with requested ID or Course with requested Code doesn't exist..." << endl;
    cout << "\nUnable to access requested details of Course... Try Again using valid Code...\n" << endl;
    clear_screen();
    return;
    }
    search_strength = "SELECT EXISTS(SELECT * from GRADEREPORT G INNER JOIN STUDENT S ON G.STUDENTID = S.STUDENTID WHERE G.SECTIONID = '"+ section_id +"' AND G.COURSEID = '" + course_id +"' AND S.STUDYING = 'Y');";
    line = search_strength.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nERROR: No Student Details Found.... Please award marks to students and Try Again..." << endl;
	cout << endl;
	clear_screen();
	return;
    }
    string search_type = "SELECT COURSETYPE FROM COURSE WHERE COURSECODE = '" + course_id +"' AND SECTION = '"+ section_id +"';";
    line = search_type.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course_type = gbl_info[0];
    system("CLS");
    cout << "\nDisplaying Details of student(s)..." << endl;
    if(course_type == "T"){
    cout << "\nROLL NUMBER\tNAME\t\t\t\t\t SECTION\tM1\tA1\tMSE1\tM2\tA2\tMSE2\tINT\tESE"<< endl;
    string search_grade = "SELECT G.STUDENTID, S.STUDENTNAME, G.SECTIONID, G.M1, G.A1, G.MSE1, G.M2, G.A2, G.MSE2, G.INTERNALS, G.EXTERNALS FROM GRADEREPORT G LEFT JOIN STUDENT S ON G.STUDENTID = S.STUDENTID WHERE COURSEID = '" + course_id + "' AND SECTIONID = '" + section_id + "' AND S.STUDYING = 'Y'";
    line = search_grade.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_graded_details, 0, &zErrMsg);
    search_grade = "SELECT S.STUDENTID, S.STUDENTNAME, S.SECTION FROM STUDENT S WHERE S.STUDENTID NOT IN (SELECT G.STUDENTID FROM GRADEREPORT G WHERE G.COURSEID = '"+ course_id +"' AND G.SECTIONID = '"+ section_id +"') AND S.SECTION = '"+ section_id +"' AND S.STUDYING = 'Y'";
    line = search_grade.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_theoretical_ungraded_details, 0, &zErrMsg);
    cout << endl;
    cout << "Note : NA ---> Not Awarded\n\nDetails of all student(s) displayed successfully..." << endl;
    cout << endl;
    }
    else{
    cout << "\nROLL NUMBER\tNAME\t\t\t\t\t SECTION\tINT\tESE"<< endl;
    string search_grade = "SELECT G.STUDENTID, S.STUDENTNAME, G.SECTIONID, G.INTERNALS, G.EXTERNALS FROM GRADEREPORT G LEFT JOIN STUDENT S ON G.STUDENTID = S.STUDENTID WHERE COURSEID = '" + course_id + "' AND SECTIONID = '" + section_id + "' AND S.STUDYING = 'Y'";
    line = search_grade.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_graded_details, 0, &zErrMsg);
    search_grade = "SELECT S.STUDENTID, S.STUDENTNAME, S.SECTION FROM STUDENT S WHERE S.STUDENTID NOT IN (SELECT G.STUDENTID FROM GRADEREPORT G WHERE G.COURSEID = '"+ course_id +"' AND G.SECTIONID = '"+ section_id +"') AND S.SECTION = '"+ section_id +"' AND S.STUDYING = 'Y'";
    line = search_grade.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_practical_ungraded_details, 0, &zErrMsg);
    cout << endl;
    cout << "Note : NA ---> Not Awarded\n\nDetails of all student(s) displayed successfully..." << endl;
    cout << endl;
    }
    clear_screen();
    return;
}

void course_teacher :: update_tchrpass(string faculty_id){
    int rc, choice = 0;
    sqlite3 *db;
	char *zErrMsg, *sql;
	string new_password, con_password;
	a: cout << "Enter new password : ";
	cin >> new_password;
	cout << "\nConfirm password : ";
	cin >> con_password;
	if(con_password == new_password){
    b : cout << "\n\nAre you sure to change your password ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	c: error_message();
    cout << "Enter new password : " << new_password << endl;
    cout << "\nConfirm password : " << con_password << endl;
	goto b;
	}
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto c;
	}
    if(choice == 1){
    rc = sqlite3_open("SAMS.db", &db);
    string update_faculty = "UPDATE FACULTY SET FACULTYPASSWORD = '"+ new_password +"' WHERE FACULTYID = '" + faculty_id + "';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    }
    if(choice == 2)
    cout << "\nNo changes were made...." << endl;
    cout << endl;
    }
    else{
    cout << "\nPlease enter correct password...." << endl;
    cout << endl;
    clear_screen();
    goto a;
    }
    clear_screen();
    return;
}

void course_teacher :: view_account(string faculty_id){
    int rc;
    sqlite3 *db;
	char *zErrMsg, *sql;
	rc = sqlite3_open("SAMS.db", &db);
    string search_faculty = "SELECT FACULTYNAME,QUALIFICATION,DESIGNATION,RESEARCHAREA from FACULTY WHERE FACULTYID = '" + faculty_id + "';";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_faculty_account, 0, &zErrMsg);
    sqlite3_close(db);
    return;
}

void course_teacher :: teacher_main_menu(course_teacher &t, string clstchr_id){
	int option = 0;
    if(clstchr_id != "")
    teacher_id = clstchr_id;
	while(option !=5){
    a: cout << "\nWelcome " << teacher_id << "\n\n\n";
	cout << "Personal Info : " << endl;
	t.view_account(teacher_id);
    cout << "\nType '1' ----> Add Student Marks\n";
    cout << "Type '2' ----> Update Student Marks\n";
    cout << "Type '3' ----> View Overall Marks\n";
	cout << "Type '4' ----> Update My Password\n";
	cout << "Type '5' ----> Back to Main Menu\n";
    cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>5 || option<1){
    goto b;
	}
    else if(option == 1){
    system("CLS");
    t.add_student_marks(teacher_id);
    }
    else if(option == 2){
    system("CLS");
    t.update_student_marks(teacher_id);
    }
    else if(option == 3){
    system("CLS");
    t.view_overall_marks(teacher_id);
    }
    else if(option == 4){
    system("CLS");
	t.update_tchrpass(teacher_id);
    }
	}
	if(option == 5){
    if(clstchr_id != "")
	cout << "Redirecting back....\n";
    else
	cout << "Redirecting back to Main Menu\n";
	clear_screen();
	return;
	}
}

//STUDENT CLASS FUNCTIONS
int student :: login_student(){
	sqlite3 *db;
    int rc, count = 3;
    string student_deptno;
    char *zErrMsg = 0, *sql;
    rc = sqlite3_open("SAMS.db", &db);
	string search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDYING = 'Y');";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    sqlite3_close(db);
	if(rc == 0){
    cout << "\nERROR: No Student Details Found...." << endl;
	cout << endl;
	clear_screen();
	return -1;
    }
	a: cout << "WELCOME TO STUDENT PAGE\n";
	cout << "\nEnter Student ID : ";
	cin >> student_id;
	cout << "Enter Password : ";
	cin >> student_password;
	rc = sqlite3_open("SAMS.db", &db);
	search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ student_id +"' AND STUDENTPASSWORD = '"+ student_password +"'  AND STUDYING = 'Y');";
    line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    sqlite3_close(db);
	if(rc == 0){
    cout << "Login Unsuccessful!!!";
	if(count <= 0){
	cout << "\n\nLogin Failed... Max no. of Attempts Reached!!!\n";
	cout << "Exiting Program for Security Reasons...\n";
	exit(0);
	}
	count--;
	cout << "\n\nYou have '" << count + 1 << "' more chances!!!\n";
	clear_screen();
	goto a;
	}
	else
	cout << "\nLogin Successful!!!\n";
	clear_screen();
	return 0;
}

void student :: view_marks(string student_id){
    sqlite3 *db;
    int rc, num;
	char *zErrMsg, *sql;
    string course[10], section_id;
    sqlite3_open("SAMS.db", &db);
    string search_student = "SELECT EXISTS(SELECT STUDENTID FROM STUDENT WHERE FOURTHSEM IS NOT NULL AND STUDENTID = '"+ student_id +"');";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nSGPA for the Student with requested ID is not yet generated..." << endl;
    cout << "\nUnable to access requested details of Student... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    string search_section = "SELECT SECTION FROM STUDENT WHERE STUDENTID = '"+ student_id +"';";
    line = search_section.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    section_id = gbl_info[0];
    string search_count = "SELECT COUNT(*) FROM COURSE WHERE SECTION = '"+ section_id +"';";
    line = search_count.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course_count = stoi(gbl_info[0]);
    for(int i = 0; i<course_count; i++){
    string search_type = "SELECT COURSECODE,COURSETYPE,CREDITS FROM COURSE WHERE SECTION = '"+ section_id +"' AND ROWID = '" +  to_string(i+1) +"'";
    line = search_type.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course_id[i] = gbl_info[0];
    course_type[i] = gbl_info[1];
    course_credits[i] = stoi(gbl_info[2]);
    }
    for(int i=0; i<course_count; i++){
    for(int i=0; i<100; i++)
    gbl_info[i] = "";
    if(course_type[i] == "T"){
    string search_marks = "SELECT COURSEID, M1, A1, MSE1, M2, A2, MSE2, INTERNALS, GRADE, GRADEPOINT FROM GRADEREPORT WHERE STUDENTID = '"+ student_id +"' AND SECTIONID = '" + section_id +"' AND COURSEID = '"+ course_id[i] +"'";
    const char *line = search_marks.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course[i] = gbl_info[0];
    m1[i] = stof(gbl_info[1]); a1[i] = stof(gbl_info[2]); ms1[i] = stof(gbl_info[3]);
    m2[i] = stof(gbl_info[4]); a2[i] = stof(gbl_info[5]); ms2[i] = stof(gbl_info[6]);
    internal_marks[i] = stof(gbl_info[7]);
    char grade_array[gbl_info[8].size() + 1];
    strcpy(grade_array, gbl_info[8].c_str());
    grade[i] = grade_array[0];
    gradepoint[i] = stof(gbl_info[9]);
    }
    else{
    string search_marks = "SELECT COURSEID, INTERNALS, GRADE, GRADEPOINT FROM GRADEREPORT WHERE STUDENTID = '"+ student_id +"' AND SECTIONID = '" + section_id +"' AND COURSEID = '"+ course_id[i] +"'";
    const char *line = search_marks.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    course[i] = gbl_info[0],
    internal_marks[i] = stof(gbl_info[1]);
    char grade_array[gbl_info[2].size() + 1];
    strcpy(grade_array, gbl_info[2].c_str());
    grade[i] = grade_array[0];
    gradepoint[i] = stof(gbl_info[3]);
    }
    }
    cout << "\nDisplaying your marks..." << endl;
    cout << "\n\n";
    cout << "Course\t\tM1\tA1\tMse1\tM2\tA2\tMse2\tInternal   Grade   Gradepoint" << endl;
    for(int i=0; i<course_count; i++){
    int len = course[i].length();
    char data[len+1];
    strcpy(data,course[i].c_str());
    padded_input_string(data,16);
    course[i] = data;
    }
    for(int i=0; i<course_count; i++){
    if(course_type[i] == "T")
    cout << course[i] << m1[i] << "\t" << a1[i] << "\t " << ms1[i] << "\t" << m2[i] << "\t" << a2[i] << "\t " << ms2[i] << "\t   " << internal_marks[i] << "  \t     " << grade[i] << "\t       " << gradepoint[i] << endl;
    else
    cout << course[i] << "--\t--\t --\t--\t--\t --\t   " << internal_marks[i] << "  \t     " << grade[i] << "\t       " << gradepoint[i] << endl;
    }
	gbl_data = stoi(section_id.substr(2,1));
    string sem_index = semester[gbl_data-1];
    string search_sgpa = "SELECT " + sem_index + "SEM FROM STUDENT WHERE STUDENTID = '"+ student_id +"' AND SECTION = '" + section_id +"'";
    line = search_sgpa.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, extract_details, 0, &zErrMsg);
    float final_sgpa = stof(gbl_info[0]);
    cout << "\nFinal Gradepoint : " << final_sgpa << endl;
    cout << endl;
    clear_screen();
    return;
}

void student :: update_stdpass(string student_id){
    int rc, choice = 0;
    sqlite3 *db;
	char *zErrMsg, *sql;
	string new_password, con_password;
	a: cout << "Enter new password : ";
	cin >> new_password;
	cout << "\nConfirm password : ";
	cin >> con_password;
	if(con_password == new_password){
    b : cout << "\n\nAre you sure to change your password ? ";
	cout << "\nPress '1' if 'YES' or '2' if 'NO'";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	c: error_message();
    cout << "Enter new password : " << new_password << endl;
    cout << "\nConfirm password : " << con_password << endl;
	goto b;
	}
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto c;
	}
    if(choice == 1){
    rc = sqlite3_open("SAMS.db", &db);
    string update_student = "UPDATE STUDENT SET STUDENTPASSWORD = '"+ new_password +"' WHERE STUDENTID = '" + student_id + "';";
    const char *line = update_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
    cout << "\nRequested details updated successfully..." << endl;
    }
    if(choice == 2)
    cout << "\nNo changes were made...." << endl;
    cout << endl;
    }
    else{
    cout << "\nPlease enter correct password...." << endl;
    cout << endl;
    clear_screen();
    goto a;
    }
    clear_screen();
    return;
}

void student :: view_account(string student_id){
    int rc;
    sqlite3 *db;
	char *zErrMsg, *sql;
	rc = sqlite3_open("SAMS.db", &db);
    string search_student = "SELECT STUDENTNAME,SEMESTER,SECTION,CONTACTNUMBER from STUDENT WHERE STUDENTID = '" + student_id + "';";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_student_account, 0, &zErrMsg);
    sqlite3_close(db);
    return;
}

void student :: student_main_menu(student &s){
	int option = 0;
	while(option !=3){
	a: cout << "\nWelcome " << student_id << "\n\n\n";
    cout << "Personal Info : " << endl;
	s.view_account(student_id);
	cout << "\nType '1' ----> View My Marks\n";
	cout << "Type '2' ----> Update My Password\n";
	cout << "Type '3' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>3 || option<1){
    goto b;
	}
	else if(option == 1){
	system("CLS");
	s.view_marks(student_id);
	}
	else if(option == 2){
    system("CLS");
    s.update_stdpass(student_id);
	}
	}
	if(option == 3){
	cout << "Redirecting back to Main Menu\n";
	clear_screen();
	return;
	}
}

//global functions
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
	b : error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>4 || option<1){
	goto b;
	}
	else if(option == 1){
	cout << "Redirecting to Admin Login Page\n";
	clear_screen();
	admin a;
	a.login_master();
	a.master_main_menu(a);
	}
	else if(option == 2){
    bool cls = false;
    string clstchr_id = "";
    string section_id = "";
	cout << "Redirecting to Faculty Login Page\n";
	clear_screen();
	course_teacher t;
	int exist = t.login_teacher(cls,clstchr_id,section_id);
	if(exist!=-1 && cls==false)
	t.teacher_main_menu(t, "");
	else if(exist!=-1 && cls==true){
    class_teacher ct;
    ct.class_teacher_main_menu(ct, clstchr_id, section_id);
	}
	}
	else if(option == 3){
	cout << "Redirecting to Student Login Page\n";
	clear_screen();
	student s;
	int exist = s.login_student();
	if(exist!=-1)
	s.student_main_menu(s);
	}
	}
	if(option == 4){
    cout << "\nProgram Terminated Successfully...";
	exit(0);
	}
}

void table_creation_function(sqlite3 *db){
    int rc;
    char *sql;
    sql = strdup("CREATE TABLE BRANCH(BRANCHID VARCHAR2 (3) PRIMARY KEY, BRANCHCODE VARCHAR2 (8) NOT NULL, BRANCHNAME  VARCHAR2 (40) NOT NULL, HOD VARCHAR2 (6) REFERENCES FACULTY (FACULTYID), PHONE VARCHAR2 (10) NOT NULL);");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
    sql = strdup("CREATE TABLE SECTION(SECTIONID VARCHAR2 (10) PRIMARY KEY, CLSTCHR VARCHAR2 (6) REFERENCES FACULTY (FACULTYID), SEMESTER INTEGER NOT NULL, DEPTNO VARCHAR2 (3) NOT NULL REFERENCES BRANCH (BRANCHID));");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
    sql = strdup("CREATE TABLE FACULTY(FACULTYID VARCHAR2(6) PRIMARY KEY, FACULTYNAME  VARCHAR2 (40) NOT NULL, QUALIFICATION VARCHAR2 (30), DESIGNATION VARCHAR2 (40), RESEARCHAREA VARCHAR2 (60), WORKING VARCHAR2 (1) NOT NULL, DEPTNO VARCHAR2 (3) NOT NULL REFERENCES BRANCH (BRANCHID), FACULTYPASSWORD VARCHAR2(8));");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
    sql = strdup("CREATE TABLE STUDENT(STUDENTID VARCHAR2 (8) PRIMARY KEY, STUDENTNAME  VARCHAR2 (40) NOT NULL, YEARJOINED VARCHAR2 (4) NOT NULL, SEMESTER NUMBER NOT NULL, SECTION VARCHAR2 (10) NOT NULL REFERENCES SECTION (SECTIONID), STUDYING VARCHAR2 (1) NOT NULL, CONTACTNUMBER NUMBER(10), FIRSTSEM NUMBER (7,2), SECONDSEM NUMBER (7,2), THIRDSEM NUMBER (7,2), FOURTHSEM NUMBER (7,2), FIFTHSEM NUMBER (7,2), SIXTHSEM NUMBER (7,2), SEVENTHSEM NUMBER (7,2), EIGHTHSEM NUMBER (7,2), DEPTNO VARCHAR2 (3) NOT NULL REFERENCES BRANCH (BRANCHID), STUDENTPASSWORD VARCHAR2(8));");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
    sql = strdup("CREATE TABLE COURSE(COURSEID VARCHAR2 (10), COURSECODE VARCHAR2 (10) NOT NULL, COURSETYPE VARCHAR2 (1) NOT NULL, COURSENAME VARCHAR2 (30) NOT NULL, CREDITS NUMBER, SECTION VARCHAR2 (10) REFERENCES SECTION (SECTIONID), CRSTCHR VARCHAR2 (6) REFERENCES FACULTY (FACULTYID), DEPTNO VARCHAR2 (3) NOT NULL REFERENCES BRANCH (BRANCHID), PRIMARY KEY (COURSEID,SECTION));");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
    sql = strdup("CREATE TABLE GRADEREPORT(COURSEID VARCHAR2 (10) NOT NULL REFERENCES COURSE (COURSEID), STUDENTID VARCHAR2 (8) NOT NULL REFERENCES STUDENT (STUDENTID), SECTIONID VARCHAR2(10) NOT NULL REFERENCES SECTION (SECTIONID), M1 NUMBER, A1 NUMBER, MSE1 NUMBER, M2 NUMBER, A2 NUMBER, MSE2 NUMBER, INTERNALS NUMBER, EXTERNALS NUMBER, GRADE VARCHAR2 (1), GRADEPOINT INTEGER, PRIMARY KEY (COURSEID, STUDENTID))");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
    sql = strdup("INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('CS', '   CSE', 'Computer Science & Engineering', '7633301122'); \
    INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('AI', '   CSM', 'Computer Science & Engineering (Artificial Intelligence & Machine Learning)', '9000128377'); \
    INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('CN', '   CSN', 'Computer Science & Engineering (Networks)', '8639901325'); \
    INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('IN', '   CSO', 'Computer Science & Engineering (Internet Of Things)', '9899635464'); \
    INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('IT', '    IT', 'Information Technology', '9007636116'); \
    INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('EC', '   ECE', 'Electronics & Communication Engineering', '8113911440'); \
    INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('EE', '   EEE', 'Electrical & Electronics Engineering', '9443849990'); \
    INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('CI', '   ECI', 'Electronics Communication & Instrumentation', '9066333055'); \
    INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('ME', '  MECH', 'Mechanical Engineering', '90898441300'); \
    INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('CE', '   CIV', 'Civil Engineering', '9666263740'); \
    INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('MH', '  MATH', 'Mathematics', '8634401872'); \
    INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('PS', '   SCI', 'Physical Sciences', '8634203872'); \
	INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('EN', '   ENG', 'English', '9774340001'); \
    INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('PE', '  PE/S', 'Physical Education & Sports', '9774340942');");
	rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
    sql = strdup("INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1CSE1',1,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2CSE1',2,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3CSE1',3,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4CSE1',4,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5CSE1',5,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6CSE1',6,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7CSE1',7,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8CSE1',8,'CS'); \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1CSE2',1,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2CSE2',2,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3CSE2',3,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4CSE2',4,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5CSE2',5,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6CSE2',6,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7CSE2',7,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8CSE2',8,'CS'); \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1CSE3',1,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2CSE3',2,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3CSE3',3,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4CSE3',4,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5CSE3',5,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6CSE3',6,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7CSE3',7,'CS'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8CSE3',8,'CS'); \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1CSM1',1,'AI'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2CSM1',2,'AI'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3CSM1',3,'AI'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4CSM1',4,'AI'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5CSM1',5,'AI'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6CSM1',6,'AI'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7CSM1',7,'AI'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8CSM1',8,'AI'); \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1CSN1',1,'CN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2CSN1',2,'CN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3CSN1',3,'CN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4CSN1',4,'CN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5CSN1',5,'CN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6CSN1',6,'CN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7CSN1',7,'CN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8CSN1',8,'CN'); \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1CSO1',1,'IN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2CSO1',2,'IN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3CSO1',3,'IN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4CSO1',4,'IN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5CSO1',5,'IN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6CSO1',6,'IN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7CSO1',7,'IN'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8CSO1',8,'IN'); \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1IT1',1,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2IT1',2,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3IT1',3,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4IT1',4,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5IT1',5,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6IT1',6,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7IT1',7,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8IT1',8,'IT');  \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1IT2',1,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2IT2',2,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3IT2',3,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4IT2',4,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5IT2',5,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6IT2',6,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7IT2',7,'IT');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8IT2',8,'IT');  \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1ECE1',1,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2ECE1',2,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3ECE1',3,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4ECE1',4,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5ECE1',5,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6ECE1',6,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7ECE1',7,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8ECE1',8,'EC'); \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1ECE2',1,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2ECE2',2,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3ECE2',3,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4ECE2',4,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5ECE2',5,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6ECE2',6,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7ECE2',7,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8ECE2',8,'EC'); \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1ECE3',1,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2ECE3',2,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3ECE3',3,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4ECE3',4,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5ECE3',5,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6ECE3',6,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7ECE3',7,'EC'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8ECE3',8,'EC'); \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1EEE1',1,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2EEE1',2,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3EEE1',3,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4EEE1',4,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5EEE1',5,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6EEE1',6,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7EEE1',7,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8EEE1',8,'EE'); \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1EEE2',1,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2EEE2',2,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3EEE2',3,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4EEE2',4,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5EEE2',5,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6EEE2',6,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7EEE2',7,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8EEE2',8,'EE'); \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1ECI1',1,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2ECI1',2,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3ECI1',3,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4ECI1',4,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5ECI1',5,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6ECI1',6,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7ECI1',7,'EE'); \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8ECI1',8,'EE'); \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1CE1',1,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2CE1',2,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3CE1',3,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4CE1',4,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5CE1',5,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6CE1',6,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7CE1',7,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8CE1',8,'CE');  \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1CE2',1,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2CE2',2,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3CE2',3,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4CE2',4,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5CE2',5,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6CE2',6,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7CE2',7,'CE');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8CE2',8,'CE');  \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1ME1',1,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2ME1',2,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3ME1',3,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4ME1',4,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5ME1',5,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6ME1',6,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7ME1',7,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8ME1',8,'ME');  \
    INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  1ME2',1,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  2ME2',2,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  3ME2',3,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  4ME2',4,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  5ME2',5,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  6ME2',6,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  7ME2',7,'ME');  \ INSERT INTO SECTION (SECTIONID,SEMESTER,DEPTNO) VALUES ('  8ME2',8,'ME');");
	rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
	sql = strdup("INSERT INTO COURSE (COURSEID,COURSECODE,COURSETYPE,COURSENAME,CREDITS,SECTION,DEPTNO) VALUES ('U18OE401A','AM','T','APPLICABLE MATHEMATICS',4,'  4IT1','MH'); \
	INSERT INTO COURSE (COURSEID,COURSECODE,COURSETYPE,COURSENAME,CREDITS,SECTION,DEPTNO) VALUES ('U18TP402','SIS','P','SOFT AND INTERPERSONAL SKILLS',1,'  4IT1','MH'); \
	INSERT INTO COURSE (COURSEID,COURSECODE,COURSETYPE,COURSENAME,CREDITS,SECTION,DEPTNO) VALUES ('U18OE403E','MP','T','MICRO PROCESSORS',3,'  4IT1','ECE'); \
	INSERT INTO COURSE (COURSEID,COURSECODE,COURSETYPE,COURSENAME,CREDITS,SECTION,DEPTNO) VALUES ('U18IT404','TOC','T','THEORY OF COMPUTATION',3,'  4IT1','IT'); \
	INSERT INTO COURSE (COURSEID,COURSECODE,COURSETYPE,COURSENAME,CREDITS,SECTION,DEPTNO) VALUES ('U18IT405','DBMS','T','DATABASE MANAGEMENT SYSTEMS',4,'  4IT1','IT'); \
	INSERT INTO COURSE (COURSEID,COURSECODE,COURSETYPE,COURSENAME,CREDITS,SECTION,DEPTNO) VALUES ('U18IT406','JP','T','JAVA PROGRAMMING',3,'  4IT1','IT'); \
	INSERT INTO COURSE (COURSEID,COURSECODE,COURSETYPE,COURSENAME,CREDITS,SECTION,DEPTNO) VALUES ('U18IT407','JPLAB','P','JAVA PROGRAMMING LAB',1,'  4IT1','IT'); \
	INSERT INTO COURSE (COURSEID,COURSECODE,COURSETYPE,COURSENAME,CREDITS,SECTION,DEPTNO) VALUES ('U18IT408','DBMSLAB','P','DATABASE MANAGEMENT SYSTEMS LAB',1,'  4IT1','IT'); \
	INSERT INTO COURSE (COURSEID,COURSECODE,COURSETYPE,COURSENAME,CREDITS,SECTION,DEPTNO) VALUES ('U18OE411','MPLAB','P','MICROPROCESSORS LAB',1,'  4IT1','ECE');");
	rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
}

void database_creation_function(){
    sqlite3 *db;
    sqlite3_open("SAMS.db", &db);
    table_creation_function(db);
    sqlite3_close(db);
}

// MAIN FUNCTION
int main(){
    database_creation_function();
	main_menu();
}
