//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <PetStore_c.h>
#include "searchF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


TformSearch *formSearch;
//---------------------------------------------------------------------------
__fastcall TformSearch::TformSearch(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TformSearch::buttonOkClick(TObject *Sender)
{
//add code
        
}
//---------------------------------------------------------------------------

void __fastcall TformSearch::buttonCancelClick(TObject *Sender)
{
   formSearch->Close();
}
//---------------------------------------------------------------------------

void __fastcall TformSearch::FormClose(TObject *Sender, TCloseAction &Action)
{
  formSearch->Close();
}
//---------------------------------------------------------------------------

