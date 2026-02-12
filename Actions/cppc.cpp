#include "cppc.h"
#include "..\Statements\Statement.h"
#include "..\Statements\Condition.h"
#include "..\Statements\Write.h"
#include "..\Statements\Read.h"
#include "..\Statements\ValueAssign.h"
#include "..\Statements\VariableAssign.h"
#include "..\Statements\Declare.h"
#include "..\Statements\OperationAssign.h"
cppc::cppc(ApplicationManager* pAppManager) :Action(pAppManager) //constructor
{
}
void cppc::ReadActionParameters() {
   
}

void cppc::Execute() {
    Output* pOut = pManager->GetOutput();
    Input* pIn = pManager->GetInput();
    Statement* pCurrent = pManager->GetStartStatement();
    if (!pCurrent)
    {
        return;
    }
    ofstream codeFile("generated.cpp");

    codeFile << "#include <iostream>\n";
    codeFile << "using namespace std;\n\n";
    codeFile << "int main()\n{\n";

    while (pCurrent) {
        if (auto* cond = dynamic_cast<Condition*>(pCurrent))
        {
            codeFile << "if ("
                << cond->getLHS() << " "
                << cond->getComp() << " "
                << cond->getRHS()
                << ")\n{\n";

            // TRUE path
            Statement* trueStat =
                cond->getOutConnectorTrue()->getDstStat();

            Statement* temp = trueStat;
            while (temp)
            {
                if (dynamic_cast<Condition*>(temp))
                    break;

                if (auto* w = dynamic_cast<Declare*>(temp))
                {
                    codeFile << "double " << w->GetVarName() << ";\n";
                }
                if (auto* w = dynamic_cast<Write*>(temp))
                {
                    codeFile << "cout << " << w->getVar() << " << endl;\n";
                }
                else if (auto* r = dynamic_cast<Read*>(temp))
                {
                    codeFile << "cin >> " << r->getVar() << ";\n";
                }
                else if (auto* v = dynamic_cast<ValueAssign*>(temp))
                {
                    codeFile << v->getLHS() << " = " << v->getRHS() << ";\n";
                }
                else if (auto* v = dynamic_cast<VariableAssign*>(temp))
                {
                    codeFile << v->getLHS() << " = " << v->getRHS() << ";\n";
                }
                else if (auto* op = dynamic_cast<OperationAssign*>(temp))
                {
                    codeFile << op->getLHS()
                        << " = "
                        << op->getOp1()
                        << " "
                        << op->getOperator()
                        << " "
                        << op->getOp2()
                        << ";\n";
                };

                Connector* c = temp->getOutConnector();
                temp = c ? c->getDstStat() : nullptr;
            }

            codeFile << "}\n";

            // FALSE path 
            if (cond->getOutConnectorFalse())
            {
                codeFile << "else\n{\n";

                Statement* falseStat =
                    cond->getOutConnectorFalse()->getDstStat();

                temp = falseStat;
                while (temp)
                {
                    if (dynamic_cast<Condition*>(temp))
                        break;

                    if (auto* w = dynamic_cast<Declare*>(temp))
                    {
                        codeFile << "double " << w->GetVarName() << ";\n";
                    }
                    if (auto* w = dynamic_cast<Write*>(temp))
                    {
                        codeFile << "cout << " << w->getVar() << " << endl;\n";
                    }
                    else if (auto* r = dynamic_cast<Read*>(temp))
                    {
                        codeFile << "cin >> " << r->getVar() << ";\n";
                    }
                    else if (auto* v = dynamic_cast<ValueAssign*>(temp))
                    {
                        codeFile << v->getLHS() << " = " << v->getRHS() << ";\n";
                    }
                    else if (auto* v = dynamic_cast<VariableAssign*>(temp))
                    {
                        codeFile << v->getLHS() << " = " << v->getRHS() << ";\n";
                    }
                    else if (auto* op = dynamic_cast<OperationAssign*>(temp))
                    {
                        codeFile << op->getLHS()
                            << " = "
                            << op->getOp1()
                            << " "
                            << op->getOperator()
                            << " "
                            << op->getOp2()
                            << ";\n";
                    }
                    Connector* c = temp->getOutConnector();
                    temp = c ? c->getDstStat() : nullptr;
                }

                codeFile << "}\n";
            }

            
            pCurrent = nullptr;  
            continue;
        }
        
        if (auto* w = dynamic_cast<Declare*>(pCurrent))
        {
            codeFile<<"double " << w->GetVarName() << ";\n";
        }
        if (auto* w = dynamic_cast<Write*>(pCurrent))
        {
            codeFile << "cout << " << w->getVar() << " << endl;\n";
        }
        else if (auto* r = dynamic_cast<Read*>(pCurrent))
        {
            codeFile << "cin >> " << r->getVar() << ";\n";
        }
        else if (auto* v = dynamic_cast<ValueAssign*>(pCurrent))
        {
            codeFile << v->getLHS() << " = " << v->getRHS() << ";\n";
        }
        else if (auto* v = dynamic_cast<VariableAssign*>(pCurrent))
        {
            codeFile << v->getLHS() << " = " << v->getRHS() << ";\n";
        }
        else if (auto* op = dynamic_cast<OperationAssign*>(pCurrent))
        {
            codeFile << op->getLHS()
                << " = "
                << op->getOp1()
                << " "
                << op->getOperator()
                << " "
                << op->getOp2()
                << ";\n";
        }

        Connector* nextConn = pCurrent->getOutConnector();
        pCurrent = nextConn ? nextConn->getDstStat() : nullptr;


    }

    codeFile << "return 0;\n}\n";
    codeFile.close();




    pOut->PrintMessage("Code Generation Completed Successfuly");

}
