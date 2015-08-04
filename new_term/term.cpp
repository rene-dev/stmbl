#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "mathplot.h"

#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/print.h>
#include <wx/defs.h>

#include <math.h>
// #include <time.h>

class mpScopeWave : public mpFX{
private:
   enum{
      FLOATING,
      ROLLING
   } mode;

   std::vector<double> m_ys;
   double m_minY, m_maxY;

   float sampleFreq;

   float sampleTime;

   int buffer_pos; // current write pos in data buffer
   int start_pos; // current start pos in data buffer

   int min_index;
   int max_index;


   void updateMinMaxY(){
      for(int i = 0; i < m_ys.size(); i++){
         if(m_ys[i] < m_minY){
            m_minY = m_ys[i];
            min_index = i;
         }
         if(m_ys[i] > m_maxY){
            m_maxY = m_ys[i];
            max_index = i;
         }
      }
   }

public:
   mpScopeWave(wxString name = wxEmptyString, int flags = mpALIGN_NE) : mpFX(name, flags){
      sampleFreq = 1.0;
      sampleTime = 1.0;
      mode = FLOATING;
      buffer_pos = 0;
      start_pos = 0;
      m_ys.push_back(0.0);
      //createTimeVector();
   }

   double GetY(double  x){
      if(x >= -sampleTime && x <= 0.0){
         return(m_ys[(int)((x + sampleTime) * sampleFreq + start_pos) % m_ys.size()]);
      }
      else{
         return(0.0);
      }
   }

   double GetMinX(){
      return(-sampleTime);
   }

   double GetMaxX(){
      return(0.0);
   }

   void AddData(float y){
      buffer_pos++;
      buffer_pos %= m_ys.size();

      if(mode == FLOATING){
         start_pos = buffer_pos;
      }

      m_ys[buffer_pos] = y;

      if(buffer_pos == min_index || buffer_pos == max_index){
         updateMinMaxY();
      }
      else{
         if(y < m_minY){
            m_minY = y;
            min_index = buffer_pos;
         }
         if(y > m_maxY){
            m_maxY = y;
            max_index = buffer_pos;
         }
      }
   }

   void setFloating(){
      mode = FLOATING;

   }
   void setRolling(){
      mode = ROLLING;
   }

   void setSampleFreq(float f){
      sampleFreq = fmax(f, 0.0);
      int l = fmax(sampleTime * sampleFreq, 1.0);
      if(m_ys.size() > l){
         min_index -= m_ys.size() - l;
         max_index -= m_ys.size() - l;
         m_ys.erase(m_ys.begin(), m_ys.begin() + (m_ys.size() - l));

         buffer_pos = fmin(buffer_pos, m_ys.size());
         start_pos = fmin(start_pos, m_ys.size());

         if(min_index < 0 || max_index < 0){
            updateMinMaxY();
         }
      }
      else if(m_ys.size() < l){
         int j = l - m_ys.size();
         for(int i = 0; i < j; i++){
            m_ys.push_back((m_maxY + m_minY) / 2.0);
         }
      }
   }

   void setSampleTime(float t){
      sampleTime = fmax(t, 0.0);
      int l = fmax(sampleTime * sampleFreq, 1.0);
      if(m_ys.size() > l){
         min_index -= m_ys.size() - l;
         max_index -= m_ys.size() - l;
         m_ys.erase(m_ys.begin(), m_ys.begin() + (m_ys.size() - l));

         buffer_pos = fmin(buffer_pos, m_ys.size());
         start_pos = fmin(start_pos, m_ys.size());

         if(min_index < 0 || max_index < 0){
            updateMinMaxY();
         }
      }
      else if(m_ys.size() < l){
         int j = l - m_ys.size();
         for(int i = 0; i < j; i++){
            m_ys.push_back((m_maxY + m_minY) / 2.0);
         }
      }
   }
};

class MyFrame;
class MyApp;

class MyFrame: public wxFrame
{
public:
   MyFrame();

   void OnAbout( wxCommandEvent &event );
   void OnQuit( wxCommandEvent &event );
   void OnFit( wxCommandEvent &event );
   void OnTimer(wxTimerEvent& event);
   void OnRefresh(wxCommandEvent& event);
   void OnConnect(wxCommandEvent& event);
   void OnDisconnect(wxCommandEvent& event);
   void OnRB(wxCommandEvent& event);

   void AddWave(wxString name);

   mpWindow        *m_plot;
   wxTextCtrl      *m_log;
   wxTextCtrl      *m_cli;
   wxTimer         *m_Timer;
   wxRadioButton   *rb_floating;
   wxRadioButton   *rb_rolling;
   wxTextCtrl      *tc_sampleFreq;
   wxTextCtrl      *tc_sampleTime;


   wxBoxSizer *bs_mainVSizer;
   wxBoxSizer *bs_topHSizer;
   wxBoxSizer *bs_midHSizer;
   wxBoxSizer *bs_midVSizer;

   float sampleTime;
   float sampleFreq;

private:
   int axesPos[2];
   bool ticks;

   struct wave_t{
      // wxRadioButton *rb_ac;
      // wxRadioButton *rb_dc;
      wxComboBox *cb_channel;
      wxTextCtrl *tc_gain;
      wxTextCtrl *tc_offset;
      // wxBoxSizer *bs_vsizer;
      // wxBoxSizer *bs_hlsizer;
      wxBoxSizer *bs_sizer;

      mpScopeWave *data;
   };

   std::vector<wave_t> waves;

   DECLARE_DYNAMIC_CLASS(MyFrame)
   DECLARE_EVENT_TABLE()
};

// MyApp

class MyApp: public wxApp
{
public:
   virtual bool OnInit();
};

// main program

IMPLEMENT_APP(MyApp)

// MyFrame

enum {
   ID_QUIT  = 108,
   ID_ABOUT,
   ID_PRINT,
   ID_PRINT_PREVIEW,
   ID_ALIGN_X_AXIS,
   ID_ALIGN_Y_AXIS,
   ID_TOGGLE_GRID,
   TIMER_ID,
   RB_ID
};

IMPLEMENT_DYNAMIC_CLASS( MyFrame, wxFrame )

BEGIN_EVENT_TABLE(MyFrame,wxFrame)
EVT_MENU(ID_ABOUT, MyFrame::OnAbout)
EVT_MENU(ID_QUIT,  MyFrame::OnQuit)
EVT_MENU(mpID_FIT, MyFrame::OnFit)
EVT_RADIOBUTTON(RB_ID, MyFrame::OnRB)
// EVT_MENU(mpID_Refresh, MyFrame::OnRefresh)
// EVT_MENU(mpID_Connect, MyFrame::OnConnect)
// EVT_MENU(mpID_Disconnect, MyFrame::OnDisconnect)
EVT_TIMER(TIMER_ID, MyFrame::OnTimer)
END_EVENT_TABLE()

void MyFrame::AddWave(wxString name){
   struct wave_t wave;
   // wave.rb_ac = new wxRadioButton(this, RB_ID, "ac", wxPoint(0,0), wxDefaultSize, wxRB_GROUP);
   // wave.rb_dc = new wxRadioButton(this, RB_ID, "dc", wxPoint(0,0), wxDefaultSize);
   wave.cb_channel = new wxComboBox (this, -1, "signal", wxDefaultPosition, wxDefaultSize, 0, NULL, wxTE_PROCESS_ENTER);
   wave.tc_gain = new wxTextCtrl( this, -1, "gain", wxDefaultPosition, wxDefaultSize);
   wave.tc_offset = new wxTextCtrl( this, -1, "offset", wxDefaultPosition, wxDefaultSize);
   wave.data = new mpScopeWave(name);
   wave.data->SetPen( wxPen(wxColour(128*cos(waves.size()) + 128, 128*cos(waves.size() + 1) + 128, 128*cos(waves.size() + 2) + 128), 1, wxPENSTYLE_SOLID));
   wave.data->setSampleTime(sampleTime);
   wave.data->SetContinuity(true);
   wave.data->setSampleFreq(sampleFreq);
   // wave.bs_vsizer = new wxBoxSizer(wxHORIZONTAL);
   // wave.bs_hlsizer = new wxBoxSizer(wxVERTICAL);
   wave.bs_sizer = new wxBoxSizer(wxVERTICAL);

   // wave.bs_vsizer->Add(wave.bs_hlsizer, 0, wxEXPAND);
   // wave.bs_vsizer->Add(wave.bs_hrsizer, 0, wxEXPAND);


   m_plot->AddLayer(wave.data);
   // wave.bs_hlsizer->Add(wave.rb_ac, 0, wxEXPAND);
   // wave.bs_hlsizer->Add(wave.rb_dc, 0, wxEXPAND);
   wave.bs_sizer->Add(wave.cb_channel, 0, wxEXPAND);
   wave.bs_sizer->Add(wave.tc_gain, 0, wxEXPAND);
   wave.bs_sizer->Add(wave.tc_offset, 0, wxEXPAND);
   bs_midVSizer->Add(wave.bs_sizer, 0, wxEXPAND);
   waves.push_back(wave);
}

MyFrame::MyFrame() : wxFrame( (wxFrame *)NULL, -1, "Servoterm", wxDefaultPosition, wxSize(500,500) ){
   sampleTime = 10.0;
   sampleFreq = 5000;

   wxMenu *file_menu = new wxMenu();
   wxMenu *view_menu = new wxMenu();

   file_menu->Append( ID_ABOUT, "&About...");
   file_menu->Append( ID_QUIT,  "E&xit\tAlt-X");
   //  file_menu->Append( mpID_Refresh,  "Refresh\tCtrl-R");
   //  file_menu->Append( mpID_Connect,  "Connect\tCtrl-C");
   //  file_menu->Append( mpID_Disconnect,  "Disconnect\tCtrl-D");

   view_menu->Append( mpID_FIT,      "&Fit bounding box", "Set plot view to show all items");
   view_menu->Append( mpID_ZOOM_IN,  "Zoom in",           "Zoom in plot view.");
   view_menu->Append( mpID_ZOOM_OUT, "Zoom out",          "Zoom out plot view.");

   wxMenuBar *menu_bar = new wxMenuBar();
   menu_bar->Append(file_menu, "&File");
   menu_bar->Append(view_menu, "&View");

   SetMenuBar( menu_bar );
   CreateStatusBar(1);

   m_plot = new mpWindow( this, -1, wxPoint(0,0), wxSize(100,100), wxSUNKEN_BORDER );
   m_plot->SetMargins(0,0,40,40);
   mpScaleX* xaxis = new mpScaleX("t", mpALIGN_BOTTOM, true);
   mpScaleY* yaxis = new mpScaleY("y", mpALIGN_LEFT, true);
   xaxis->SetDrawOutsideMargins(false);
   yaxis->SetDrawOutsideMargins(false);
   m_plot->AddLayer(xaxis);
   m_plot->AddLayer(yaxis);

   m_log = new wxTextCtrl( this, -1, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
   m_cli = new wxTextCtrl( this, -1, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

   rb_floating = new wxRadioButton(this, RB_ID, "floating", wxPoint(0,0), wxDefaultSize, wxRB_GROUP);
   rb_rolling = new wxRadioButton(this, RB_ID, "rolling", wxPoint(0,0), wxDefaultSize);
   tc_sampleFreq = new wxTextCtrl( this, -1, "sampleFreq", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
   tc_sampleTime = new wxTextCtrl( this, -1, "sampleTime", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);


   bs_mainVSizer = new wxBoxSizer(wxVERTICAL);
   bs_topHSizer = new wxBoxSizer(wxHORIZONTAL);
   bs_midHSizer = new wxBoxSizer(wxHORIZONTAL);
   bs_midVSizer = new wxBoxSizer(wxVERTICAL);

   bs_mainVSizer->Add(bs_topHSizer, 0, wxEXPAND);
      bs_topHSizer->Add(rb_floating, 0, wxEXPAND);
      bs_topHSizer->Add(rb_rolling, 0, wxEXPAND);
      bs_topHSizer->Add(tc_sampleFreq, 0, wxEXPAND);
      bs_topHSizer->Add(tc_sampleTime, 0, wxEXPAND);
   bs_mainVSizer->Add(bs_midHSizer, 2, wxEXPAND);
      bs_midHSizer->Add(m_plot, 1, wxEXPAND);
      bs_midHSizer->Add(bs_midVSizer, 0, wxEXPAND);
   bs_mainVSizer->Add(m_log, 1, wxEXPAND);
   bs_mainVSizer->Add(m_cli, 0, wxEXPAND);

   AddWave("wave0");
   AddWave("wave1");


   SetAutoLayout( TRUE );
   SetSizer(bs_mainVSizer);
   axesPos[0] = 0;
   axesPos[1] = 0;
   ticks = true;


   m_plot->EnableDoubleBuffer(true);
   m_plot->LockAspect( false );
   m_plot->Fit();

   m_Timer = new wxTimer(this,TIMER_ID);
   m_Timer->Start( 20 );
}

void MyFrame::OnQuit( wxCommandEvent &WXUNUSED(event) )
{
   Close( TRUE );
}

void MyFrame::OnFit( wxCommandEvent &WXUNUSED(event) )
{
   m_plot->Fit();
}

void MyFrame::OnAbout( wxCommandEvent &WXUNUSED(event) )
{
   wxMessageBox( "STMBL Term");
}

void MyFrame::OnRB(wxCommandEvent& event){
   if(rb_rolling->GetValue()){
      for(int i = 0; i < waves.size(); i++){
         waves[i].data->setRolling();
      }
   }
   else{
      for(int i = 0; i < waves.size(); i++){
         waves[i].data->setFloating();
      }
   }
}

void MyFrame::OnTimer(wxTimerEvent& event)
{
   static float time = 0.0;
   for(int i = 0; i < 100; i++){
      for(int j = 0; j < waves.size(); j++){
         waves[j].data->AddData(sin(time + j));
      }
      time += m_Timer->GetInterval()/1000.0/100.0*1.0;
   }
   m_plot->UpdateAll();
}

//-----------------------------------------------------------------------------
// MyApp
//-----------------------------------------------------------------------------

bool MyApp::OnInit()
{
   wxFrame *frame = new MyFrame();
   frame->Show( TRUE );

   return TRUE;
}
