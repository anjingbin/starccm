//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "login.h"
#include "Unit4.h"
#include "fstream.h"
#include <Windows.h>



//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;


using namespace std;

//---------------------------------------------------------------------------

char* __fastcall TForm1::GetCCMProfileString(const char* strSectionName,
								const char* keyName, 
								const char* defautValue,
								const char* fileName)
{
	const char* homeDir = getenv("CCM_HOME");
	string filepath = homeDir;
	filepath += "\\";
	filepath += fileName;

	FILE* iniFile = fopen(filepath.c_str(),"r");
	if(iniFile == NULL)
		return NULL;

	char *break_set = " \t\n=";
	char dataBuf[128];
	string section = "[";
	section += strSectionName;
	section += "]";
	
	CORBA::String_var strRtn = defautValue;

	while(!feof(iniFile))
	{
		char *tokp,*sp;
		sp = dataBuf;
		fgets(dataBuf,128,iniFile);
		tokp = strtok(sp,break_set);
		if(tokp == NULL)
			continue;

		if(strcmp(tokp,section.c_str()) == 0)
		{
			while(!feof(iniFile))
			{
				fgets(dataBuf,128,iniFile);
				sp = dataBuf;
				tokp = strtok(sp,break_set);
				if(tokp == NULL)
					continue;
				if(tokp[0] == '[')
					goto EXIT;
				if(strcmp(tokp,keyName)==0)
				{
					sp = NULL;
					tokp = strtok(sp,break_set);
					if(tokp != NULL) 
					{
						strRtn = CORBA::string_dup(tokp);
						goto EXIT;
					}
				}//for statement if(strcmp(tokp,keyName)==0)
			}//for statement while(!feof(iniFile))
		}//for statement if(strcmp(tokp,section.c_str()) == 0)
	}//for statement while(!feof(iniFile))
EXIT:
	fclose(iniFile);
	return strRtn._retn();
}
//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
  
  int dummy = 0;

  orb = ::CORBA::ORB_init(dummy,(char* const*) NULL);
  assert(!CORBA::is_nil(orb));


//get the NameService IP,and save it.
string nameService_ = CORBA::string_dup("corbaloc::");
nameService_ += GetCCMProfileString("domain","main_host_ip","","starccm.properties");
nameService_ += CORBA::string_dup(":");
nameService_ += GetCCMProfileString("domain","NameServicePort","","starccm.properties");	
nameService_ += CORBA::string_dup("/NameService");


  CosNaming::NamingContext_var rootContext =
        CosNaming::NamingContext::_narrow(
          orb->string_to_object(nameService_.c_str()));

   CosNaming::Name name;

      name.length(1);
      name[0].id = (const char *) "ShoppingManager";
      name[0].kind = (const char *) "";
      manager = PetStore::Manager::_narrow(rootContext->resolve(name));


      name[0].id = (const char *) "ShoppingSales";
      name[0].kind = (const char *) "";

      sales = PetStore::Sales::_narrow(rootContext->resolve(name));
      businessOp = sales->provide_for_client();


      assert(!CORBA::is_nil(businessOp));

     AddProduct();

      RefreshData();


  StringGrid1->Cells[0][0] = "�����";
  StringGrid1->Cells[1][0] = "����";
  StringGrid1->Cells[2][0] = "����";
  StringGrid1->Cells[3][0] = "�۸�(��)";
  StringGrid1->Cells[4][0] = "����";
  bStatusAdd=false;
  FileName= "";

}
//---------------------------------------------------------------------------


void __fastcall TForm1::BtnSelectImageClick(TObject *Sender)
{
  if(OpenPictureDialog1->Execute())
  {
     FileName=OpenPictureDialog1->FileName;
     Image1->Picture->LoadFromFile(FileName) ;
     Image1->Show();
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonListAllOrdersClick(TObject *Sender)
{
   Form2->Show();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::StringGrid1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(!bStatusAdd)
   {
     X= StringGrid1->Row-1;
     if(X>= seqpro.size())
     {
      return;
      }
     LabeledEdit2->Text= seqpro[X].product_name;
     Edit1->Text= seqpro[X].product_name;
     RichEdit1->Text=seqpro[X].product_description;
     if(seqpro[X].product_pic!="")
       Image1->Picture->LoadFromFile(seqpro[X].product_pic);
   }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::BtnCommitClick(TObject *Sender)
{
    bStatusAdd=false;

  if(strcmp(StringGrid1->Cells[0][1].c_str(),"")==0)
  {
     Application->MessageBox("��Ų���Ϊ��", "����", MB_OK);
     bStatusAdd=true;
     return;
  }

 for(int i=0;i<seqpro.size();i++)
  {
         if(strcmp(StringGrid1->Cells[0][1].c_str(),seqpro[i].product_id.c_str())==0)
         {
            Application->MessageBox("�����ԭ������ظ�,����������", "", MB_OK);
            bStatusAdd=true;
            return;
         }
 }

   char* s=StringGrid1->Cells[2][1].c_str();
  if(!(strcmp(s,"DOG")==0| strcmp(s,"CAT")==0 | strcmp(s,"FISH")==0 |strcmp(s,"BIRD")==0
      | strcmp(s,"dog")==0| strcmp(s,"cat")==0 | strcmp(s,"fish")==0 |strcmp(s,"bird")==0))
  {
     Application->MessageBox("����Ʒ�ֱ�����DOG,CAT,FISH,BIRD��һ��", "", MB_OK);
     bStatusAdd=true;
     return;
  }

    PetStore::ProductInfo_var  ProductInfo = new PetStore::ProductInfo;
    ProductInfo->product_id=CORBA::string_dup(StringGrid1->Cells[0][1].c_str());
    ProductInfo->product_name=CORBA::string_dup(StringGrid1->Cells[1][1].c_str());
    ProductInfo->product_category=CORBA::string_dup(StringGrid1->Cells[2][1].c_str());
    ProductInfo->product_price=atoi(StringGrid1->Cells[3][1].c_str());
    ProductInfo->product_qty=atoi(StringGrid1->Cells[4][1].c_str());
    ProductInfo->product_description=CORBA::string_dup(RichEdit1->Text.c_str());
    ProductInfo->product_picture=CORBA::string_dup(FileName.c_str());

    try
    {
         manager->addProduct(ProductInfo.in());
    }
    catch(Components::CCMException& ex)
    {
        assert(false);
    }

  RefreshData();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonAddClick(TObject *Sender)
{
  bStatusAdd=true;
  Edit1->Visible=false;
  LabeledEdit2->Visible=false;
  BtnSelectImage->Enabled=true;
  ButtonAdd->Enabled=false;
  BtnCommit->Enabled=true;
  BtnSelectDescrip->Enabled=true;
  RichEdit1->ReadOnly=false;
  FileName="";
  StringGrid1->Options =StringGrid1->Options>>goRowSelect;
  StringGrid1->Options =StringGrid1->Options<<goEditing;
  StringGrid1->Options =StringGrid1->Options<<goAlwaysShowEditor;
  StringGrid1->EditorMode=true;
  StringGrid1->RowCount=2;
  for(int i=0;i< 5;i++)
    StringGrid1->Cells[i][1]="";
  RichEdit1->Text="";
  Image1->Hide();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::BtnExitClick(TObject *Sender)
{
  PasswordDlg->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonCountClick(TObject *Sender)
{
   Form4->Show();
}
//---------------------------------------------------------------------------
  SeqPro* TForm1::GetSeqPro(void)
  {
     return &seqpro;
  };
void __fastcall TForm1::BtnSelectDescripClick(TObject *Sender)
{
   if(OpenDialog1->Execute())
   {
    char* FileName=OpenDialog1->FileName.c_str();
     char ch;
     if(FileName!="")
     {
       std::ifstream file(FileName);
       string s;
       while(file.get(ch))
         s+=ch;
        RichEdit1->Text=s.c_str();
     }
   }
}
//---------------------------------------------------------------------------
PetStore::Manager_ptr
TForm1::getManager()
 {
   return manager;
 }
void TForm1::AddProduct()
{
    PetStore::ProductInfo_var  ProductInfo = new PetStore::ProductInfo;

    ProductInfo->product_id= CORBA::string_dup("1");
    ProductInfo->product_name=CORBA::string_dup("�¹�����Ȯ");
    ProductInfo->product_category=CORBA::string_dup("dog");
    ProductInfo->product_price=4500;
    ProductInfo->product_qty=15;
    ProductInfo->product_description=CORBA::string_dup("Ӣ����Germen Shepherd Dog,��ߣ�56-66����,���أ�31-38����.ԭ���¹�,�ֳƵ¹���Ȯ.����ĵ¹�����Ȯ���������ţ��¸ҵ����жԣ�����ḻ,���κ�����¶��������˵�����.��Ȯԭ��ѱ������ҪĿ���ǿ�����Ⱥ,������ǿ׳�����Ǻʹ����¸ҵ��Ը�,����ǿ���˶���ѵ��.��һ�������սʱ,�����������顢��ץ��²���Ȼ���Ա�ȷ���Ĺ���,��������������.�����ֱ���Ϊ��Ȯ������Ȯ����äȮʹ��.");
    ProductInfo->product_picture=CORBA::string_dup("..\\����\\�¹�����Ȯ.jpg");

    try
    {
        manager->addProduct(ProductInfo.in());
    }
    catch(Components::CCMException& ex)
    {
        assert(false);
    }
	//-----------------------------------------------------------------------------
    ProductInfo->product_id= CORBA::string_dup("2");
    ProductInfo->product_name=CORBA::string_dup("����");
    ProductInfo->product_category=CORBA::string_dup("fish");
    ProductInfo->product_price=200;
    ProductInfo->product_qty=650;
    ProductInfo->product_description=CORBA::string_dup("������㷺�ֲ�����������´����ȴ�ˮ�����絭ˮ����һ���������ڴ����㡣���������ҹ���ԣ��۾��ܶ��Ѿ��˻�������Ϸ������������ˮ��Ѱ�ٵ���Ҫ�ĸо����١�");
    ProductInfo->product_picture=CORBA::string_dup("..\\����\\����.jpg");

    try
    {
        manager->addProduct(ProductInfo.in());
    }
    catch(Components::CCMException& ex)
    {
        assert(false);
    }
    //-----------------------------------------------------------------------------
    ProductInfo->product_id= CORBA::string_dup("3");
    ProductInfo->product_name=CORBA::string_dup("��ͷ����");
    ProductInfo->product_category=CORBA::string_dup("bird");
    ProductInfo->product_price=650;
    ProductInfo->product_qty=25;
    ProductInfo->product_description=CORBA::string_dup("������ͷ����,Ӣ����Cacatua galerita�����л���С������ͷ���ġ���ɫ������ͷ���ġ���׿�����ͷ���ĵȼ��֡�ԭ���ڰĴ����Ǳ��������������ϲ�����ʿ�����������¼����Ǽ���������������ȵء�������ͷ�����峤40-50���ף�������ҪΪ��ɫ��������ͷ�����ܳ�������һ��Ϊ40�����ң�Ҳ�еĻ60-80�ꡣ����������ëѩ��Ư����ͷ���л�ɫ�������ܵ�������ʱ����������״��������������һ��ʢ���Ŀ�����������ͷ���ĵ���ë����ͬɫ���޷�����ɫ������ۡ�������ɳΪ��ɫ������Ϊ��ɫ��ʳ���Կ����ѡ����ס������ס�����������Ϊ����ÿ���ιЩƻ����������ˡ��˹�����Ӧ���ý�����������ܣ����������н�����ʳ�ޡ�ˮ�ޡ�������ͷ���������Ӳ���ƻ���ǿ������Ҫ������������ι��������������ҧ�������ܡ������Ĵ��������ɣ����и��顣������������ѧ������������İ�����ϲ����");
    ProductInfo->product_picture=CORBA::string_dup("..\\����\\��ͷ����.jpg");

    try
    {
        manager->addProduct(ProductInfo.in());
    }
    catch(Components::CCMException& ex)
    {
        assert(false);
    }
    //-----------------------------------------------------------------------------
    ProductInfo->product_id= CORBA::string_dup("4");
    ProductInfo->product_name=CORBA::string_dup("��ëè");
    ProductInfo->product_category=CORBA::string_dup("cat");
    ProductInfo->product_price=450;
    ProductInfo->product_qty=60;
    ProductInfo->product_description=CORBA::string_dup("��ëè ��ëè��ë�ϳ�,һ��ë������5-10cm,�Ӽ��ڲ�ͬ���仯.��ëè���ͽϴ�,���Ӻ�ݻ���,�Ը�Ҳ����˳,������ǿ,ϲ�������׽�,����ϲ��.��������ë,���ϱ�ë�Ϻ���������ϸ��,����ÿ�춼��������ë�;���Ҫ����ϴ��,�����ѶȽϴ�.");
    ProductInfo->product_picture=CORBA::string_dup("..\\����\\��ëè.jpg");

    try
    {
        manager->addProduct(ProductInfo.in());
    }
    catch(Components::CCMException& ex)
    {
        assert(false);
    }
    //-----------------------------------------------------------------------------
    ProductInfo->product_id= CORBA::string_dup("5");
    ProductInfo->product_name=CORBA::string_dup("��ëè");
    ProductInfo->product_category=CORBA::string_dup("cat");
    ProductInfo->product_price=550;
    ProductInfo->product_qty=48;
    ProductInfo->product_description=CORBA::string_dup("��ëèƷ�ֽ϶�,Ŀǰ�����ϴ�Լ��200����,���������ҹ����ѷ��ܳ����������ֺ���������ֲ�ȫ�����,��ëè��ë�϶�,�������������չ�,����ǿ׳,�ȳ�ëè���������������ëè��������Ҳ�Ƚ�ǿ�����Է���ΪĿ�ĵļ�ͥ����ѡ��");
    ProductInfo->product_picture=CORBA::string_dup("..\\����\\��ëè.jpg");

    try
    {
        manager->addProduct(ProductInfo.in());
    }
    catch(Components::CCMException& ex)
    {
        assert(false);
    }
    //-----------------------------------------------------------------------------
    

 }
void TForm1::RefreshData()
{
   seqpro.clear();
   PetStore::ProductIndexInfoList_var listproduct = businessOp -> listAllProducts();
   for(int i=0;i<listproduct->length();i++)
  {
      PetStore::ProductInfo_var  ProductInfo=businessOp->listProduct(listproduct[i].product_id);

      product pProduct;
      pProduct.product_id=ProductInfo->product_id.in();
      pProduct.product_name=ProductInfo->product_name.in();
      pProduct.product_pic=ProductInfo->product_picture.in();
      pProduct.product_description=ProductInfo->product_description.in();
      pProduct.product_qty=ProductInfo->product_qty;
      pProduct.product_price=ProductInfo->product_price;
      pProduct.product_category=ProductInfo->product_category.in();
      seqpro.insert(seqpro.end(),pProduct);
  }

  BtnSelectImage->Enabled=false;
  Edit1->Visible=true;
  LabeledEdit2->Visible=true;
  RichEdit1->ReadOnly=true;
  ButtonAdd->Enabled=true;
  BtnCommit->Enabled=false;
  BtnSelectDescrip->Enabled=false;
  StringGrid1->Options =StringGrid1->Options<<goRowSelect;
  StringGrid1->Options =StringGrid1->Options>>goEditing;
  StringGrid1->Options =StringGrid1->Options>>goAlwaysShowEditor;
  StringGrid1->EditorMode=false;

  Image1->Show();

  StringGrid1->RowCount=seqpro.size()+1;
  for(int i=0;i<seqpro.size();i++)
  {
        StringGrid1->Cells[0][i+1]=seqpro[i].product_id;
        StringGrid1->Cells[1][i+1]=seqpro[i].product_name;
        StringGrid1->Cells[2][i+1]=seqpro[i].product_category;
        StringGrid1->Cells[3][i+1]=seqpro[i].product_price;
        StringGrid1->Cells[4][i+1]=seqpro[i].product_qty;
   }
 if (seqpro.size()!=0)
 {
   LabeledEdit2->Text= seqpro[0].product_name;
   Edit1->Text= seqpro[0].product_name;
   Image1->Picture->LoadFromFile(seqpro[0].product_pic) ;
   RichEdit1->Text=seqpro[0].product_description;
   }
}
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
    seqpro.clear();
}
//---------------------------------------------------------------------------


