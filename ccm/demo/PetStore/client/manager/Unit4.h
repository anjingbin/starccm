//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <QuickRpt.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Buttons.hpp>
#include <vector>

#include "Unit2.h"
using namespace std;
//---------------------------------------------------------------------------

class TForm4 : public TForm
{
__published:	// IDE-managed Components
        TChart *Chart1;
        TBarSeries *Series1;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
        TPieSeries *Series2;
        TPanel *Panel1;
        TStaticText *StaticText1;
        TLabeledEdit *LabeledEdit1;
        void __fastcall BitBtn3Click(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
struct Acount
  {
     AnsiString category;
     int qty;
     AnsiString money;
  }*pAcount;
  vector<Acount> seqacount;
  SeqOrder* pseqorder;
  SeqPro* pseqpro;

public:		// User declarations
     __fastcall TForm4(TComponent* Owner);
     int InSeqPro(AnsiString product_id);
     int InSeqAcount(AnsiString product_category);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
