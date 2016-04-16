/* Deploying this Add Item wizard...
** In this directory:
** C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcprojectitems
**
** Create the subdirectory named this, and the file named this:
**    LocalItems
**    LocalItems.vsdir
**
** And in LocalItems folder, place these:
**    WL12ModuleWizard.ico
**    WL12ModuleWizard.vsdir
**    WL12ModuleWizard.vsz
**
** and that last one has the ABSOLUTE_PATH to the rest of this wizard
*/

function UserException(message) {
    this.description = message;
    this.name = "UserException";
}

function OnFinish(selProj, selObj) {
    var oCM;
    try {

        /* validate */
        var strClassName = wizard.FindSymbol("MM_CLASS_NAME");
        var strContestDisplay = wizard.FindSymbol("CONTEST_NAME");
        if ((strClassName == "") ||  (strContestDisplay == ""))
            throw new UserException("Class name and contest display name cannot be empty");

        // distinguish the name of the C++ class from the name of its COM coclass
        var strShortName;
        if (strClassName.length > 1 && strClassName.substring(0,1) == "C")
            strShortName = strClassName.substring(1);
        else
            strShortName = strClassName + "CoClass";

        wizard.AddSymbol("COCLASS", strShortName);
        oCM = selProj.CodeModel;
        var bDevice = false;
        wizard.AddSymbol("DEVICE", bDevice);
        AddDeviceSymbols(false);

        var L_TRANSACTION_Text = "Add WL Module ";
        oCM.StartTransaction(L_TRANSACTION_Text + strShortName);
        if (!AddATLSupportToProject(selProj)) {
            oCM.AbortTransaction();
            return;
        }

        var bDLL;
        if (typeDynamicLibrary == selProj.Object.Configurations(1).ConfigurationType)
            bDLL = true;
        else
            bDLL = false;
        if (!bDLL)
            throw new UserException("WriteLog contest modules only work in-process");

        wizard.AddSymbol("DLL_APP", bDLL);
        wizard.AddSymbol("SUPPORT_DCOM", false);
        wizard.AddSymbol("SUPPORT_NON_DCOM", false);

        var strProjectName = wizard.FindSymbol("PROJECT_NAME");
        var strProjectPath = wizard.FindSymbol("PROJECT_PATH");
        var strTemplatePath = wizard.FindSymbol("TEMPLATES_PATH");
        var strUpperShortName = CreateASCIIName(strShortName.toUpperCase());
        wizard.AddSymbol("UPPER_SHORT_NAME", strUpperShortName);

         var strProjectRC = GetProjectFile(selProj, "RC", true, false);

        // Create necessary GUIDS
        CreateGUIDs();

        var MidlTool = GetIDLConfig(selProj, true);
        var strMidlHeader = MidlTool.HeaderFileName;
        strMidlHeader = selProj.Object.Configurations(1).Evaluate(strMidlHeader);
        wizard.AddSymbol("MIDL_H_FILENAME", strMidlHeader);

        // Get LibName
        wizard.AddSymbol("LIB_NAME", oCM.IDLLibraries(1).Name);

        // Get LibID
        var oUuid = oCM.IDLLibraries(1).Attributes.Find("uuid");
        if (oUuid)
            wizard.AddSymbol("LIBID_REGISTRY_FORMAT", oUuid.Value);

        // add RGS file resource
        var strRGSFile = GetUniqueFileName(strProjectPath, CreateASCIIName(strShortName) + ".rgs");
        var strRGSID = "IDR_" + strUpperShortName;

        RenderAddTemplate(wizard, "object.rgs", strRGSFile, false, false);

        var oResHelper = wizard.ResourceHelper;
        oResHelper.OpenResourceFile(strProjectRC);

        var strSymbolValue = oResHelper.AddResource(strRGSID, strProjectPath + strRGSFile, "REGISTRY");
        if (strSymbolValue == null) return;
        wizard.AddSymbol("RGS_ID", strSymbolValue.split("=").shift());

        oResHelper.CloseResourceFile();


        // Render objco.idl and insert into strProject.idl
        AddCoclassFromFile(oCM, "objco.idl");

        // Add header
        var strHeaderFile = GetUniqueFileName(strProjectPath, CreateASCIIName(strShortName) + ".h");
        wizard.AddSymbol("HEADER_FILE", strHeaderFile);
        RenderAddTemplate(wizard, "object.h", strHeaderFile, selObj, true);

        // Add CPP
        var strImplFile = GetUniqueFileName(strProjectPath, CreateASCIIName(strShortName) + ".cpp");
        RenderAddTemplate(wizard, "object.cpp", strImplFile, selObj, false);

        oCM.CommitTransaction();

        //var newClass = oCM.Classes.Find(strClassName);
        //if (newClass)
        //    newClass.StartPoint.TryToShow(vsPaneShowTop);
    }
    catch (e) {
        if (oCM)
            oCM.AbortTransaction();

        if (e.description.length != 0)
            SetErrorInfo(e);
        return e.number
    }
}

function CreateGUIDs() {
    try {
        // create CLSID
        var strRawGUID = wizard.CreateGuid();
        var strFormattedGUID = wizard.FormatGuid(strRawGUID, 0);
        wizard.AddSymbol("CLSID_REGISTRY_FORMAT", strFormattedGUID);
    }
    catch (e) {
        throw e;
    }
}
