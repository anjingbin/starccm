object Form1: TForm1
  Left = 207
  Top = -3
  Align = alCustom
  BiDiMode = bdLeftToRight
  BorderIcons = [biMinimize]
  BorderStyle = bsSingle
  Caption = 'manager'
  ClientHeight = 620
  ClientWidth = 724
  Color = clSkyBlue
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  ParentBiDiMode = False
  OnClose = FormClose
  DesignSize = (
    724
    620)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 289
    Top = 44
    Width = 28
    Height = 56
    Alignment = taCenter
    Anchors = []
    Color = clBtnFace
    Font.Charset = GB2312_CHARSET
    Font.Color = clMaroon
    Font.Height = -56
    Font.Name = #38582#20070
    Font.Style = [fsItalic]
    ParentColor = False
    ParentFont = False
    Transparent = True
  end
  object ButtonAdd: TButton
    Left = 152
    Top = 571
    Width = 97
    Height = 38
    Caption = #22686#21152#23456#29289
    TabOrder = 0
    OnClick = ButtonAddClick
  end
  object ButtonListAllOrders: TButton
    Left = 337
    Top = 571
    Width = 91
    Height = 38
    Caption = #26597#30475#25152#26377#35746#21333
    TabOrder = 1
    OnClick = ButtonListAllOrdersClick
  end
  object ButtonCount: TButton
    Left = 427
    Top = 571
    Width = 86
    Height = 38
    Caption = #32467#24080
    TabOrder = 2
    OnClick = ButtonCountClick
  end
  object StringGrid1: TStringGrid
    Left = 40
    Top = 392
    Width = 641
    Height = 169
    BiDiMode = bdLeftToRight
    Color = clBackground
    FixedColor = clTeal
    FixedCols = 0
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goColSizing, goRowSelect]
    ParentBiDiMode = False
    ParentFont = False
    TabOrder = 3
    OnMouseDown = StringGrid1MouseDown
    ColWidths = (
      95
      140
      221
      136
      159)
  end
  object BtnExit: TBitBtn
    Left = 512
    Top = 571
    Width = 79
    Height = 38
    Caption = #36864#20986
    TabOrder = 4
    OnClick = BtnExitClick
  end
  object BtnSelectImage: TBitBtn
    Left = 272
    Top = 359
    Width = 75
    Height = 27
    Caption = #36873#25321#22270#29255
    TabOrder = 5
    OnClick = BtnSelectImageClick
  end
  object Panel1: TPanel
    Left = 64
    Top = 312
    Width = 281
    Height = 41
    Color = clBackground
    TabOrder = 6
    object LabeledEdit2: TLabeledEdit
      Left = 24
      Top = 6
      Width = 241
      Height = 29
      AutoSelect = False
      BorderStyle = bsNone
      Color = clBackground
      Ctl3D = True
      EditLabel.Width = 3
      EditLabel.Height = 13
      EditLabel.Caption = 'LabeledEdit2'
      Font.Charset = GB2312_CHARSET
      Font.Color = clRed
      Font.Height = -24
      Font.Name = #21326#25991#34892#26999
      Font.Style = [fsBold, fsItalic]
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentCtl3D = False
      ParentFont = False
      ParentShowHint = False
      ReadOnly = True
      ShowHint = False
      TabOrder = 0
    end
  end
  object Panel2: TPanel
    Left = 360
    Top = 96
    Width = 273
    Height = 33
    Color = clBackground
    TabOrder = 7
    object StaticText1: TStaticText
      Left = 8
      Top = 3
      Width = 44
      Height = 28
      Caption = #20851#20110
      Color = clBackground
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -21
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 0
    end
    object Edit1: TEdit
      Left = 52
      Top = 3
      Width = 228
      Height = 28
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -17
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = True
      ParentFont = False
      TabOrder = 1
    end
  end
  object RichEdit1: TRichEdit
    Left = 360
    Top = 128
    Width = 273
    Height = 177
    Color = clBackground
    Font.Charset = GB2312_CHARSET
    Font.Color = clAqua
    Font.Height = -19
    Font.Name = #38582#20070
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 8
  end
  object BtnCommit: TBitBtn
    Left = 248
    Top = 571
    Width = 89
    Height = 38
    Caption = #25552#20132
    TabOrder = 9
    OnClick = BtnCommitClick
  end
  object Panel3: TPanel
    Left = 160
    Top = 16
    Width = 401
    Height = 73
    Caption = 'Panel3'
    Color = clBackground
    TabOrder = 10
    object TLabeledEdit
      Left = 13
      Top = 8
      Width = 377
      Height = 61
      Color = 8421440
      EditLabel.Width = 3
      EditLabel.Height = 13
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 4194432
      Font.Height = -53
      Font.Name = #38582#20070
      Font.Style = []
      LabelPosition = lpAbove
      LabelSpacing = 3
      ParentFont = False
      TabOrder = 0
      Text = ' '#23456' '#29289' '#21830' '#24215
    end
  end
  object TPanel
    Left = 64
    Top = 96
    Width = 281
    Height = 209
    Color = clBackground
    TabOrder = 11
    object Image1: TImage
      Left = 1
      Top = 1
      Width = 279
      Height = 207
      Align = alClient
      Stretch = True
    end
  end
  object BtnSelectDescrip: TBitBtn
    Left = 372
    Top = 359
    Width = 87
    Height = 27
    Caption = #36873#25321#25551#36848#25991#20214
    TabOrder = 12
    OnClick = BtnSelectDescripClick
  end
  object OpenPictureDialog1: TOpenPictureDialog
    Left = 584
    Top = 32
  end
  object OpenDialog1: TOpenDialog
    Left = 968
    Top = 336
  end
end
