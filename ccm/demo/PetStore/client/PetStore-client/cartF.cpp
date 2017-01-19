//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "cartF.h"
#include "orderF.h"
#include "saleF.h"
#include "viewF.h"
#include "string.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;
TformCart *formCart;

//---------------------------------------------------------------------------
__fastcall TformCart::TformCart(TComponent* Owner)
        : TForm(Owner)
{
stringGridIntroduction->ColCount = 6;
stringGridIntroduction->Cells[0][0] = "宠物编号";
stringGridIntroduction->Cells[1][0] = "宠物名";
stringGridIntroduction->Cells[2][0] = "单  价";
stringGridIntroduction->Cells[3][0] = "数  量";
stringGridIntroduction->Cells[4][0] = "合  计";
stringGridIntroduction->Cells[5][0] = "删  除";
formCart->stringGridIntroduction->RowCount = 1;
stringGridIntroduction->FixedColor = clGradientActiveCaption;
}
//---------------------------------------------------------------------------
void __fastcall TformCart::showProduction()
{

//show the production.
PetStore::SalesRecord_var salesRecord = cart->list();
PetStore::SalesRecordInfoList salesInfoList = salesRecord->sales_record_list;

//stringGridIntroduction->ColCount = 6;
//stringGridIntroduction->Cells[0][0] = "宠物编号";
//stringGridIntroduction->Cells[1][0] = "宠物名";
//stringGridIntroduction->Cells[2][0] = "单  价";
//stringGridIntroduction->Cells[3][0] = "数  量";
//stringGridIntroduction->Cells[4][0] = "合  计";
//stringGridIntroduction->Cells[5][0] = "删  除";

formCart->stringGridIntroduction->RowCount = salesInfoList.length()+1;
for (unsigned int i = 0; i < salesInfoList.length();i++)
{
formCart->stringGridIntroduction->Cells[0][i+1] = salesInfoList[i].product_id.in();
formCart->stringGridIntroduction->Cells[1][i+1] = salesInfoList[i].product_name.in();
formCart->stringGridIntroduction->Cells[2][i+1] = IntToStr(salesInfoList[i].unit_price);
formCart->stringGridIntroduction->Cells[3][i+1] = IntToStr(salesInfoList[i].quantity);
formCart->stringGridIntroduction->Cells[4][i+1] = IntToStr(salesInfoList[i].sub_total);
formCart->stringGridIntroduction->Cells[5][i+1] = "否";
}

}
//---------------------------------------------------------------------------
void __fastcall TformCart::setTotalPrice()
{

//set the total price.
PetStore::SalesRecord_var salesRecord = cart->list();
formCart->labelTotalPrice->Caption = IntToStr(salesRecord->sub_total);

}
//---------------------------------------------------------------------------
void __fastcall TformCart::FormClose(TObject *Sender, TCloseAction &Action)
{
Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TformCart::speedButtonExitClick(TObject *Sender)
{
Application->Terminate();
}
//---------------------------------------------------------------------------


void __fastcall TformCart::imageHomeClick(TObject *Sender)
{
formView->Show();
formCart->Hide();


}
//---------------------------------------------------------------------------

void __fastcall TformCart::imageNextClick(TObject *Sender)
{
//show the formOrder::stringGridIntroduction.

//stringGridIntroduction->Cells[0][0] = "宠物编号";
//stringGridIntroduction->Cells[1][0] = "宠物名";
//stringGridIntroduction->Cells[2][0] = "单  价";
//stringGridIntroduction->Cells[3][0] = "数  量";
//stringGridIntroduction->Cells[4][0] = "合  计";

//show the production.



if (debug)
{
    PetStore::SalesRecord_var salesRecord = cart->list();
    PetStore::SalesRecordInfoList salesInfoList = salesRecord->sales_record_list;

    formOrder->stringGridIntroduction->RowCount = salesInfoList.length()+1;

    for (unsigned int i = 0; i < salesInfoList.length();i++)
    {
        formOrder->stringGridIntroduction->Cells[0][i+1] = salesInfoList[i].product_id.in();
        formOrder->stringGridIntroduction->Cells[1][i+1] = salesInfoList[i].product_name.in();
        formOrder->stringGridIntroduction->Cells[2][i+1] = IntToStr(salesInfoList[i].unit_price);
        formOrder->stringGridIntroduction->Cells[3][i+1] = IntToStr(salesInfoList[i].quantity);
        formOrder->stringGridIntroduction->Cells[4][i+1] = IntToStr(salesInfoList[i].sub_total);
    }


    formOrder->labelTotalPrice->Caption = IntToStr(salesRecord->sub_total);
}


formOrder->Show();
formCart->Hide();

}
//---------------------------------------------------------------------------

void __fastcall TformCart::imageFirstClick(TObject *Sender)
{
formSale->Show();
formCart->Hide();
}
//---------------------------------------------------------------------------



void __fastcall TformCart::imageDogClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::DOG;
formSale->setStringGrid(productC_);

formSale->Show();
formCart->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TformCart::imageCatClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::CAT;
formSale->setStringGrid(productC_);

formSale->Show();
formCart->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TformCart::imageFishClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::FISH;
formSale->setStringGrid(productC_);

formSale->Show();
formCart->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TformCart::imageBirdClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::BIRD;
formSale->setStringGrid(productC_);

formSale->Show();
formCart->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TformCart::buttonSearchClick(TObject *Sender)
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
formCart->Hide();
}
//---------------------------------------------------------------------------




void __fastcall TformCart::imageCartClick(TObject *Sender)
{
//set the total price in labelTotalPrice.
//show the productinformation in the stringGridIntroduction.
if (debug)
{
formCart->setTotalPrice();
formCart->showProduction();
}        
}
//---------------------------------------------------------------------------

void __fastcall TformCart::buttonClearClick(TObject *Sender)
{

for (int i = 1; i < (TformCart::stringGridIntroduction->RowCount);i++)
{
formCart->stringGridIntroduction->Cells[0][i] = "";
formCart->stringGridIntroduction->Cells[1][i] = "";
formCart->stringGridIntroduction->Cells[2][i] = "";
formCart->stringGridIntroduction->Cells[3][i] = "";
formCart->stringGridIntroduction->Cells[4][i] = "";
formCart->stringGridIntroduction->Cells[5][i] = "";
}

//TformCart::stringGridIntroduction->RowCount = 0;

TformCart::labelTotalPrice->Caption = "";
formCart->stringGridIntroduction->RowCount = 1;


cart->clear();

}
//---------------------------------------------------------------------------
void __fastcall TformCart::stringGridIntroductionMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{

stringGridIntroduction->FixedColor = clGradientActiveCaption;

if( stringGridIntroduction->Row == 0)
{
return;
}

TformCart::stringGridIntroduction->MouseToCell(X,Y,YCol,XRow);

switch (YCol)
{
case 3 :
        {
        TformCart::stringGridIntroduction->Cursor = crHandPoint;
        break;
        }
case 5 :
        {
        TformCart::stringGridIntroduction->Cursor = crHandPoint;
        break;
        }
default:
        {
        TformCart::stringGridIntroduction->Cursor = crDefault;
        break;
        }
}

}
//---------------------------------------------------------------------------

void __fastcall TformCart::stringGridIntroductionClick(TObject *Sender)
{


stringGridIntroduction->FixedColor = clGradientActiveCaption;

if( stringGridIntroduction->Row == 0)
{
return;
}

TformCart::stringGridIntroduction->EditorMode = false;
goEditing << false;
goAlwaysShowEditor << false;
TformCart::stringGridIntroduction->Options << goAlwaysShowEditor << goEditing;

switch (YCol)
{
case 3 :
        {
        TformCart::stringGridIntroduction->EditorMode = true;
        TformCart::stringGridIntroduction->Cursor = crHandPoint;
        goEditing << true;
        TformCart::stringGridIntroduction->Options << goAlwaysShowEditor << goEditing;

        //refresh the sub_total price.
        if (debug)
        {
        PetStore::SalesRecord_var salesRecord = cart->list();
        PetStore::SalesRecordInfoList salesInfoList = salesRecord->sales_record_list;

        int currentRow = formCart->stringGridIntroduction->Row;

        char * currentProductID = formCart->stringGridIntroduction->Cells[0][currentRow].c_str();
        unsigned int currentProductQty = StrToInt(formCart->stringGridIntroduction->Cells[3][currentRow]);

        cart->updateProductQty(currentProductID,currentProductQty);
        formCart->stringGridIntroduction->Cells[4][currentRow] = IntToStr(salesRecord->sub_total);
        formCart->labelTotalPrice->Caption = IntToStr(salesInfoList[currentRow].sub_total);
        }
        break;
        }
case 5 :
        {
        TformCart::stringGridIntroduction->EditorMode = false;
        //TformCart::stringGridIntroduction->Cursor = crDefault;
        goEditing << false;
        goAlwaysShowEditor << false;
        TformCart::stringGridIntroduction->Options << goAlwaysShowEditor << goEditing;

        int currentRow = formCart->stringGridIntroduction->Row;
        int currentCol = formCart->stringGridIntroduction->Col;
        formCart->stringGridIntroduction->Cells[currentCol][currentRow] = "是";

        //refresh the total price.

        char* productID = formCart->stringGridIntroduction->Cells[0][currentRow].c_str();
        cart->deleteProduct(productID);
        
        formCart->showProduction();
        formCart->setTotalPrice();

        break;
        }
default:
        {
        TformCart::stringGridIntroduction->EditorMode = false;
        TformCart::stringGridIntroduction->Cursor = crDefault;
        goEditing << false;
        goAlwaysShowEditor << false;
        TformCart::stringGridIntroduction->Options << goAlwaysShowEditor << goEditing;
        break;
        }
}
}
//---------------------------------------------------------------------------

void __fastcall TformCart::stringGridIntroductionKeyPress(TObject *Sender,
      char &Key)
{
stringGridIntroduction->FixedColor = clGradientActiveCaption;
if( stringGridIntroduction->Row == 0)
{
return;
}
switch (YCol)
{
case 3 :
        {
        TformCart::stringGridIntroduction->Cursor = crHandPoint;
        goEditing << true;
        TformCart::stringGridIntroduction->Options << goAlwaysShowEditor << goEditing;
        break;
        }
case 5 :
        {
        TformCart::stringGridIntroduction->EditorMode = false;
        TformCart::stringGridIntroduction->Cursor = crDefault;
        goEditing << false;
        goAlwaysShowEditor << false;
        TformCart::stringGridIntroduction->Options << goAlwaysShowEditor << goEditing;
        
        break;
        }
default:
        {
        TformCart::stringGridIntroduction->EditorMode = false;
        TformCart::stringGridIntroduction->Cursor = crDefault;
        goEditing << false;
        goAlwaysShowEditor << false;
        TformCart::stringGridIntroduction->Options << goAlwaysShowEditor << goEditing;
        break;
        }
}


}
//---------------------------------------------------------------------------





void __fastcall TformCart::speedButtonViewClick(TObject *Sender)
{
        formSale->setStringGridAll();
        formCart->Hide();
        formSale->Show();
}
//---------------------------------------------------------------------------


