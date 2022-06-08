# Student Grade Management System

_A SQLite3 based C/C++ Project on Student Grade Management System._

## Installation & Connection Guide

<b>Download Installation & Connection Guide [PDF Format] : </b>[Installation & Connection Guide](https://github.com/BlaiseMariaJames/Student-Grade-Management-System/files/10326903/Installation.Connection.Guide.pdf)

<br><h3 align="center"><b>Chapter 1: Installation of CodeBlocks IDE 20.03 with MinGW Setup</b></h3><br> 

For any assistance, please check out the video by **LearningLad** youtube channel on

[**How to Install CodeBlocks ( IDE 20.03 ) with MinGW for C and C++ Programming on Windows 10**](https://www.youtube.com/watch?v=GWJqsmitR2I)

<br>**STEP 1:** Installation

It is highly recommended to go with the **default installation** of the **mingw setup**.

**For Latest Updates** , follow official website: [https://www.codeblocks.org/downloads/binaries/](https://www.codeblocks.org/downloads/binaries/)

**For Windows 32 bit users:**

[https://drive.google.com/file/d/1\_eHTnrOXr2ckvJ7Y4avrYi9Ge4-lVzyQ/view?usp=sharing](https://drive.google.com/file/d/1_eHTnrOXr2ckvJ7Y4avrYi9Ge4-lVzyQ/view?usp=sharing)

**For Windows 64 bit users:**

[https://drive.google.com/file/d/1xAcbEATginqMja94OuzVVvbNCgCyvhxW/view?usp=sharing](https://drive.google.com/file/d/1xAcbEATginqMja94OuzVVvbNCgCyvhxW/view?usp=sharing)

**For other platform users/versions:** [https://www.codeblocks.org/downloads/binaries/](https://www.codeblocks.org/downloads/binaries/)

<br>**STEP 2:** Adding Path

Add the path to the **MinGW folder** in Code Blocks, **'C:\Program Files\CodeBlocks\MinGW\bin'** (default path or the path you mentioned while installation) in the **Edit System Environment Variables** in **System Settings**.

<br>**STEP 3:** Verify

Open CMD, and enter the command, **"gcc -- version"** , if you see the version being specified, you're good to go.

<br><h3 align="center"><b>Chapter 2: Installation of SQLite3</b></h3><br>

<br>**STEP 1:** Copying Folder

Copy the **sqlite3\_folder** from the **Student Grade Management System** Project Folderto C Drive.

<br>**STEP 2:** Adding Path

Add the path to the **sqlite\_tools folder** in C Drive, **'C:\sqlite3\_folder\sqlite3\_tools'** in the **Edit System Environment Variables** in **System Settings**.

<br>**STEP 3:** Verify

Open CMD, and enter the command, **"sqlite3 --version",** if you see the version being specified, you're good to go.

<br><h3 align="center"><b>Chapter 3: Connecting SQLite3 to CodeBlocks IDE</b></h3><br>

<br>**STEP 1:** Adding .a file to CodeBlocks Linker

Open CodeBlocks, click on Settings, and select **Compiler**. Under the **'Linker Settings'** tab, add the following path **'C:\sqlite3\_folder\sqlite3\_amalgamation\libcpptosqlite.a'** and click on **OK**.

<p align="center"><img src="https://user-images.githubusercontent.com/100013400/210125677-d543acf0-4d43-4bab-90b4-e8de666da313.png" width="500px" height="400px"></p>

<br>**STEP 2:** Adding sqlite3\_folder paths to CodeBlocks Search Directories

Open CodeBlocks, click on Settings, and select **Compiler**. Under the **'Search Directories'** tab, select the **'Compiler'** taband add the following path **'C:\sqlite3\_folder\sqlite3\_amalgamation'** and click on **OK**.

<p align="center"><img src="https://user-images.githubusercontent.com/100013400/210125874-f1a06fb4-bc00-4f49-830a-c0cd9ca92df7.png" width="500px" height="400px"></p>

Open CodeBlocks, click on Settings, and select **Compiler**. Under the **'Search Directories'** tab, select the **'Linker'** tab and add the following path **'C:\sqlite3\_folder'** and click on **OK**.

<p align="center"><img src="https://user-images.githubusercontent.com/100013400/210125889-111905d4-b963-4eb7-be89-d9cda0ef5c3f.png" width="500px" height="400px"></p>

<br><h3 align="center"><b>Chapter 4: Executing the Program</b></h3><br>

Open **Student Grade Management System** Project Folder, right click on **"Student Grade Management System (Module 1) v1.cpp"** and open with **"CodeBlocks IDE".**

Click on Build, and select **Build and Run (F9)**, if you see the following output, you're good to go.

<p align="center"><img src="https://user-images.githubusercontent.com/100013400/210125897-04195a25-f557-44f8-94f3-2013a8399899.png" width="600px" height="400px"></p>

By following the above instructions, we were successfully able to connect our CodeBlocks IDE with the SQLite3 database and were successfully able to run our Project Source Code.

<br>**References followed:**

1. **Installation of Code Blocks:**

&emsp; &ensp; Video by **LearningLad** Youtube Channel:

&emsp; &ensp; [**How to Install CodeBlocks ( IDE 20.03 ) with MinGW for C and C++ Programming on Windows 10**](https://www.youtube.com/watch?v=GWJqsmitR2I)

2. **Installation of SQLite3 and connecting it to CodeBlocks:**

&emsp; &ensp; Video by **Channa Bou** Youtube Channel : [**01 Config SQLite db and CodeBlock**](https://www.youtube.com/watch?v=Y_Zl6VlQQ6M&list=LL&index=1)

&emsp; &ensp; Online Reference for SQLite3 by **Tutorialspoint** : [**SQLite - C/C++**](https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm)

<br>

## Concise Project Report

<b>Download Concise Project Report [PDF Format] : </b>[Concise Project Report](https://github.com/BlaiseMariaJames/Student-Grade-Management-System/files/10326904/Project.Report.pdf)