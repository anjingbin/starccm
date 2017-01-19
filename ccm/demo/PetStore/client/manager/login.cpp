//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "login.h"
#include "Unit1.h"
#include "Unit4.h"
#include "Unit2.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TPasswordDlg *PasswordDlg;
//---------------------------------------------------------------------
__fastcall TPasswordDlg::TPasswordDlg(TComponent* AOwner)
	: TForm(AOwner)
{

}
//---------------------------------------------------------------------
void __fastcall TPasswordDlg::CancelBtnClick(TObject *Sender)
{
     Form4->Close();
     Form2->Close();
     Form1->Close();
     Close();
}
//---------------------------------------------------------------------------


void __fastcall TPasswordDlg::OKBtnClick(TObject *Sender)
{
   if(strcmp(Password->Text.c_str(),"Manager")==0)
   {
      Hide();
      Form1 ->Show();
   }
   else
   {
       Application->MessageBox("ÃÜÂë´íÎóÇëÖØÐÂÊäÈë!", "´íÎó", MB_OK);

   }
}
//---------------------------------------------------------------------------

