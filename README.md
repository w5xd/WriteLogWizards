# WriteLogWizards
Tools for WriteLog contest module development
This repo contains:
1) Source code for Visual Studio 2008 wizards that create WriteLog contest module implementations.
2) The same for Visual Studio 2013
3) The necessary header files and import libraries to build WriteLog contest modules.

Developing in VS 2008 can target both WriteLog versions 11 and 12.
Developing in VS 2013 targets only WriteLog version 12. (The VS2013
C++ runtime is not installed by the WL11 installer. But modules
developed with VS 2013 will work on WL11 if that runtime happens
to be already installed.)

The Directory Structure.
The wizards generate code that assumes the relative file paths set up in this repo:

Projects/
Is not part of this repo, and is in .gitignore. It is where you should have the 
VS File/New-project wizard place any newly created WL module development projects.

WL11ModuleWizard/
contains the source of the VS2008 module create wizard. 
Do NOT use VS to open any vcproj/sln/vcxproj here (more about the wizards below)

WL11ProjectWizard/
contains the source of the VS2008 project create wizard.

WL12ModuleWizard/
contains the source of the VS2013 module create wizard.

WL12ProjectWizard/
contains the source of the VS2013 project create wizard.

WriteLog/
contains the source of the various headers and import libraries required by the generated project. These headers
and libraries suffice for both WL11 and WL12 (and VS2008 and VS2013). You do NOT edit anything here, with ONE exception:
WriteLog/include/
is where it is suggested you put your own common headers you want available across multiple modules. 
The wizards arrange for this directory to be in the include path for both C++ and .rc compiles, so this
is a good place to put, for example, source code for any copyright or version information you want
in common across your modules.

WriteLog/generated/
WriteLog/MmdCom/
WriteLog/wlogtool/
Don't touch these. It is also recommended that you NOT use any headers you find in there
for your development because much of it is archaic. The reason the archaic stuff remains is
to enable source-code compatible development of module developed using the old Visual Studio 6
wizard last updated in 2008 (and most of which dates from 2000)

The wizards.
You must already have VS 2008 and/or VS 2013 installed. The Express versions are NOT supported.
It takes two wizards to create a WriteLog contest module:
1. The WL11ProjectWizard (or WL12ProjectWizard) creates a skeleton project in VS File/New-Project menu.
2. The WL11ModuleWizard (or WL12ModuleWizard) adds skeleton header/cpp files to such a project for a contest. 

Deploying the wizards.
There are a total of four wizards: two each for VS 2008 and VS 2013. The instructions for deployment are
almost identical for the two versions. There is more than one way to accomplish deployment, but here
is one that works and is minimally intrusive on your system.

In all four cases is is NOT necessary to use Visual Studio to File/Open-Project of any of the
.sln/.vcproj/.vcxproj files in the repo WLnn___Wizard/ folders. Doing so MIGHT cause VS to auto-magically 
deploy the wizard, which might or might not conflict with the instructions below. Of course, if you
don't like the way the wizards work, you are welcome to change them to suit yourself.


