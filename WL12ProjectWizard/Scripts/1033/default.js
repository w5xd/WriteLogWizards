
function OnFinish(selProj, selObj)
{
	try
	{
		var strProjectPath = wizard.FindSymbol('PROJECT_PATH');
		var strProjectName = wizard.FindSymbol('PROJECT_NAME');

		wizard.AddSymbol('PROJECT_NAME_CAPS', strProjectName.toUpperCase());
	    // add symbols for the filters of the project
		wizard.AddSymbol('MY_SOURCE_FOLDER_NAME', 'Source Files');
		wizard.AddSymbol('MY_HEADER_FOLDER_NAME', 'Header Files');
		wizard.AddSymbol('MY_RESOURCE_FOLDER_NAME', 'Resource Files');
		wizard.AddSymbol('MY_GENERATED_FOLDER_NAME', 'Generated Files');

		var strGuid = wizard.CreateGuid();
		strVal = wizard.FormatGuid(strGuid, 0);
		wizard.AddSymbol("LIBID_REGISTRY_FORMAT", strVal);
		strGuid = wizard.CreateGuid();
		strVal = wizard.FormatGuid(strGuid, 0);
		wizard.AddSymbol("APPID_REGISTRY_FORMAT", strVal);
        CreateGUIDs();

		selProj = CreateCustomProject(strProjectName, strProjectPath);
		AddConfig(selProj, strProjectName);
		AddFilters(selProj);


		var InfFile = CreateCustomInfFile();
		AddFilesToCustomProj(selProj, strProjectName, strProjectPath, InfFile);
		PchSettings(selProj, strProjectName);
		InfFile.Delete();

		selProj.Object.Save();
	}
	catch(e)
	{
		if (e.description.length != 0)
			SetErrorInfo(e);
		return e.number
	}
}

function CreateCustomProject(strProjectName, strProjectPath)
{
	try
	{
		var strProjTemplatePath = wizard.FindSymbol('START_PATH');
		var strProjTemplate = '';
		strProjTemplate = strProjTemplatePath + '\\default.vcxproj';

		var Solution = dte.Solution;
		var strSolutionName = "";
		if (wizard.FindSymbol("CLOSE_SOLUTION"))
		{
			Solution.Close();
			strSolutionName = wizard.FindSymbol("VS_SOLUTION_NAME");
			if (strSolutionName.length)
			{
				var strSolutionPath = strProjectPath.substr(0, strProjectPath.length - strProjectName.length);
				Solution.Create(strSolutionPath, strSolutionName);
			}
		}

		var strProjectNameWithExt = '';
		strProjectNameWithExt = strProjectName + '.vcxproj';

		var oTarget = wizard.FindSymbol("TARGET");
		var prj;
		if (wizard.FindSymbol("WIZARD_TYPE") == vsWizardAddSubProject)  // vsWizardAddSubProject
		{
			var prjItem = oTarget.AddFromTemplate(strProjTemplate, strProjectNameWithExt);
			prj = prjItem.SubProject;
		}
		else
		{
			prj = oTarget.AddFromTemplate(strProjTemplate, strProjectPath, strProjectNameWithExt);
		}
		var fxtarget = wizard.FindSymbol("TARGET_FRAMEWORK_VERSION");
		if (fxtarget != null && fxtarget != "")
		{
		    fxtarget = fxtarget.split('.', 2);
		    if (fxtarget.length == 2)
			prj.Object.TargetFrameworkVersion = parseInt(fxtarget[0]) * 0x10000 + parseInt(fxtarget[1])
		}
		return prj;
	}
	catch(e)
	{
		throw e;
	}
}

function AddFilters(proj)
{
	try
	{
	    // Add the folders to your project
	    var group1 = proj.Object.AddFilter(wizard.FindSymbol('MY_SOURCE_FOLDER_NAME'));
	    group1.Filter = ".cpp;.cxx;.c;.idl;.def";
	    var group2 = proj.Object.AddFilter(wizard.FindSymbol('MY_HEADER_FOLDER_NAME'));
	    group2.Filter = ".h;.hpp";
	    var group3 = proj.Object.AddFilter(wizard.FindSymbol('MY_RESOURCE_FOLDER_NAME'));
	    group3.Filter = ".rc;.rc2;.ico;.gif;.bmp;.rgs";
	    var group4 = proj.Object.AddFilter(wizard.FindSymbol('MY_GENERATED_FOLDER_NAME'));
	    group4.SourceControlFiles = "false";
	}
	catch(e)
	{
		throw e;
	}
}

function AddConfig(proj, strProjectName)
{
	try
	{
		var config = proj.Object.Configurations('Debug');

		var MIDLTool = config.Tools('VCMIDLTool');
		MIDLTool.PreprocessorDefinitions = "_DEBUG";
		MIDLTool.MkTypLibCompatible = "false";
		MIDLTool.GenerateStublessProxies = "true";
		MIDLTool.HeaderFileName = strProjectName + "_i.h";
		MIDLTool.DLLDataFileName = "";
		MIDLTool.InterfaceIdentifierFileName = strProjectName + "_i.c";
		MIDLTool.ValidateParameters = "true";
		MIDLTool.GenerateTypeLibrary = "false";
		MIDLTool.TargetEnvironment = "1";

		var CLTool = config.Tools('VCCLCompilerTool');
		CLTool.Optimization = "0";
		CLTool.PreprocessorDefinitions = "WIN32;_WINDOWS;_DEBUG;_USRDLL";
		CLTool.RuntimeLibrary="3";
		CLTool.UsePrecompiledHeader = "2";
		CLTool.WarningLevel = "3";
		CLTool.DebugInformationFormat="4";
		CLTool.CompileAsManaged = "0";

		var RCTool = config.Tools('VCResourceCompilerTool');
		RCTool.PreprocessorDefinitions = "_DEBUG";
		RCTool.Culture = "1033";
		RCTool.AdditionalIncludeDirectories = "$(IntDir)";

		var LinkTool = config.Tools('VCLinkerTool');
		LinkTool.RegisterOutput = "false";
		LinkTool.AdditionalDependencies = "wlogsh32.lib Dxpref32.lib";
		LinkTool.ModuleDefinitionFile = ".\\" + strProjectName + "Module.def";
		LinkTool.GenerateDebugInformation = "true";
		LinkTool.SubSystem = "2";
		LinkTool.IgnoreImportLibrary = true;
		LinkTool.TargetMachine = "1";

        // *****************Release ***********************
		config = proj.Object.Configurations('Release');

		var MIDLTool = config.Tools('VCMIDLTool');
		MIDLTool.PreprocessorDefinitions = "NDEBUG";
		MIDLTool.MkTypLibCompatible = "false";
		MIDLTool.GenerateStublessProxies = "true";
		MIDLTool.HeaderFileName = strProjectName + "_i.h";
		MIDLTool.DLLDataFileName = "";
		MIDLTool.InterfaceIdentifierFileName = strProjectName + "_i.c";
		MIDLTool.ValidateParameters = "true";
		MIDLTool.GenerateTypeLibrary = "false";
		MIDLTool.TargetEnvironment = "1";

		var CLTool = config.Tools('VCCLCompilerTool');
		CLTool.Optimization = "2";
		CLTool.PreprocessorDefinitions = "WIN32;_WINDOWS;NDEBUG;_USRDLL";
		CLTool.RuntimeLibrary="2";
		CLTool.UsePrecompiledHeader = "2";
		CLTool.WarningLevel = "3";
		CLTool.DebugInformationFormat="3";
		CLTool.CompileAsManaged = "0";

		var RCTool = config.Tools('VCResourceCompilerTool');
		RCTool.PreprocessorDefinitions = "NDEBUG";
		RCTool.Culture = "1033";
		RCTool.AdditionalIncludeDirectories = "$(IntDir)";

		var LinkTool = config.Tools('VCLinkerTool');
		LinkTool.RegisterOutput = "false";
		LinkTool.AdditionalDependencies = "wlogsh32.lib Dxpref32.lib";
		LinkTool.ModuleDefinitionFile = ".\\" + strProjectName + "Module.def";
		LinkTool.GenerateDebugInformation = "true";
		LinkTool.SubSystem = "2";
		LinkTool.TargetMachine = "1";
		LinkTool.IgnoreImportLibrary = true;
    }
	catch(e)
	{
		throw e;
	}
}

var nNumConfigs = 2;
var astrConfigName = new Array();
astrConfigName[0] = "Debug";
astrConfigName[1] = "Release";

function PchSettings(proj, strProjectName) {
    try {
        var files = proj.Object.Files;
        var fStdafx = files("StdAfx.cpp");
        var fDllmain = files("dllmain.cpp");
        var fProject_i = files(strProjectName + "_i.c");

        var nCntr;
        for (nCntr = 0; nCntr < nNumConfigs; nCntr++) {
            var config = fStdafx.FileConfigurations(astrConfigName[nCntr]);
            config.Tool.UsePrecompiledHeader = "1";

            config = fDllmain.FileConfigurations(astrConfigName[nCntr]);
            config.Tool.UsePrecompiledHeader = "0";

            config = fProject_i.FileConfigurations(astrConfigName[nCntr]);
            config.Tool.UsePrecompiledHeader = "0";
        }
    }
    catch (e) {
        throw e;
    }
}

function DelFile(fso, strWizTempFile)
{
	try
	{
		if (fso.FileExists(strWizTempFile))
		{
			var tmpFile = fso.GetFile(strWizTempFile);
			tmpFile.Delete();
		}
	}
	catch(e)
	{
		throw e;
	}
}

function CreateCustomInfFile()
{
	try
	{
		var fso, TemplatesFolder, TemplateFiles, strTemplate;
		fso = new ActiveXObject('Scripting.FileSystemObject');

		var TemporaryFolder = 2;
		var tfolder = fso.GetSpecialFolder(TemporaryFolder);
		var strTempFolder = tfolder.Drive + '\\' + tfolder.Name;

		var strWizTempFile = strTempFolder + "\\" + fso.GetTempName();

		var strTemplatePath = wizard.FindSymbol('TEMPLATES_PATH');
		var strInfFile = strTemplatePath + '\\Templates.inf';
		wizard.RenderTemplate(strInfFile, strWizTempFile);

		var WizTempFile = fso.GetFile(strWizTempFile);
		return WizTempFile;
	}
	catch(e)
	{
		throw e;
	}
}

function GetTargetName(strName, strProjectName)
{
	try
	{
	    var strTarget = strName;

	    if (strName == 'TemplateContestModule.cpp')
	        strTarget = strProjectName + 'Module.cpp';
	    else if (strName == 'TemplateContestModule.def')
	        strTarget = strProjectName + 'Module.def';
	    else if (strName == 'TemplateContestModule.idl')
	        strTarget = strProjectName + 'Module.idl';
	    else if (strName == 'TemplateContestModule.rc')
	        strTarget = strProjectName + 'Module.rc';
	    else if (strName == 'TemplateContestModule.rgs')
	        strTarget = strProjectName + 'Module.rgs';

	    return strTarget;
	}
	catch(e)
	{
		throw e;
	}
}

function AddFilesToCustomProj(proj, strProjectName, strProjectPath, InfFile)
{
	try
	{
		var projItems = proj.ProjectItems

		var strTemplatePath = wizard.FindSymbol('TEMPLATES_PATH');
		var projFilters = proj.Object.Filters;
		var filterSrc =
         projFilters.Item(wizard.FindSymbol('MY_SOURCE_FOLDER_NAME'));
		var filterHdr =
         projFilters.Item(wizard.FindSymbol('MY_HEADER_FOLDER_NAME'));
		var filterRc =
         projFilters.Item(wizard.FindSymbol('MY_RESOURCE_FOLDER_NAME'));
		var filterGen =
         projFilters.Item(wizard.FindSymbol('MY_GENERATED_FOLDER_NAME'));

		var strTpl = '';
		var strName = '';

		var strTextStream = InfFile.OpenAsTextStream(1, -2);
		while (!strTextStream.AtEndOfStream)
		{
			strTpl = strTextStream.ReadLine();
			if (strTpl != '')
			{
				strName = strTpl;
				var strTarget = GetTargetName(strName, strProjectName);
				var strTemplate = strTemplatePath + '\\' + strTpl;
				var strFile = strProjectPath + '\\' + strTarget;

				var bCopyOnly = false;  //"true" will only copy the file from strTemplate to strTarget without rendering/adding to the project
				var strExt = strName.substr(strName.lastIndexOf("."));
				wizard.RenderTemplate(strTemplate, strFile, bCopyOnly);
				if (strExt == '.cpp' || strExt == '.idl' || strExt == '.c' || strExt == '.def') {
				    filterSrc.AddFile(strTarget);
				}
				else if (strExt == '.hpp' || strExt == '.h') {
				    filterHdr.AddFile(strTarget);
				}
				else if (strExt == '.rc' || strExt == '.rgs') {
				    filterRc.AddFile(strTarget);
				}
            }
		}
		strTextStream.Close();
		filterGen.AddFile(strProjectName + "_i.c");
		filterGen.AddFile(strProjectName + "_i.h");
    }
	catch(e)
	{
		throw e;
	}
}

function CreateGUIDs() {
    try {
        // create CLSID
        var strRawGUID = wizard.CreateGuid();
        var strFormattedGUID = wizard.FormatGuid(strRawGUID, 0);
        wizard.AddSymbol("INSTALLER_PROJECT_ID", strFormattedGUID);
        strRawGUID = wizard.CreateGuid();
        strFormattedGUID = wizard.FormatGuid(strRawGUID, 0);
        wizard.AddSymbol("INSTALLER_UPGRADECODE", strFormattedGUID);
    }
    catch (e) {
        throw e;
    }
}
