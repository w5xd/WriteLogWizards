using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WL12ModuleItem.CppTemplates.WL12ModuleTemplate
{
    public partial class ContestModuleDlg : Form
    {
        public string Basename { set { m_BaseName = value; } }
        public Dictionary<string, string> Symbols { set { dict = value; } }

        Dictionary<string, string> dict;

        string m_BaseName;

        public ContestModuleDlg()
        {
            InitializeComponent();
        }

        void formToDict()
        {
            dict["$RTTY$"] = checkBoxExcRtty.Checked ? "1" : "0";
            dict["$CABRILLO$"] = checkBoxExcCabrillo.Checked ? "1" : "0";
            dict["$NR_IN_EXCHANGE$"] = checkBoxExcNR.Checked ? "1" : "0";
            dict["$PTS_COLUMN$"] = checkBoxExcPts.Checked ? "1" : "0";
            dict["$RST_IN_EXCHANGE$"] = checkBoxExcRst.Checked ? "1" : "0";

            dict["$SINGLE_MODE$"] = radioButtonModeSingle.Checked ? "1" : "0";
            dict["$ASK_MODE$"] = radioButtonModeAsk.Checked ? "1" : "0";
            dict["$MULTI_MODE$"] = radioButtonModeMulti.Checked ? "1" : "0";

            dict["$NO_DXCC$"] = radioButtonDxNone.Checked ? "1" : "0";
            dict["$DXCC_SINGLE_BAND$"] = radioButtonDxYes.Checked ? "1" : "0";
            dict["$DXCC_MULTI_BAND$"] = radioButtonDxByBand.Checked ? "1" : "0"; ;

            dict["$NO_AYGMULT$"] = radioButtonAygNone.Checked ? "1" : "0";
            dict["$AYGMULT_SINGLE_BAND$"] = radioButtonAygYes.Checked ? "1" : "0";
            dict["$AYGMULT_MULTI_BAND$"] = radioButtonAygByBand.Checked ? "1" : "0";

            dict["$NO_ZONE$"] = radioButtonZnone.Checked ? "1" : "0";
            dict["$ZONE_SINGLE_BAND$"] = radioButtonZyes.Checked ? "1" : "0";
            dict["$ZONE_MULTI_BAND$"] = radioButtonZbyBand.Checked ? "1" : "0";

            dict["$NO_NAMEDMULT$"] = radioButtonNmNone.Checked ? "1" : "0";
            dict["$NAMEDMULT_SINGLE_BAND$"] = radioButtonNmYes.Checked ? "1" : "0";
            dict["$NAMEDMULT_MULTI_BAND$"] = radioButtonNmByBand.Checked ? "1" : "0"; ;
            dict["$MULTIPLE_NAMED_IN_QSO$"] = checkBox2inQSO.Checked ? "1" : "0";
            dict["$MULTIPLE_NAMED_IN_QSO_TX$"] = checkBox2TXinQSO.Checked ? "1" : "0";
            dict["$NAMED_MULTI_REGION$"] = checkBox2NamedPages.Checked ? "1" : "0";

            dict["$CAN_LOG_ROVER$"] = checkBoxRoveLogging.Checked ? "1" : "0";
            dict["$AM_ROVER$"] = checkBoxRoveCanBe.Checked ? "1" : "0";
            dict["$TQSL_ROVER$"] = checkBoxRoveTQSL.Checked ? "1" : "0";
            dict["$AM_COUNTYLINE$"] = checkBoxRoveCountyLine.Checked ? "1" : "0";

            dict["$MM_CLASS_NAME$"] = textBoxClassName.Text;
            dict["$CONTEST_NAME$"] = textBoxDisplayName.Text;

        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            formToDict();
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void ContestModuleDlg_Load(object sender, EventArgs e)
        {
            labelBaseName.Text = m_BaseName;
            textBoxClassName.Text = m_BaseName;
            if (m_BaseName[0] != 'C')
                textBoxClassName.Text = m_BaseName.Insert(0, "C");
            perNamedMult();
        }

        private void splitContainerMultRove_SizeChanged(object sender, EventArgs e)
        {
           splitContainerMultRove.SplitterDistance = splitContainerMultRove.Size.Width/2;
        }

        private void radioButtonNmNone_CheckedChanged(object sender, EventArgs e)
        {
            perNamedMult();
        }

        void perNamedMult()
        {
            checkBoxRoveCanBe.Enabled = !radioButtonNmNone.Checked;
            checkBoxRoveCountyLine.Enabled = !radioButtonNmNone.Checked;
            checkBoxRoveTQSL.Enabled = !radioButtonNmNone.Checked;
            checkBoxRoveLogging.Enabled = !radioButtonNmNone.Checked;
            checkBox2inQSO.Enabled = !radioButtonNmNone.Checked;
            checkBox2NamedPages.Enabled = !radioButtonNmNone.Checked;
            checkBox2TXinQSO.Enabled= !radioButtonNmNone.Checked;

            if (!checkBoxRoveCanBe.Enabled)
                checkBoxRoveCanBe.Checked = false;
            if (!checkBoxRoveCountyLine.Enabled)
                checkBoxRoveCountyLine.Checked = false;
            if (!checkBoxRoveTQSL.Enabled)
                checkBoxRoveTQSL.Checked = false;
            if (!checkBoxRoveLogging.Enabled)
                checkBoxRoveLogging.Checked = false;
            if (!checkBox2NamedPages.Enabled)
                checkBox2NamedPages.Checked = false;
            if (!checkBox2TXinQSO.Enabled)
                checkBox2TXinQSO.Checked = false;
        }

        private void checkBoxRoveCountyLine_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxRoveCountyLine.Checked)
            {
                checkBoxRoveCanBe.Checked = true;
                checkBoxRoveLogging.Checked = true;
            }
        }

        private void checkBoxRoveCanBe_CheckedChanged(object sender, EventArgs e)
        {
            checkBoxRoveTQSL.Enabled = checkBoxRoveCanBe.Checked;
            if (!checkBoxRoveCanBe.Checked)
            {
                checkBoxRoveTQSL.Checked = false;
                checkBoxRoveCountyLine.Checked = false;
            }
            else
                checkBoxRoveLogging.Checked = true;
        }

        private void checkBoxRoveLogging_CheckedChanged(object sender, EventArgs e)
        {
            if (!checkBoxRoveLogging.Checked)
                checkBoxRoveCanBe.Checked = false;
        }
    }
}
