//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "orderF.h"
#include "finishF.h"
#include "cartF.h"
#include "saleF.h"
#include "viewF.h"
#include "string.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;
TformOrder *formOrder;

//---------------------------------------------------------------------------
__fastcall TformOrder::TformOrder(TComponent* Owner)
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
void __fastcall TformOrder::imageNextClick(TObject *Sender)
{
CORBA::String_var orderId;
AnsiString orderIdS;
//set the user name and address.
//OrderID string placeOrder(in string userName,
//                          in string userAddress,
//                          in Cart myCart);
if ( stringGridIntroduction->RowCount == 1)
{
return ;
}

if (debug)
{
//formFinish::stringGrid填写内容。
PetStore::SalesRecord_var salesRecord = cart->list();
PetStore::SalesRecordInfoList salesInfoList = salesRecord->sales_record_list;

formFinish->stringGridIntroduction->RowCount = salesInfoList.length()+1;

for (unsigned int i = 0; i < salesInfoList.length();i++)
{
formFinish->stringGridIntroduction->Cells[0][i+1] = salesInfoList[i].product_id.in();
formFinish->stringGridIntroduction->Cells[1][i+1] = salesInfoList[i].product_name.in();
formFinish->stringGridIntroduction->Cells[2][i+1] = IntToStr(salesInfoList[i].unit_price);
formFinish->stringGridIntroduction->Cells[3][i+1] = IntToStr(salesInfoList[i].quantity);
formFinish->stringGridIntroduction->Cells[4][i+1] = IntToStr(salesInfoList[i].sub_total);
}

formFinish->labelTotalPrice->Caption = "总 计：" + IntToStr(salesRecord->sub_total) + "元";
formFinish->labelName->Caption = editName->Text;
formFinish->labelAddress->Caption = editAddress->Text;

}

try
{
assert(!CORBA::is_nil(cart));
orderId = businessOp->placeOrder(editName->Text.c_str(),editAddress->Text.c_str(),cart);
orderIdS = orderId.in();
formFinish->labelOrderNumber->Caption = "第： " + orderIdS + "号";
}
catch(const CORBA::Exception& ex)
{
    ex;
}

formFinish->Show();
formOrder->Hide();

}
//---------------------------------------------------------------------------
void __fastcall TformOrder::imageFirstClick(TObject *Sender)
{
formCart->Show();
formOrder->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TformOrder::imageCartClick(TObject *Sender)
{
formCart->Show();
formOrder->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TformOrder::imageHomeClick(TObject *Sender)
{
formView->Show();
formOrder->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TformOrder::speedButtonExitClick(TObject *Sender)
{
Application->Terminate();        
}
//---------------------------------------------------------------------------

void __fastcall TformOrder::imageDogClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::DOG;
formSale->setStringGrid(productC_);

formSale->Show();
formOrder->Hide();        
}
//---------------------------------------------------------------------------

void __fastcall TformOrder::imageCatClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::CAT;
formSale->setStringGrid(productC_);

formSale->Show();
formOrder->Hide();        
}
//---------------------------------------------------------------------------

void __fastcall TformOrder::imageFishClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::FISH;
formSale->setStringGrid(productC_);

formSale->Show();
formOrder->Hide();        
}
//---------------------------------------------------------------------------

void __fastcall TformOrder::imageBirdClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::BIRD;
formSale->setStringGrid(productC_);

formSale->Show();
formOrder->Hide();        
}
//---------------------------------------------------------------------------



void __fastcall TformOrder::buttonSearchClick(TObject *Sender)
{
  string s = editSearchContext->Text.c_str();//AnsiString  to char*
  searchContext_ =s.begin();
  string s1 = comboBoxKind->Items->Strings[comboBoxKind->ItemIndex].c_str(); //[];//AnsiString to char*
  kindText_ = s1.begin();

if ( strcmp(kindText_,"BY 类别") == 0 )
{
            if ( (strcmp(searchContext_ , "DOG")==0 )||strcmp(searchContext_ , "dog")==0)
        {
                productC_ = PetStore::DOG;
        }

         if ( (strcmp(searchContext_ , "FISH")==0 )||strcmp(searchContext_ , "fish")==0)
        {
                productC_ = PetStore::FISH;
        }

        if ( (strcmp(searchContext_ , "cat")==0 )||strcmp(searchContext_ , "CAT")==0)
        {
                productC_ = PetStore::CAT;
        }

         if ( (strcmp(searchContext_ , "BIRD")==0 )||strcmp(searchContext_ , "bird")==0)
        {
                productC_ = PetStore::BIRD;
        }
        formSale->setStringGrid(productC_);
} ;

if ( strcmp(kindText_,"BY  ID")==0 )
{
        PetStore::ProductInfo_var productInfo = businessOp->listProduct(searchContext_);
        
        formSale->stringGridIntroduction->Cells[0][1] = productInfo->product_id.in();
        formSale->stringGridIntroduction->Cells[1][1] = productInfo->product_name.in();
        formSale->stringGridIntroduction->Cells[2][1] = productInfo->product_category.in();
        formSale->stringGridIntroduction->Cells[3][1] = IntToStr(productInfo->product_price);
        formSale->stringGridIntroduction->Cells[4][1] = IntToStr(productInfo->product_qty);
        formSale->stringGridIntroduction->RowCount = 2;
        formSale->imageProduct->Picture->LoadFromFile(productInfo->product_picture.in());

} ;

if (strcmp(kindText_,"BY ALL")==0)
{
        formSale->setStringGridAll();
}



formSale->Show();
formOrder->Hide();        
}
//---------------------------------------------------------------------------


void __fastcall TformOrder::FormClose(TObject *Sender,
      TCloseAction &Action)
{
Application->Terminate();        
}
//---------------------------------------------------------------------------


void __fastcall TformOrder::speedButtonViewClick(TObject *Sender)
{
        formSale->setStringGridAll();
        formOrder->Hide();
        formSale->Show();    
}
//---------------------------------------------------------------------------


