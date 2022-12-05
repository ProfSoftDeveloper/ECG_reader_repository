1. The ECG_reader project

This is the C++ 17 and QT 6.4.1 application for reading the ECG data from the set of *.txt files,
as required by the task.

1.1 BUILD

The project folder is ECG_reader. Inside this folder there are no subfolders for headers and sources.

If not done before, install QT 6.4.1 with support of MinGW compiler on the computer
The project is build in QT creator 9.0.0 IDE. Since IDE is installed,
place the "ECG_reader" folder in some directory on the computer. Then open the
project at QT Creator by clicking "ECG_reader.pro" file.
Go to settings "Projects", "Build", and check that the following is true:
- qmake is used to build the project
- MinGW 64-bit is used to build the project (for 32 bit systems it should be 32 bit)
- build directory is specified

Since there is also another QT project, used for unit testing, an additional operation is required,
since header file "mainwindow.h" is used in both major project and unit testing project,
so it contains more detailed instruction for #include of "ui_mainwindow.h" file (line 9 in "mainwindow.h").
In general, a QT project is built in a directory, which is different from project directory,
and this directory can be setup in "Projects", "Build" settings
Choose build directory to be present inside the same folder as project directory.
Open the header file "mainwindow.h" and manually specify the #include path to "ui_mainwindow.h" file,
which will be present in the build directory. This path must show compiler how to go
from project directory to build directory to include "ui_mainwindow.h" file, so notation ".."
is recommended to be used, as, for example, in the current header file:

#include "..\build-ECG_reader-Desktop_Qt_6_4_1_MinGW_64_bit-Debug\ui_mainwindow.h"

This is necessary for using "mainwindow.h" in both major project and testing project. 

After this setup is done, launch the application in QT Creator by clicking the "run" button

1.2 Program description

ECG data are supposed to be present on the computer inside one folder. Data are written in multiple txt files,
which include numbers in their names to show right order of reading these data, such as "ecg-0.txt".
Program reads the data from txt, according to their order, first from "ecg-0.txt", then from "ecg-1.txt",
then "ecg-2.txt", etc. Only these *.txt files are supposed to be present in data directory.
The first line of each txt data must include information about amount of numbers in this txt file,
for example "NbData=108". Presence of "=" symbol before number is necessary for the program to work.
In this way all data from all *.txt files are read and wrote in the special class object ECG_data.

When application is launched, graphical interface window appears.
Press the button "Choose data directory" and choose directory where ECG data are stored in txt files.
Program will read the data. Then press the "Show data" button. Data will be shown.

There are multiple options, which can be used:
- Default scale shows data along entire time range and along entire signal range. So, one can see all the data on the screen
- Watch window shows data in a window, with specified time limits. In this mode, there are multiple options available:
	- Beginning time of a show window is controlled by time slider. Move the slider to change position of the window.
	- "Time width, s" (default 6) regulates width of time window
	- Vertical scale options regulate how to show data along vertical scale (signal scale)
		- "Auto" mode means that top and bottom show limits are automatically adjusted to show current part of data
		- "Constant" mode uses the same vertical limits as for entire signal range, even if this range is not inside
		the time window watched right now. It is the same as it was in "Default" mode
		- "Symmetrical" mode adjusts vertical limits to show current part of data inside time window,
		but also makes the top and bottom limits to have the same absolute values
		- "User defined" mode allows user to input directly the top and bottom vertical limits in the input fields,
		and when time slider will be moved, these changes will work to show selected vertical limits scale.
- Finally, "vertical scale spacing" defines size of vertical margins below and above of the field, occupied by data.
This parameter is adjustable for all show modes.

"Clear" data is used to delete all data and clear graph.
Application can be closed by clicking the cross sign at top right.

2. Google_test_ECG_reader

2.1. BUILD

This project contains Google Test based application to test the functioning of the "ECG_reader" project.
The ECG_reader" project, the "Google_test_ECG_reader" project, "googletest-main" folder and build directories
for both projects must be inside the same folder.
"googletest-main" folder contains the Google Test framework which is used in the "Google_test_ECG_reader" project

Before launching this project, the project "ECG_reader" must be compiled at least one time to create "ui_mainwindow.h" file,
and header file "mainwindow.h" must contain the path to this file, as it is described above.
After that, open "main.cpp" file and correct, if necessary, the path for the compiler to find the "Google_test_main_cpp_header.h" file,
which is contained inside the "ECG_reader" folder and allows to include other files from "ECG_reader" project.
This is line 3 at "main.cpp" file, if current instructions are followed, it will be

#include "..\ECG_reader\Google_test_main_cpp_header.h" (at main.cpp, line 3)

Also, to run tests, there must be a directory containing ECG data as a set of txt files, as described above.
Make sure that this directory is present on the computer
Write full path to this directory to the DATA_PATH preprocessor constant, main.cpp, line 6, for example:

#define DATA_PATH "C:/QT6.4.1Projects/Google_test_ECG_reader/data"

In the project settings, make sure that
- qmake is used to build the project
- MinGW 64-bit compiler is used
- Build directory is specified

2.2 Google test project run

Launch the project to execute all the tests. Testing report is written in console.




"# project" 
