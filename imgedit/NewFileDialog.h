#pragma once

namespace imgedit {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ NewFileDialog
	/// </summary>			

	struct ImageSize{
		bool valid;
		int width;
		int height;
		
		ImageSize(){
			width = 0;
			height = 0;
			valid = false;
		}

		ImageSize(int _width, int _height, bool _valid = false) {
			width = _width;
			height = _height;
			valid = _valid;
		}
	};

	public ref class NewFileDialog : public System::Windows::Forms::Form
	{
	public:
		NewFileDialog(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

		NewFileDialog(ImageSize *_pimgsz)
		{
			pimgsz = _pimgsz;
			InitializeComponent();
			wNumeric->Value = pimgsz->width;
			hNumeric->Value = pimgsz->height;
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~NewFileDialog()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  okDialogBtn;
	protected:

	/*public: int width = System::Convert::ToInt32((this->wTextBox->Text)->ToString());
	public: int height = System::Convert::ToInt32((this->hTextBox->Text)->ToString());*/

	private: System::Windows::Forms::Button^  cancelDialogBtn;


	private: System::Windows::Forms::Label^  hDialogLbl;
	private: System::Windows::Forms::Label^  wDialogLbl;
	

	private:
		/// <summary>
		/// “ребуетс€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::NumericUpDown^  hNumeric;
	private: System::Windows::Forms::NumericUpDown^  wNumeric;

		ImageSize *pimgsz;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// ќб€зательный метод дл€ поддержки конструктора - не измен€йте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->okDialogBtn = (gcnew System::Windows::Forms::Button());
			this->cancelDialogBtn = (gcnew System::Windows::Forms::Button());
			this->hDialogLbl = (gcnew System::Windows::Forms::Label());
			this->wDialogLbl = (gcnew System::Windows::Forms::Label());
			this->hNumeric = (gcnew System::Windows::Forms::NumericUpDown());
			this->wNumeric = (gcnew System::Windows::Forms::NumericUpDown());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->hNumeric))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->wNumeric))->BeginInit();
			this->SuspendLayout();
			// 
			// okDialogBtn
			// 
			this->okDialogBtn->Location = System::Drawing::Point(41, 93);
			this->okDialogBtn->Name = L"okDialogBtn";
			this->okDialogBtn->Size = System::Drawing::Size(75, 23);
			this->okDialogBtn->TabIndex = 0;
			this->okDialogBtn->Text = L"OK";
			this->okDialogBtn->UseVisualStyleBackColor = true;
			this->okDialogBtn->Click += gcnew System::EventHandler(this, &NewFileDialog::okDialogBtn_Click);
			// 
			// cancelDialogBtn
			// 
			this->cancelDialogBtn->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->cancelDialogBtn->Location = System::Drawing::Point(150, 93);
			this->cancelDialogBtn->Name = L"cancelDialogBtn";
			this->cancelDialogBtn->Size = System::Drawing::Size(75, 23);
			this->cancelDialogBtn->TabIndex = 1;
			this->cancelDialogBtn->Text = L"Cancel";
			this->cancelDialogBtn->UseVisualStyleBackColor = true;
			this->cancelDialogBtn->Click += gcnew System::EventHandler(this, &NewFileDialog::cancelDialogBtn_Click);
			// 
			// hDialogLbl
			// 
			this->hDialogLbl->AutoSize = true;
			this->hDialogLbl->Location = System::Drawing::Point(166, 20);
			this->hDialogLbl->Name = L"hDialogLbl";
			this->hDialogLbl->Size = System::Drawing::Size(38, 13);
			this->hDialogLbl->TabIndex = 4;
			this->hDialogLbl->Text = L"Height";
			// 
			// wDialogLbl
			// 
			this->wDialogLbl->AutoSize = true;
			this->wDialogLbl->Location = System::Drawing::Point(57, 20);
			this->wDialogLbl->Name = L"wDialogLbl";
			this->wDialogLbl->Size = System::Drawing::Size(35, 13);
			this->wDialogLbl->TabIndex = 5;
			this->wDialogLbl->Text = L"Width";
			// 
			// hNumeric
			// 
			this->hNumeric->Location = System::Drawing::Point(150, 47);
			this->hNumeric->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 768, 0, 0, 0 });
			this->hNumeric->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->hNumeric->Name = L"hNumeric";
			this->hNumeric->Size = System::Drawing::Size(75, 20);
			this->hNumeric->TabIndex = 6;
			this->hNumeric->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// wNumeric
			// 
			this->wNumeric->Location = System::Drawing::Point(41, 47);
			this->wNumeric->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1366, 0, 0, 0 });
			this->wNumeric->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->wNumeric->Name = L"wNumeric";
			this->wNumeric->Size = System::Drawing::Size(75, 20);
			this->wNumeric->TabIndex = 7;
			this->wNumeric->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// NewFileDialog
			// 
			this->AcceptButton = this->okDialogBtn;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->cancelDialogBtn;
			this->ClientSize = System::Drawing::Size(266, 128);
			this->Controls->Add(this->wNumeric);
			this->Controls->Add(this->hNumeric);
			this->Controls->Add(this->wDialogLbl);
			this->Controls->Add(this->hDialogLbl);
			this->Controls->Add(this->cancelDialogBtn);
			this->Controls->Add(this->okDialogBtn);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->Name = L"NewFileDialog";
			this->Text = L"Input size of new image";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->hNumeric))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->wNumeric))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void okDialogBtn_Click(System::Object^  sender, System::EventArgs^  e) {
	pimgsz->valid = true;
	int newImageWidth, newImageHeight;
	newImageWidth = System::Convert::ToInt32(wNumeric->Value);
	newImageHeight = System::Convert::ToInt32(hNumeric->Value);

	pimgsz->width = newImageWidth;
	pimgsz->height = newImageHeight;

	this->Close();
}
private: System::Void cancelDialogBtn_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
}
};
}
