#include "EditForm.h"
#include "NewFileDialog.h"

using namespace imgedit;
using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	
	Application::Run(gcnew EditForm());
	return 0;
}