//---------------------------------------------------------------------------

#ifndef viewFH
#define viewFH
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
//#include <Classes.hpp>
//#include <Controls.hpp>
//#include <StdCtrls.hpp>
//#include <Forms.hpp>
//#include <ExtCtrls.hpp>
//#include <Graphics.hpp>

#include "pet.h"
#include "cartF.h"
#include <Buttons.hpp>

//---------------------------------------------------------------------------
class TformView : public TForm
{
__published:	// IDE-managed Components
        TPanel *panelTop;
        TPanel *panelLeft;
        TSplitter *splitterTop;
        TSplitter *splitterLeft;
        TPanel *panelRight;
        TImage *imageBackGround;
        TImage *imageLeftBackGround;
        TImage *imageLeftDog;
        TImage *imageLeftCat;
        TImage *imageLeftFish;
        TImage *imageRightBackGround;
        TImage *imageRightFish;
        TImage *imageRightDog;
        TImage *imageRightCat;
        TImage *imageRightBird;
        TImage *imageLeftBird;
        TLabel *labelLeftDog;
        TLabel *labelLeftCat;
        TLabel *labelLeftFish;
        TLabel *labelLeftBird;
        TImage *imageRightBigBird;
        TButton *buttonSearch;
        TComboBox *comboBoxKind;
        TEdit *editSearchContext;
        TLabel *labelPet;
        TImage *imageCart;
        TImage *imageHome;
        TSpeedButton *speedButtonExit;
        TSpeedButton *speedButtonView;
        void __fastcall buttonSearchClick(TObject *Sender);
        void __fastcall imageRightBigBirdClick(TObject *Sender);
        void __fastcall imageRightFishClick(TObject *Sender);
        void __fastcall imageRightDogClick(TObject *Sender);
        void __fastcall imageRightCatClick(TObject *Sender);
        void __fastcall imageRightBirdClick(TObject *Sender);
        void __fastcall imageLeftDogClick(TObject *Sender);
        void __fastcall imageLeftCatClick(TObject *Sender);
        void __fastcall imageLeftFishClick(TObject *Sender);
        void __fastcall imageLeftBirdClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall imageCartClick(TObject *Sender);
        void __fastcall speedButtonViewClick(TObject *Sender);
        void __fastcall speedButtonExitClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TformView(TComponent* Owner);

        enum PetStore::ProductCategory productC_;
        char* searchContext_;
        char* kindText_;
        
        void __fastcall  TformView::setStringGrid(PetStore::ProductCategory productC);
        void __fastcall  TformView::setStringGridAll(void);

};
//---------------------------------------------------------------------------
extern PACKAGE TformView *formView;
//---------------------------------------------------------------------------
#endif
