program MapEditor;

uses
  Forms,
  Main in 'Main.pas' {Form1},
  About in 'About.pas' {AboutBox};
//  Options in 'Options.pas' {Form2};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(TAboutBox, AboutBox);
//  Application.CreateForm(TForm2, Form2);
  Application.Run;
end.
