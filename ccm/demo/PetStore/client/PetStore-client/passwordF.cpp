//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "passwordF.h"
#include "saleF.h"
#include "searchF.h"
#include "viewF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformPassword *formPassword;
//---------------------------------------------------------------------------
__fastcall TformPassword::TformPassword(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TformPassword::maskEditPasswordChange(TObject *Sender)
{
 //  if ( (editName->Text == "client" )&& (maskEditPassword->Text == "client"))
 {

    formView->Show();
    formPassword->Hide();
 }
 
}
//---------------------------------------------------------------------------


