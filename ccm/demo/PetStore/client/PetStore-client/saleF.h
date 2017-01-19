//---------------------------------------------------------------------------

#ifndef saleFH
#define saleFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <jpeg.hpp>


//#include "PetStore_c.h"
//#include "CCMHome_c.h"
//#include "CCMObject_c.h"
//#include "HomeFinder_c.h"
//#include "CCMCommon_c.h"

#include "pet.h"
#include <string.h>



//---------------------------------------------------------------------------
class TformSale : public TForm
{
__published:	// IDE-managed Components
        TPanel *panelLeft;
        TPanel *panelTop;
        TImage *imageBackGround;
        TImage *imageProduct;
        TMemo *memoProductIntroduction;
        TSplitter *spliterLeftPanel;
        TSplitter *topSplitter;
        TSpeedButton *speedButtonView;
        TSplitter *leftSplitter;
        TImage *imageHome;
        TImage *imageCart;
        TLabel *labelPet;
        TEdit *editSearchContext;
        TComboBox *comboBoxKind;
        TButton *buttonSearch;
        TSpeedButton *speedButtonExit;
        TImage *imageDog;
        TImage *imageCat;
        TImage *imageFish;
        TImage *imageBird;
        TStringGrid *stringGridIntroduction;
        TEdit *editProductionNumber;
        TLabel *labelProduction;
        TButton *buttonAdd;

        void __fastcall FormCreate(TObject *Sender);
        void __fastcall SpeedButton9Click(TObject *Sender);
        void __fastcall imageHomeClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall imageDogClick(TObject *Sender);
        void __fastcall imageCatClick(TObject *Sender);
        void __fastcall imageFishClick(TObject *Sender);
        void __fastcall imageBirdClick(TObject *Sender);
        void __fastcall speedButtonViewClick(TObject *Sender);
        void __fastcall buttonSearchClick(TObject *Sender);
        void __fastcall imageCartClick(TObject *Sender);
        void __fastcall buttonAddClick(TObject *Sender);
        void __fastcall stringGridIntroductionClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TformSale(TComponent* Owner);

        enum PetStore::ProductCategory productC_;
        char* searchContext_;
        char* kindText_;
        
        void __fastcall  TformSale::setStringGrid(PetStore::ProductCategory productC);
        void __fastcall  TformSale::setStringGridAll(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TformSale *formSale;
//---------------------------------------------------------------------------
#endif
