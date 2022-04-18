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
    printf("%s ", argv[i] ? argv[i] : "NULL");
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

static int display_student_table(void *NotUsed, int argc, char **argv, char **azColName){
	string gbl_input[100];
	for(int i = 0; i<argc; i++){
    gbl_input[i] = argv[i] ? argv[i] : "NULL";
    }
    cout << "Academic Year : " << gbl_input[0] << endl;
    cout << "Name : " << gbl_input[1] << endl;
    return 0;
}

static int display_faculty_table(void *NotUsed, int argc, char **argv, char **azColName){
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
	if(str == "FACULTY")
    view = "SELECT FACULTYID,FACULTYNAME,QUALIFICATION,DESIGNATION,RESEARCHAREA from " + str + " WHERE DEPTNO = '" + id + "' ORDER BY " + (str + "ID");
    else
    view = "SELECT STUDENTID,ACADEMICYEAR,STUDENTNAME from " + str + " WHERE DEPTNO = '" + id + "' ORDER BY " + (str + "ID");
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
    string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ faculty_id +"');";
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
	string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ old_faculty_id +"');";
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
	search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ new_faculty_id +"');";
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
    while(old_offset_index>=new_offset_index){
    create_greater_padded_string(old_offset_index,faculty_deptno,old_offset_string,new_offset_string);
    string update_faculty = "UPDATE FACULTY set FACULTYID = '"+ new_offset_string +"' WHERE FACULTYID = '"+ old_offset_string +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    old_offset_index--;
    }
    update_faculty = "UPDATE FACULTY set FACULTYID = '"+ new_faculty_id +"' WHERE FACULTYID = 'TEMP';";
    line = update_faculty.c_str();
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
    while(old_offset_index<=new_offset_index){
    create_lesser_padded_string(old_offset_index,faculty_deptno,old_offset_string,new_offset_string);
    string update_faculty = "UPDATE FACULTY set FACULTYID = '"+ new_offset_string +"' WHERE FACULTYID = '"+ old_offset_string +"';";
    const char *line = update_faculty.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    old_offset_index++;
    }
    update_faculty = "UPDATE FACULTY set FACULTYID = '"+ new_faculty_id +"' WHERE FACULTYID = 'TEMP';";
    line = update_faculty.c_str();
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
	string search_faculty = "SELECT EXISTS(SELECT * from FACULTY WHERE FACULTYID = '"+ faculty_id +"');";
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
    while(offset_index<=gbl_data){
    create_padded_string(offset_index,faculty_deptno,old_offset_string,new_offset_string);
    string update_faculty = "UPDATE FACULTY set FACULTYID = '"+ new_offset_string +"' WHERE FACULTYID = '"+ old_offset_string +"';";
    const char *line = update_faculty.c_str();
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

void admin :: master_faculty_menu(admin &a){
	int option = 0;
	while(option !=6){
	a: cout << "\nAccessing Faculty Operations...\n\n\n";
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
	cout << "Redirecting back to Admin Page\n";
	clear_screen();
	return;
	}
}

void add_student_function(string &student_id, string student_deptno){
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
	if(gbl_data == 0){
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
    system("CLS");
    }
}

void admin :: add_student(){
	sqlite3 *db;
    int rc, n, dept_id;
    sqlite3_open("SAMS.db", &db);
	char *zErrMsg, *sql, ch, name[40], academicyear[20];
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
    string student_id = dept_no[id];
    string student_deptno = dept_no[id];
    add_student_function(student_id, student_deptno);
	cout << "Enter the number of student(s) : " << n << endl;
	cout << "\nEntering Details of " << n << " student(s)..." << endl;
	cout << "\nEntering Details of student " << i+1 << endl;
	cout << "\nEnter Branch : " << dept[id] << endl;
	cout << "Enter Academic Year : ";
	cin >> ch;
	int j=0;
	while (ch != '\n' && j<20){
    academicyear[j] = ch;
    ch = cin.get();
    j++;
	}
	academicyear[j] = '\0';
	padded_input_string(academicyear, 19);
	cout << "Enter Student Name : ";
	cin >> ch;
	j=0;
	while (ch != '\n' && j<30){
    name[j] = ch;
    ch = cin.get();
    j++;
	}
	name[j] = '\0';
	padded_input_string(name, 29);
    string student_name = name;
    string student_academicyear = academicyear;
    string insert_student = "INSERT INTO STUDENT VALUES ('" + student_id + "', '" + student_name + "', '" + student_academicyear + "', '" + student_deptno + "', '" + gbl_password + "');";
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
    cout << "\n\nID    ACADEMIC YEAR       NAME" << endl;
    view_table("STUDENT", student_deptno);
	rc = sqlite3_open("SAMS.db", &db);
    cout << "\n\nEnter the ID to be updated : ";
    cin >> student_id;
    string search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ student_id +"');";
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
	cout << "Type '2' ----> Update Academic Year\n";
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
	char ch, academicyear[20];
    rc = update_student_function(student_id, id, student_deptno);
	if(rc == 0)
    return;
	else if(rc == 1){
    sqlite3_open("SAMS.db", &db);
    cout << "Enter the New Academic Year : ";
	cin >> ch;
	int j=0;
	while (ch != '\n' && j<20){
    academicyear[j] = ch;
    ch = cin.get();
    j++;
	}
	academicyear[j] = '\0';
	padded_input_string(academicyear, 19);
    string new_academicyear = academicyear;
	string update_student = "UPDATE STUDENT set ACADEMICYEAR = '"+ new_academicyear +"' WHERE STUDENTID = '"+ student_id +"';";
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

void admin :: shift_student(){
    sqlite3 *db;
	char *zErrMsg, *sql;
    int rc = 0, dept_id = 0;
    select_branch_view_function(dept_id);
    string old_student_id, new_student_id;
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
    cout << "\n\nID    ACADEMIC YEAR       NAME" << endl;
    view_table("STUDENT", student_deptno);
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be shifted : ";
	cin >> old_student_id;
	string search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ old_student_id +"');";
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
	search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ new_student_id +"');";
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
    string update_student = "UPDATE STUDENT set STUDENTID = 'TEMP' WHERE STUDENTID = '"+ old_student_id +"';";
    const char *line = update_student.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    while(old_offset_index>=new_offset_index){
    create_greater_padded_string(old_offset_index,student_deptno,old_offset_string,new_offset_string);
    string update_student = "UPDATE STUDENT set STUDENTID = '"+ new_offset_string +"' WHERE STUDENTID = '"+ old_offset_string +"';";
    const char *line = update_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    old_offset_index--;
    }
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
    string update_student = "UPDATE STUDENT set STUDENTID = 'TEMP' WHERE STUDENTID = '"+ old_student_id +"';";
    const char *line = update_student.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    while(old_offset_index<=new_offset_index){
    create_lesser_padded_string(old_offset_index,student_deptno,old_offset_string,new_offset_string);
    string update_student = "UPDATE STUDENT set STUDENTID = '"+ new_offset_string +"' WHERE STUDENTID = '"+ old_offset_string +"';";
    const char *line = update_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    old_offset_index++;
    }
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
    cout << "\n\nID    ACADEMIC YEAR       NAME" << endl;
    view_table("STUDENT", student_deptno);
    sqlite3_open("SAMS.db", &db);
	cout << "\n\nEnter the id to be deleted : ";
	cin >> student_id;
	string search_student = "SELECT EXISTS(SELECT * from STUDENT WHERE STUDENTID = '"+ student_id +"');";
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
    cout << "\n\nID    ACADEMIC YEAR       NAME" << endl;
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
    string delete_student = "DELETE from STUDENT where STUDENTID = '"+ student_id + "' AND DEPTNO = '" + student_deptno + "';";
    const char *line = delete_student.c_str();
    sql = strdup(line);
	rc = sqlite3_exec(db, sql, select_table, 0, &zErrMsg);
    while(offset_index<=gbl_data){
    create_padded_string(offset_index,student_deptno,old_offset_string,new_offset_string);
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
    cout << "\n\nID    ACADEMIC YEAR       NAME" << endl;
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
    rc = sqlite3_exec(db, sql, display_faculty_table, 0, &zErrMsg);
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
    string search_student = "SELECT ACADEMICYEAR,STUDENTNAME from STUDENT WHERE STUDENTID = '" + student_id + "';";
    const char *line = search_student.c_str();
    sql = strdup(line);
    rc = sqlite3_exec(db, sql, display_student_table, 0, &zErrMsg);
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
    char *zErrMsg, *sql;
    sql = strdup("PRAGMA FOREIGN_KEYS = ON;");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    }
    sql = strdup("CREATE TABLE FACULTY(FACULTYID VARCHAR2(6) PRIMARY KEY, FACULTYNAME  VARCHAR2 (40) NOT NULL, QUALIFICATION VARCHAR2 (30), DESIGNATION VARCHAR2 (40), RESEARCHAREA VARCHAR2 (60), DEPTNO VARCHAR2 (3) NOT NULL REFERENCES BRANCH (BRANCHID), FACULTYPASSWORD VARCHAR2(8));");
    rc = sqlite3_exec(db, sql, create_insert_table, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    }
    sql = strdup("CREATE TABLE STUDENT(STUDENTID VARCHAR2 (8) PRIMARY KEY, STUDENTNAME  VARCHAR2 (40) NOT NULL, ACADEMICYEAR VARCHAR2 (20) NOT NULL, DEPTNO VARCHAR2 (3) NOT NULL REFERENCES BRANCH (BRANCHID), STUDENTPASSWORD VARCHAR2(8));");
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
