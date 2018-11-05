using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TF_PrepareTestCase
{
    class CFunctionLine
    {
        public string m_Function;
        public string m_Argument;
        public string m_commentLine;

        public bool m_occured;

        public CFunctionLine()
        {
            m_occured = false;
            m_Function = "";
            m_Argument = "";
        }

        public void run (string a_DocLine)
        {
            m_occured = false;
            int my_startIdx = a_DocLine.IndexOf("(");
            int my_EndIdx = a_DocLine.IndexOf(")");

            /* Did we found the braces */
            if((my_startIdx > 0)&&(my_EndIdx > 0))
            {
                /* Are these Function braces */
                if(my_EndIdx > my_startIdx + 1)
                {
                    m_Function = a_DocLine.Substring(0, my_startIdx);
                    m_Argument = a_DocLine.Substring(my_startIdx + 1, my_EndIdx - my_startIdx - 1);
                    m_occured = true;
                }
            } /* Did we found the braces */
        }

        public void make_comment(string myLine)
        {
            m_commentLine = "# -- " + myLine;
        }
    }
}
