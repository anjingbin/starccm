//---------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <vector>
#include "corba.h"
#include "CosNaming_c.hh"

#include "PetStore_c.h"

#include <assert.h>

using namespace std;
 struct product
   {
       AnsiString   product_id;
       AnsiString   product_name;
       AnsiString   product_pic;
       AnsiString   product_category ;
       AnsiString   product_price;
       int      product_qty;
       AnsiString   product_description;
   };
   typedef vector<product>  SeqPro;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TButton *ButtonAdd;
        TLabel *Label1;
        TButton *ButtonListAllOrders;
        TButton *ButtonCount;
        TStringGrid *StringGrid1;
        TBitBtn *BtnExit;
        TBitBtn *BtnSelectImage;

        TOpenPictureDialog *OpenPictureDialog1;
        TPanel *Panel1;
        TPanel *Panel2;
        TLabeledEdit *LabeledEdit2;
        TStaticText *StaticText1;
        TBitBtn *BtnCommit;
        TPanel *Panel3;
        TImage *Image1;
        TRichEdit *RichEdit1;
        TBitBtn *BtnSelectDescrip;
        TOpenDialog *OpenDialog1;
        TEdit *Edit1;
        void __fastcall BtnSelectImageClick(TObject *Sender);
        void __fastcall ButtonListAllOrdersClick(TObject *Sender);
        void __fastcall StringGrid1MouseDown(TObject *Sender,
                TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall BtnCommitClick(TObject *Sender);
        void __fastcall ButtonAddClick(TObject *Sender);
        void __fastcall BtnExitClick(TObject *Sender);
        void __fastcall ButtonCountClick(TObject *Sender);
        void __fastcall BtnSelectDescripClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations
   AnsiString FileName;
   product* pProduct;
   SeqPro  seqpro;
   bool bStatusAdd;

public:		// User declarations
        __fastcall TForm1(TComponent* Owner);  
        void AddProduct(void);
        SeqPro* GetSeqPro(void);
        PetStore::Manager* getManager();
        void RefreshData();
        
        char* __fastcall GetCCMProfileString(const char* strSectionName,
								const char* keyName, 
								const char* defautValue,
								const char* fileName);
        
        

      PetStore::Manager_var manager;

        PetStore::BusinessOp_var businessOp;
        PetStore::Sales_var sales;
        CORBA::ORB_var orb;  

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
