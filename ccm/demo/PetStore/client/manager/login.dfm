object PasswordDlg: TPasswordDlg
  Left = 361
  Top = 237
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Password Dialog'
  ClientHeight = 167
  ClientWidth = 292
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 40
    Width = 97
    Height = 20
    Caption = #21333#20301#21517#31216#65306
    Color = clAqua
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clAqua
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    Transparent = True
    Visible = False
  end
  object Label2: TLabel
    Left = 16
    Top = 80
    Width = 88
    Height = 20
    Caption = #23494'          '#30721#65306
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clAqua
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Transparent = True
  end
  object Password: TEdit
    Left = 118
    Top = 75
    Width = 121
    Height = 21
    Color = clSkyBlue
    PasswordChar = '*'
    TabOrder = 0
    Text = 'Manager'
  end
  object OKBtn: TButton
    Left = 54
    Top = 131
    Width = 75
    Height = 25
    Caption = #30331#24405
    Default = True
    ModalResult = 1
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 174
    Top = 131
    Width = 75
    Height = 25
    Cancel = True
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 2
    OnClick = CancelBtnClick
  end
  object UserName: TEdit
    Left = 118
    Top = 40
    Width = 121
    Height = 21
    Color = clSkyBlue
    TabOrder = 3
    Text = 'Manager'
  end
end
