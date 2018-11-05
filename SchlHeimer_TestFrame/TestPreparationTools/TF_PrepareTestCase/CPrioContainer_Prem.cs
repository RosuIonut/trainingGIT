using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace TF_PrepareTestCase
{
    class CPrioContainer_Prem
    {
        public List<string> m_Result;
        private string m_Folder;
        private List<string> m_CATList, m_PremIDs, m_AlwaysOn;
        public CPrioContainer_Prem()
        {
            bool q_drive = System.IO.Directory.Exists("Q:");
            if (q_drive)
            {

                m_Folder = "Q:\\adapt\\gensrc\\prj\\hmimodel\\Widgets\\";
                m_CATList = new List<string>();
                m_PremIDs = new List<string>();
                m_AlwaysOn = new List<string>();
                m_Result = new List<string>();
            }
            else
            { 
                Console.WriteLine("No q drive adjusted");
            }
        }

        public void run(string a_line)
        {
            // d:\Projects\A0\a0_simu_its\adapt\gensrc\prj\hmimodel\Widgets\KombiView_MFA_RANGE.xml
            string XMLCatName = "KombiView_" + a_line + ".xml";
            
            m_CATList.Add(XMLCatName);
            try
            {
                while (m_CATList.Count != 0)
                {
                    List<string> myList = new List<string>(m_CATList);
                    m_CATList.Clear();
                    foreach (string _element in myList)
                    {
                        string XMLCatFullPath = System.IO.Path.Combine(m_Folder, _element);
                        XElement myElement = XElement.Load(XMLCatFullPath);
                        Recursion(myElement, 0);
                    }
                }

                bool ListStart = true;
                foreach (string myString in m_PremIDs)
                {
                    if(ListStart)
                    {
                        m_Result.Add("liststart(" + myString + ")");
                    }
                    else
                    {
                        m_Result.Add("listinsert(" + myString + ")");
                    }
                    ListStart = false;
                }
                foreach (string myString in m_AlwaysOn)
                {
                    m_Result.Add("allwayson(" + myString + ")");
                }

            }
            catch(Exception myExc)
            {
                Console.WriteLine(myExc.ToString());
            }
        }

        private void PrioContainer_element(XElement a_element)
        {
            bool my_alwayson = false;
            var cust = from c in a_element.Elements("Properties").Elements("Property") select c;
            foreach (var customer in cust)
            {
                if (customer.Attribute("Name").Value.ToLower().Equals("alwayson"))
                {
                    var myalwayson_query = from premid in customer.Elements("Constant") select premid;
                    foreach (var myallwayson_iterator in myalwayson_query)
                    {
                        if (myallwayson_iterator.Value.ToString().Equals("true"))
                        {
                            my_alwayson = true;
                        }
                    }
                }
            }
            foreach (var customer in cust)
            {
                if (customer.Attribute("Name").Value.ToLower().Equals("premid"))
                {
                    var mypremid_query = from premid in customer.Elements("Constant") select premid;
                    foreach (var mypremid in mypremid_query)
                    {
                        m_PremIDs.Add(mypremid.Value.ToString());
                        if (my_alwayson)
                        {
                            m_AlwaysOn.Add(mypremid.Value.ToString());
                        }
                    }
                }
            }
        }

        private void Recursion(XElement a_element, int a_RecursionDepth)
        {
            a_RecursionDepth++;

            if (a_element.Name.ToString().Equals("FileLink"))
            {
                System.Xml.Linq.XAttribute myName = a_element.Attribute("Name");
                m_CATList.Add(myName.Value.ToString());
            }

            if (a_element.Name.ToString().Equals("Widget"))
            {
                System.Xml.Linq.XAttribute myType = a_element.Attribute("Type");
                if(myType.Value.ToString().Equals("PrioContainer"))
                {
                    PrioContainer_element(a_element);
                }
                if (myType.Value.ToString().Equals("VWInitialPopupController"))
                {
                    /* Do not recurse through Initial popup controller */
                    return;
                }
            }

            foreach (XElement a in a_element.Elements())
            {
                Recursion(a, a_RecursionDepth);
            }
        }
    }
}
