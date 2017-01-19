//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "saleF.h"
#include "viewF.h"
#include "passwordF.h"
#include "cartF.h"
#include "searchF.h"
#include "string.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;
TformSale *formSale;

//---------------------------------------------------------------------------
__fastcall TformSale::TformSale(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------

//show the pets in formSale::stringGridIntroduction.
void __fastcall  TformSale::setStringGrid(PetStore::ProductCategory productC)
{
assert(!CORBA::is_nil(businessOp));
PetStore::ProductIndexInfoList_var productCategory = businessOp->listProductByCategory(productC);

//StringGrid->Cells[0][0] = "宠物编号";
//StringGrid->Cells[1][0] = "宠物名";
//StringGrid->Cells[2][0] = "宠物分类";
//StringGrid->Cells[3][0] = "单价";
//StringGrid->Cells[4][0] = "数量";


//formSale->stringGridIntroduction to been clear.



//set the pets's attributes in formSale::stringGridIntroduction
int  productLength = productCategory->length();
if (productLength == 0)
{
    formSale->stringGridIntroduction->RowCount = 1;
    return ;
}
formSale->stringGridIntroduction->ColCount = 5;
formSale->stringGridIntroduction->RowCount = productCategory->length()+1;

//show the first pet picture
if (productCategory->length())
{
    PetStore::ProductInfo_var productInfo = businessOp->listProduct(productCategory[0].product_id);
    formSale->imageProduct->Picture->LoadFromFile(productInfo->product_picture.in());
    formSale->memoProductIntroduction->Lines->Clear();
    formSale->memoProductIntroduction->Lines->Add(productInfo->product_description.in());
}

for (unsigned int i = 0 ; i< productCategory->length(); i++)
{

        PetStore::ProductInfo_var productInfo = businessOp->listProduct(productCategory[i].product_id);

        formSale->stringGridIntroduction->Cells[0][i+1] = productInfo->product_id.in();
        formSale->stringGridIntroduction->Cells[1][i+1] = productInfo->product_name.in();
        formSale->stringGridIntroduction->Cells[2][i+1] = productInfo->product_category.in();
        formSale->stringGridIntroduction->Cells[3][i+1] = IntToStr(productInfo->product_price);
        formSale->stringGridIntroduction->Cells[4][i+1] = IntToStr(productInfo->product_qty);

}//end of for

}


//---------------------------------------------------------------------------

void __fastcall  TformSale::setStringGridAll(void)
{
PetStore::ProductIndexInfoList_var productCategory = businessOp->listAllProducts();

//StringGrid->Cells[0][0] = "宠物编号";
//StringGrid->Cells[1][0] = "宠物名";
//StringGrid->Cells[2][0] = "宠物分类";
//StringGrid->Cells[3][0] = "单价";
//StringGrid->Cells[4][0] = "数量";

//set the pets's attributes in formSale::stringGridIntroduction
formSale->stringGridIntroduction->ColCount = 5;
formSale->stringGridIntroduction->RowCount = productCategory->length()+1;

//set the pet's picture and introduction.
PetStore::ProductInfo_var productInfo = businessOp->listProduct(productCategory[0].product_id);
formSale->imageProduct->Picture->LoadFromFile(productInfo->product_picture.in());

formSale->memoProductIntroduction->Lines->Clear();
formSale->memoProductIntroduction->Lines->Add(productInfo->product_description.in());

for (unsigned int i = 0 ; i< productCategory->length(); i++)
{

        PetStore::ProductInfo_var productInfo = businessOp->listProduct(productCategory[i].product_id);
        formSale->stringGridIntroduction->Cells[0][i+1] = productInfo->product_id.in();
        formSale->stringGridIntroduction->Cells[1][i+1] = productInfo->product_name.in();
        formSale->stringGridIntroduction->Cells[2][i+1] = productInfo->product_category.in();
        formSale->stringGridIntroduction->Cells[3][i+1] = IntToStr(productInfo->product_price);
        formSale->stringGridIntroduction->Cells[4][i+1] = IntToStr(productInfo->product_qty);

}//end of for

}

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------





void __fastcall TformSale::FormCreate(TObject *Sender)
{
//initialize the TformSale
formSale->stringGridIntroduction->ColCount = 5;

stringGridIntroduction->Cells[0][0] = "宠物编号";
stringGridIntroduction->Cells[1][0] = "宠物名";
stringGridIntroduction->Cells[2][0] = "宠物分类";
stringGridIntroduction->Cells[3][0] = "单价";
stringGridIntroduction->Cells[4][0] = "数量";

//stringGridIntroduction->Cells[5][0] = "订单编号";
//stringGridIntroduction->Cells[6][0] = "合计";
//stringGridIntroduction->Cells[7][0] = "用户姓名";
//stringGridIntroduction->Cells[8][0] = "用户地址";
//stringGridIntroduction->Cells[9][0] = "定购时间";

}
//---------------------------------------------------------------------------


void __fastcall TformSale::SpeedButton9Click(TObject *Sender)
{
Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TformSale::imageHomeClick(TObject *Sender)
{
  
   formView->Show();
   formSale->Hide();
   //
}
//---------------------------------------------------------------------------




void __fastcall TformSale::FormClose(TObject *Sender, TCloseAction &Action)
{
 Application->Terminate(); 
}
//---------------------------------------------------------------------------




void __fastcall TformSale::imageDogClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::DOG;
setStringGrid(productC_);
}
//---------------------------------------------------------------------------

void __fastcall TformSale::imageCatClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::CAT;
setStringGrid(productC_);
}
//---------------------------------------------------------------------------

void __fastcall TformSale::imageFishClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::FISH;
setStringGrid(productC_);
}
//---------------------------------------------------------------------------

void __fastcall TformSale::imageBirdClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::BIRD;
setStringGrid(productC_);
}
//---------------------------------------------------------------------------

void __fastcall TformSale::speedButtonViewClick(TObject *Sender)
{
TformSale::setStringGridAll();        
}
//---------------------------------------------------------------------------

void __fastcall TformSale::buttonSearchClick(TObject *Sender)
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

        TformSale::setStringGrid(productC_);
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
        //set the pet's picture and introduction.
        formSale->imageProduct->Picture->LoadFromFile(productInfo->product_picture.in());

        formSale->memoProductIntroduction->Lines->Clear();
        formSale->memoProductIntroduction->Lines->Add(productInfo->product_description.in());
} ;

if (strcmp(kindText_,"BY ALL")==0)
{
        TformSale::setStringGridAll();
}
        
}
//---------------------------------------------------------------------------

void __fastcall TformSale::imageCartClick(TObject *Sender)
{

//set the total price in labelTotalPrice.
//show the productinformation in the stringGridIntroduction.
if (debug)
{
formCart->setTotalPrice();
formCart->showProduction();
}

formCart->Show();
formSale->Hide();
}
//---------------------------------------------------------------------------





void __fastcall TformSale::buttonAddClick(TObject *Sender)
{

int selectedRow = formSale->stringGridIntroduction->Row ;
if ( selectedRow == 0 )
{
return;
}
//add the product.
PetStore::SalesRecordInfo_var selectProductInfo = new PetStore::SalesRecordInfo;

//stringGridIntroduction->Cells[0][0] = "宠物编号";
//stringGridIntroduction->Cells[1][0] = "宠物名";
//stringGridIntroduction->Cells[2][0] = "宠物分类";
//stringGridIntroduction->Cells[3][0] = "单价";
//stringGridIntroduction->Cells[4][0] = "数量";


selectProductInfo->product_id = CORBA::string_dup(formSale->stringGridIntroduction->Cells[0][selectedRow].c_str());
selectProductInfo->product_name = CORBA::string_dup(formSale->stringGridIntroduction->Cells[1][selectedRow].c_str());
selectProductInfo->unit_price = StrToInt(formSale->stringGridIntroduction->Cells[3][selectedRow]);
selectProductInfo->quantity = StrToInt(formSale->editProductionNumber->Text);
selectProductInfo->sub_total = (selectProductInfo->unit_price) * (selectProductInfo->quantity);

cart->addProduct(selectProductInfo);
}
//---------------------------------------------------------------------------

void __fastcall TformSale::stringGridIntroductionClick(TObject *Sender)
{
if( stringGridIntroduction->Row == 0)
{
return;
}

int selectedRow = formSale->stringGridIntroduction->Row ;

char* productID = formSale->stringGridIntroduction->Cells[0][selectedRow].c_str();

if (productID ==NULL)
{
        return;
}

PetStore::ProductInfo_var productInfo = businessOp->listProduct(productID);

//set the pet's picture and introduction.
formSale->imageProduct->Picture->LoadFromFile(productInfo->product_picture.in());

formSale->memoProductIntroduction->Lines->Clear();
formSale->memoProductIntroduction->Lines->Add(productInfo->product_description.in());

}
//---------------------------------------------------------------------------



