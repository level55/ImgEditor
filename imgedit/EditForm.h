#pragma once
#include <cmath>
#include <vector>

#include "NewFileDialog.h"

namespace imgedit {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	enum TTool{
		TTPen, TTLine, TTEraser, TTBrush, TTFill, TTRectangle, TTEllipse, TTSaturation, TTHue, TTBrightness
	};

	public ref class EditForm : public System::Windows::Forms::Form
	{
	public:
		EditForm(void)
		{
			InitializeComponent();
			InitializePicture(pw, ph);
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~EditForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:

		Graphics^ graphics;
		Image^ oldImage;
		int lastX, lastY;
		int w, h, rx, ry;
		int ph = 265;
		int pw = 385;
		bool lastValid = false;
		TTool currentTool = TTPen;
		Color currentColor = Color(Color::Black);
		float currentSize = 3.0;
		
	private: System::Windows::Forms::CheckBox^  checkBoxFill;
	private: System::Windows::Forms::ToolStripMenuItem^  effectsStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  saturationToolStripMenuItem;
	private: System::Windows::Forms::NumericUpDown^  effectsNumUpDown;
	private: System::Windows::Forms::Button^  effectResetBtn;
	private: System::Windows::Forms::ToolStripMenuItem^  hueToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^  brightnessToolStripMenuItem;
	private: System::Windows::Forms::TrackBar^  hueTrackBar;
	private: System::Windows::Forms::ToolStripMenuItem^  fillToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  brushToolStripMenuItem;
	private: System::Windows::Forms::ToolStripComboBox^  toolStripComboBox1;
	private: System::Windows::Forms::ToolStripMenuItem^  eraserToolStripMenuItem;
	private: System::Windows::Forms::PictureBox^  picture;
	private: System::Windows::Forms::ToolStripMenuItem^  colorToolStripMenuItem;
		

	 void InitializePicture(int pw, int ph)
		{
			delete picture->Image;
			Bitmap^ flag = gcnew Bitmap(pw, ph);
			Graphics^ flagGraphics = Graphics::FromImage(flag);
			flagGraphics->FillRectangle(Brushes::White, 0, 0, pw, ph);
			picture->Image = flag;
			
		}

		void UncheckTools()
		{
			penToolStripMenuItem->Checked = false;
			eraserToolStripMenuItem->Checked = false;
			rectangleToolStripMenuItem->Checked = false;
			ellipseToolStripMenuItem->Checked = false;
			fillToolStripMenuItem->Checked = false;
			saturationToolStripMenuItem->Checked = false;
			brightnessToolStripMenuItem->Checked = false;
			hueToolStripMenuItem->Checked = false;
			checkBoxFill->Visible = false;
			effectsNumUpDown->Visible = false;
			effectResetBtn->Visible = false;
			hueTrackBar->Visible = false;
		}


		void RgbToHsv(Color color, float &h, float &s, float &b)
		{
			int max = Math::Max(color.R, Math::Max(color.G, color.B));
			int min = Math::Min(color.R, Math::Min(color.G, color.B));

			h = color.GetHue();
			b = (float)max;

			if (max == 0)
				s = 0.00;
			else s = (float)((1.00 - (1.00 * min / max)));

		}

		void HsvToRgbOnBitmap(Bitmap^ bitmap, int x, int y, float h, float s, float b)
		{
			int k = (int)(floor(h / 60)) % 6;
			double f = h / 60 - floor(h / 60);

			int v = (int)b;
			int p = (int)(b * (1 - s));
			int q = (int)(b * (1 - f * s));
			int t = (int)(b * (1 - (1 - f) * s));

			switch (k)
			{
			case 0:
				bitmap->SetPixel(x, y, Color::FromArgb(v, t, p));
				break;
			case 1:
				bitmap->SetPixel(x, y, Color::FromArgb(q, v, p));
				break;
			case 2:
				bitmap->SetPixel(x, y, Color::FromArgb(p, v, t));
				break;
			case 3:
				bitmap->SetPixel(x, y, Color::FromArgb(p, q, v));
				break;
			case 4:
				bitmap->SetPixel(x, y, Color::FromArgb(t, p, v));
				break;
			default:	//case 5
				bitmap->SetPixel(x, y, Color::FromArgb(v, p, q));
				break;
			}
		}


		void HueChange(float value)
		{
			Bitmap^ bitmap = gcnew Bitmap(this->oldImage);
			float h, s, b;

			for (int i = 0; i < bitmap->Height; i++)
				for (int j = 0; j < bitmap->Width; j++)
				{
					Color color = bitmap->GetPixel(j, i);
					RgbToHsv(color, h, s, b);
					h += value;

					if (h > 360)
						h -= 360;

					if (s > 1.0)
						s = 1.0;
					if (s < 0.0)
						s = 0.0;

					if (s == 0)
					{
						int v = (int)b;
						bitmap->SetPixel(j, i, Color::FromArgb(v, v, v));
						continue;
					}

					HsvToRgbOnBitmap(bitmap, j, i, h, s, b);
				}
			picture->Image = bitmap;
			picture->Refresh();
		}

		void SaturationChange(float value)
		{
			Bitmap^ bitmap = gcnew Bitmap(this->oldImage);
			value /= 100.00;
			float h, s, b;

			for (int i = 0; i < bitmap->Height; i++)
			for (int j = 0; j < bitmap->Width; j++)
			{
				Color color = bitmap->GetPixel(j, i);
				RgbToHsv(color, h, s, b);
				s *= value;

				if (s > 1.0)
					s = 1.0;
				if (s < 0.0)
					s = 0.0;

				if (s == 0)
				{
					int v = (int)b;
					bitmap->SetPixel(j, i, Color::FromArgb(v, v, v));
					continue;
				}

				HsvToRgbOnBitmap(bitmap, j, i, h, s, b);
			}
					picture->Image = bitmap;
					picture->Refresh();

		}

		void BrightnessChange(float value)
		{
			Bitmap^ bitmap = gcnew Bitmap(this->oldImage);
			value /= 100.00;
			float h, s, b;

			for (int i = 0; i < bitmap->Height; i++)
			for (int j = 0; j < bitmap->Width; j++)
			{
				Color color = bitmap->GetPixel(j, i);
				RgbToHsv(color, h, s, b);
				b *= value;

				if (b > 255.0)
					b = 255.0;
				else if (b < 0.0)
					b = 0.0;

				if (s > 1.0)
					s = 1.0;
				else if (s < 0.0)
					s = 0.0;

				if (s == 0)
				{
					int v = (int)b;
					bitmap->SetPixel(j, i, Color::FromArgb(v, v, v));
					continue;
				}

				HsvToRgbOnBitmap(bitmap, j, i, h, s, b);
			}
			picture->Image = bitmap;
			picture->Refresh();

		}

		void Fill(int x, int y, Color ctar)
		{
			Bitmap^ bitmap = gcnew Bitmap(picture->Image);

			if (x > bitmap->Width || y > bitmap->Height || x < 0 || y < 0)
				return;
			
			Color crepl = bitmap->GetPixel(x, y);

			if (Color::FromArgb(255, crepl) == Color::FromArgb(255, ctar))
				return;

			std::vector<std::pair<int, int>> workq, nworkq;
			std::vector<std::pair<int, int>>::iterator k, last;

			workq.push_back(std::pair<int, int>(x, y));

			bitmap->SetPixel(x, y, ctar);
			
			while (!workq.empty())
			{
				k = workq.begin(); last = workq.end();

				for (; k != last; ++k)
				{
					int i = k->first;
					int j = k->second;

					if (i + 1 < bitmap->Width && bitmap->GetPixel(i + 1, j) == crepl){
							nworkq.push_back(std::pair<int, int>(i + 1, j));
							bitmap->SetPixel(i + 1, j, ctar);
					}

					if (i - 1 > 0 && bitmap->GetPixel(i - 1, j) == crepl){
							nworkq.push_back(std::pair<int, int>(i - 1, j));
							bitmap->SetPixel(i - 1, j, ctar);
					}

					if (j + 1 < bitmap->Height && bitmap->GetPixel(i, j + 1) == crepl){
							nworkq.push_back(std::pair<int, int>(i, j + 1));
							bitmap->SetPixel(i, j + 1, ctar);
					}

					if (j - 1 > 0 && bitmap->GetPixel(i, j - 1) == crepl){
							nworkq.push_back(std::pair<int, int>(i, j - 1));
							bitmap->SetPixel(i, j - 1, ctar);
					}
				}
				
				workq.swap(nworkq);
				nworkq.clear();

			}
			picture->Image = bitmap;
			this->picture->Refresh();
		}


	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  newToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  penToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  rectangleToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ellipseToolStripMenuItem;
	private: System::ComponentModel::IContainer^  components;
	protected:

	private:
		/// <summary>
		/// “ребуетс€ переменна€ конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// ќб€зательный метод дл€ поддержки конструктора - не измен€йте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(EditForm::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->newToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->penToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->brushToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripComboBox1 = (gcnew System::Windows::Forms::ToolStripComboBox());
			this->rectangleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ellipseToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->fillToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->eraserToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->colorToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->effectsStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->hueToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saturationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->brightnessToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->checkBoxFill = (gcnew System::Windows::Forms::CheckBox());
			this->effectsNumUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->effectResetBtn = (gcnew System::Windows::Forms::Button());
			this->hueTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->picture = (gcnew System::Windows::Forms::PictureBox());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->effectsNumUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->hueTrackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picture))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->AutoSize = false;
			this->menuStrip1->BackColor = System::Drawing::SystemColors::Menu;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->fileToolStripMenuItem,
					this->toolsToolStripMenuItem, this->colorToolStripMenuItem, this->effectsStripMenuItem1
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(385, 24);
			this->menuStrip1->TabIndex = 1;
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->newToolStripMenuItem,
					this->openToolStripMenuItem, this->saveToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// newToolStripMenuItem
			// 
			this->newToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"newToolStripMenuItem.Image")));
			this->newToolStripMenuItem->Name = L"newToolStripMenuItem";
			this->newToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->newToolStripMenuItem->Text = L"New";
			this->newToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::newToolStripMenuItem_Click);
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"openToolStripMenuItem.Image")));
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"saveToolStripMenuItem.Image")));
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::saveToolStripMenuItem_Click);
			// 
			// toolsToolStripMenuItem
			// 
			this->toolsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->penToolStripMenuItem,
					this->brushToolStripMenuItem, this->rectangleToolStripMenuItem, this->ellipseToolStripMenuItem, this->fillToolStripMenuItem,
					this->eraserToolStripMenuItem
			});
			this->toolsToolStripMenuItem->Name = L"toolsToolStripMenuItem";
			this->toolsToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->toolsToolStripMenuItem->Text = L"Tools";
			// 
			// penToolStripMenuItem
			// 
			this->penToolStripMenuItem->Checked = true;
			this->penToolStripMenuItem->CheckOnClick = true;
			this->penToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->penToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"penToolStripMenuItem.Image")));
			this->penToolStripMenuItem->Name = L"penToolStripMenuItem";
			this->penToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->penToolStripMenuItem->Text = L"Brush";
			this->penToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::penToolStripMenuItem_Click);
			// 
			// brushToolStripMenuItem
			// 
			this->brushToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->toolStripComboBox1 });
			this->brushToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"brushToolStripMenuItem.Image")));
			this->brushToolStripMenuItem->Name = L"brushToolStripMenuItem";
			this->brushToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->brushToolStripMenuItem->Text = L"Width";
			// 
			// toolStripComboBox1
			// 
			this->toolStripComboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->toolStripComboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(20) {
				L"1", L"2", L"3", L"4", L"5", L"6", L"7",
					L"8", L"9", L"10", L"12", L"14", L"16", L"18", L"20", L"22", L"24", L"26", L"28", L"36"
			});
			this->toolStripComboBox1->MaxDropDownItems = 10;
			this->toolStripComboBox1->MaxLength = 2;
			this->toolStripComboBox1->Name = L"toolStripComboBox1";
			this->toolStripComboBox1->Size = System::Drawing::Size(75, 23);
			this->toolStripComboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &EditForm::toolStripComboBox1_SelectedIndexChanged);
			// 
			// rectangleToolStripMenuItem
			// 
			this->rectangleToolStripMenuItem->CheckOnClick = true;
			this->rectangleToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"rectangleToolStripMenuItem.Image")));
			this->rectangleToolStripMenuItem->Name = L"rectangleToolStripMenuItem";
			this->rectangleToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->rectangleToolStripMenuItem->Text = L"Rectangle";
			this->rectangleToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::rectangleToolStripMenuItem_Click);
			// 
			// ellipseToolStripMenuItem
			// 
			this->ellipseToolStripMenuItem->CheckOnClick = true;
			this->ellipseToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ellipseToolStripMenuItem.Image")));
			this->ellipseToolStripMenuItem->Name = L"ellipseToolStripMenuItem";
			this->ellipseToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->ellipseToolStripMenuItem->Text = L"Ellipse";
			this->ellipseToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::ellipseToolStripMenuItem_Click);
			// 
			// fillToolStripMenuItem
			// 
			this->fillToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"fillToolStripMenuItem.Image")));
			this->fillToolStripMenuItem->Name = L"fillToolStripMenuItem";
			this->fillToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->fillToolStripMenuItem->Text = L"Fill";
			this->fillToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::fillToolStripMenuItem_Click);
			// 
			// eraserToolStripMenuItem
			// 
			this->eraserToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"eraserToolStripMenuItem.Image")));
			this->eraserToolStripMenuItem->Name = L"eraserToolStripMenuItem";
			this->eraserToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->eraserToolStripMenuItem->Text = L"Eraser";
			this->eraserToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::eraserToolStripMenuItem_Click);
			// 
			// colorToolStripMenuItem
			// 
			this->colorToolStripMenuItem->Name = L"colorToolStripMenuItem";
			this->colorToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->colorToolStripMenuItem->Text = L"Color";
			this->colorToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::colorToolStripMenuItem_Click);
			// 
			// effectsStripMenuItem1
			// 
			this->effectsStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->hueToolStripMenuItem,
					this->saturationToolStripMenuItem, this->brightnessToolStripMenuItem
			});
			this->effectsStripMenuItem1->Name = L"effectsStripMenuItem1";
			this->effectsStripMenuItem1->Size = System::Drawing::Size(54, 20);
			this->effectsStripMenuItem1->Text = L"Effects";
			// 
			// hueToolStripMenuItem
			// 
			this->hueToolStripMenuItem->CheckOnClick = true;
			this->hueToolStripMenuItem->Name = L"hueToolStripMenuItem";
			this->hueToolStripMenuItem->Size = System::Drawing::Size(129, 22);
			this->hueToolStripMenuItem->Text = L"Hue";
			this->hueToolStripMenuItem->CheckedChanged += gcnew System::EventHandler(this, &EditForm::hueToolStripMenuItem_CheckedChanged);
			this->hueToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::hueToolStripMenuItem_Click);
			// 
			// saturationToolStripMenuItem
			// 
			this->saturationToolStripMenuItem->CheckOnClick = true;
			this->saturationToolStripMenuItem->Name = L"saturationToolStripMenuItem";
			this->saturationToolStripMenuItem->Size = System::Drawing::Size(129, 22);
			this->saturationToolStripMenuItem->Text = L"Saturation";
			this->saturationToolStripMenuItem->CheckedChanged += gcnew System::EventHandler(this, &EditForm::saturationToolStripMenuItem_CheckedChanged);
			this->saturationToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::saturationToolStripMenuItem_Click);
			// 
			// brightnessToolStripMenuItem
			// 
			this->brightnessToolStripMenuItem->Name = L"brightnessToolStripMenuItem";
			this->brightnessToolStripMenuItem->Size = System::Drawing::Size(129, 22);
			this->brightnessToolStripMenuItem->Text = L"Brightness";
			this->brightnessToolStripMenuItem->CheckedChanged += gcnew System::EventHandler(this, &EditForm::brightnessToolStripMenuItem_CheckedChanged);
			this->brightnessToolStripMenuItem->Click += gcnew System::EventHandler(this, &EditForm::brightnessToolStripMenuItem_Click);
			// 
			// checkBoxFill
			// 
			this->checkBoxFill->AutoSize = true;
			this->checkBoxFill->Location = System::Drawing::Point(200, 3);
			this->checkBoxFill->Name = L"checkBoxFill";
			this->checkBoxFill->Size = System::Drawing::Size(50, 17);
			this->checkBoxFill->TabIndex = 3;
			this->checkBoxFill->Text = L"Filled";
			this->checkBoxFill->UseVisualStyleBackColor = true;
			this->checkBoxFill->Visible = false;
			// 
			// effectsNumUpDown
			// 
			this->effectsNumUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->effectsNumUpDown->Location = System::Drawing::Point(195, 3);
			this->effectsNumUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 300, 0, 0, 0 });
			this->effectsNumUpDown->Name = L"effectsNumUpDown";
			this->effectsNumUpDown->Size = System::Drawing::Size(54, 20);
			this->effectsNumUpDown->TabIndex = 4;
			this->effectsNumUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100, 0, 0, 0 });
			this->effectsNumUpDown->Visible = false;
			this->effectsNumUpDown->ValueChanged += gcnew System::EventHandler(this, &EditForm::effectsNumUpDown_ValueChanged);
			// 
			// effectResetBtn
			// 
			this->effectResetBtn->Location = System::Drawing::Point(290, 2);
			this->effectResetBtn->Name = L"effectResetBtn";
			this->effectResetBtn->Size = System::Drawing::Size(44, 21);
			this->effectResetBtn->TabIndex = 5;
			this->effectResetBtn->Text = L"Reset";
			this->effectResetBtn->UseVisualStyleBackColor = true;
			this->effectResetBtn->Visible = false;
			this->effectResetBtn->Click += gcnew System::EventHandler(this, &EditForm::effectResetBtn_Click);
			// 
			// hueTrackBar
			// 
			this->hueTrackBar->AutoSize = false;
			this->hueTrackBar->LargeChange = 10;
			this->hueTrackBar->Location = System::Drawing::Point(195, 2);
			this->hueTrackBar->Maximum = 360;
			this->hueTrackBar->Name = L"hueTrackBar";
			this->hueTrackBar->Size = System::Drawing::Size(84, 21);
			this->hueTrackBar->SmallChange = 10;
			this->hueTrackBar->TabIndex = 9;
			this->hueTrackBar->TickFrequency = 10;
			this->hueTrackBar->TickStyle = System::Windows::Forms::TickStyle::None;
			this->hueTrackBar->Visible = false;
			this->hueTrackBar->ValueChanged += gcnew System::EventHandler(this, &EditForm::hueTrackBar_ValueChanged);
			// 
			// picture
			// 
			this->picture->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->picture->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->picture->Location = System::Drawing::Point(0, 26);
			this->picture->Name = L"picture";
			this->picture->Size = System::Drawing::Size(385, 268);
			this->picture->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->picture->TabIndex = 10;
			this->picture->TabStop = false;
			this->picture->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &EditForm::picture_MouseDown);
			this->picture->MouseLeave += gcnew System::EventHandler(this, &EditForm::picture_MouseLeave);
			this->picture->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &EditForm::picture_MouseMove);
			this->picture->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &EditForm::picture_MouseUp);
			// 
			// EditForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(385, 292);
			this->Controls->Add(this->effectsNumUpDown);
			this->Controls->Add(this->picture);
			this->Controls->Add(this->hueTrackBar);
			this->Controls->Add(this->effectResetBtn);
			this->Controls->Add(this->checkBoxFill);
			this->Controls->Add(this->menuStrip1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"EditForm";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Show;
			this->Text = L"ImgEditor";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->effectsNumUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->hueTrackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picture))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenFileDialog^ ofd = gcnew OpenFileDialog();
				 ofd->Filter = "All Graphics Types|*.bmp;*.jpg;*.jpeg;*.png;*.tif;*.tiff|JPEG Files(*.jpg,*.jpeg) |*.jpg;*.jpeg|PNG Files(*.png) |*.png|BMP Files(*.bmp) |*.bmp|TIFF (*.tif,*.tiff)|*.tif;*.tiff";
				 ofd->RestoreDirectory = true;
				 if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
					 picture->Image = Image::FromFile(ofd->FileName);
					 if (picture->Image->Height <= 768 && picture->Image->Width <= 1366)
					 {
						 EditForm::Text = ofd->FileName + " - ImgEditor";
						 this->oldImage = (Image^)picture->Image->Clone();
					 }
					 else
					 {
						 picture->Image = (Image^)this->oldImage;
						 MessageBox::Show("Image resolution must be less than 1366 x 768.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					 }
					 if(saturationToolStripMenuItem->Checked || brightnessToolStripMenuItem->Checked)
						 effectsNumUpDown->Value = 100;
					 if (hueToolStripMenuItem->Checked)
						 hueTrackBar->Value = 0;
				 }
	}
	
	private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 SaveFileDialog^ sfd = gcnew SaveFileDialog();
				 sfd->Filter = "PNG Files(*.png) |*.png|JPEG Files(*.jpg,*jpeg) |*.jpg;*jpeg|BMP Files(*.bmp) |*.bmp|All files(*.*) |*.*";
				 sfd->AddExtension = true;
				 sfd->RestoreDirectory = true;
				 if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK){
					 picture->Image->Save(sfd->FileName);
					 EditForm::Text = sfd->FileName + " - ImgEditor";
				 }
	}

	private: System::Void newToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 ImageSize imgsz(picture->Image->Width, picture->Image->Height);
				 NewFileDialog^ newfile = gcnew NewFileDialog(&imgsz);
				 newfile->ShowDialog();
				 if (imgsz.valid){
					 delete picture->Image;
					 InitializePicture(imgsz.width, imgsz.height);
					 EditForm::Text = "Unnamed - ImgEditor";
					 this->oldImage = (Image^)picture->Image->Clone();
					 if (saturationToolStripMenuItem->Checked || brightnessToolStripMenuItem->Checked)
						 effectsNumUpDown->Value = 100;
					 if (hueToolStripMenuItem->Checked)
						 hueTrackBar->Value = 0;
				 }
	}

	private: System::Void penToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 UncheckTools();
				 penToolStripMenuItem->Checked = true;
				 this->currentTool = TTPen;
	}

	private: System::Void eraserToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 UncheckTools();
				 eraserToolStripMenuItem->Checked = true;
				 this->currentTool = TTEraser;
	}

	private: System::Void rectangleToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 UncheckTools();
				 rectangleToolStripMenuItem->Checked = true;
				 checkBoxFill->Visible = true;
				 this->currentTool = TTRectangle;

	}
	private: System::Void ellipseToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 UncheckTools();
				 ellipseToolStripMenuItem->Checked = true;
				 checkBoxFill->Visible = true;
				 this->currentTool = TTEllipse;

	}

	private: System::Void fillToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 UncheckTools();
				 fillToolStripMenuItem->Checked = true;
				 this->currentTool = TTFill;
	}
	
	private: System::Void hueToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
					UncheckTools();
					hueToolStripMenuItem->Checked = true;
					effectResetBtn->Visible = true;
					hueTrackBar->Visible = true;
					effectResetBtn->Location = Drawing::Point(hueTrackBar->Location.X + 90, effectResetBtn->Location.Y);
					this->currentTool = TTHue;
					this->oldImage = (Image^)picture->Image->Clone();
	}

	private: System::Void saturationToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 UncheckTools();
				 saturationToolStripMenuItem->Checked = true;
				 effectsNumUpDown->Value = 100;
				 effectResetBtn->Location = Drawing::Point(effectsNumUpDown->Location.X + 60, effectResetBtn->Location.Y);
				 effectsNumUpDown->Visible = true;
				 effectResetBtn->Visible = true;
				 this->currentTool = TTSaturation;
				 this->oldImage = (Image^)picture->Image->Clone();

	}
	
	private: System::Void brightnessToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 UncheckTools();
				 brightnessToolStripMenuItem->Checked = true;
				 effectResetBtn->Location.X = effectsNumUpDown->Location.X + 100;
				 effectResetBtn->Location = Drawing::Point(effectsNumUpDown->Location.X + 60, effectResetBtn->Location.Y);
				 effectResetBtn->Visible = true;
				 effectsNumUpDown->Visible = true;
				 this->currentTool = TTBrightness;
				 this->oldImage = (Image^)picture->Image->Clone();
	}
	
	private: System::Void picture_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
				 lastValid = false;
	}
	
	private: System::Void picture_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 switch (this->currentTool)
				 {
				 case TTPen:
					 if (e->Button == System::Windows::Forms::MouseButtons::Left && lastValid == true){
						 graphics = Graphics::FromImage(picture->Image);
						 Pen^ pen = gcnew Pen(currentColor, currentSize);
						 SolidBrush^ brush = gcnew SolidBrush(currentColor);
						 graphics->DrawLine(pen, lastX, lastY, e->X, e->Y);
						 graphics->FillEllipse(brush, e->X-(currentSize/2), e->Y-(currentSize/2), currentSize, currentSize);
						 this->picture->Refresh();
					 }
					 lastX = e->X;
					 lastY = e->Y;
					 lastValid = true;
					 break;
				 case TTEraser:
					 if (e->Button == System::Windows::Forms::MouseButtons::Left && lastValid == true){
						 graphics = Graphics::FromImage(picture->Image);
						 Pen^ pen = gcnew Pen(Color(Color::White), currentSize);
						 SolidBrush^ brush = gcnew SolidBrush(Color(Color::White));
						 graphics->DrawLine(pen, lastX, lastY, e->X, e->Y);
						 graphics->FillEllipse(brush, e->X - (currentSize / 2), e->Y - (currentSize / 2), currentSize, currentSize);
						 this->picture->Refresh();
					 }
					 lastX = e->X;
					 lastY = e->Y;
					 lastValid = true;
					 break;
				 case TTRectangle:
					 if (e->Button == System::Windows::Forms::MouseButtons::Left){
						 picture->Image = (Image^)this->oldImage->Clone();
						 graphics = Graphics::FromImage(picture->Image);
						 w = e->X - lastX; h = e->Y - lastY;
						 rx = lastX + (w<0?w:0); 
						 ry = lastY + (h<0?h:0);
						 if (checkBoxFill->Checked == false){
							 Pen^ pen = gcnew Pen(currentColor, currentSize);
							 graphics->DrawRectangle(pen, rx, ry, abs(w), abs(h));
						 }
						 else{
							 SolidBrush^ brush = gcnew SolidBrush(currentColor);
							 graphics->FillRectangle(brush, rx, ry, abs(w), abs(h));
							
						 }
						 this->picture->Refresh();
					 }
					 break;
				 case TTEllipse:
					 if (e->Button == System::Windows::Forms::MouseButtons::Left){
						 picture->Image = (Image^)this->oldImage->Clone();
						 graphics = Graphics::FromImage(picture->Image);
						 Pen^ pen = gcnew Pen(currentColor, currentSize);
						 w = e->X - lastX; h = e->Y - lastY;
						 rx = lastX + (w<0 ? w : 0);
						 ry = lastY + (h<0 ? h : 0);
						 if (checkBoxFill->Checked == false){
							 Pen^ pen = gcnew Pen(currentColor, currentSize);
							 graphics->DrawEllipse(pen, rx, ry, abs(w), abs(h));
						 }
						 else{
							 SolidBrush^ brush = gcnew SolidBrush(currentColor);
							 graphics->FillEllipse(brush, rx, ry, abs(w), abs(h));
						 }
						 this->picture->Refresh();
					 }
					 break;
				 }
	}

	private: System::Void picture_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 switch (this->currentTool)
				 {
				 case TTPen:
				 {
					graphics = Graphics::FromImage(picture->Image);
					SolidBrush^ brush = gcnew SolidBrush(currentColor);
					graphics->FillEllipse(brush, e->X - (currentSize / 2), e->Y - (currentSize / 2), currentSize, currentSize);
					this->picture->Refresh();
				 }
					 break;
				 case TTRectangle:
					 this->oldImage = (Image^)picture->Image->Clone();
					 lastX = e->X;
					 lastY = e->Y;
					 break;
				 case TTEllipse:
					 this->oldImage = (Image^)picture->Image->Clone();
					 lastX = e->X;
					 lastY = e->Y;
					 break;
				 case TTFill:
					 Fill(e->X, e->Y, currentColor);
					 break;
				 }
	}

	private: System::Void picture_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 switch (this->currentTool)
				 {
				 case TTRectangle:
					 this->oldImage = (Image^)picture->Image->Clone();
					 break;
				 case TTEllipse:
					 this->oldImage = (Image^)picture->Image->Clone();
					 break;
				 }
	}

	private: System::Void colorToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 ColorDialog^ cd = gcnew ColorDialog;
				 cd->AllowFullOpen = true;
				 cd->ShowHelp = true;
				 cd->Color = currentColor;
				 if (cd->ShowDialog() == ::System::Windows::Forms::DialogResult::OK)
					 currentColor = cd->Color;
	}
	

	private: System::Void effectsNumUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) { 
				 if (currentTool == TTSaturation)
					SaturationChange(System::Convert::ToSingle(effectsNumUpDown->Value));
				 if (currentTool == TTBrightness)
					BrightnessChange(System::Convert::ToSingle(effectsNumUpDown->Value));
	}
	
	private: System::Void effectResetBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (saturationToolStripMenuItem->Checked || brightnessToolStripMenuItem->Checked)
					effectsNumUpDown->Value = 100;
				 if (hueToolStripMenuItem->Checked)
					 hueTrackBar->Value = 0;
				 picture->Image = oldImage;
	}
	
	private: System::Void saturationToolStripMenuItem_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 oldImage = picture->Image;
	}

	private: System::Void toolStripComboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 currentSize = System::Convert::ToSingle(toolStripComboBox1->SelectedItem->ToString());
	}

	private: System::Void hueToolStripMenuItem_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 oldImage = picture->Image;
	}

	private: System::Void hueTrackBar_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 HueChange(Convert::ToSingle(hueTrackBar->Value));
	}

	private: System::Void brightnessToolStripMenuItem_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 oldImage = picture->Image;
	}

	

	//Panel не работает должным образом (соответственно изображени€ высокого разрешени€ использовать не выйдет из-за того, что требуютс€ scrollbars, которые идут вместе с элементом Panel), т.к. Picturebox внутри Panel (контроль Panel над Picturebox установлен) перекрывает элемент Panel. ¬ результате такого перекрыти€ невозможно ни обратитьс€ к Picturebox (что логично, ведь Panel находитс€ на переднем плане), ни к Panel, лежащей поверх Picturebox (однако вне области Picturebox, лежащего под Panel, Panel нормально отзываетс€ на клики/движение мыши на нЄм).
/*private: System::Void panel1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 picture_MouseUp(sender, gcnew System::Windows::Forms::MouseEventArgs(e->Button, e->Clicks, e->X + panel1->HorizontalScroll->Value, e->Y + panel1->VerticalScroll->Value, e->Delta));
}
private: System::Void panel1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 picture_MouseMove(sender, gcnew System::Windows::Forms::MouseEventArgs(e->Button, e->Clicks, e->X + panel1->HorizontalScroll->Value, e->Y + panel1->VerticalScroll->Value, e->Delta));
}
private: System::Void panel1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 picture_MouseDown(sender, gcnew System::Windows::Forms::MouseEventArgs(e->Button, e->Clicks, e->X + panel1->HorizontalScroll->Value, e->Y + panel1->VerticalScroll->Value, e->Delta));
}*/

};
}
