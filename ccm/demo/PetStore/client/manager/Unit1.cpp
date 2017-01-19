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


  StringGrid1->Cells[0][0] = "宠物号";
  StringGrid1->Cells[1][0] = "名称";
  StringGrid1->Cells[2][0] = "分类";
  StringGrid1->Cells[3][0] = "价格(￥)";
  StringGrid1->Cells[4][0] = "数量";
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
     Application->MessageBox("序号不能为空", "警告", MB_OK);
     bStatusAdd=true;
     return;
  }

 for(int i=0;i<seqpro.size();i++)
  {
         if(strcmp(StringGrid1->Cells[0][1].c_str(),seqpro[i].product_id.c_str())==0)
         {
            Application->MessageBox("序号与原有序号重复,请重新输入", "", MB_OK);
            bStatusAdd=true;
            return;
         }
 }

   char* s=StringGrid1->Cells[2][1].c_str();
  if(!(strcmp(s,"DOG")==0| strcmp(s,"CAT")==0 | strcmp(s,"FISH")==0 |strcmp(s,"BIRD")==0
      | strcmp(s,"dog")==0| strcmp(s,"cat")==0 | strcmp(s,"fish")==0 |strcmp(s,"bird")==0))
  {
     Application->MessageBox("宠物品种必须是DOG,CAT,FISH,BIRD的一种", "", MB_OK);
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
    ProductInfo->product_name=CORBA::string_dup("德国牧羊犬");
    ProductInfo->product_category=CORBA::string_dup("dog");
    ProductInfo->product_price=4500;
    ProductInfo->product_qty=15;
    ProductInfo->product_description=CORBA::string_dup("英文名Germen Shepherd Dog,体高：56-66厘米,体重：31-38公斤.原产德国,又称德国狼犬.优秀的德国牧羊犬聪明、自信，勇敢但不敌对，感情丰富,在任何情况下都服从主人的命令.该犬原被驯养的主要目的是看管羊群,由于其强壮的体魄和聪明勇敢的性格,人们强化了对其训练.第一次世界大战时,它参与侦察敌情、捕抓俘虏、救护伤员等方面的工作,更被人们所重视.现在又被作为警犬、辑毒犬、导盲犬使用.");
    ProductInfo->product_picture=CORBA::string_dup("..\\宠物\\德国牧羊犬.jpg");

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
    ProductInfo->product_name=CORBA::string_dup("鲈鱼");
    ProductInfo->product_category=CORBA::string_dup("fish");
    ProductInfo->product_price=200;
    ProductInfo->product_qty=650;
    ProductInfo->product_description=CORBA::string_dup("本科鱼广泛分布在世界各地温带及热带水域，世界淡水鱼有一半以上属于此类鱼。鲇鱼类多属夜行性，眼睛很多已经退化，胡须较发达，成了它们在水底寻觅的主要的感觉器官。");
    ProductInfo->product_picture=CORBA::string_dup("..\\宠物\\鲈鱼.jpg");

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
    ProductInfo->product_name=CORBA::string_dup("凤头鹦鹉");
    ProductInfo->product_category=CORBA::string_dup("bird");
    ProductInfo->product_price=650;
    ProductInfo->product_qty=25;
    ProductInfo->product_description=CORBA::string_dup("葵花凤头鹦鹉,英文名Cacatua galerita，其中还有小葵花凤头鹦鹉、鲑色葵花凤头鹦鹉、大白葵花凤头鹦鹉等几种。原产于澳大利亚北部、东部及东南部至昆士兰岛西部，新几内亚及北部、东部岛屿等地。葵花凤头鹦鹉体长40-50厘米，体羽主要为白色。葵花凤头寿命很长，寿命一般为40年左右，也有的活到60-80年。葵花鹦鹉羽毛雪白漂亮，头顶有黄色冠羽，在受到外界干扰时，冠羽便呈扇状竖立起来，就像一朵盛开的葵花。葵花凤头鹦鹉的羽毛雌雄同色，无法从羽色鉴别雌雄。雄鸟眼沙为黑色，雌鸟为褐色。食物以葵花籽、玉米、花生米、高粱、稻子为主，每天加喂些苹果和少量青菜。人工饲养应采用金属笼或金属架，架两边设有金属的食罐、水罐。葵花凤头鹦鹉嘴掾坚硬，破坏性强，饲主要经常检查锁链牢固情况，以免鹦鹉咬坏后逃跑。该鹦鹉聪明，乖巧，富有感情。鸣声响亮，善学人语，深受朗鹦鹉爱好者喜爱。");
    ProductInfo->product_picture=CORBA::string_dup("..\\宠物\\凤头鹦鹉.jpg");

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
    ProductInfo->product_name=CORBA::string_dup("长毛猫");
    ProductInfo->product_category=CORBA::string_dup("cat");
    ProductInfo->product_price=450;
    ProductInfo->product_qty=60;
    ProductInfo->product_description=CORBA::string_dup("长毛猫 长毛猫背毛较长,一般毛长均有5-10cm,视季节不同而变化.长毛猫体型较大,外表雍容华贵,性格也较温顺,依赖性强,喜欢与人亲近,逗人喜爱.但容易脱毛,加上被毛较厚容易滋生细菌,所以每天都必须梳理被毛和经常要给它洗澡,饲养难度较大.");
    ProductInfo->product_picture=CORBA::string_dup("..\\宠物\\长毛猫.jpg");

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
    ProductInfo->product_name=CORBA::string_dup("短毛猫");
    ProductInfo->product_category=CORBA::string_dup("cat");
    ProductInfo->product_price=550;
    ProductInfo->product_qty=48;
    ProductInfo->product_description=CORBA::string_dup("短毛猫品种较多,目前世界上大约有200来种,尤其是在我国，已繁衍出大量的杂种后代，几乎分布全球各地,短毛猫被毛较短,不须梳理，易于照管,体魄强壮,比长毛猫容易饲养。另外短毛猫捕鼠能力也比较强，是以防鼠为目的的家庭的首选。");
    ProductInfo->product_picture=CORBA::string_dup("..\\宠物\\短毛猫.jpg");

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


