object AboutBox: TAboutBox
  Left = 200
  Top = 108
  BorderStyle = bsDialog
  Caption = 'About'
  ClientHeight = 213
  ClientWidth = 298
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 281
    Height = 161
    BevelInner = bvRaised
    BevelOuter = bvLowered
    ParentColor = True
    TabOrder = 0
    object ProgramIcon: TImage
      Left = 8
      Top = 8
      Width = 65
      Height = 57
      Stretch = True
      IsControl = True
    end
    object ProductName: TLabel
      Left = 88
      Top = 16
      Width = 120
      Height = 13
      Caption = 'Map Editor for Galaxy NG'
      IsControl = True
    end
    object Version: TLabel
      Left = 88
      Top = 32
      Width = 56
      Height = 13
      Caption = 'Version: 0.5'
      IsControl = True
    end
    object Copyright: TLabel
      Left = 8
      Top = 80
      Width = 3
      Height = 13
      IsControl = True
    end
    object Comments: TLabel
      Left = 8
      Top = 80
      Width = 3
      Height = 13
      WordWrap = True
      IsControl = True
    end
    object Label1: TLabel
      Left = 89
      Top = 48
      Width = 83
      Height = 13
      Caption = 'Date: 04.02.2004'
    end
  end
  object OKButton: TButton
    Left = 111
    Top = 180
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = OKButtonClick
  end
end
