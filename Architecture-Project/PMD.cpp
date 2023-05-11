#include<QFileDialog>
#include<fstream>
#include<QFile>
#include<QMessageBox>
#include<QTableWidgetItem>
#include<QtDebug>
#include<QString>
#include<sstream>
#include<ctype.h>
#include <QMenuBar>
#include "PMD.h"
#include "./ui_interface.h"

using namespace std;

Our_Window::Our_Window(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    Reseter = 1;
    Compile_Status = 0;

    ui->setupUi(this);
    setWindowTitle("PMD Mano Simulator");
    ui->TextBox->setFocus();
    setFixedSize(1920,1080);
    New_slot();

    connect(ui->New_Btn, SIGNAL(clicked()), this, SLOT(New_slot()));
    connect(ui->Run_Btn, SIGNAL(clicked()), this, SLOT(run_slot()));
    connect(ui->Compile_Btn, SIGNAL(clicked()), this, SLOT(Compile_Slot()));
    connect(ui->Reset_Btn, SIGNAL(clicked()), this, SLOT(Reset_Slot()));
    connect(ui->Save_Btn, SIGNAL(clicked()), this, SLOT(save_slot()));
    connect(ui->Next_Step_Btn, SIGNAL(clicked()), this, SLOT(Next_Slot()));
    connect(ui->Open_Btn, SIGNAL(clicked()), this, SLOT(open_slot()));
    connect(ui->Exit, SIGNAL(clicked()), this, SLOT(Exit_slot()));

}

//////////////////////////////////////////////////////// SLOTS ////////////////////////////////////////////////////////

void Our_Window::Reset_Slot()
{
    Reseter=1;
    Compile_Status=0;
    SC_int=0;
    SC.reset();
    PC.reset();
    AR.reset();
    IR.reset();
    DR.reset();
    AC.reset();
    TR.reset();
    INPR.reset();
    OUTR.reset();
    I.reset();
    S.reset();
    E.reset();
    R.reset();
    IEN.reset();
    FGI.reset();
    FGO.reset();

    ui->SC_Box->setAlignment(Qt::AlignHCenter);

    ui->PC_Box->setAlignment(Qt::AlignHCenter);

    ui->AR_Box->setAlignment(Qt::AlignHCenter);

    ui->IR_Box->setAlignment(Qt::AlignHCenter);

    ui->DR_Box->setAlignment(Qt::AlignHCenter);

    ui->AC_Box->setAlignment(Qt::AlignHCenter);

    ui->TR_Box->setAlignment(Qt::AlignHCenter);

    ui->INPR_Box->setAlignment(Qt::AlignHCenter);

    ui->OUTR_Box->setAlignment(Qt::AlignHCenter);

    ui->I_Box->setAlignment(Qt::AlignHCenter);

    ui->S_Box->setAlignment(Qt::AlignHCenter);

    ui->E_Box->setAlignment(Qt::AlignHCenter);

    ui->R_Box->setAlignment(Qt::AlignHCenter);

    ui->IEN_Box->setAlignment(Qt::AlignHCenter);

    ui->FGI_Box->setAlignment(Qt::AlignHCenter);

    ui->FGO_Box->setAlignment(Qt::AlignHCenter);

    ui->input->setAlignment(Qt::AlignHCenter);

    ui->Def_Op->setAlignment(Qt::AlignHCenter);

    for(int i=0; i<4096; RAM[i].reset(), i++);

    set_show_Table();
    set_show_Registers();
    ui->OutPut_Message->setText("");
    Variable.clear();
    Datas.clear();
    Lines.clear();
    Stage_Num=0;
    PC_int=0;
    Print_Status=1;
    Memory.clear();
    Run_Status=0;
    ui->Run_Btn->setEnabled(true);
    ui->Next_Step_Btn->setEnabled(true);
    ui->input->setText("");
    ui->Def_Op->setText("");

}

void Our_Window::Compile_Slot()
{
    Reset_Slot();
    Compile_Status=1;
    int location=0;
    int secLOc=0;
    ui->OutPut_Message->setText("");
    Lines = ui->TextBox->toPlainText().split('\n', Qt::SkipEmptyParts);

    int EP=0;

    for(int i=0; i<(Lines.size());i++)
    {
        if(location>4096)
        {
            ui->OutPut_Message->setText("ERROR: RAM Por Shode ast.\n");
            Compile_Status=0;
            break;
        }
         QStringList Data_Analyse = Lines.at(i).split(' ', Qt::SkipEmptyParts);
         if(Data_Analyse.at(0)[Data_Analyse.at(0).size()-1]==',')
                 {
                     QString stop="";
                     for(int i=0;i<Data_Analyse.at(0).size()-1;i++)
                     {
                         stop+=Data_Analyse.at(0)[i];
                     }
                     Datas[stop]=secLOc;
                 }
        else if(Data_Analyse.at(0)=="ORG")
         {
             if(Check_HEX_Number(Data_Analyse.at(1)))
             {
                 bool ok=1;
                 secLOc = Data_Analyse.at(1).toInt(&ok,16);
                 continue;
             }
             else
             {
                 ui->OutPut_Message->setText("error! there must be a number after ORG. \n");
                 Compile_Status=0;
                 break;
             }
         }
    Memory[secLOc]=i;
    secLOc+=1;
    }

    for(int i=0; i<Lines.size();i++)
    {
        if(location>4096)
        {
            ui->OutPut_Message->setText("ERROR: RAM Por Shode ast.\n");
            Compile_Status=0;
            break;
        }

        QStringList Data_Analyse = Lines.at(i).split(' ', Qt::SkipEmptyParts);

        if(Data_Analyse.at(0)=="/"||Data_Analyse.at(0)=="//" )//better be :
        {
            Stage_Num++;
           continue;
        }

        else if(Data_Analyse.at(0)=="ORG")
        {
            if(Check_HEX_Number(Data_Analyse.at(1)))
            {
                bool ok=1;
                location = Data_Analyse.at(1).toInt(&ok,16);
                continue;
            }
            else
            {
                ui->OutPut_Message->setText("error! there must be a number after ORG. \n");
                Compile_Status=0;
                break;
            }
        }

        else if(Data_Analyse.at(0)[Data_Analyse.at(0).size()-1]==',')
        {
            Variable[Data_Analyse.at(0)] = i;
            QTableWidgetItem *name = new QTableWidgetItem();
            name->setText(Data_Analyse.at(0));
            name->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,0,name);

            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);

            if(Data_Analyse.at(1)=="HEX")
            {
                if(Check_HEX_Number(Data_Analyse.at(2)))
                {
                    bool ok=1;
                    RAM[location]=Data_Analyse.at(2).toInt(&ok,16);;
                    location++;
                    continue;
                }
                else
                {
                    ui->OutPut_Message->setText("there must be a number after HEX. \n");
                    Compile_Status=0;
                    break;
                }
            }
            else if(Data_Analyse.at(1)=="DEC")
            {
                if(Check_HEX_Number(Data_Analyse.at(2)))
                {
                    bool ok=1;
                    RAM[location]=Data_Analyse.at(2).toInt(&ok,10);
                    location++;
                    continue;
                }
                else
                {
                    ui->OutPut_Message->setText("there must be a number after DEC. \n");
                    Compile_Status=0;
                    break;
                }
            }

            else
                for(int x=0;x<(Data_Analyse.length())-1; Data_Analyse[x]=Data_Analyse[x+1], x++);

        }

        if(Data_Analyse.at(0)=="DEC")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            if(Data_Analyse.length()>1 && Check_HEX_Number(Data_Analyse.at(1)) )
            {
                bool ok=1;
                RAM[location]=Data_Analyse.at(1).toInt(&ok,10);
            }
            else
            {
                ui->OutPut_Message->setText("there must be a number after DEC. \n");
                Compile_Status=0;
                break;
            }

        }

        else if(Data_Analyse.at(0)=="HEX")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            if(Data_Analyse.length()>1 && Check_HEX_Number(Data_Analyse.at(1)))
            {
                bool ok=1;
                int convertNumber=Data_Analyse.at(1).toInt(&ok,16);
                RAM[location]=convertNumber;
            }
            else
            {
                ui->OutPut_Message->setText("there must be a number after HEX. \n");
                Compile_Status=0;
                break;
            }
        }

        else if(Data_Analyse.at(0)=="AND")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            if(Datas[Data_Analyse.at(1)]==0)
            {
                ui->OutPut_Message->setText("Error in line:"+QString::number(i+1)+"there is no label by name"+Data_Analyse.at(1));
                Compile_Status=0;
                break;

            }
            if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
                RAM[location]=0x8000+Datas[Data_Analyse.at(1)];
            else
                RAM[location]=0x0000+Datas[Data_Analyse.at(1)];

        }

        else if(Data_Analyse.at(0)=="ADD")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            if(Datas[Data_Analyse.at(1)]==0)
            {
                ui->OutPut_Message->setText("line: "+QString::number(i+1)+"there is no label by name "+Data_Analyse.at(1));
                Compile_Status=0;
                break;
            }
            if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
                RAM[location]=0x9000+Datas[Data_Analyse.at(1)];
            else
                RAM[location]=0x1000+Datas[Data_Analyse.at(1)];
        }

        else if(Data_Analyse.at(0)=="LDA")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            if(Datas[Data_Analyse.at(1)]==0)
            {
                ui->OutPut_Message->setText("line: "+QString::number(i+1)+"there is no label by name "+Data_Analyse.at(1));
                Compile_Status=0;
                break;

            }
            if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
                RAM[location]=0xA000+Datas[Data_Analyse.at(1)];
            else
                RAM[location]=0x2000+Datas[Data_Analyse.at(1)];
        }

        else if(Data_Analyse.at(0)=="STA")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            if(Datas[Data_Analyse.at(1)]==0)
            {
                ui->OutPut_Message->setText("line: "+QString::number(i+1)+"there is no label by name "+Data_Analyse.at(1));
                Compile_Status=0;
                break;

            }

            if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
                RAM[location]=0xB000+Datas[Data_Analyse.at(1)];
            else
                RAM[location]=0x3000+Datas[Data_Analyse.at(1)];
        }

        else if(Data_Analyse.at(0)=="BUN")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            if(Datas[Data_Analyse.at(1)]==0)
            {
                ui->OutPut_Message->setText("line: "+QString::number(i+1)+"there is no label by name "+Data_Analyse.at(1));
                Compile_Status=0;
                break;
            }

            if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
                RAM[location]=0xC000+Datas[Data_Analyse.at(1)];
            else
                RAM[location]=0x4000+Datas[Data_Analyse.at(1)];

        }

        else if(Data_Analyse.at(0)=="BSA")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            if(Datas[Data_Analyse.at(1)]==0)
            {
                ui->OutPut_Message->setText("line: "+QString::number(i+1)+"there is no label by name "+Data_Analyse.at(1));
                Compile_Status=0;
                break;
            }

            if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
                RAM[location]=0xD000+Datas[Data_Analyse.at(1)];
            else
                RAM[location]=0x5000+Datas[Data_Analyse.at(1)];

        }

        else if(Data_Analyse.at(0)=="ISZ")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            if(Datas[Data_Analyse.at(1)]==0)
            {
                ui->OutPut_Message->setText("line: "+QString::number(i+1)+"there is no label by name "+Data_Analyse.at(1));
                Compile_Status=0;
                break;
            }
            if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
                RAM[location]=0xE000+Datas[Data_Analyse.at(1)];
            else
                RAM[location]=0x6000+Datas[Data_Analyse.at(1)];
        }

        else if(Data_Analyse.at(0)=="INP")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0xf800;
        }

        else if(Data_Analyse.at(0)=="OUT")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0xf400;
        }

        else if(Data_Analyse.at(0)=="SKI")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0xf200;
        }

        else if(Data_Analyse.at(0)=="SKO")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0xf100;
        }

        else if(Data_Analyse.at(0)=="ION")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0xf080;
        }

        else if(Data_Analyse.at(0)=="IOF")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0xf040;
        }

        else if(Data_Analyse.at(0)=="CLA")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0x7800;
        }

        else if(Data_Analyse.at(0)=="CLE")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0x7400;
        }

        else if(Data_Analyse.at(0)=="CMA")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0x7200;
        }

        else if(Data_Analyse.at(0)=="CME")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0x7100;
        }

        else if(Data_Analyse.at(0)=="CIR")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0x7080;
        }

        else if(Data_Analyse.at(0)=="CIL")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0x7040;
        }

        else if(Data_Analyse.at(0)=="INC")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0x7020;
        }

        else if(Data_Analyse.at(0)=="SPA")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0x7010;
        }

        else if(Data_Analyse.at(0)=="SNA")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0x7008;
        }

        else if(Data_Analyse.at(0)=="SZA")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0x7004;
        }

        else if(Data_Analyse.at(0)=="SZE")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0x7002;
        }

        else if(Data_Analyse.at(0)=="HLT")
        {
            QTableWidgetItem *relateditm = new QTableWidgetItem();
            relateditm->setText(Lines.at(i));
            relateditm->setTextAlignment(Qt::AlignCenter);
            ui->Answer_Table->setItem(location,2,relateditm);
            RAM[location]=0x7001;
        }

        else if(Data_Analyse.at(0)=="END")
        {
            EP=1;
            set_show_Table();
            if(Compile_Status)
                ui->OutPut_Message->setText("compile shod!\n");
            else
                ui->OutPut_Message->append("\nERROR IN COMPILING!\n");

            break;
        }

        else
        {
            ui->OutPut_Message->setText("command is wrong. \n");
            ui->OutPut_Message->append(Lines.at(i));
            Compile_Status=0;
            break;
        }

        location++;

    }

    if(!EP && Compile_Status)
    {
        ui->OutPut_Message->setText("error in compiling.\n");
        Compile_Status=0;
    }

}

void Our_Window::Next_Slot()
{
    if(!Run_Status)
        Print_Status = 1;

    if(!Compile_Status)
        ui->OutPut_Message->setText("\n Not Compile_Status \n");

    else
    {
        ui->Answer_Table->selectRow(PC_int - 1);
        ui->OutPut_Message->setText("");

        SC= SC_int;

        QStringList Data_Analyse = Lines.at(Stage_Num).split(' ', Qt::SkipEmptyParts);

        if(Data_Analyse.at(0)=="//" || Data_Analyse.at(0)[0]=='/')
           Stage_Num++;

        else if(Data_Analyse.at(0)=="ORG")
        {
            bool ok=1;
            PC_int=Data_Analyse.at(1).toInt(&ok,16);
            Stage_Num++;
        }

        Data_Analyse = Lines.at(Stage_Num).split(' ', Qt::SkipEmptyParts);

        /////////////////////////////////////////////////////// Start Program eith Fetch ///////////////////////////////////////////////////////

        if(SC_int==0)
        {
            S=1;
            SC_int++;
            SC = SC_int;
            PC = PC_int;
            AR = PC;


            if(Print_Status)
            {
                set_show_Registers();
                set_show_Table();
            }

            ui->Def_Op->setText("T0 FETCH: AR <- PC ");

            return;
        }

        else if(SC_int==1)
        {
            PC_int++;
            PC = PC_int;
            SC_int++;
            IR = RAM[AR.to_ulong()];
            SC= SC_int;

            if(Print_Status)
            {
                set_show_Registers();
                set_show_Table();
            }
            ui->Def_Op->setText("T1 FETCH: PC <- PC+1  , IR <- M[AR] ");

            return;
        }

        else if(SC_int==2)
        {
            for (int i=0; i<12; AR[i]=IR[i], i++);

            I[0]=IR[15];
            SC_int++;

            ui->Def_Op->setText("T2 DECODE: AR <- IR(0-11) , I <- IR(15) ");
            return;
        }

        if(Data_Analyse.at(0)[Data_Analyse.at(0).size()-1]==',')
            for(int i=0;i<(Data_Analyse.length())-1; Data_Analyse[i]=Data_Analyse[i+1], i++);

        if(Data_Analyse.at(0)=="DEC" || Data_Analyse.at(0)=="HEX")
            Reset_SC();

        /////////////////////////////////////////////////////// Memory Refrence ///////////////////////////////////////////////

        else if(Data_Analyse.at(0)=="AND")
            AND_Command(Data_Analyse);

        else if(Data_Analyse.at(0)=="ADD")
            ADD_Command(Data_Analyse);

        else if(Data_Analyse.at(0)=="LDA")
            LDA_Command(Data_Analyse);

        else if(Data_Analyse.at(0)=="STA")
            STA_Command(Data_Analyse);

        else if(Data_Analyse.at(0)=="BUN")
            BUN_Command(Data_Analyse);

        else if(Data_Analyse.at(0)=="BSA")
            BSA_Command(Data_Analyse);

        else if(Data_Analyse.at(0)=="ISZ")
            ISZ_Command(Data_Analyse);

        /////////////////////////////////////////////////////// I/O Refrence ///////////////////////////////////////////////

        else if(Data_Analyse.at(0)=="INP")
            INP_Command();

        else if(Data_Analyse.at(0)=="OUT")
            OUT_Command();

        else if(Data_Analyse.at(0)=="SKI")
            SKI_Command();

        else if(Data_Analyse.at(0)=="SKO")
            SKO_Command();

        else if(Data_Analyse.at(0)=="ION")
            ION_Command();

        else if(Data_Analyse.at(0)=="IOF")
            IOF_Command();

        /////////////////////////////////////////////////////// Register Refrence ///////////////////////////////////////////////

        else if(Data_Analyse.at(0)=="CLA")
            CLA_Command();

        else if(Data_Analyse.at(0)=="CLE")
            CLE_Command();

        else if(Data_Analyse.at(0)=="CMA")
            CMA_Command();

        else if(Data_Analyse.at(0)=="CME")
            CME_Command();

        else if(Data_Analyse.at(0)=="CIR")
            CIR_Command();

        else if(Data_Analyse.at(0)=="CIL")
            CIL_Command();

        else if(Data_Analyse.at(0)=="INC")
            INC_Command();

        else if(Data_Analyse.at(0)=="SPA")
            SPA_Command();

        else if(Data_Analyse.at(0)=="SNA")
            SNA_Command();

        else if(Data_Analyse.at(0)=="SZA")
            SZA_Command();

        else if(Data_Analyse.at(0)=="SZE")
            SZE_Command();

        else if(Data_Analyse.at(0)=="HLT")
            HLT_Command();

        else if(Data_Analyse.at(0)=="END")
            END_Command();

        else
        {
            SC_int=0;
            Stage_Num++;
            if(Print_Status)
            {
                set_show_Registers();
                set_show_Table();
            }
            ui->OutPut_Message->setText("Error in running progRAM!\n");
            ui->Run_Btn->setEnabled(false);
            ui->Next_Step_Btn->setEnabled(false);
            return;
        }

        if(Print_Status)
        {
            set_show_Registers();
            set_show_Table();
        }
    }
}

void Our_Window::file_save(QString fileName){
    ofstream f;
        f.open(fileName.toLocal8Bit(),ios::out);
        f << ui->TextBox->toPlainText().toStdString();
        f.flush();
        f.close();
}

void Our_Window::file_open(QString fileName)
{
    ui->TextBox->clear();
    Save_Place=fileName;
    ifstream infile;
    infile.open(fileName.toLocal8Bit());
    char tmp[300];

    while(infile.eof()!=true)
    {
        infile.getline(tmp,300);
        ui->TextBox->insertPlainText(QString::fromStdString(tmp));
        ui->TextBox->setAlignment(Qt::AlignCenter);
        ui->TextBox->insertPlainText("\n");
    }

}

void Our_Window::save_slot()
{
    if(Save_Place.size()==0)
        action_Save_slot();
    else
        file_save(Save_Place);

}

void Our_Window::action_Save_slot()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save"), "",tr("Mytext TextBox file (*.txt)"));

    if (fileName.size()==0)
         return;
     else {
        file_save(fileName);
        Save_Place=fileName;
    }

}

void Our_Window::open_slot()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open"), "",tr("MyTextBox file (*.txt)"));

    if (fileName.isEmpty())
        return;
    else {
            file_open(fileName);
            Compile_Status=0;
    }

}

void Our_Window::New_slot()
{
    ui->TextBox->setText("");
    ui->TextBox->setAlignment(Qt::AlignCenter);
    Reset_Slot();
    Save_Place="";
    Compile_Status = 0;
}

void Our_Window::run_slot()
{
    if(Compile_Status==0)
        ui->OutPut_Message->setText("\n ebteda barname ro compile konid!\n");

    else
    {
        S=1;
        Print_Status=0;
        Run_Status=1;
        while ((S.to_ulong()))
            Next_Slot();

        set_show_Registers();
        set_show_Table();
    }
}

void Our_Window::About_slot()
{
    QMessageBox msgBox;
    msgBox.setText("🖥️ An assembler and hardware simulator for the Mano Basic Computer, a 16 bit computer.\n Programming By: Mohammad Mahdi Barghi \n GitHub link 🔗: https://github.com/mmahdibarghi/mano-simulator");
    msgBox.exec();
}


////////////////////////////////////////////////////// Lines ////////////////////////////////////////////////////////

                               ///////////////////////// Memory Refrence /////////////////////////

void Our_Window::ADD_Command(QStringList Data_Analyse)
{
    //SC = 3
    if(SC_int==3)
    {
        SC_int++;

        if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
        {
            ui->Def_Op->setText("T3 EXECUTE: AR <- M[AR] ");
            bitset<12> Temp=AR;
            for (int i=0; i<12; AR[i]=RAM[Temp.to_ulong()][i], i++);
        }
    }

    //SC = 4
    if(SC_int==4)
    {
        SC_int++;
        DR=RAM[AR.to_ulong()];
        ui->Def_Op->setText("T4 EXECUTE: DR <- M[AR]");
    }

    //SC = 5
    if(SC_int==5)
    {
        Reset_SC();
        std::bitset<1> COUT(0);
        std::bitset<16> Result(0);

        for(quint16 i = 0; i < AC.size(); i++){
            Result[i] = AC[i] ^ DR[i] ^ COUT[0];
            COUT = COUT[0] & (AC[i] ^ DR[i]) | (AC[i] & DR[i]);
        }

        E = COUT;
        AC = Result;

        ui->Def_Op->setText("T5 EXECUTE: AC <- AC+DR , E<-COUT , SC <- 0");

    }
}

void Our_Window::LDA_Command(QStringList Data_Analyse)
{
    //SC = 3
    if(SC_int==3)
    {
        SC_int++;
        if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
        {
            ui->Def_Op->setText("T3 EXECUTE: AR <- M[AR] ");
            bitset<12> Temp=AR;
            for (int i=0; i<12; AR[i]=RAM[Temp.to_ulong()][i], i++);
        }
    }

    //SC = 4
    else if(SC_int==4)
    {
        SC_int++;
        DR=RAM[AR.to_ulong()];
        ui->Def_Op->setText("T4 EXECUTE: DR <- M[AR]");
    }

    //SC = 5
    else if (SC_int==5) {
        Reset_SC();
        AC=DR;
        ui->Def_Op->setText("T5 EXECUTE: AC <- DR , SC <- 0");
    }
}

void Our_Window::STA_Command(QStringList Data_Analyse)
{
    //SC = 3
    if(SC_int==3)
    {
        SC_int++;
        if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
        {
            ui->Def_Op->setText("T3 EXECUTE: AR <- M[AR] ");
            bitset<12> tmpAR=AR;
            for (int i=0; i<12; AR[i]=RAM[tmpAR.to_ulong()][i], i++);
        }
    }

    //SC = 4
    else if(SC_int==4)
    {
        ui->Def_Op->setText("T4 EXECUTE: M[AR] <- AC , SC <- 0");
        Reset_SC();
        RAM[AR.to_ulong()]=AC;
        QTableWidgetItem *empty = new QTableWidgetItem();
        empty->setText("");
        empty->setTextAlignment(Qt::AlignCenter);
        ui->Answer_Table->setItem(Datas[Data_Analyse.at(1)],2,empty);
    }
}

void Our_Window::BUN_Command(QStringList Data_Analyse)
{
    //SC = 3
    int check;
    if(SC_int==3)
    {
        SC_int++;
        if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
        {
            ui->Def_Op->setText("T3 EXECUTE: AR <- M[AR] ");
            bitset<12> Temp=AR;
            for (int i=0; i<12; AR[i]=RAM[Temp.to_ulong()][i], i++);

            check=1;
        }
        else
            check=0;
    }

    //SC = 4
    else if(SC_int==4)
    {
        ui->Def_Op->setText("T4 EXECUTE: PC <- AR , SC <- 0");
        PC=AR;

        if(check)
           Stage_Num=Memory[AR.to_ulong()]-1;
        else
           Stage_Num=Variable[Data_Analyse.at(1)+","]-1;

        Reset_SC();
        PC_int=PC.to_ulong();
    }
}

void Our_Window::BSA_Command(QStringList Data_Analyse)
{
    //SC = 3
    if(SC_int==3)
    {
        SC_int++;
        if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
        {
            ui->Def_Op->setText("T3 EXECUTE: AR <- M[AR] ");
            bitset<12> Temp=AR;
            for (int i=0; i<12; AR[i]=RAM[Temp.to_ulong()][i], i++);
        }
    }

    //SC = 4
    else if(SC_int==4)
    {
        ui->Def_Op->setText("T4 EXECUTE: M[AR] <- PC , AR <- AR+1");
        SC_int++;
        AR = AR.to_ulong() + 1;
        for (int i=0; i<12; RAM[(AR.to_ulong())-1][i]=PC[i], i++);
    }

    //SC = 5
    else if(SC_int==5)
    {
        PC=AR;
        Stage_Num=Variable[Data_Analyse.at(1)+","];
        Reset_SC();

        PC_int=PC.to_ulong();
        ui->Def_Op->setText("T5 EXECUTE: PC <- AR , SC <- 0");
    }
}

void Our_Window::ISZ_Command(QStringList Data_Analyse)
{
    //SC = 3
    if(SC_int==3)
    {
        SC_int++;

        if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
        {
            ui->Def_Op->setText("T3: EXECUTE: AR <- M[AR] ");

            bitset<12> Temp=AR;

            for (int i=0; i<12; AR[i]=RAM[Temp.to_ulong()][i], i++);

        }
    }

    //SC = 4
    else if(SC_int==4)
    {
        ui->Def_Op->setText("T4 EXECUTE: DR <- M[AR]");
        SC_int++;
        DR=RAM[AR.to_ulong()];
    }

    //SC = 5
    else if(SC_int==5)
    {
        ui->Def_Op->setText("T5 EXECUTE: DR <- DR+1");
        SC_int++;
        DR = DR.to_ulong() + 1;
    }

    //SC = 6
    else if(SC_int==6)
    {
        Reset_SC();

        RAM[AR.to_ulong()]=DR;

        if(DR.to_ulong()==0)
        {
            Stage_Num++;
            PC_int++;
            PC = PC_int;
            ui->Def_Op->setText("PC <- PC+1 , M[AR] <- DR, SC <- 0");
        }
        else
            ui->Def_Op->setText("T6 EXECUTE: M[AR] <- DR, SC <- 0");
    }
}

void Our_Window::AND_Command(QStringList Data_Analyse)
{
    //SC = 3
    if(SC_int==3)
    {
        SC_int++;
        if(Data_Analyse.size()>=3 && Data_Analyse.at(2)=='I')
        {
            ui->Def_Op->setText("T3 EXECUTE: AR <- M[AR] ");
            bitset<12> Temp=AR;
            for (int i=0; i<12; i++)
            {
                AR[i]=RAM[Temp.to_ulong()][i];

            }

        }
    }

    //SC = 4
    else if (SC_int==4) {
        ui->Def_Op->setText("T4 EXECUTE: DR <- M[AR]");
        SC_int++;
        DR=RAM[AR.to_ulong()];
    }

    //SC = 5
    else if (SC_int==5) {
        SC_int=0;
        SC=0;
        Stage_Num++;
        AC = AC & DR;
        ui->Def_Op->setText("T5 EXECUTE: AC <- AC^DR  , SC <- 0");
    }
}

                               ///////////////////////// Register Refrence /////////////////////////

void Our_Window::CIR_Command()
{
    Reset_SC();
    E[0] = AC[0];
    AC = AC >> 1;
    AC[15] = E[0];
    ui->Def_Op->setText("T3 EXECUTE: AC <- shr  , AC(15)<-E , E <- AC(0) ");
}

void Our_Window::CIL_Command()
{
    Reset_SC();
    E[0] = AC[15];
    AC = AC << 1;
    AC[0] = E[0];
    ui->Def_Op->setText("T3 EXECUTE: AC <- shl  , AC(0)<-E , E <- AC(15) ");
}

void Our_Window::SPA_Command()
{
    Reset_SC();

    if(AC[15]==0)
    {
        ui->Def_Op->setText("T3 DECODE: PC <- PC+1  , SC <- 0 ");
        Stage_Num++;
        PC_int++;
        PC = PC_int;
    }
    else
        ui->Def_Op->setText("SC <- 0 ");
}

void Our_Window::SNA_Command()
{
    Reset_SC();

    if(AC[15]==1)
    {
        Stage_Num++;
        PC_int++;
        PC = PC_int;
        ui->Def_Op->setText("T3 DECODE: PC <- PC+1  , SC <- 0 ");
    }
    else
        ui->Def_Op->setText("SC <- 0 ");
}

void Our_Window::SZA_Command()
{
    Reset_SC();

    if(AC.to_ulong()==0)
    {
        Stage_Num++;
        PC_int++;
        PC = PC_int;
        ui->Def_Op->setText("T3 DECODE: PC <- PC+1  , SC <- 0 ");
    }
    else
        ui->Def_Op->setText("SC <- 0 ");
}

void Our_Window::SZE_Command()
{
    Reset_SC();

    if(E.to_ulong()==0)
    {
        Stage_Num++;
        PC_int++;
        PC = PC_int;
        ui->Def_Op->setText("T3 DECODE: PC <- PC+1  , SC <- 0 ");
    }
    else
        ui->Def_Op->setText("SC <- 0 ");
}

void Our_Window::HLT_Command()
{
    S=0;
    Reset_SC();
    ui->Run_Btn->setEnabled(false);
    ui->Next_Step_Btn->setEnabled(false);
    ui->Def_Op->setText("S <- 0");
    ui->OutPut_Message->setText("Done. \n");

    if(Print_Status)
    {
        set_show_Registers();
        set_show_Table();
    }
    return;
}

                               /////////////////////////// I/O Refrence /////////////////////////////

void Our_Window::SKO_Command()
{
    Reset_SC();

    ui->Def_Op->setText("SC <- 0");

    if(FGO==1)
    {
        Stage_Num++;
        PC_int++;
        PC = PC_int;
        ui->Def_Op->setText("PC <- PC+1 , SC <- 0");
    }
}

void Our_Window::SKI_Command()
{
    Reset_SC();

    ui->Def_Op->setText("SC <- 0");

    if(FGI==1)
    {
        Stage_Num++;
        PC_int++;
        PC = PC_int;
        ui->Def_Op->setText("PC <- PC+1 , SC <- 0");
    }
}

void Our_Window::INP_Command()
{
    Reset_SC();

    bool ok=1;
    INPR = ui->input->text().toInt(&ok,16);

    for (int i=0; i<8; AC[i]=INPR[i], i++);

    FGI=0;

    ui->Def_Op->setText("AC(0-7) <- INPR , FGI <- 0 , SC <- 0");
}

void Our_Window::OUT_Command()
{
    Reset_SC();

    for (int i=0; i<8; OUTR[i]=AC[i], i++);

    FGO=0;

    ui->Def_Op->setText("OUTR <- AC(0-7), FGO <- 0 , SC <- 0");
}

void Our_Window::END_Command()
{
    Reset_SC();

    if(Print_Status)
    {
        set_show_Registers();
        set_show_Table();
    }
    S = 0;
    ui->OutPut_Message->setText("Done. \n");
    ui->Run_Btn->setEnabled(false);
    ui->Next_Step_Btn->setEnabled(false);

    return;
}

//////////////////////////////////////////////////////// OTHERS ////////////////////////////////////////////////////////

void Our_Window::Reset_RAM()
{
    for(int i=0; i < 4096; i++)
    {
        QString result = QString::number( i, 16 ).toUpper();
        QTableWidgetItem *temp_hex_addr = new QTableWidgetItem();
        QTableWidgetItem *temp_binary_addr = new QTableWidgetItem();
        temp_hex_addr->setText(result);
        temp_binary_addr->setText("0000");
        ui->Answer_Table->insertRow(i);

        temp_hex_addr->setTextAlignment(Qt::AlignCenter);
        temp_binary_addr->setTextAlignment(Qt::AlignCenter);

        ui->Answer_Table->setItem(i,1,temp_hex_addr);
        ui->Answer_Table->setItem(i,3,temp_binary_addr);
    }
}

void Our_Window::set_show_Registers()
{
    ui->SC_Box->setText(QString::number( SC.to_ulong(), 16 ).toUpper());
    ui->SC_Box->setAlignment(Qt::AlignCenter);
    ui->PC_Box->setText(QString::number( PC.to_ulong(), 16 ).toUpper());
    ui->PC_Box->setAlignment(Qt::AlignCenter);
    ui->AR_Box->setText(QString::number( AR.to_ulong(), 16 ).toUpper());
    ui->AR_Box->setAlignment(Qt::AlignCenter);
    ui->IR_Box->setText(QString::number( IR.to_ulong(), 16 ).toUpper());
    ui->IR_Box->setAlignment(Qt::AlignCenter);
    ui->DR_Box->setText(QString::number( DR.to_ulong(), 16 ).toUpper());
    ui->DR_Box->setAlignment(Qt::AlignCenter);
    ui->AC_Box->setText(QString::number( AC.to_ulong(), 16 ).toUpper());
    ui->AC_Box->setAlignment(Qt::AlignCenter);
    ui->TR_Box->setText(QString::number( TR.to_ulong(), 16 ).toUpper());
    ui->TR_Box->setAlignment(Qt::AlignCenter);
    ui->INPR_Box->setText(QString::number( INPR.to_ulong(), 16 ).toUpper());
    ui->INPR_Box->setAlignment(Qt::AlignCenter);
    ui->OUTR_Box->setText(QString::number( OUTR.to_ulong(), 16 ).toUpper());
    ui->OUTR_Box->setAlignment(Qt::AlignCenter);
    ui->I_Box->setText(QString::number( I.to_ulong(), 16 ).toUpper());
    ui->I_Box->setAlignment(Qt::AlignCenter);
    ui->S_Box->setText(QString::number( S.to_ulong(), 16 ).toUpper());
    ui->S_Box->setAlignment(Qt::AlignCenter);
    ui->E_Box->setText(QString::number( E.to_ulong(), 16 ).toUpper());
    ui->E_Box->setAlignment(Qt::AlignCenter);
    ui->R_Box->setText(QString::number( R.to_ulong(), 16 ).toUpper());
    ui->R_Box->setAlignment(Qt::AlignCenter);
    ui->IEN_Box->setText(QString::number( IEN.to_ulong(), 16 ).toUpper());
    ui->IEN_Box->setAlignment(Qt::AlignCenter);
    ui->FGI_Box->setText(QString::number( FGI.to_ulong(), 16 ).toUpper());
    ui->FGI_Box->setAlignment(Qt::AlignCenter);
    ui->FGO_Box->setText(QString::number( FGO.to_ulong(), 16 ).toUpper());
    ui->FGO_Box->setAlignment(Qt::AlignCenter);
}

void Our_Window::set_show_Table()
{
    if(Reseter)
    {
        ui->Answer_Table->setRowCount(0);
        Reset_RAM();
        Reseter = 0;
    }
    else
    {
        for(int i = 0; i < 4096; i++)
        {
            QString address = QString::number( i, 16 ).toUpper();
            QTableWidgetItem *itmaddr = new QTableWidgetItem();
            QTableWidgetItem *itmHex = new QTableWidgetItem();
            itmaddr->setText(address);
            QString checkerString =QString::number( RAM[i].to_ulong(), 16 ).toUpper();
            if(checkerString.size()==3){
                checkerString = "0" + checkerString;
            }
            else if(checkerString.size()==2){
                checkerString = "00" + checkerString;
            }
            else if(checkerString.size()==1){
                checkerString = "000" + checkerString;
            }
            itmHex->setText(checkerString);

            itmHex->setTextAlignment(Qt::AlignCenter);
            itmaddr->setTextAlignment(Qt::AlignCenter);

            ui->Answer_Table->setItem(i,1,itmaddr);
            ui->Answer_Table->setItem(i,3,itmHex);
        }
    }
}

bool Our_Window::Check_HEX_Number(const QString &str)
{
    string check = str.toUpper().toStdString();

    for (int i=0; i<check.size(); i++)
    {
        if(check[i] == 'A' || check[i] == 'B' || check[i] == 'C' || check[i] == 'D' || check[i] == 'E' || check[i] == 'F'|| check[i] == '-' || check[i] == '+')
            continue;

        if (isdigit(check[i]) == 0) return false;
    }
    return true;
}

Our_Window::~Our_Window()
{
    delete ui;
}
