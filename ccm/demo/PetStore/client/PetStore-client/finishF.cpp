//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "finishF.h"
#include "viewF.h"
#include "saleF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformFinish *formFinish;

//---------------------------------------------------------------------------
__fastcall TformFinish::TformFinish(TComponent* Owner)
        : TForm(Owner)
{
stringGridIntroduction->ColCount = 5;
stringGridIntroduction->Cells[0][0] = "宠物编号";
stringGridIntroduction->Cells[1][0] = "宠物名";
stringGridIntroduction->Cells[2][0] = "单  价";
stringGridIntroduction->Cells[3][0] = "数  量";
stringGridIntroduction->Cells[4][0] = "合  计";

}
//---------------------------------------------------------------------------

void __fastcall TformFinish::imageHomeClick(TObject *Sender)
{
formView->Show();
formFinish->Hide();        
}
//---------------------------------------------------------------------------

void __fastcall TformFinish::speedButtonExitClick(TObject *Sender)
{
Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TformFinish::FormClose(TObject *Sender,
      TCloseAction &Action)
{
Application->Terminate();
}
//---------------------------------------------------------------------------




void __fastcall TformFinish::speedButtonViewClick(TObject *Sender)
{
        formSale->setStringGridAll();
        formFinish->Hide();
        formSale->Show();
}
//---------------------------------------------------------------------------

void __fastcall TformFinish::imageDogClick(TObject *Sender)
{
  productC_ = PetStore::DOG;
  formSale->setStringGrid(productC_);
  formFinish->Hide();
  formSale->Show();
}
//---------------------------------------------------------------------------

void __fastcall TformFinish::imageCatClick(TObject *Sender)
{
  productC_ = PetStore::CAT;
  formSale->setStringGrid(productC_);
  formFinish->Hide();
  formSale->Show();
}
//---------------------------------------------------------------------------

void __fastcall TformFinish::imageFishClick(TObject *Sender)
{
  productC_ = PetStore::FISH;
  formSale->setStringGrid(productC_);
  formFinish->Hide();
  formSale->Show();
}
//---------------------------------------------------------------------------

void __fastcall TformFinish::imageBirdClick(TObject *Sender)
{
  productC_ = PetStore::BIRD;
  formSale->setStringGrid(productC_);
  formFinish->Hide();
  formSale->Show();
}
//---------------------------------------------------------------------------


