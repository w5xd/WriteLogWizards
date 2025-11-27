@rem Run this file after the building the full WL12ModuleItemVsix solution in Release-AnyCPU
@rem first we package the Project wizard
@set outputFile=WL12_VS2022_ContestModuleWizards.zip
@set projWizardZip=WL12ProjectWizard.zip
@if exist "%projWizardZip%" del /q "%projWizardZip%"
@pushd ..\WL12ProjectWizard\WL12ContestTemplate
@7z a "..\..\kit\%projWizardZip%" *.*
@popd
@rem then we package the installer command files and the wizards (one vsix and one zip) for 
@rem end developer download
copy ..\WL12ModuleItem\bin\Release\*.vsix .
@if exist "%outputFile%" del /q "%outputFile%"
@7z a "%outputFile%" *.vsix WlModulewizardVsinstaller.cmd WLProjectWizardInstaller.cmd "%projWizardZip%" ReadMe.txt
