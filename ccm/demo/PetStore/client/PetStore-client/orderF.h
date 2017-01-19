//---------------------------------------------------------------------------

#ifndef orderFH
#define orderFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>


#include"pet.h"
//---------------------------------------------------------------------------
class TformOrder : public TForm
{
__published:	// IDE-managed Components
        TSplitter *splitterTop;
        TPanel *topPanel;
        TImage *imageBackGround;
        TSpeedButton *speedButtonView;
        TSpeedButton *speedButtonExit;
        TImage *imageHome;
        TImage *imageCart;
        TLabel *labelPet;
        TImage *imageDog;
        TImage *imageCat;
        TImage *imageFish;
        TImage *imageBird;
        TImage *imageNext;
        TImage *imageFirst;
        TEdit *editSearchContext;
        TComboBox *comboBoxKind;
        TButton *buttonSearch;
        TStringGrid *stringGridIntroduction;
        TGroupBox *groupBoxUser;
        TSplitter *splitterLeft;
        TLabel *labelName;
        TLabel *labelAddress;
        TEdit *editAddress;
        TEdit *editName;
        TPanel *panelBottom;
        TLabel *labelTotal;
        TLabel *labelTotalPrice;
        TLabel *labelRMB;
        void __fastcall imageNextClick(TObject *Sender);
        void __fastcall imageFirstClick(TObject *Sender);
        void __fastcall imageCartClick(TObject *Sender);
        void __fastcall imageHomeClick(TObject *Sender);
        void __fastcall speedButtonExitClick(TObject *Sender);
        void __fastcall imageDogClick(TObject *Sender);
        void __fastcall imageCatClick(TObject *Sender);
        void __fastcall imageFishClick(TObject *Sender);
        void __fastcall imageBirdClick(TObject *Sender);
        void __fastcall buttonSearchClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall speedButtonViewClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TformOrder(TComponent* Owner);
         Components::HomeFinder_var HomeFinder_;
        enum PetStore::ProductCategory productC_;
        char* searchContext_;
        char* kindText_;
        int YCol,XRow;
};
//---------------------------------------------------------------------------
extern PACKAGE TformOrder *formOrder;
//---------------------------------------------------------------------------
#endif
