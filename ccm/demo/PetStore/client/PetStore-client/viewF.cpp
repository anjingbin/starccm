//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "viewF.h"
#include "saleF.h"
#include <string.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;
TformView *formView;



//---------------------------------------------------------------------------
__fastcall TformView::TformView(TComponent* Owner)
        : TForm(Owner)
{

}

//---------------------------------------------------------------------------
//show the pets in formSale::stringGridIntroduction.
void __fastcall  TformView::setStringGrid(PetStore::ProductCategory productC)
{
assert(!CORBA::is_nil(businessOp));
PetStore::ProductIndexInfoList_var productCategory = businessOp->listProductByCategory(productC);

//StringGrid->Cells[0][0] = "宠物编号";
//StringGrid->Cells[1][0] = "宠物名";
//StringGrid->Cells[2][0] = "宠物分类";
//StringGrid->Cells[3][0] = "单价";
//StringGrid->Cells[4][0] = "数量";

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

void __fastcall  TformView::setStringGridAll(void)
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

//show the first pet picture
if ( productCategory->length() )
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

void __fastcall TformView::buttonSearchClick(TObject *Sender)
{
  string s = editSearchContext->Text.c_str();//AnsiString  to char*
  searchContext_ =s.begin();
  string s1 = comboBoxKind->Items->Strings[comboBoxKind->ItemIndex].c_str(); //[];//AnsiString to char*
  kindText_ = s1.begin();//comboBoxKind->Items[comboBoxKind->ItemIndex];

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

        TformView::setStringGrid(productC_);
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
        TformView::setStringGridAll();
}



formSale->Show();
formView->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TformView::imageRightBigBirdClick(TObject *Sender)
{

//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::BIRD;
setStringGrid(productC_);

formSale->Show();
formView->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TformView::imageRightFishClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::FISH;
setStringGrid(productC_);

formSale->Show();
formView->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TformView::imageRightDogClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::DOG;
setStringGrid(productC_);

formSale->Show();
formView->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TformView::imageRightCatClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::CAT;
setStringGrid(productC_);

formSale->Show();
formView->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TformView::imageRightBirdClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::BIRD;
setStringGrid(productC_);

formSale->Show();
formView->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TformView::imageLeftDogClick(TObject *Sender)
{

//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::DOG;
setStringGrid(productC_);

formSale->Show();
formView->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TformView::imageLeftCatClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::CAT;
setStringGrid(productC_);


formSale->Show();
formView->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TformView::imageLeftFishClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::FISH;
setStringGrid(productC_);

formSale->Show();
formView->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TformView::imageLeftBirdClick(TObject *Sender)
{
//show the pets in formSale::stringGridIntroduction.
productC_ = PetStore::BIRD;
setStringGrid(productC_);

formSale->Show();
formView->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TformView::FormClose(TObject *Sender, TCloseAction &Action)
{
 Application->Terminate(); 
}
//---------------------------------------------------------------------------

void __fastcall TformView::imageCartClick(TObject *Sender)
{
//此处要更改，要添加代码。
//should to delete
formCart->Show();
formView->Hide();
}
//---------------------------------------------------------------------------






void __fastcall TformView::speedButtonViewClick(TObject *Sender)
{

        TformView::setStringGridAll();
        formView->Hide();
        formSale->Show();
}
//---------------------------------------------------------------------------

void __fastcall TformView::speedButtonExitClick(TObject *Sender)
{
Application->Terminate();

}
//---------------------------------------------------------------------------

