
@rem This installs ONLY the WL12 Contest module wizard, and only for VS version 2026
@rem The trick here that I found documented only at:
@rem https://learn.microsoft.com/en-us/visualstudio/install/import-export-installation-configurations?view=visualstudio
@rem It is the --allowUnsignedExtensions.
@set installPath=C:\Program Files\Microsoft Visual Studio\18\Community
@set vsix=%~dp0WL12ModuleItem.vsix
@rem json requires doubled backslashes
@set vsix=%vsix:\=\\%
@set contents={  "version": "1.0",  "components": [], "extensions": ["%vsix%" ] }
@set vsconfig=%~dp0wlwizard.vsconfig
@echo %contents% > "%vsconfig%"
"C:\Program Files (x86)\Microsoft Visual Studio\Installer\setup.exe" modify --installPath "%installPath%" --config "%vsconfig%" --allowUnsignedExtensions
