#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<bitset>
#include "./ui_interface.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Our_Window : public QWidget
{
    Q_OBJECT

public:

    Our_Window(QWidget *parent = nullptr);
    ~Our_Window();

    /////// Commands ///////

            /////// Memory Refrence ///////

    void ADD_Command(QStringList);
    void LDA_Command(QStringList);
    void STA_Command(QStringList);
    void BUN_Command(QStringList);
    void BSA_Command(QStringList);
    void ISZ_Command(QStringList);
    void AND_Command(QStringList);

            ////// Register Refrence //////

    void CMA_Command() { AC.flip(), Reset_SC(), ui->Def_Op->setText("T3 EXECUTE: AC <- ~AC  , SC <- 0 "); }
    void CME_Command() { E.flip(), Reset_SC(), ui->Def_Op->setText("T3 EXECUTE: E <- ~E  , SC <- 0 "); }
    void CLA_Command() { AC=0, Reset_SC(), ui->Def_Op->setText("T3 EXECUTE: AC <- 0  , SC <- 0 "); }
    void CLE_Command() { E=0, Reset_SC(), ui->Def_Op->setText("T3 EXECUTE: E <- 0  , SC <- 0 "); }
    void INC_Command() { AC = AC.to_ulong() + 1, Reset_SC(), ui->Def_Op->setText("T3 EXECUTE: AC <- AC+1  , SC <- 0 ");}
    void CIR_Command();
    void CIL_Command();
    void SPA_Command();
    void SNA_Command();
    void SZA_Command();
    void SZE_Command();
    void HLT_Command();

            //////// I/O Refrence ////////

    void SKO_Command();
    void SKI_Command();
    void ION_Command() { IEN=1, Reset_SC(), ui->Def_Op->setText("IEN <- 1 , SC <- 0");}
    void IOF_Command() { IEN=0, Reset_SC(), ui->Def_Op->setText("IEN <- 0 , SC <- 0");}
    void INP_Command();
    void OUT_Command();

    ///////// Others /////////

    void END_Command();
    void Reset_RAM();
    void set_show_Registers();
    void set_show_Table();
    bool Check_HEX_Number(const QString& str);
    void Reset_SC() { SC=0, SC_int=0, Stage_Num++;}

private slots:
    void Reset_Slot();
    void Compile_Slot();
    void Next_Slot();
    void New_slot();
    void open_slot();
    void action_Save_slot();
    void save_slot();
    void file_open(QString fileName);
    void file_save(QString fileName);
    void Exit_slot(){ this->close(); }
    void run_slot();
    void About_slot();

private:
    Ui::Widget *ui;
    int Reseter;
    int Print_Status;
    int Run_Status;
    int Compile_Status;
    int Stage_Num;
    int SC_int;
    int PC_int;

    map<QString,int> Variable;
    map<QString,int> Datas;
    map<int,int> Memory;
    QStringList Lines;
    QString Save_Place="";

    bitset<16> RAM[4096];
    bitset<16> DR;
    bitset<16> SC;
    bitset<16> AC;
    bitset<16> IR;
    bitset<16> TR;
    bitset<12> AR;
    bitset<12> PC;
    bitset<8> INPR;
    bitset<8> OUTR;
    bitset<1> E;
    bitset<1> I;
    bitset<1> S;
    bitset<1> R;
    bitset<1> IEN;
    bitset<1> FGI;
    bitset<1> FGO;

};
#endif // WIDGET_H
