**********************************************
How To Set Up Project Properties/Compiler/Linker
**********************************************
1) Project -> $(ProjectName) Properties (or hit Alt+F7)
   a. Click on "Toolchain" and select Configuration "All Configurations"
2) ARM/GNU C++ Compiler -> Directories
   a. Add the directories for any files you want to include in the project. 
	  Without them being included here, the compiler will have no clue where
      any of your source code is located. This is the Include, Source, Peripherals
	  folders.
3) Optimization -> Optimize Most (-O3)
   a. Check "Prepare functions for garbage collection"
   b. Check last three (3) boxes
4) Linker -> Libraries
   a. Check to make sure libm is in the Libraries(-I) box
   b. Ensure "Library Search Path(-L)" is $(ProjectDir)\Device_Startup
5) Linker -> Check "Garbage collect unused sections"
6) Hit save and then exit the project properties tab.


**********************************************
How To Set Up Programmer 
**********************************************
1) Tools -> External Tools -> Add
2) Name your tool
3) Under "Command" use this: (Replace $(UserDirectory) with location of .bat file)
   a. $(UserDirectory)\programmerDesktop.bat
   b. Open "programmerDesktop.bat" and replace the directory for bossac.exe with 
      the location of that file on your computer. 
4) Under "Arguments" use this: (Replace COM4 with whatever COM port your device uses)
   a. COM4 $(TargetDir)$(TargetName).bin
5) Under "Initial Directory" use this:
   a. $(TargetDir)
6) Check "Use Output Window"
7) Hit apply.
