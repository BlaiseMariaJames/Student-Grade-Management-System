#include<ctime>
#include<sstream>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<sqlite3.h>
using namespace std;

int gbl_data = 0;
bool roc = false;
string excp = "";
string gbl_password = "1234";
char dept[12][4] = {"CSE","CSM","CSN","CSO"," IT","ECE","EEE","ECI","CIV","MEC","EMH","ENG"};
char dept_no[12][4] = {"CS","AI","CN","IN","IT","EC","EE","CI","CE","ME","MH","EN"};

class student{
    protected:
	int gradepoint[10];
	char grade[10], roll_number[20];
	float m1[10], a1[10], ms1[10], m2[10], a2[10], ms2[10], internal_marks[10], external_marks[10];
	float resultm1[10], resulta1[10], resultms1[10], resultm2[10], resulta2[10], resultms2[10], finalmse[10], final_marks[10], final_gradepoint[10];
};

class class_teacher : public student{
	protected:
	string teacher_id, teacher_password;
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
    string student_id, student_password, subject;
	public:
	int login_student();
	void student_main_menu(section &s);
	void view_marks();
	void view_attendance();
	void view_account(string student_id);
	void update_account(string student_id);
};

class course_teacher : public class_teacher{
	protected:
	string teacher_id, teacher_password, subject;
	public:
	int login_teacher();
	void teacher_main_menu(course_teacher &t);
	void add_student_marks();
	void view_overall_marks();
	void view_account(string faculty_id);
	void update_account(string faculty_id);
};

class counsellor{
};

class admin{
	string id, password;
	public:
	void login_master();
	void add_faculty();
	void update_faculty();
	void shift_faculty();
	void delete_faculty();
	void view_faculty();
    void master_faculty_edit_menu(admin &a);
    void assign_course();
    void remove_course();
    void view_course();
    void master_assign_course_menu(admin &a);
    void assign_counsellor();
    void remove_counsellor();
    void view_counsellor();
    void master_assign_counsellor_menu(admin &a);
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
    void shift_student();
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

void clear_screen(){
	system("PAUSE");
	system("CLS");
}

void error_message(){
	cout << "\nChoose a Valid Input (ERROR : Input Data Type or Range Mismatch)\n\n";
	clear_screen();
	return;
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
    return 0;
}

void set_foreignkeys(sqlite3 *db){
    int rc;
    char *sql;
    sql = strdup("PRAGMA FOREIGN_KEYS = ON;");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
}

void select_branch_function(){
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
	cout << "Type '12' ----> ENG\n";
	cout << "\nEnter Here : ";
}

void select_branch_insert_function(int i, int n, string str1, string str2, int &dept_id){
	a: select_branch_function();
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
    if(dept_id>12 || dept_id<1){
    goto b;
	}
}

void select_branch_view_function(int &dept_id){
	a: select_branch_function();
    cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	goto a;
	}
	dept_id = stoi(excp);
    if(dept_id>12 || dept_id<1){
    goto b;
	}
}

void view_faculty_function(string faculty_deptno){
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
}

void view_student_function(string student_deptno){
    int rc = 0;
    sqlite3 *db;
	string str = "";
	char *zErrMsg, *sql;
	sqlite3_open("SAMS.db", &db);
    string search_student = "SELECT COUNT(*) FROM STUDENT WHERE DEPTNO = '" + student_deptno + "';";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, search_table, 0, &zErrMsg);
    sqlite3_close(db);
}

void view_table(string str, string id){
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
    view = "SELECT FACULTYID,FACULTYNAME,QUALIFICATION,DESIGNATION,RESEARCHAREA from FACULTY WHERE DEPTNO = '" + id + "' ORDER BY " + (str + "ID");
    else
    view = "SELECT STUDENTID,STUDENTNAME from STUDENT WHERE DEPTNO = '" + id + "' ORDER BY " + (str + "ID");
	const char *line = view.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    sqlite3_close(db);
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

void create_padded_string(int offset_index, string str, string &old_offset_string, string &new_offset_string){
	if(offset_index<10)
    old_offset_string = str + "00" + to_string(offset_index);
    else if(offset_index<100)
    old_offset_string = str + "0" + to_string(offset_index);
    else if(offset_index<1000)
    old_offset_string =  str + to_string(offset_index);
    if(offset_index - 1 <10)
    new_offset_string = str + "00" + to_string(offset_index - 1);
    else if(offset_index - 1 <100)
    new_offset_string = str + "0" + to_string(offset_index - 1);
    else if(offset_index - 1 <1000)
    new_offset_string = str + to_string(offset_index - 1);
}

void create_lesser_padded_string(int old_offset_index, string str, string &old_offset_string, string &new_offset_string){
    if(old_offset_index<10)
    old_offset_string = str + "00" + to_string(old_offset_index);
    else if(old_offset_index<100)
    old_offset_string = str + "0" + to_string(old_offset_index);
    else if(old_offset_index<1000)
    old_offset_string =  str + to_string(old_offset_index);
    if(old_offset_index - 1 <10)
    new_offset_string = str + "00" + to_string(old_offset_index - 1);
    else if(old_offset_index - 1 <100)
    new_offset_string = str + "0" + to_string(old_offset_index - 1);
    else if(old_offset_index - 1 <1000)
    new_offset_string = str + to_string(old_offset_index - 1);
}

void create_greater_padded_string(int old_offset_index, string str, string &old_offset_string, string &new_offset_string){
    if(old_offset_index<10)
    old_offset_string = str + "00" + to_string(old_offset_index);
    else if(old_offset_index<100)
    old_offset_string = str + "0" + to_string(old_offset_index);
    else if(old_offset_index<1000)
    old_offset_string = str + to_string(old_offset_index);
    if(old_offset_index + 1 <10)
    new_offset_string = str + "00" + to_string(old_offset_index + 1);
    else if(old_offset_index + 1 <100)
    new_offset_string = str + "0" + to_string(old_offset_index + 1);
    else if(old_offset_index + 1 <1000)
    new_offset_string = str + to_string(old_offset_index + 1);
}

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
    int rc, n, dept_id;
    sqlite3_open("SAMS.db", &db);
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
	string insert_faculty = "INSERT INTO FACULTY VALUES ('" + faculty_id + "', '" + faculty_name + "', '" + faculty_qualification + "', '" + faculty_designation + "', '" + faculty_researcharea + "', '" + faculty_deptno + "', '" + gbl_password + "');";
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

int update_faculty_function(string &faculty_id, int id, string faculty_deptno){
    int rc = 1;
    sqlite3 *db;
	char *zErrMsg, *sql;
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno);
	rc = sqlite3_open("SAMS.db", &db);
    cout << "\n\nEnter the ID to be updated : ";
    cin >> faculty_id;
    string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ faculty_id +"' AND DEPTNO = '"+ faculty_deptno +"');";
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
	select_branch_view_function(dept_id);
	int id = dept_id - 1;
	string faculty_deptno = dept_no[id];
    system("CLS");
	view_faculty_function(faculty_deptno);
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

void admin :: shift_faculty(){
	sqlite3 *db;
	char *zErrMsg, *sql;
    int rc = 0, dept_id = 0;
    select_branch_view_function(dept_id);
    string old_faculty_id, new_faculty_id;
	int id = dept_id - 1;
	string faculty_deptno = dept_no[id];
    system("CLS");
	view_faculty_function(faculty_deptno);
	if(gbl_data == 0){
    cout << "\nERROR: No Faculty Details Found...." << endl;
	cout << endl;
	clear_screen();
	return;
	}
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno);
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be shifted : ";
	cin >> old_faculty_id;
	string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ old_faculty_id +"' AND DEPTNO = '"+ faculty_deptno +"');";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    sqlite3_close(db);
    cout << "\nFaculty with requested ID doesn't exist..." << endl;
    cout << "\nUnable to shift requested details of faculty... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    cout << "\nEnter the id corresponding to the location where " << old_faculty_id << " is to be shifted : ";
	cin >> new_faculty_id;
	search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ new_faculty_id +"' AND DEPTNO = '"+ faculty_deptno +"');";
    line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    sqlite3_close(db);
    cout << "\nFaculty with requested ID doesn't exist..." << endl;
    cout << "\nUnable to shift requested details of faculty... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    string old_offset_string = old_faculty_id.substr(2,3);
    string new_offset_string = new_faculty_id.substr(2,3);
    int old_offset_index = stoi(old_offset_string);
    int new_offset_index = stoi(new_offset_string);
	if(old_offset_index == new_offset_index){
	sqlite3_close(db);
    cout << "\nRequested details shifted successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
	else if(old_offset_index > new_offset_index){
    old_offset_index--;
	sqlite3_open("SAMS.db", &db);
    string update_faculty = "UPDATE FACULTY set FACULTYID = 'TEMP' WHERE FACULTYID = '"+ old_faculty_id +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	string update_hod = "UPDATE BRANCH set HOD = 'TEMP' WHERE HOD = '"+ old_faculty_id +"';";
    line = update_hod.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	string update_clstchr = "UPDATE SECTION set CLSTCHR = 'TEMP' WHERE CLSTCHR = '"+ old_faculty_id +"';";
    line = update_clstchr.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    while(old_offset_index>=new_offset_index){
    create_greater_padded_string(old_offset_index,faculty_deptno,old_offset_string,new_offset_string);
    string update_faculty = "UPDATE FACULTY set FACULTYID = '"+ new_offset_string +"' WHERE FACULTYID = '"+ old_offset_string +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    string update_hod = "UPDATE BRANCH set HOD = '"+ new_offset_string +"' WHERE HOD = '"+ old_offset_string +"';";
    line = update_hod.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	string update_clstchr = "UPDATE SECTION set CLSTCHR = '"+ new_offset_string +"' WHERE CLSTCHR = '"+ old_offset_string +"';";
    line = update_clstchr.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    old_offset_index--;
    }
    update_faculty = "UPDATE FACULTY set FACULTYID = '"+ new_faculty_id +"' WHERE FACULTYID = 'TEMP';";
    line = update_faculty.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	update_hod = "UPDATE BRANCH set HOD = '"+ new_faculty_id +"' WHERE HOD = 'TEMP';";
    line = update_hod.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    update_clstchr = "UPDATE SECTION set CLSTCHR = '"+ new_faculty_id +"' WHERE CLSTCHR = 'TEMP';";
    line = update_clstchr.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested details shifted successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
	else if(old_offset_index < new_offset_index){
    old_offset_index++;
	sqlite3_open("SAMS.db", &db);
    string update_faculty = "UPDATE FACULTY set FACULTYID = 'TEMP' WHERE FACULTYID = '"+ old_faculty_id +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	string update_hod = "UPDATE BRANCH set HOD = 'TEMP' WHERE HOD = '"+ old_faculty_id +"';";
    line = update_hod.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	string update_clstchr = "UPDATE SECTION set CLSTCHR = 'TEMP' WHERE CLSTCHR = '"+ old_faculty_id +"';";
    line = update_clstchr.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    while(old_offset_index<=new_offset_index){
    create_lesser_padded_string(old_offset_index,faculty_deptno,old_offset_string,new_offset_string);
    string update_faculty = "UPDATE FACULTY set FACULTYID = '"+ new_offset_string +"' WHERE FACULTYID = '"+ old_offset_string +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    string update_hod = "UPDATE BRANCH set HOD = '"+ new_offset_string +"' WHERE HOD = '"+ old_offset_string +"';";
    line = update_hod.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	string update_clstchr = "UPDATE SECTION set CLSTCHR = '"+ new_offset_string +"' WHERE CLSTCHR = '"+ old_offset_string +"';";
    line = update_clstchr.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    old_offset_index++;
    }
    update_faculty = "UPDATE FACULTY set FACULTYID = '"+ new_faculty_id +"' WHERE FACULTYID = 'TEMP';";
    line = update_faculty.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	update_hod = "UPDATE BRANCH set HOD = '"+ new_faculty_id +"' WHERE HOD = 'TEMP';";
    line = update_hod.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	update_clstchr = "UPDATE SECTION set CLSTCHR = '"+ new_faculty_id +"' WHERE CLSTCHR = 'TEMP';";
    line = update_clstchr.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested details shifted successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
}

void admin :: delete_faculty(){
    sqlite3 *db;
    string faculty_id;
	char *zErrMsg, *sql;
    int rc = 0, dept_id = 0;
    select_branch_view_function(dept_id);
	int id = dept_id - 1;
	string faculty_deptno = dept_no[id];
    system("CLS");
	view_faculty_function(faculty_deptno);
	if(gbl_data == 0){
    cout << "\nERROR: No Faculty Details Found...." << endl;
	cout << endl;
	clear_screen();
	return;
	}
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno);
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be deleted : ";
	cin >> faculty_id;
	string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ faculty_id +"' AND DEPTNO = '"+ faculty_deptno +"');";
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
    view_table("FACULTY", faculty_deptno);
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
    string old_offset_string = "";
    string new_offset_string = "";
    string offset_string = faculty_id.substr(2,3);
    int offset_index = stoi(offset_string);
    offset_index++;
    sqlite3_open("SAMS.db", &db);
    string delete_faculty = "DELETE from FACULTY where FACULTYID = '"+ faculty_id + "' AND DEPTNO = '" + faculty_deptno + "';";
    const char *line = delete_faculty.c_str();
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
    while(offset_index<=gbl_data){
    create_padded_string(offset_index,faculty_deptno,old_offset_string,new_offset_string);
    string update_faculty = "UPDATE FACULTY set FACULTYID = '"+ new_offset_string +"' WHERE FACULTYID = '"+ old_offset_string +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    string update_hod = "UPDATE BRANCH set HOD = '"+ new_offset_string +"' WHERE HOD = '"+ old_offset_string +"';";
    line = update_hod.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	string update_clstchr = "UPDATE SECTION set CLSTCHR = '"+ new_offset_string +"' WHERE CLSTCHR = '"+ old_offset_string +"';";
    line = update_clstchr.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    offset_index++;
    }
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
	select_branch_view_function(dept_id);
	int id = dept_id - 1;
	string faculty_deptno = dept_no[id];
	system("CLS");
	view_faculty_function(faculty_deptno);
	if(gbl_data == 0){
    cout << "\nERROR: No Faculty Details Found...." << endl;
	cout << endl;
	}
	else{
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno);
    cout << "\nDetails of all faculty(ies) of branch " << dept[id] << " displayed successfully..." << endl;
    cout << endl;
	}
    clear_screen();
    return;
}

void admin :: master_faculty_edit_menu(admin &a){
    int option = 0;
	while(option !=6){
	a: cout << "\nAccessing Faculty EDIT Operations...\n\n\n";
	cout << "Type '1' ----> Add Faculty\n";
	cout << "Type '2' ----> Update Faculty\n";
	cout << "Type '3' ----> Shift Faculty\n";
	cout << "Type '4' ----> Delete Faculty\n";
	cout << "Type '5' ----> View Faculty\n";
	cout << "Type '6' ----> Back to Main Menu\n";
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
	a.shift_faculty();
	}
	else if(option == 4){
	system("CLS");
	a.delete_faculty();
	}
	else if(option == 5){
	system("CLS");
	a.view_faculty();
	}
	}
	if(option == 6){
	cout << "Redirecting back...\n";
	clear_screen();
	return;
	}
}

void admin :: assign_course(){
}

void admin :: remove_course(){
}

void admin :: view_course(){
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
	//a.assign_course();
	}
	else if(option == 2){
	system("CLS");
	//a.remove_course();
	}
	else if(option == 3){
	system("CLS");
	//a.view_course();
	}
	}
	if(option == 4){
	cout << "Redirecting back...\n";
	clear_screen();
	return;
	}
}

void admin :: assign_counsellor(){
    sqlite3 *db;
    int rc, dept_id;
    string faculty_id;
    char *zErrMsg = 0, *sql;
	select_branch_view_function(dept_id);
	int id = dept_id - 1;
	string faculty_deptno = dept_no[id];
	string branch_id = dept[id];
	system("CLS");
	view_faculty_function(faculty_deptno);
	if(gbl_data == 0){
    cout << "\nERROR: No Faculty Details Found...." << endl;
	cout << endl;
	}
	else{
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno);
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be made Counsellor : ";
	cin >> faculty_id;
	string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ faculty_id +"' AND DEPTNO = '"+ faculty_deptno +"');";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nFaculty with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of faculty... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    }
    clear_screen();
    return;
}

void admin :: remove_counsellor(){
}

void admin :: view_counsellor(){
}

void admin :: master_assign_counsellor_menu(admin &a){
    int option = 0;
	while(option !=4){
	a: cout << "\nAccessing Faculty ASSIGN COUNSELLOR Operations...\n\n\n";
	cout << "Type '1' ----> Add Students\n";
	cout << "Type '2' ----> Remove Students\n";
	cout << "Type '3' ----> View Students\n";
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
	//a.assign_counsellor();
	}
	else if(option == 2){
	system("CLS");
	//a.remove_counsellor();
	}
	else if(option == 3){
	system("CLS");
	//a.view_counsellor();
	}
	}
	if(option == 4){
	cout << "Redirecting back...\n";
	clear_screen();
	return;
	}
}

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
    view_table("FACULTY", faculty_deptno);
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
    int rc, dept_id;
    char *zErrMsg = 0, *sql;
    string faculty_id, section_id;
	select_branch_view_function(dept_id);
	int id = dept_id - 1;
	string faculty_deptno = dept_no[id];
	string branch_id = dept[id];
	system("CLS");
	view_faculty_function(faculty_deptno);
	if(gbl_data == 0){
    cout << "\nERROR: No Faculty Details Found...." << endl;
	cout << endl;
	}
	else{
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno);
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be made Class Teacher : ";
	cin >> faculty_id;
	string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ faculty_id +"' AND DEPTNO = '"+ faculty_deptno +"');";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nFaculty with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of faculty... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    cout << "Enter the Section : ";
    cin >> section_id;
    section_id = "  " + section_id;
    string search_section = "SELECT EXISTS(SELECT * from SECTION WHERE SECTIONID = '"+  section_id +"');";
    line = search_section.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    cout << "\nSection with requested ID doesn't exist..." << endl;
    cout << "\nUnable to access requested details of section... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    check_clstchr(branch_id,faculty_deptno,faculty_id,section_id);
    cout << endl;
    clear_screen();
    return;
	}
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
    view_table("CLSTCHR", to_string(semester));
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
    view_table("CLSTCHR", to_string(semester));
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
    view_table("CLSTCHR", to_string(semester));
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
	select_branch_view_function(dept_id);
	int id = dept_id - 1;
	string faculty_deptno = dept_no[id];
	string branch_id = dept[id];
	system("CLS");
	view_faculty_function(faculty_deptno);
	if(gbl_data == 0){
    cout << "\nERROR: No Faculty Details Found...." << endl;
	cout << endl;
	}
	else{
    cout << "\nDisplaying Details of faculty(ies) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID    NAME\t\t\t\t      QUALIFICATION\t\t    DESIGNATION\t\t\t\t    RESEARCH AREA" << endl;
    view_table("FACULTY", faculty_deptno);
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be made Head of Department : ";
	cin >> faculty_id;
	string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ faculty_id +"' AND DEPTNO = '"+ faculty_deptno +"');";
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
    view_table("FACULTY", faculty_deptno);
	cout << "\n\nEnter the id to be made Head of Department : " << faculty_id << endl;
	goto a;
	}
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
    goto b;
	}
    if(choice == 1){
    sqlite3_open("SAMS.db", &db);
    set_foreignkeys(db);
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
    view_table("HOD", "");
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
    view_table("HOD", "");
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
    view_table("HOD", "");
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
	while(option !=5){
	a: cout << "\nAccessing Faculty ASSIGN Operations...\n\n\n";
	cout << "Type '1' ----> Assign a Course\n";
	cout << "Type '2' ----> Assign as Counsellor\n";
	cout << "Type '3' ----> Assign as Class Teacher\n";
	cout << "Type '4' ----> Assign as Head of Department\n";
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
	a.master_assign_course_menu(a);
	}
	else if(option == 2){
	system("CLS");
	a.master_assign_counsellor_menu(a);
	}
	else if(option == 3){
	system("CLS");
	a.master_assign_clstchr_menu(a);
	}
	else if(option == 4){
	system("CLS");
    a.master_assign_head_menu(a);
	}
	}
	if(option == 5){
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

void add_student_function(string &student_id, string &section, string year){
    int rc = 0;
    sqlite3 *db;
	string str = "";
	char *zErrMsg, *sql;
	sqlite3_open("SAMS.db", &db);
    string search_student = "SELECT COUNT(*) FROM STUDENT WHERE DEPTNO = '" + student_id + "' AND YEARJOINED = '"+ year +"';";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, search_table, 0, &zErrMsg);
    sqlite3_close(db);
	if(gbl_data == 0){
	section = section + "1";
    student_id = student_id + "001";
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
    student_id = student_id + str;
    if(gbl_data<61)
    section = section + "1";
    else if(gbl_data<121)
    section = section + "2";
    else if(gbl_data<181)
    section = section + "3";
    system("CLS");
    }
}

void admin :: add_student(){
	sqlite3 *db;
	sqlite3_open("SAMS.db", &db);
    int rc, n, sem, dept_id, yearjoined;
	char *zErrMsg, *sql, ch, name[40];
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
	select_branch_function();
	cout << dept[id] << endl;
	c: cout << "Enter Year Joined (>=2015): ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	d: error_message();
    cout << "Enter the number of student(s) : " << n << endl;
	cout << "\nEntering Details of " << n << " student(s)..." << endl;
	cout << "\nEntering Details of student " << i+1 << endl;
	select_branch_function();
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
	select_branch_function();
	cout << dept[id] << endl;
	cout << "Enter Year Joined : " << yearjoined << endl;
	goto e;
	}
	sem = stoi(excp);
    if(sem>8 || sem<1){
    goto f;
	}
	string student_sem = to_string(sem);
    add_student_function(branch_id, student_sec, student_yearjoined);
    student_sec = "  " + student_sem + student_sec;
    cout << student_sec << endl;
	cout << "Enter the number of student(s) : " << n << endl;
	cout << "\nEntering Details of " << n << " student(s)..." << endl;
	cout << "\nEntering Details of student " << i+1 << endl;
	cout << "\nEnter Branch : " << dept[id] << endl;
	cout << "Enter Year Joined : " << yearjoined << endl;
	cout << "Enter Semester : " << sem << endl;
	cout << "Enter Student Name : ";
	cin >> ch;
	int j=0;
	while (ch != '\n' && j<30){
    name[j] = ch;
    ch = cin.get();
    j++;
	}
	name[j] = '\0';
	padded_input_string(name, 29);
    string student_name = name;
    string year_offset = student_yearjoined.substr(2,2);
	string student_id = "B" + year_offset + branch_id;
    char char_student_yearjoined[student_yearjoined.size() + 1];
    strcpy(char_student_yearjoined, student_yearjoined.c_str());
	string string_student_academic_year = char_student_yearjoined;
    set_foreignkeys(db);
    string insert_student = "INSERT INTO STUDENT (STUDENTID,STUDENTNAME,YEARJOINED,SEMESTER,SECTION,DEPTNO,STUDENTPASSWORD) VALUES ('" + student_id + "', '" + student_name + "', '" + string_student_academic_year + "', " + student_sem + ", '" + student_sec + "', '" + student_deptno + "', '" + gbl_password + "');";
	const char *line = insert_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
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

int update_student_function(string &student_id, int id, string student_deptno){
    int rc = 1;
    sqlite3 *db;
	char *zErrMsg, *sql;
	cout << "\nDisplaying Details of student(s) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID       NAME" << endl;
    view_table("STUDENT", student_deptno);
	rc = sqlite3_open("SAMS.db", &db);
    cout << "\n\nEnter the ID to be updated : ";
    cin >> student_id;
    string search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ student_id +"' AND DEPTNO = '"+ student_deptno +"');";
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
    select_branch_view_function(dept_id);
	int id = dept_id - 1;
	string student_deptno = dept_no[id];
    system("CLS");
	view_student_function(student_deptno);
	if(gbl_data == 0){
    cout << "\nERROR: No Student Details Found...." << endl;
	cout << endl;
	clear_screen();
	return;
	}
	a: cout << "Selected Branch : " << dept[id] << endl;
    cout << "\nSelect the field to be updated...\n\n\n";
	cout << "Type '1' ----> Update Name\n";
	cout << "Type '2' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>2 || option<1){
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
	if(option == 2){
	cout << "Redirecting back...\n";
	clear_screen();
	return;
	}
}
}

void admin :: shift_student(){
    sqlite3 *db;
	char *zErrMsg, *sql;
    int rc = 0, dept_id = 0;
    select_branch_view_function(dept_id);
    string old_student_id, new_student_id;
	int id = dept_id - 1;
	string student_deptno = dept_no[id];
	string student_id = "B" + student_deptno;
    system("CLS");
	view_student_function(student_deptno);
	if(gbl_data == 0){
    cout << "\nERROR: No Student Details Found...." << endl;
	cout << endl;
	clear_screen();
	return;
	}
    cout << "\nDisplaying Details of student(s) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID       NAME" << endl;
    view_table("STUDENT", student_deptno);
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be shifted : ";
	cin >> old_student_id;
	string search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ old_student_id +"' AND DEPTNO = '"+ student_deptno +"');";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    sqlite3_close(db);
    cout << "\nStudent with requested ID doesn't exist..." << endl;
    cout << "\nUnable to shift requested details of Student... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    cout << "\nEnter the id corresponding to the location where " << old_student_id << " is to be shifted : ";
	cin >> new_student_id;
	search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ new_student_id +"' AND DEPTNO = '"+ student_deptno +"');";
    line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    if(rc == 0){
    sqlite3_close(db);
    cout << "\nStudent with requested ID doesn't exist..." << endl;
    cout << "\nUnable to shift requested details of student... Try Again using valid ID...\n" << endl;
    clear_screen();
    return;
    }
    string old_offset_string = old_student_id.substr(2,3);
    string new_offset_string = new_student_id.substr(2,3);
    int old_offset_index = stoi(old_offset_string);
    int new_offset_index = stoi(new_offset_string);
	if(old_offset_index == new_offset_index){
	sqlite3_close(db);
    cout << "\nRequested details shifted successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
	else if(old_offset_index > new_offset_index){
    old_offset_index--;
	sqlite3_open("SAMS.db", &db);
    set_foreignkeys(db);
    string update_student = "UPDATE STUDENT set STUDENTID = 'TEMP' WHERE STUDENTID = '"+ old_student_id +"';";
    const char *line = update_student.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    while(old_offset_index>=new_offset_index){
    create_greater_padded_string(old_offset_index,student_deptno,old_offset_string,new_offset_string);
    set_foreignkeys(db);
    string update_student = "UPDATE STUDENT set STUDENTID = '"+ new_offset_string +"' WHERE STUDENTID = '"+ old_offset_string +"';";
    const char *line = update_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    old_offset_index--;
    }
    set_foreignkeys(db);
    update_student = "UPDATE STUDENT set STUDENTID = '"+ new_student_id +"' WHERE STUDENTID = 'TEMP';";
    line = update_student.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested details shifted successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
	else if(old_offset_index < new_offset_index){
    old_offset_index++;
	sqlite3_open("SAMS.db", &db);
	set_foreignkeys(db);
    string update_student = "UPDATE STUDENT set STUDENTID = 'TEMP' WHERE STUDENTID = '"+ old_student_id +"';";
    const char *line = update_student.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    while(old_offset_index<=new_offset_index){
    create_lesser_padded_string(old_offset_index,student_deptno,old_offset_string,new_offset_string);
    set_foreignkeys(db);
    string update_student = "UPDATE STUDENT set STUDENTID = '"+ new_offset_string +"' WHERE STUDENTID = '"+ old_offset_string +"';";
    const char *line = update_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    old_offset_index++;
    }
    set_foreignkeys(db);
    update_student = "UPDATE STUDENT set STUDENTID = '"+ new_student_id +"' WHERE STUDENTID = 'TEMP';";
    line = update_student.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
	sqlite3_close(db);
    cout << "\nRequested details shifted successfully..." << endl;
    cout << endl;
    clear_screen();
    return;
	}
}

void admin :: delete_student(){
    sqlite3 *db;
    string student_id;
	char *zErrMsg, *sql;
	int rc = 0, dept_id = 0;
    select_branch_view_function(dept_id);
	int id = dept_id - 1;
	string student_deptno = dept_no[id];
    system("CLS");
	view_student_function(student_deptno);
	if(gbl_data == 0){
    cout << "\nERROR: No Student Details Found...." << endl;
	cout << endl;
	clear_screen();
	return;
	}
    cout << "\nDisplaying Details of student(s) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID       NAME" << endl;
    view_table("STUDENT", student_deptno);
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be deleted : ";
	cin >> student_id;
	string search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ student_id +"' AND DEPTNO = '"+ student_deptno +"');";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, exist_table, 0, &zErrMsg);
    sqlite3_close(db);
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
    cout << "\n\nID       NAME" << endl;
    view_table("STUDENT", student_deptno);
    cout << "\n\nEnter the id to be deleted : " << student_id << endl;
	goto a;
	}
    sqlite3_close(db);
	int choice = stoi(excp);
	if(choice!=1 && choice!=2){
	goto b;
	}
	if(choice == 1){
    string old_offset_string = "";
    string new_offset_string = "";
    string offset_string = student_id.substr(2,3);
    int offset_index = stoi(offset_string);
    offset_index++;
    sqlite3_open("SAMS.db", &db);
    set_foreignkeys(db);
    string delete_student = "DELETE from STUDENT where STUDENTID = '"+ student_id + "' AND DEPTNO = '" + student_deptno + "';";
    const char *line = delete_student.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    while(offset_index<=gbl_data){
    create_padded_string(offset_index,student_deptno,old_offset_string,new_offset_string);
    set_foreignkeys(db);
    string update_student = "UPDATE STUDENT set STUDENTID = '"+ new_offset_string +"' WHERE STUDENTID = '"+ old_offset_string +"';";
    const char *line = update_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    offset_index++;
    }
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
    select_branch_view_function(dept_id);
	int id = dept_id - 1;
	string student_deptno = dept_no[id];
	system("CLS");
	view_student_function(student_deptno);
	if(gbl_data == 0){
    cout << "\nERROR: No Student Details Found...." << endl;
	cout << endl;
	}
	else{
    cout << "\nDisplaying Details of student(s) of branch " << dept[id] << "..."<< endl;
    cout << "\n\nID       NAME" << endl;
    view_table("STUDENT", student_deptno);
    cout << "\nDetails of all student(s) of branch " << dept[id] << " displayed successfully..." << endl;
    cout << endl;
	}
    clear_screen();
    return;
}

void admin :: master_student_menu(admin &a){
	int option = 0;
	while(option !=6){
	a: cout << "\nAccessing Student Operations...\n\n\n";
	cout << "Type '1' ----> Add Student\n";
	cout << "Type '2' ----> Update Student\n";
	cout << "Type '3' ----> Shift Student\n";
	cout << "Type '4' ----> Delete Student\n";
	cout << "Type '5' ----> View Student\n";
	cout << "Type '6' ----> Back to Main Menu\n";
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
	a.add_student();
	}
	else if(option == 2){
	system("CLS");
	a.update_student();
	}
	else if(option == 3){
	system("CLS");
	a.shift_student();
	}
	else if(option == 4){
	system("CLS");
	a.delete_student();
	}
	else if(option == 5){
	system("CLS");
	a.view_student();
	}
	}
	if(option == 6){
	cout << "Redirecting back to Admin Page\n";
	clear_screen();
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
	b : error_message();
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
	clear_screen();
	return;
	}
}

int course_teacher :: login_teacher(){
    sqlite3 *db;
    int rc, count = 3;
    string faculty_deptno;
    char *zErrMsg = 0, *sql;
    rc = sqlite3_open("SAMS.db", &db);
	string search_faculty = "SELECT EXISTS(SELECT * from FACULTY);";
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
	search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ teacher_id +"' AND FACULTYPASSWORD = '"+ teacher_password +"');";
    line = search_faculty.c_str();
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

void course_teacher :: view_account(string faculty_id){
    int rc;
    sqlite3 *db;
	char *zErrMsg, *sql;
	rc = sqlite3_open("SAMS.db", &db);
    string search_faculty = "SELECT FACULTYNAME,QUALIFICATION,DESIGNATION,RESEARCHAREA from FACULTY WHERE FACULTYID = '" + faculty_id + "';";
    const char *line = search_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_faculty_account, 0, &zErrMsg);
    cout << "\n\n" << endl;
    sqlite3_close(db);
    return;
}

void course_teacher :: teacher_main_menu(course_teacher &t){
	int option = 0;
	while(option !=4){
    a: cout << "\nWelcome " << teacher_id << "\n\n\n";
	cout << "Personal Info : " << endl;
	t.view_account(teacher_id);
    cout << "Type '1' ----> Edit Student Marks\n";
    cout << "Type '2' ----> View Overall Marks\n";
	cout << "Type '3' ----> Update My Account/Password\n";
	cout << "Type '4' ----> Back to Main Menu\n";
    cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>4 || option<1){
    goto b;
	}
    else if(option == 1){
    system("CLS");
    //t.add_student_marks();
    }
    else if(option == 2){
    system("CLS");
    //t.view_overall_marks();
    }
    else if(option == 3){
    //t.update_account(teacher_id);
    system("CLS");
    }
	}
	if(option == 4){
	cout << "Redirecting back to Main Menu\n";
	clear_screen();
	return;
	}
}

int section :: login_student(){
	sqlite3 *db;
    int rc, count = 3;
    string student_deptno;
    char *zErrMsg = 0, *sql;
    rc = sqlite3_open("SAMS.db", &db);
	string search_student = "SELECT EXISTS(SELECT * from STUDENT);";
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
	search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ student_id +"' AND STUDENTPASSWORD = '"+ student_password +"');";
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

void section :: view_account(string student_id){
    int rc;
    sqlite3 *db;
	char *zErrMsg, *sql;
	rc = sqlite3_open("SAMS.db", &db);
    string search_student = "SELECT STUDENTNAME from STUDENT WHERE STUDENTID = '" + student_id + "';";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, view_student_account, 0, &zErrMsg);
    cout << "\n\n" << endl;
    sqlite3_close(db);
    return;
}

void section :: student_main_menu(section &s){
	int option = 0;
	while(option !=4){
	a: cout << "\nWelcome " << student_id << "\n\n\n";
    cout << "Personal Info : " << endl;
	s.view_account(student_id);
	cout << "Type '1' ----> View My Marks\n";
	cout << "Type '2' ----> View My Attendance\n";
	cout << "Type '3' ----> Update My Account/Password\n";
	cout << "Type '4' ----> Back to Main Menu\n";
	cout << "\nEnter Here : ";
	cin >> excp;
	roc = check_exception(excp);
	while(roc){
	b: error_message();
	goto a;
	}
	option = stoi(excp);
	if(option>4 || option<1){
    goto b;
	}
	else if(option == 1){
	system("CLS");
	//s.view_marks();
	}
	else if(option == 2){
	system("CLS");
	//s.view_attendance();
	}
	else if(option == 3){
    system("CLS");
	//s.update_account(student_id);
	}
	}
	if(option == 4){
	cout << "Redirecting back to Main Menu\n";
	clear_screen();
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
	cout << "Redirecting to Faculty Login Page\n";
	clear_screen();
	course_teacher t;
	int exist = t.login_teacher();
	if(exist!=-1)
	t.teacher_main_menu(t);
	}
	else if(option == 3){
	cout << "Redirecting to Student Login Page\n";
	clear_screen();
	section s;
	int exist = s.login_student();
	if(exist!=-1)
	s.student_main_menu(s);
	}
	}
	if(option == 4){
	exit(0);
	}
}

void table_creation_function(sqlite3 *db){
    int rc;
    char *sql, *zErrMsg = 0;
    sql = strdup("CREATE TABLE BRANCH(BRANCHID VARCHAR2 (3) PRIMARY KEY, BRANCHCODE VARCHAR2 (8) NOT NULL, BRANCHNAME  VARCHAR2 (40) NOT NULL, HOD VARCHAR2 (6) REFERENCES FACULTY (FACULTYID), PHONE VARCHAR2 (10) NOT NULL);");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
    sql = strdup("CREATE TABLE SECTION(SECTIONID VARCHAR2 (10) PRIMARY KEY, CLSTCHR VARCHAR2 (6) REFERENCES FACULTY (FACULTYID), DEPTNO VARCHAR2 (3) NOT NULL REFERENCES BRANCH (BRANCHID));");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
    sql = strdup("CREATE TABLE FACULTY(FACULTYID VARCHAR2(6) PRIMARY KEY, FACULTYNAME  VARCHAR2 (40) NOT NULL, QUALIFICATION VARCHAR2 (30), DESIGNATION VARCHAR2 (40), RESEARCHAREA VARCHAR2 (60), DEPTNO VARCHAR2 (3) NOT NULL REFERENCES BRANCH (BRANCHID), FACULTYPASSWORD VARCHAR2(8));");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
    sql = strdup("CREATE TABLE STUDENT(STUDENTID VARCHAR2 (8) PRIMARY KEY, STUDENTNAME  VARCHAR2 (40) NOT NULL, YEARJOINED VARCHAR2 (4) NOT NULL, SEMESTER NUMBER NOT NULL, SECTION VARCHAR2 (10) NOT NULL REFERENCES SECTION (SECTIONID), COUNSELLORID VARCHAR2(6) REFERENCES FACULTY (FACULTYID), FIRSTSEM NUMBER (7,2), SECONDSEM NUMBER (7,2), THIRDSEM NUMBER (7,2), FOURTHSEM NUMBER (7,2), FIFTHSEM NUMBER (7,2), SIXTHSEM NUMBER (7,2), SEVENTHSEM NUMBER (7,2), EIGHTHSEM NUMBER (7,2), DEPTNO VARCHAR2 (3) NOT NULL REFERENCES BRANCH (BRANCHID), STUDENTPASSWORD VARCHAR2(8));");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
    sql = strdup("CREATE TABLE COURSE(COURSEID VARCHAR2 (10), COURSECODE VARCHAR2 (10) NOT NULL, COURSETYPE VARCHAR2 (1) NOT NULL, COURSENAME VARCHAR2 (30) NOT NULL, SECTION VARCHAR2 (10) REFERENCES SECTION (SECTIONID), CRSTCHR VARCHAR2 (6) REFERENCES FACULTY (FACULTYID), DEPTNO VARCHAR2 (3) NOT NULL REFERENCES BRANCH (BRANCHID), PRIMARY KEY (COURSEID,SECTION));");
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
    INSERT INTO BRANCH (BRANCHID,BRANCHCODE,BRANCHNAME,PHONE) VALUES ('EN', '   ENG', 'English', '9774340001');");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
    sql = strdup("INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1CSE1','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2CSE1','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3CSE1','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4CSE1','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5CSE1','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6CSE1','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7CSE1','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8CSE1','CS'); \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1CSE2','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2CSE2','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3CSE2','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4CSE2','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5CSE2','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6CSE2','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7CSE2','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8CSE2','CS'); \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1CSE3','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2CSE3','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3CSE3','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4CSE3','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5CSE3','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6CSE3','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7CSE3','CS'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8CSE3','CS'); \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1CSM1','AI'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2CSM1','AI'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3CSM1','AI'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4CSM1','AI'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5CSM1','AI'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6CSM1','AI'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7CSM1','AI'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8CSM1','AI'); \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1CSN1','CN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2CSN1','CN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3CSN1','CN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4CSN1','CN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5CSN1','CN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6CSN1','CN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7CSN1','CN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8CSN1','CN'); \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1CSO1','IN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2CSO1','IN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3CSO1','IN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4CSO1','IN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5CSO1','IN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6CSO1','IN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7CSO1','IN'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8CSO1','IN'); \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1IT1','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2IT1','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3IT1','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4IT1','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5IT1','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6IT1','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7IT1','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8IT1','IT');  \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1IT2','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2IT2','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3IT2','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4IT2','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5IT2','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6IT2','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7IT2','IT');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8IT2','IT');  \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1ECE1','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2ECE1','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3ECE1','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4ECE1','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5ECE1','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6ECE1','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7ECE1','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8ECE1','EC'); \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1ECE2','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2ECE2','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3ECE2','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4ECE2','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5ECE2','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6ECE2','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7ECE2','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8ECE2','EC'); \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1ECE3','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2ECE3','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3ECE3','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4ECE3','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5ECE3','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6ECE3','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7ECE3','EC'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8ECE3','EC'); \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1EEE1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2EEE1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3EEE1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4EEE1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5EEE1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6EEE1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7EEE1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8EEE1','EE'); \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1EEE2','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2EEE2','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3EEE2','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4EEE2','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5EEE2','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6EEE2','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7EEE2','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8EEE2','EE'); \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1ECI1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2ECI1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3ECI1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4ECI1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5ECI1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6ECI1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7ECI1','EE'); \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8ECI1','EE'); \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1CE1','CE');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2CE1','CE');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3CE1','CE');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4CE1','CE');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5CE1','CE');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6CE1','CE');  \	INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7CE1','CE');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8CE1','CE');  \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1CE2','CE');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2CE2','CE');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3CE2','CE');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4CE2','CE');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5CE2','CE');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6CE2','CE');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7CE2','CE');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8CE2','CE');  \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1ME1','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2ME1','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3ME1','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4ME1','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5ME1','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6ME1','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7ME1','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8ME1','ME');  \
    INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  1ME2','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  2ME2','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  3ME2','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  4ME2','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  5ME2','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  6ME2','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  7ME2','ME');  \ INSERT INTO SECTION (SECTIONID,DEPTNO) VALUES ('  8ME2','ME');");
	rc = sqlite3_exec(db, sql, create_insert_table, 0, 0);
}

void database_creation_function(){
    sqlite3 *db;
    sqlite3_open("SAMS.db", &db);
    table_creation_function(db);
    sqlite3_close(db);
}

int main(){
    database_creation_function();
	main_menu();
}
