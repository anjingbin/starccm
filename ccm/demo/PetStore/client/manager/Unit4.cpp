//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
#include "Unit1.h"
#include "Unit2.h"
#include "predebug.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;

//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
        : TForm(Owner)
{

/*if (debug)
{

  Form1->RefreshData();

  pseqorder=Form2->GetOrder();
  pseqpro=Form1->GetSeqPro();

  Manager = Form1->getManager();
  PetStore::OrderRecord_var OrderRecord=Manager->account();
  LabeledEdit1->Text =OrderRecord->sub_total;
  Series2->Active=false;
  for(int i=0;i<OrderRecord->order_index_list.length();i++)
  {
    Series1->Add(OrderRecord->order_index_list[i].sub_total ,OrderRecord->order_index_list[i].order_id.in() , clTeeColor );
  }
}
else  */
{


} 

}
//---------------------------------------------------------------------------
int TForm4::InSeqPro(AnsiString product_id)
{
  for(int j=0;j<pseqpro->size();j++)
  {
      if((*pseqpro)[j].product_id==product_id)
      {
       return j;
       break;
      }
  }
  return -1;
}

int TForm4::InSeqAcount(AnsiString product_category)
{
    for(int k=0;k<seqacount.size();k++)
     {
        if(seqacount[k].category==product_category)
        {
          return k;
          break;
        }
      }
   return -1;
}



void __fastcall TForm4::BitBtn3Click(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm4::BitBtn1Click(TObject *Sender)
{
  Series2->Active=false;
  Series1->Active=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm4::BitBtn2Click(TObject *Sender)
{
  Series1->Active=false;

  for(int i=0;i<pseqorder->size();i++)
  {
    for(int n=0;n<(*pseqorder)[i].seqitem.size();n++)
    {
      int j=InSeqPro((*pseqorder)[i].seqitem[n].product_id);
      int k=InSeqAcount((*pseqpro)[j].product_category);
      if(k!=-1)//此宠物种类在Acount链中
      {
        seqacount[k].qty+=(*pseqorder)[i].seqitem[n].quantity;
        seqacount[k].money+=(*pseqorder)[i].seqitem[n].quantity*(*pseqorder)[i].seqitem[n].unit_price;
      }
      else//此宠物种类不在Acount链中
      {
        pAcount=new Acount;
        pAcount->category=(*pseqpro)[j].product_category;
        pAcount->qty+= (*pseqorder)[i].seqitem[n].quantity;
        pAcount->money+=(*pseqorder)[i].seqitem[n].quantity*(*pseqorder)[i].seqitem[n].unit_price;
        seqacount.insert( seqacount.end(),(*pAcount));
        delete pAcount;
      }
    }
  }
  for(int i=0;i<seqacount.size();i++)
  {
    Series2->Add(seqacount[i].money.ToDouble(),seqacount[i].category.c_str(),clTeeColor);
  }

  Series2->Active=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm4::FormClose(TObject *Sender, TCloseAction &Action)
{
   seqacount.clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm4::FormShow(TObject *Sender)
{
    seqacount.clear();
    Series2->Active=false;  // {Where Series2 is your LineSeries}1
    Series1->Add( 25 ,"dog" , clTeeColor );
    Series1->Add( 50 , "fa" , clTeeColor );
    Series1->Add( 530 , "gfggsfdg" , clTeeColor );
    Series1->Add( 50 , "" , clTeeColor );
    Series1->Add( 40 , "" , clTeeColor );
    Series1->Add( 35 , "" , clTeeColor );
}
//---------------------------------------------------------------------------

