@rem the file WL12ProjectWizard2026.zip should have been downloaded and unzipped colocated here
@rem This installer is for VS2026 and ONLY for the WL12 Project wizard (not the Item wizard). 
@set VSfolderName=Visual Studio 18
@set projWizardZip=WL12ProjectWizard.zip
@set KEY_NAME=HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders
@set VALUE_NAME=Personal
@set DOCUMENTS=
@FOR /F "tokens=2* skip=2" %%a in ('@reg query "%KEY_NAME%" /v "%VALUE_NAME%"') do @set DOCUMENTS=%DOCUMENTS%%%b
@rem The DOCUMENTS variable can (and will) contain %USERPROFILE% in most cases. The "call" expands it
@call :expand_docs %DOCUMENTS%
@set DOCUMENTS=%_RETURN_VALUE%
copy /y "%projWizardZip%" "%DOCUMENTS%\%VSfolderName%\Templates\ProjectTemplates"
@echo That's all it should take for File/New-Project in Visual Studio to show a WriteLog Project
@goto :EOF

:expand_docs 
@set _RETURN_VALUE=%1%

