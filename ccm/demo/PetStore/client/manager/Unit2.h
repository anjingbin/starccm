//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include "Unit1.h"
#include <vector>
#include <string>
using namespace std;
     struct Item
   {
       AnsiString Item_id;
       AnsiString product_id;
       AnsiString product_name;
       unsigned long quantity;
       unsigned long unit_price;
       unsigned long sub_total;
   };
   struct Order
   {
     AnsiString order_id;
     AnsiString user_name;
     AnsiString user_address;
     AnsiString order_time;
     unsigned long sub_total;
     vector<Item>  seqitem;
   };
   typedef vector<Order> SeqOrder;

//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *StringGrid1;
        TPanel *Panel1;
        TBitBtn *btn_ok;
        TStringGrid *StringGrid2;
        TPanel *Panel2;
        TEdit *Edit1;
        TEdit *Edit2;
        void __fastcall btn_okClick(TObject *Sender);
        void __fastcall StringGrid2MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   
private:
  Order* pOrder;
  Item* pItem;
  SeqOrder  seqorder;
  PetStore::Manager* Manager;

public:		// User declarations
        __fastcall TForm2(TComponent* Owner);
        SeqOrder*  GetOrder(void);
        void RefreshData();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
 