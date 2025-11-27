// WriteLog Contest module wizard.
// inspired by: https://blog.sverrirs.com/2017/05/vs-itemtemplates-wizards-and-vsix.html
//
//

using EnvDTE;
using Microsoft.VisualStudio.TemplateWizard;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Resources;
using System.Text;

namespace WL12ModuleItem.CppTemplates.WL12ModuleTemplate
{
    /// <summary>
    /// Template wizard extensions are run when Visual Studio is instantiating a project or item from a template.
    /// Sequence of calls for Items is:
    ///     RunStarted 
    ///         - First call (should collect data from user)
    ///     ShouldAddProjectItem 
    ///         - The template object is passed in (e.g. "unittest.cpp") 
    ///         - Nothing has been done, this is an option to selectively exclude files
    ///     ProjectItemFinishedGenerating 
    ///         - Values have been applied to template and file final result file added to Project
    ///     BeforeOpeningFile 
    ///         - File is being opened in Editor
    ///     RunFinished 
    ///         - The template operation is completed
    /// </summary>
    public class ModuleWizard : IWizard
    {

        bool userCanceled = false;

        DTE m_dte;
        string m_itemName;

        void IWizard.RunStarted(object automationObject, Dictionary<string, string> replacementsDictionary, 
            WizardRunKind runKind, object[] customParams)
        {
            // Resolve the main Visual Studio core automation entry point
            // https://msdn.microsoft.com/en-us/library/envdte.aspx

            replacementsDictionary["$AM_COUNTYLINE$"] = "0";
            replacementsDictionary["$AM_ROVER$"] = "0";
            replacementsDictionary["$ASK_MODE$"] = "0";
            replacementsDictionary["$AYGMULT_MULTI_BAND$"] = "0";
            replacementsDictionary["$AYGMULT_SINGLE_BAND$"] = "0";
            replacementsDictionary["$CABRILLO$"] = "0";
            replacementsDictionary["$CAN_LOG_ROVER$"] = "0";
            replacementsDictionary["$DXCC_MULTI_BAND$"] = "0";
            replacementsDictionary["$DXCC_SINGLE_BAND$"] = "0";
            replacementsDictionary["$MULTIPLE_NAMED_IN_QSO$"] = "0";
            replacementsDictionary["$MULTIPLE_NAMED_IN_QSO_TX$"] = "0";
            replacementsDictionary["$MULTI_MODE$"] = "0";
            replacementsDictionary["$NAMEDMULT_MULTI_BAND$"] = "0";
            replacementsDictionary["$NAMEDMULT_SINGLE_BAND$"] = "0";
            replacementsDictionary["$NAMED_MULTI_REGION$"] = "0";
            replacementsDictionary["$NO_AYGMULT$"] = "0";
            replacementsDictionary["$NO_DXCC$"] = "0";
            replacementsDictionary["$NO_NAMEDMULT$"] = "0";
            replacementsDictionary["$NO_ZONE$"] = "1";
            replacementsDictionary["$NR_IN_EXCHANGE$"] = "0";
            replacementsDictionary["$PTS_COLUMN$"] = "0";
            replacementsDictionary["$RST_IN_EXCHANGE$"] = "0";
            replacementsDictionary["$RTTY$"] = "0";
            replacementsDictionary["$TQSL_ROVER$"] = "0";
            replacementsDictionary["$ZONE_MULTI_BAND$"] = "0";
            replacementsDictionary["$ZONE_SINGLE_BAND$"] = "0";

            var wiz = new ContestModuleDlg();
            m_itemName =  replacementsDictionary["$safeitemname$"];
            replacementsDictionary["$BASE_NAME$"] = m_itemName;
            wiz.Symbols = replacementsDictionary;
            wiz.Basename = m_itemName;
            if (wiz.ShowDialog() != System.Windows.Forms.DialogResult.OK)
            {
                userCanceled = true;
                return;
            }

            m_dte = automationObject as DTE;

            replacementsDictionary["$CLSID_REGISTRY_FORMAT$"] =  replacementsDictionary["$guid1$"] ;
            replacementsDictionary["$WXS_COMPONENT_GUID_1$"] =  replacementsDictionary["$guid2$"]  ;
            replacementsDictionary["$WXS_COMPONENT_GUID_2$"] =  replacementsDictionary["$guid3$"] ;

            string FileNamebase = m_itemName;

            replacementsDictionary["$MM_DLG_CLASS_NAME$"] = replacementsDictionary["$MM_CLASS_NAME$"] + "Dialog";
            replacementsDictionary["$MM_ROVERDLG_CLASS_NAME$"] = replacementsDictionary["$MM_CLASS_NAME$"] + "RoverSelectDialog";

            replacementsDictionary["$COCLASS$"] = m_itemName;
            replacementsDictionary["$MM_CABRILLOITERATOR_CLASS_NAME$"] = replacementsDictionary["$MM_CLASS_NAME$"] + "CabrilloIterator";

            replacementsDictionary["$FILE_NAME_BASE$"] = FileNamebase;
            replacementsDictionary["$CPP_FILE$"] = FileNamebase + ".cpp";
            replacementsDictionary["$DLG_HEADER_FILE$"] = FileNamebase + "Dlg.h";
            replacementsDictionary["$HEADER_FILE$"] = FileNamebase + ".h";
            projectMisc.rgsFileName =
            replacementsDictionary["$RGS_FILE$"] = FileNamebase + ".rgs";
            replacementsDictionary["$WXS_FILE$"] = FileNamebase + ".wxs";
            replacementsDictionary["$MM_CABRILLOITERATOR_CLASS_FILENAME$"] = FileNamebase + "CabrilloIterator.h";
            replacementsDictionary["$MM_ROVERDLG_CLASS_FILENAME$"] = FileNamebase + "SelectDlg.h";

            projectMisc.needMultdent = replacementsDictionary["$NO_DXCC$"] == "0";
            projectMisc.amRover = replacementsDictionary["$AM_ROVER$"] == "1";
            projectMisc.needCabrilloIterator = replacementsDictionary["$MULTIPLE_NAMED_IN_QSO$"] == "1" && replacementsDictionary["$CABRILLO$"] == "1";

            checkFilesExist();

            replacementsDictionary["$PROJECT_NAME$"] = projectMisc.projectName;
            replacementsDictionary["$MIDL_H_FILENAME$"] = projectMisc.projectName + "_i.h";

            var resourceDefines = parseResourceH();

            int firstKeytoCheck = 130;

            while (resourceDefines.TryGetValue(firstKeytoCheck, out var f))
                firstKeytoCheck += 1;

            string DlgIdName = "IDD_" + FileNamebase.ToUpper() + "_DLG";
            replacementsDictionary["$IDD_DIALOGID$"] = DlgIdName;
            projectMisc.dialogIdsToAdd.Add(DlgIdName, firstKeytoCheck++);

            if (projectMisc.amRover)
            {
                while (resourceDefines.TryGetValue(firstKeytoCheck, out var f))
                    firstKeytoCheck += 1;
                DlgIdName = "IDD_" + FileNamebase.ToUpper() + "SELECT_DLG";
                replacementsDictionary["$IDD_ROVERSELECT_DIALOGID$"] = DlgIdName;
                projectMisc.dialogIdsToAdd.Add(DlgIdName, firstKeytoCheck++);
            }

            while (resourceDefines.TryGetValue(firstKeytoCheck, out var f))
                firstKeytoCheck += 1;
            DlgIdName = "IDR_" + FileNamebase.ToUpper();
            projectMisc.rgsIdString =
            replacementsDictionary["$RGS_ID$"] = DlgIdName;
            projectMisc.dialogIdsToAdd.Add(DlgIdName, firstKeytoCheck++);

            createCompoundExpressions(replacementsDictionary);
        }

        SortedDictionary<int, string> parseResourceH()
        {
            foreach (var d in m_dte.Documents)
            {
                Document doc = d as Document;
                var y = doc.Name;
                if (y.ToUpper() == "RESOURCE.H")
                {
                    doc.Close();
                    break;
                }
            }
            var resourceDefines = new SortedDictionary<int, string>();

            if (!String.IsNullOrEmpty(projectMisc.resourcehFilePath))
            {
                var lines = System.IO.File.ReadLines(projectMisc.resourcehFilePath);
                foreach (var line in lines)
                {
                    var words = line.Split(new char[0], StringSplitOptions.RemoveEmptyEntries);
                    string id = null;
                    bool found = false;
                    foreach (var w in words)
                    {
                        if (w.StartsWith("//"))
                            break;
                        if (w.StartsWith("/*"))
                            break;
                        if (w == "#define")
                        {
                            found = true;
                            continue;
                        }
                        if (found)
                        {
                            if (String.IsNullOrEmpty(id))
                            {
                                id = w;
                                if (w.StartsWith("_APS_NEXT"))
                                    break;
                                continue;
                            }
                            else
                            {
                                int v = 0;
                                if (int.TryParse(w, out v))
                                    resourceDefines[v] = id;
                                break;
                            }
                        }
                    }
                }
            }
            return resourceDefines;
        }

        void createCompoundExpressions(Dictionary<string, string> replacementsDictionary)
        {
            var separators = new string[] { "!", "&&", "||" };
            foreach (string comp in CompositeSymbols.conditionArray)
            {
                List<string> split = new List<string>();
                string toSplit = comp;
                while (toSplit.Any())
                {
                    int idxmin = toSplit.Length;
                    string found = null;
                    foreach (string sep in separators)
                    {
                        var idx = toSplit.IndexOf(sep);
                        if ((idx >= 0) && (idx < idxmin))
                        {
                            idxmin = idx;
                            found = sep;
                        }
                    }
                    if (idxmin >= toSplit.Length)
                        break;
                    string p;
                    if (idxmin == 0)
                    {
                        p = found;
                        toSplit = toSplit.Substring(p.Length);
                    }
                    else
                    {
                        p = toSplit.Substring(0, idxmin);
                        toSplit = toSplit.Substring(idxmin);
                    }
                    split.Add(p);
                }
                if (toSplit.Any())
                    split.Add(toSplit);

                bool negate = false;
                bool result = false;
                bool boolOpOr = false;
                bool boolOpAnd = false;
                string curvar = null;
                bool firstVal = true;
                foreach (string s in split)
                {
                    if (s == "!")
                        negate = true;
                    else if (s == "||")
                        boolOpOr = true;
                    else if (s == "&&")
                        boolOpAnd = true;
                    else if (String.IsNullOrEmpty(curvar))
                    {
                        curvar = s;
                        string lookedup;
                        bool thisVal = false;
                        if (replacementsDictionary.TryGetValue("$" + s + "$", out lookedup))
                            thisVal = lookedup == "1" ? !negate : negate;
                        negate = false;
                        if (firstVal)
                            result = thisVal;
                        else
                        {
                            if (boolOpOr)
                                result |= thisVal;
                            else if (boolOpAnd)
                                result &= thisVal;
                            boolOpOr = boolOpAnd = false;
                        }
                        firstVal = false;
                        curvar = null;
                    }
                }
                replacementsDictionary["$" + comp + "$"] = result ? "1" : "0";
            }
        }

        static readonly ReadOnlyCollection<string> filesToCheckExist = new ReadOnlyCollection<string>(new string[] {
            "MulDEntInc.cpp",
            });

        bool [] filesExist = new bool[filesToCheckExist.Count];

        bool IWizard.ShouldAddProjectItem(string filePath)
        {
            if (userCanceled)
                return false;

            if (filePath == "MulDEntInc.cpp")
            {
                if (!projectMisc.needMultdent)
                    return false;
            }
            else if (filePath.StartsWith("Rover"))
                return projectMisc.amRover;
            else if (filePath == "CabrilloIterator.h")
                return projectMisc.needCabrilloIterator;
            int i = 0;
            foreach (var fn in filesToCheckExist)
            {
                if (fn == filePath)
                    return !filesExist[i];
                i += 1;
            }
            return true;
        }

        bool haveChecked = false;
        void checkFilesExist()
        {
            if (haveChecked)
                return;
            haveChecked = true;
            Microsoft.VisualStudio.Shell.ThreadHelper.ThrowIfNotOnUIThread();
            int i = 0;
            foreach (var s in filesToCheckExist)
            {
                foreach (var y in GetExistingFileNamesInProject())
                    if (y == s)
                        filesExist[i] = true;
                i += 1;
            }
        }

        private List<string> GetExistingFileNamesInProject()
        {
            var fileNames = new List<string>();

            if (m_dte.Solution == null || m_dte.Solution.Projects.Count == 0)
                return fileNames;

            // The project into which the new item is being added
            Project project = GetTargetProject();

            if (project == null)
                return fileNames;

            projectMisc.projectName = project.Name;

            GetFileNamesRecursive(project.ProjectItems, fileNames);
            return fileNames;
        }

        private Project GetTargetProject()
        {
            if (m_dte.SelectedItems.Count > 0)
            {
                var selected = m_dte.SelectedItems.Item(1);
                if (selected.Project != null)
                    return selected.Project;
                if (selected.ProjectItem != null)
                    return selected.ProjectItem.ContainingProject;
            }

            // Final fallback: first project in solution
            return m_dte.Solution.Projects.Item(1);
        }

        class ProjectMisc {
            public string resourcehFilePath;
            public string resourceFilePath;
            public string idlFilePath;
            public bool needMultdent;
            public string projectName;
            public bool amRover;
            public bool needCabrilloIterator;
            public string rgsFileName;
            public string rgsIdString;
            public Dictionary<string, int> dialogIdsToAdd = new Dictionary<string, int>();
            public ProjectMisc()
            {
                amRover = false;
                needMultdent = false;
                needCabrilloIterator = false; 
            }
        }

        ProjectMisc projectMisc = new ProjectMisc();

        private void GetFileNamesRecursive(ProjectItems items, List<string> fileNames)
        {
            if (items == null) return;

            foreach (ProjectItem item in items)
            {
                // Recurse into subfolders
                if (item.ProjectItems != null && item.ProjectItems.Count > 0)
                {
                    GetFileNamesRecursive(item.ProjectItems, fileNames);
                }
                else if (item.FileCount > 0)
                {
                    // FileNames[0] is the full path, use Name for just the file name
                    fileNames.Add(item.Name);

                    if (item.Name.ToUpper() == "RESOURCE.H")
                        projectMisc.resourcehFilePath = item.FileNames[0];
                    else if (item.Name.ToUpper().EndsWith(".RC"))
                        projectMisc.resourceFilePath = item.FileNames[0];
                    else if (item.Name.ToUpper().EndsWith(".IDL"))
                        projectMisc.idlFilePath = item.FileNames[0];
                }
            }
        }

        void IWizard.BeforeOpeningFile(ProjectItem projectItem)
        {
        }

        Dictionary<string, FinishEditFileInterface> itemsToEdit = new Dictionary<string, FinishEditFileInterface>();

        void IWizard.ProjectItemFinishedGenerating(ProjectItem projectItem)
        {
            if (projectItem == null) return;
            if (projectItem.FileCount > 0)
            {
                var fn = projectItem.FileNames[0];
                if (fn.ToLower().EndsWith(".tmp"))
                {
                    var len = fn.Length;
                    var extPos = fn.LastIndexOf('.', len - 5);
                    if (extPos >= 0)
                    {
                        var ext = fn.Substring(extPos + 1);
                        ext = ext.Substring(0, ext.Length - 4);
                        ext = ext.ToLower();
                        FinishEditProjectItem fEditPi = null;
                        if (!itemsToEdit.ContainsKey(ext))
                        {
                            if (ext == "rc")
                                itemsToEdit["rc"] = fEditPi = new FinishEditRC(projectMisc.resourceFilePath);
                            else if (ext == "idl")
                                itemsToEdit["idl"] = fEditPi = new FinishEditIDL(projectMisc.idlFilePath);
                        }
                        else
                            fEditPi = itemsToEdit[ext] as FinishEditProjectItem;
                        fEditPi.Add(projectItem);
                    }
                }
                else if (fn.ToLower().EndsWith(".cpp"))
                {
                    if (projectMisc.dialogIdsToAdd.Any() && !itemsToEdit.ContainsKey("Resource.h"))
                        itemsToEdit["Resource.h"] = new FinishEditRscH(projectMisc.resourcehFilePath, projectMisc.dialogIdsToAdd);
                }
                else if (fn.ToLower().EndsWith(".rgs"))
                {
                    FinishEditRC fEdit = null;
                    FinishEditFileInterface fFileInterface = null;
                    if (!itemsToEdit.TryGetValue("rc", out fFileInterface))
                        itemsToEdit["rc"] = fEdit = new FinishEditRC(projectMisc.resourceFilePath);
                    else
                        fEdit = fFileInterface as FinishEditRC;
                    fEdit.AddRgs(projectMisc.rgsFileName, projectMisc.rgsIdString);
                }
            }
            return;
        }

        void IWizard.ProjectFinishedGenerating(Project project)
        {
        }
        void IWizard.RunFinished()
        {
            if (userCanceled)
                return;
            foreach (var d in m_dte.Documents)
            {
                Document doc = d as Document;
                var y = doc.Name;
                if ((y.EndsWith(".rc") || y.Contains(".rc.")) ||
                    (y.EndsWith(".idl") || y.Contains(".idl.")) ||
                    (y.Substring(0, m_itemName.Length) == m_itemName && y.EndsWith(".tmp")))
                     doc.Close();
            }
            postFinishEdit();
        }

        delegate void writeItem(string s);

        abstract class FinishEditFileInterface
        {
            public abstract string initDestinationFile(out IEnumerable<string> inLines);
            public abstract void doInsert(int lineNum, string inLine, writeItem cb);
        }
        abstract class FinishEditProjectItem : FinishEditFileInterface
        {
            public FinishEditProjectItem()
            {            }

            public void Add(ProjectItem pi)
            { this.items.Add(pi); }

            protected void insert(writeItem cb)
            {
                foreach (var item in items)
                {
                    var fn = item.FileNames[0];
                    var lines = System.IO.File.ReadLines(fn);
                    foreach (var line in lines)
                        cb(line);
                    item.Remove();
                    System.IO.File.Delete(fn);
                }
            }

            protected List<ProjectItem> items = new List<ProjectItem>();
        }

        class FinishEditRC : FinishEditProjectItem
        {   // class to manage edit of project resource, .rc file
            public FinishEditRC(string resourceFilePath)
            {   
                this.resourceFilePath = resourceFilePath;
            }

            public override string initDestinationFile(out IEnumerable<string> inLines)
            {
                inLines = System.IO.File.ReadLines(resourceFilePath);
                return resourceFilePath;
             }

            public override void doInsert(int lineNum, string inLine, writeItem cb)
            {
                var spl = inLine.Split(new char[0] { }, StringSplitOptions.RemoveEmptyEntries);
                if (!inserted)
                {
                    if (spl.Length > 1 && spl[0].StartsWith("IDD_") && spl[1].ToUpper() == "DIALOG")
                    {
                        if (rgsItems.Any() && !insertedRGS)
                            insertRGS(cb); // if we get to dialogs and no RGS yet, then do RGS now
                        inserted = true;
                        insert(cb);
                    }
                }
                if (!insertedRGS)
                {
                    if (spl.Length > 1 && spl[0].StartsWith("IDR_") && spl[1].ToUpper() == "REGISTRY")
                            insertRGS(cb);
                }
            }

            protected void insertRGS(writeItem cb)
            {
                if (insertedRGS || !rgsItems.Any())
                    return;
                insertedRGS = true;
                foreach (var r in rgsItems)
                {
                    StringBuilder sb = new StringBuilder();
                    sb.Append(r.rgsId);
                    sb.Append(" REGISTRY \"");
                    sb.Append(r.rgsFile);
                    sb.Append('"');
                    cb(sb.ToString());
                }
            }

            public void AddRgs(string fname, string idstring)
            {
                rgsItems.Add(new RGS(fname, idstring));
            }

            string resourceFilePath;
            bool inserted = false;
            bool insertedRGS = false;
            struct RGS
            {
                public RGS(string f, string id)
                {
                    rgsFile = f;
                    rgsId = id;
                }
                public string rgsFile;
                public string rgsId;
            }

            List<RGS> rgsItems = new List<RGS>();
        }

        class FinishEditIDL : FinishEditProjectItem
        {   // class to manage edit of project IDL file
            public FinishEditIDL(string idlFilePath) 
            {
                this.idlFilePath = idlFilePath;
            }

            public override string initDestinationFile(out IEnumerable<string> inLines)
            {
                int lnum = 0;
                inLines = System.IO.File.ReadLines(idlFilePath);
                foreach (var l in inLines)
                {
                    var comment1 = l.IndexOf("//");
                    if (comment1 < 0)
                        comment1 = l.Length;
                    var comment2 = l.IndexOf("/*");
                    if (comment2 < 0)
                        comment2 = l.Length;
                    var closeBracket = l.IndexOf('}');
                    if ((closeBracket >= 0) && (closeBracket < comment1) && (closeBracket < comment2))
                        bracketLine = lnum;
                    lnum += 1;
                }
                return idlFilePath;
            }

            public override void doInsert(int lineNum, string inLine, writeItem cb)
            {
                if (inserted)
                    return;
                if (lineNum == bracketLine)
                {
                    inserted = true;
                    insert(cb);
                }
            }

            string idlFilePath;
            int bracketLine = -1;
            bool inserted = false;
        }

        class FinishEditRscH : FinishEditFileInterface
        {   // class to managed editing of project resource header file
            public FinishEditRscH(string resourcehFile, Dictionary<string, int> ins)
            {
                resourcehFilePath = resourcehFile;
                toInsert = ins;
            }

            public override string initDestinationFile(out IEnumerable<string> inLines)
            {
                int lnum = 0;
                inLines = System.IO.File.ReadLines(resourcehFilePath);
                foreach (var l in inLines)
                {
                    var spl = l.Split(new char[0], StringSplitOptions.RemoveEmptyEntries);
                    bool foundDefine = false;
                    foreach (var w in spl)
                    {
                        if (w.StartsWith("//") || w.StartsWith("/*"))
                            break;
                        if (!foundDefine)
                        {
                            if (w == "#define")
                                foundDefine = true;
                        }
                        else
                        {
                            if (w.StartsWith("IDD_"))
                                bracketLine = lnum;
                            break;
                        }
                    }
                    lnum += 1;
                }
                return resourcehFilePath;
            }

            public override void doInsert(int lineNum, string inLine, writeItem cb)
            {
                if (inserted) return;
                if (lineNum == bracketLine)
                {
                    inserted = true;
                    insert(cb);
                }
            }

            void insert(writeItem cb)
            {
                foreach (var item in toInsert)
                {
                    StringBuilder sb = new StringBuilder();
                    sb.Append("#define");
                    sb.Append(" ");
                    string it = item.Key.ToString();
                    sb.Append(it);
                    const int TAB_LEN = 32;
                    var tab = TAB_LEN - it.Length;
                    while (true)
                    {
                        sb.Append(" ");
                        if (--tab < 0)
                            break;
                    }
                    sb.Append(item.Value);
                    cb(sb.ToString());
                }
            }

            string resourcehFilePath;
            int bracketLine = -1;
            Dictionary<string, int> toInsert;
            bool inserted = false;
        }

        void postFinishEdit()
        {
            if (!itemsToEdit.Any())
                return;
            foreach (var dict in itemsToEdit)
            {
                IEnumerable<String> inLines = null;
                string destFileName = dict.Value.initDestinationFile(out inLines);
                string tempFileName = destFileName + ".tmp";
                var resFile = new System.IO.StreamWriter(tempFileName);
                using (resFile)
                {
                    int lineNum = 0;
                    foreach (var l in inLines)
                    {
                        dict.Value.doInsert(lineNum, l, (string s) =>
                        {
                            resFile.WriteLine(s);
                        });
                        resFile.WriteLine(l);
                        lineNum++;
                    }
                    resFile.Close();
                }
                System.IO.File.Replace(tempFileName, destFileName, destFileName + ".bak");
            }
        }
    }
}
