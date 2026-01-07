@rem This installs ONLY the WL12 Contest module wizard, and only for VS version 2022
@rem The trick here that I found documented only at:
@rem https://learn.microsoft.com/en-us/visualstudio/install/import-export-installation-configurations?view=visualstudio
@rem It is the --allowUnsignedExtensions.
@set installPath=C:\Program Files\Microsoft Visual Studio\2022\Community
@set vsix=%~dp0WL12ModuleItem.vsix
@rem json requires doubled backslashes
@set vsix=%vsix:\=\\%
@set contents={  "version": "1.0",  "components": [], "extensions": ["%vsix%" ] }
@set vsconfig=%~dp0wlwizard.vsconfig
@echo If the WL12 Module Wizard is already installed, you must use Visual Studio Installer to manually remove it before this one will work!
@pause
@echo %contents% > "%vsconfig%"
"C:\Program Files (x86)\Microsoft Visual Studio\Installer\setup.exe" modify --installPath "%installPath%" --config "%vsconfig%" --allowUnsignedExtensions
