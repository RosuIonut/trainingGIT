using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TF_PrepareTestCase
{
    class Program
    {
        static void Main(string[] args)
        {
            /* Initialize operating classes */
            CFunctionLine myFunctionLine = new CFunctionLine();
            CPrioContainer_WarnPrem myWarnPrem = new CPrioContainer_WarnPrem();

            /* check arguments that were passed to the tool */
            if(args.Length != 2)
            {
                Console.WriteLine("usage : TF_PrepareTestCase <source> <target>");
                return;
            }

            if(args[0].ToString().Equals(args[1].ToString()))
            {
                Console.WriteLine("source and target have to be different");
                return;
            }

            try
            {
                /* Open source and target, read the source */
                System.IO.TextWriter myTarget = System.IO.File.CreateText(args[1].ToString());
                string[] myReadLines = System.IO.File.ReadAllLines(args[0].ToString());

                /* Process all the lines */
                foreach (string myDocLine in myReadLines)
                {
                    bool myPassLineToTarget = true;
                    myFunctionLine.run(myDocLine);
                    if (myFunctionLine.m_occured)
                    {
                        if (myFunctionLine.m_Function.Equals("PremIds"))
                        {
                            /* Provide obj which will collect PremIDs */
                            CPrioContainer_Prem myPremIdColl = new CPrioContainer_Prem();

                            /*line shall not stay as original */
                            myPassLineToTarget = false;

                            /* pass the original line as comment */
                            myFunctionLine.make_comment(myDocLine);
                            myTarget.WriteLine(myFunctionLine.m_commentLine);

                            /* collect the Prem IDs */
                            myPremIdColl.run(myFunctionLine.m_Argument);
                            foreach (string myLine in myPremIdColl.m_Result)
                            {
                                myTarget.WriteLine(myLine);
                            }
                        }
                        if (myFunctionLine.m_Function.Equals("WarnPrem"))
                        {
                            /*line shall not stay as original */
                            myPassLineToTarget = false;

                            /* pass the original line as comment */
                            myFunctionLine.make_comment(myDocLine);
                            myTarget.WriteLine(myFunctionLine.m_commentLine);

                            /*search Prem ID for passed warning ID */
                            myWarnPrem.run(myFunctionLine.m_Argument);
                            if (myWarnPrem.FoundPrem)
                            {
                                myTarget.WriteLine("prioscheduler(" + myWarnPrem.PremID + ")");
                            }
                            else
                            {
                                myTarget.WriteLine("#====FAILURE PRIO CONTAINER for " + myFunctionLine.m_Argument + "============");
                            }
                        }
                        if (myFunctionLine.m_Function.Equals("WarnPrem_Dpool"))
                        {
                            /*line shall not stay as original */
                            myPassLineToTarget = false;

                            /* pass the original line as comment */
                            myFunctionLine.make_comment(myDocLine);
                            myTarget.WriteLine(myFunctionLine.m_commentLine);

                            /*search Prem ID for passed warning ID */
                            myWarnPrem.run(myFunctionLine.m_Argument);
                            if (myWarnPrem.FoundPrem)
                            {
                                myTarget.WriteLine("fuid_dpoolreg(" + myWarnPrem.PremID + ")");
                            }
                            else
                            {
                                myTarget.WriteLine("#====FAILURE PRIO CONTAINER for " + myFunctionLine.m_Argument + "============");
                            }
                        }
                    }
                    if(myPassLineToTarget)
                    {
                        myTarget.WriteLine(myDocLine);
                    }
                }
                myTarget.Close();
            }
            catch(FieldAccessException Exc)
            {
                Console.WriteLine(Exc.ToString());
            }
        }
    }
}
