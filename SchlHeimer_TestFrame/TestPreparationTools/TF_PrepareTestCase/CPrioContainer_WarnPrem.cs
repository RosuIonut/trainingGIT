using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace TF_PrepareTestCase
{
    class CPrioContainer_WarnPrem
    {
        enum TraverseState
        {
            TracePrioContainer,
            FindPrioContainer,
            FindWarningID
        }
        private string m_Document;
        private string m_WarningID;
        private bool m_breakRecursion;
        private string m_PremID;
        private TraverseState enState;
        public string PremID
        {
            get
            {
                return m_PremID;
            }
        }
        public bool FoundPrem
        {
            get
            {
                return m_breakRecursion;
            }
        }

        public CPrioContainer_WarnPrem()
        {
            bool q_drive = System.IO.Directory.Exists("Q:");
            m_breakRecursion = false;
            enState = TraverseState.TracePrioContainer;
            m_PremID = "";
            if (q_drive)
            {
                m_Document = "Q:\\adapt\\gensrc\\prj\\hmimodel\\Widgets\\CommonWidgetTree_VWInitialPopupControllerWarningPopupView.xml";
            }
            else
            {
                Console.WriteLine("No q drive adjusted");
            }
        }

        private void PrioContainer_element(XElement a_element, ref string a_PremID)
        {
            var cust = from c in a_element.Elements("Properties").Elements("Property") select c;
            foreach (var customer in cust)
            {
                if (customer.Attribute("Name").Value.ToLower().Equals("premid"))
                {
                    var mypremid_query = from premid in customer.Elements("Constant") select premid;
                    foreach (var mypremid in mypremid_query)
                    {
                        a_PremID = mypremid.Value.ToString();
                    }
                    if(enState == TraverseState.FindPrioContainer)
                    {
                        m_breakRecursion = true;
                        m_PremID = a_PremID;
                    }
                }
            }
        }

        private void Search_WarningID(XElement a_element, ref string a_PremID)
        {
            var cust = from c in a_element.Elements("Properties").Elements("Property") select c;
            foreach (var customer in cust)
            {
                if(customer.Attribute("Name").Value.Equals("keys"))
                {
                    var warning_cust = from wac in customer.Elements("ListValue").Elements("Constant") select wac;
                    foreach(var warning_id in warning_cust)
                    {
                        if (warning_id.Value.Equals(m_WarningID) )
                        {
                            if(enState == TraverseState.TracePrioContainer && !a_PremID.Equals(""))
                            {
                                m_PremID = a_PremID;
                                m_breakRecursion = true;
                            }
                            else if(enState == TraverseState.FindWarningID)
                            {
                                enState = TraverseState.FindPrioContainer;
                            }
                        }
                    }
                }
            }
        }

        private void Recursion(XElement a_element, int a_RecursionDepth, ref string a_PremID)
        {
            string my_PremID = a_PremID;
            a_RecursionDepth++;

            if (a_element.Name.ToString().Equals("Widget"))
            {
                System.Xml.Linq.XAttribute myType = a_element.Attribute("Type");
                if (myType.Value.ToString().Equals("PrioContainer"))
                {
                    PrioContainer_element(a_element, ref my_PremID);
                }
                else if ((myType.Value.ToString().Equals("VWWarningPopupContent"))/*&&(!my_PremID.Equals(""))*/)
                {
                    Search_WarningID(a_element, ref my_PremID);
                }
            }

            if(m_breakRecursion == true)
            {
                return;
            }

            foreach (XElement a in a_element.Elements())
            {
                Recursion(a, a_RecursionDepth, ref my_PremID);
                if(m_breakRecursion == true)
                {
                    return;
                }
            }
        }


        private bool IsPrioContainerParent(XElement a_element)
        {
            int prio_cont_depth = 0;
            CountDepth(a_element,ref prio_cont_depth, "PrioContainer");
            int vwwarningpopup_depth = 0;
            CountDepth(a_element, ref vwwarningpopup_depth, "VWWarningPopupContent");
            if(prio_cont_depth == vwwarningpopup_depth)
                throw new System.InvalidOperationException("Depth of PrioContainer and VWWarningPopupContent are the same. ");
            if (prio_cont_depth < vwwarningpopup_depth)
                return true;
            return false;
        }

        private bool CountDepth(XElement a_element, ref int a_RecursionDepth, string widget_name)
        {
            a_RecursionDepth++;

            if (a_element.Name.ToString().Equals("Widget"))
            {
                System.Xml.Linq.XAttribute myType = a_element.Attribute("Type");
                if (myType.Value.ToString().Equals(widget_name))
                {
                    return true;
                }
            }

            foreach (XElement a in a_element.Elements())
            {
                if (CountDepth(a, ref a_RecursionDepth, widget_name))
                {
                    return true;
                }
            }
            return false;
        }

        public void run(string a_line)
        {
            string my_PremID = "";
            //convert hex to dec
            if(a_line.StartsWith("0X",true,null))
            {
                int decimal_value = Convert.ToInt32(a_line, 16);
                a_line = decimal_value.ToString();
            }
            m_WarningID = a_line;
            XElement myElement = XElement.Load(m_Document);
            if (IsPrioContainerParent(myElement))
                enState = TraverseState.TracePrioContainer;
            else
                enState = TraverseState.FindWarningID;
            Recursion(myElement, 0, ref my_PremID);
        }
    }
}
