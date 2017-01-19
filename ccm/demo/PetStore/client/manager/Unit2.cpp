//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
#include "Unit1.h"

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{
  for(int i=0;i<seqorder.size();i++)
      seqorder[i].seqitem.clear();
  seqorder.clear();

  Manager = Form1->getManager();

  StringGrid2->Cells[0][0] = "订单号";
  StringGrid2->Cells[1][0] = "用户名";
  StringGrid2->Cells[2][0] = "用户住址";
  StringGrid2->Cells[3][0] = "订单日期";
  StringGrid2->Cells[4][0] = "合计";

  StringGrid1->Cells[0][0] = "订单号";
  StringGrid1->Cells[1][0] = "宠物号";
  StringGrid1->Cells[2][0] = "宠物名";
  StringGrid1->Cells[3][0] = "数量";
  StringGrid1->Cells[4][0] = "单价";
  StringGrid1->Cells[5][0] = "合计";



/*
  pOrder=new Order;
  pOrder->order_id="sdf";
  pOrder->user_name="dfsg";

  pItem=new   Item;
  pItem->Item_id="a";
  pOrder->seqitem.insert(pOrder->seqitem.end(),*pItem);

  pItem=new Item;
  pItem->Item_id="1itemgh";
  pItem->product_name="dog";
  pOrder->seqitem.insert(pOrder->seqitem.end(),*pItem);

  seqorder.insert(seqorder.end(),*pOrder);

  pOrder=new Order;
  pOrder->order_id="2低速发";
  pOrder->user_name="2规范化";
  pItem=new   Item;
  pItem->Item_id="56";
  pOrder->seqitem.insert(pOrder->seqitem.end(),*pItem);
  seqorder.insert(seqorder.end(),*pOrder);   */

}
//---------------------------------------------------------------------------



void __fastcall TForm2::btn_okClick(TObject *Sender)
{
   Close(); 
}
//---------------------------------------------------------------------------


void __fastcall TForm2::StringGrid2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   X= StringGrid2->Row-1;

   if(X< seqorder.size())
   {
     if( seqorder[X].seqitem.size()+1>2)
       StringGrid1-> RowCount=seqorder[X].seqitem.size()+1;
     else
        StringGrid1-> RowCount=2;
     for(int i=0;i<seqorder[X].seqitem.size();i++)
     {
      StringGrid1->Cells[0][i+1]=seqorder[X].seqitem[i].Item_id;
      StringGrid1->Cells[1][i+1]=seqorder[X].seqitem[i].product_id;
      StringGrid1->Cells[2][i+1]=seqorder[X].seqitem[i].product_name;
      StringGrid1->Cells[3][i+1]=seqorder[X].seqitem[i].quantity;
      StringGrid1->Cells[4][i+1]=seqorder[X].seqitem[i].unit_price;
      StringGrid1->Cells[5][i+1]=seqorder[X].seqitem[i].sub_total;
     }
  }
  else
  {
     StringGrid1-> RowCount=2;
     for(int i=0;i< 6;i++)
     {
       StringGrid1->Cells[i][1]="";
     }
   }
}
//---------------------------------------------------------------------------
 SeqOrder*
 TForm2::GetOrder(void)
 {
   return &seqorder;
 }
 
 void
 TForm2::RefreshData()
 {
   for(int i=0;i<seqorder.size();i++)
      seqorder[i].seqitem.clear();
//
//   if( seqorder.size() != 0)
//      seqorder.clear();

  PetStore::OrderInfoList_var OrderInfoList=Manager->listAllOrders();
  
//  if( OrderInfoList->length() == 0 )
//  	return ;
//  	
  	
  for(int i=0;i<OrderInfoList->length();i++)
  {
    pOrder=new   Order;
    pOrder->order_id =OrderInfoList[i].order_id.in();
    pOrder->user_name =OrderInfoList[i].user_name.in();
    pOrder->user_address =OrderInfoList[i].user_address.in();
    pOrder->order_time =OrderInfoList[i].order_time.in();
    pOrder->sub_total =OrderInfoList[i].sub_total;

    for(int j=0;j< OrderInfoList[i].order_items.length();j++)
    {
       pItem =new Item;
       pItem->Item_id =OrderInfoList[i].order_items[j].orderItem_id.in();
       pItem->product_id =OrderInfoList[i].order_items[j].product_id.in();
       pItem->product_name =OrderInfoList[i].order_items[j].product_name.in();
       pItem->quantity =OrderInfoList[i].order_items[j].quantity;
       pItem->unit_price =OrderInfoList[i].order_items[j].unit_price;
       pItem->sub_total =OrderInfoList[i].order_items[j].sub_total;
       pOrder->seqitem.insert(pOrder->seqitem.end(),*pItem);
       delete pItem;
    }
    seqorder.insert(seqorder.end(),*pOrder);
    delete pOrder;
  }
 }

void __fastcall
TForm2::FormShow(TObject *Sender)
{
//	
// 
//  if (seqorder.size() == 0 )
//  return;
  
  RefreshData();

  for(int i=0;i<seqorder.size();i++)
  {
    StringGrid2->Cells[0][i+1]=seqorder[i].order_id;
    StringGrid2->Cells[1][i+1]=seqorder[i].user_name;
    StringGrid2->Cells[2][i+1]=seqorder[i].user_address;
    StringGrid2->Cells[3][i+1]=seqorder[i].order_time;
    StringGrid2->Cells[4][i+1]=seqorder[i].sub_total;
  }
  
  if (seqorder.size() == 0 )
  return;
  
  StringGrid1-> RowCount = seqorder[0].seqitem.size()+1;
  for(int i=0;i<seqorder[0].seqitem.size();i++)
  {
    StringGrid1->Cells[0][i+1]=seqorder[0].seqitem[i].Item_id;
    StringGrid1->Cells[1][i+1]=seqorder[0].seqitem[i].product_id;
    StringGrid1->Cells[2][i+1]=seqorder[0].seqitem[i].product_name;
    StringGrid1->Cells[3][i+1]=seqorder[0].seqitem[i].quantity;
    StringGrid1->Cells[4][i+1]=seqorder[0].seqitem[i].unit_price;
    StringGrid1->Cells[5][i+1]=seqorder[0].seqitem[i].sub_total;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
    for(int i=0;i<seqorder.size();i++)
      seqorder[i].seqitem.clear();
   seqorder.clear();   
}
//---------------------------------------------------------------------------

