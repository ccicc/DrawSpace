/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2017                        
*                                                                          
* This file is part of DrawSpace.                                          
*                                                                          
*    DrawSpace is free software: you can redistribute it and/or modify     
*    it under the terms of the GNU General Public License as published by  
*    the Free Software Foundation, either version 3 of the License, or     
*    (at your option) any later version.                                   
*                                                                          
*    DrawSpace is distributed in the hope that it will be useful,          
*    but WITHOUT ANY WARRANTY; without even the implied warranty of        
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
*    GNU General Public License for more details.                          
*                                                                          
*    You should have received a copy of the GNU General Public License     
*    along with DrawSpace.  If not, see <http://www.gnu.org/licenses/>.    
*
*/

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "panel.h"

///////////////////////////////////////////////////////////////////////////

ObjectPropertiesDialog::ObjectPropertiesDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_propertyGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer12->Add( m_propertyGrid, 5, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	m_close_button = new wxButton( this, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_close_button, 0, wxALL, 5 );
	
	m_apply_button = new wxButton( this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_apply_button, 0, wxALL, 5 );
	
	m_specific_button_0 = new wxButton( this, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_specific_button_0, 0, wxALL, 5 );
	
	m_specific_button_1 = new wxButton( this, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_specific_button_1, 0, wxALL, 5 );
	
	m_specific_button_2 = new wxButton( this, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_specific_button_2, 0, wxALL, 5 );
	
	
	bSizer12->Add( bSizer13, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer12 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_close_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectPropertiesDialog::OnCloseButtonClicked ), NULL, this );
	m_apply_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectPropertiesDialog::OnApplyButtonClicked ), NULL, this );
	m_specific_button_0->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectPropertiesDialog::OnSpecificButton0Clicked ), NULL, this );
	m_specific_button_1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectPropertiesDialog::OnSpecificButton1Clicked ), NULL, this );
	m_specific_button_2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectPropertiesDialog::OnSpecificButton2Clicked ), NULL, this );
}

ObjectPropertiesDialog::~ObjectPropertiesDialog()
{
	// Disconnect Events
	m_close_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectPropertiesDialog::OnCloseButtonClicked ), NULL, this );
	m_apply_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectPropertiesDialog::OnApplyButtonClicked ), NULL, this );
	m_specific_button_0->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectPropertiesDialog::OnSpecificButton0Clicked ), NULL, this );
	m_specific_button_1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectPropertiesDialog::OnSpecificButton1Clicked ), NULL, this );
	m_specific_button_2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectPropertiesDialog::OnSpecificButton2Clicked ), NULL, this );
	
}

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer28->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	m_consoleinput_textCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 760,120 ), wxTE_MULTILINE );
	m_consoleinput_textCtrl->SetFont( wxFont( 10, 74, 90, 92, false, wxT("System") ) );
	m_consoleinput_textCtrl->SetForegroundColour( wxColour( 255, 255, 0 ) );
	m_consoleinput_textCtrl->SetBackgroundColour( wxColour( 0, 0, 255 ) );
	
	bSizer31->Add( m_consoleinput_textCtrl, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxVERTICAL );
	
	m_consoleinsend_button = new wxButton( this, wxID_ANY, wxT("Send"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer32->Add( m_consoleinsend_button, 0, wxALL, 5 );
	
	m_consoleinclear_button = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer32->Add( m_consoleinclear_button, 0, wxALL, 5 );
	
	m_consoleinload_button = new wxButton( this, wxID_ANY, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer32->Add( m_consoleinload_button, 0, wxALL, 5 );
	
	
	bSizer31->Add( bSizer32, 1, wxEXPAND, 5 );
	
	
	bSizer28->Add( bSizer31, 0, wxEXPAND, 5 );
	
	
	bSizer25->Add( bSizer28, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_resourcesPanel = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_assets_listCtrl = new wxListCtrl( m_resourcesPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL );
	bSizer9->Add( m_assets_listCtrl, 1, 0, 5 );
	
	m_configs_listCtrl = new wxListCtrl( m_resourcesPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL );
	bSizer9->Add( m_configs_listCtrl, 1, 0, 5 );
	
	
	m_resourcesPanel->SetSizer( bSizer9 );
	m_resourcesPanel->Layout();
	bSizer9->Fit( m_resourcesPanel );
	m_notebook2->AddPage( m_resourcesPanel, wxT("Resources"), true );
	m_passesPanel = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_passes_listCtrl = new wxListCtrl( m_passesPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL );
	bSizer7->Add( m_passes_listCtrl, 1, 0, 5 );
	
	m_shadersparams_listCtrl = new wxListCtrl( m_passesPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL );
	bSizer7->Add( m_shadersparams_listCtrl, 1, 0, 5 );
	
	
	m_passesPanel->SetSizer( bSizer7 );
	m_passesPanel->Layout();
	bSizer7->Fit( m_passesPanel );
	m_notebook2->AddPage( m_passesPanel, wxT("Rendering passes"), false );
	m_scenePanel = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer291;
	bSizer291 = new wxBoxSizer( wxHORIZONTAL );
	
	m_scenegraphs_treeCtrl = new wxTreeCtrl( m_scenePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer291->Add( m_scenegraphs_treeCtrl, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer11->Add( bSizer291, 1, wxEXPAND, 5 );
	
	
	m_scenePanel->SetSizer( bSizer11 );
	m_scenePanel->Layout();
	bSizer11->Fit( m_scenePanel );
	m_notebook2->AddPage( m_scenePanel, wxT("Scene"), false );
	
	bSizer33->Add( m_notebook2, 1, wxEXPAND, 0 );
	
	m_consoleoutput_textCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300,170 ), wxTE_MULTILINE|wxTE_READONLY );
	m_consoleoutput_textCtrl->SetFont( wxFont( 6, 75, 90, 90, false, wxT("Terminal") ) );
	
	bSizer33->Add( m_consoleoutput_textCtrl, 0, wxALL|wxEXPAND, 0 );
	
	
	bSizer25->Add( bSizer33, 0, wxEXPAND, 5 );
	
	
	bSizer8->Add( bSizer25, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer8 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( MainFrame::OnIdle ) );
	this->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainFrame::OnMouseLeftDown ) );
	this->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( MainFrame::OnMouseLeftUp ) );
	this->Connect( wxEVT_MOTION, wxMouseEventHandler( MainFrame::OnMouseMotion ) );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( MainFrame::OnPaint ) );
	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( MainFrame::OnMouseRightDown ) );
	this->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( MainFrame::OnMouseRightUp ) );
	m_consoleinsend_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnConsoleInSendButtonClicked ), NULL, this );
	m_consoleinclear_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnConsoleInClearButtonClicked ), NULL, this );
	m_consoleinload_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnConsoleInLoadButtonClicked ), NULL, this );
	m_assets_listCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::OnAssetsListItemActivated ), NULL, this );
	m_configs_listCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::OnConfigsListItemActivated ), NULL, this );
	m_passes_listCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::OnPassesListItemActivated ), NULL, this );
	m_passes_listCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( MainFrame::OnPassesListItemSelected ), NULL, this );
	m_shadersparams_listCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::OnShadersListItemActivated ), NULL, this );
	m_scenegraphs_treeCtrl->Connect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( MainFrame::OnSceneNodeGraphsListRightClick ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainFrame::OnIdle ) );
	this->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainFrame::OnMouseLeftDown ) );
	this->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( MainFrame::OnMouseLeftUp ) );
	this->Disconnect( wxEVT_MOTION, wxMouseEventHandler( MainFrame::OnMouseMotion ) );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MainFrame::OnPaint ) );
	this->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( MainFrame::OnMouseRightDown ) );
	this->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( MainFrame::OnMouseRightUp ) );
	m_consoleinsend_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnConsoleInSendButtonClicked ), NULL, this );
	m_consoleinclear_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnConsoleInClearButtonClicked ), NULL, this );
	m_consoleinload_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnConsoleInLoadButtonClicked ), NULL, this );
	m_assets_listCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::OnAssetsListItemActivated ), NULL, this );
	m_configs_listCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::OnConfigsListItemActivated ), NULL, this );
	m_passes_listCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::OnPassesListItemActivated ), NULL, this );
	m_passes_listCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( MainFrame::OnPassesListItemSelected ), NULL, this );
	m_shadersparams_listCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::OnShadersListItemActivated ), NULL, this );
	m_scenegraphs_treeCtrl->Disconnect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( MainFrame::OnSceneNodeGraphsListRightClick ), NULL, this );
	
}

ScriptEditFrame::ScriptEditFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	m_script_textCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_MULTILINE );
	m_script_textCtrl->SetFont( wxFont( 10, 74, 90, 92, false, wxT("System") ) );
	m_script_textCtrl->SetForegroundColour( wxColour( 255, 255, 0 ) );
	m_script_textCtrl->SetBackgroundColour( wxColour( 0, 0, 255 ) );
	
	bSizer17->Add( m_script_textCtrl, 15, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	m_close_button = new wxButton( this, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_close_button, 0, wxALL, 5 );
	
	m_apply_button = new wxButton( this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_apply_button, 0, wxALL, 5 );
	
	m_load_button = new wxButton( this, wxID_ANY, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_load_button, 0, wxALL, 5 );
	
	m_clear_button = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_clear_button, 0, wxALL, 5 );
	
	
	bSizer18->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_activation_checkBox = new wxCheckBox( this, wxID_ANY, wxT("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_activation_checkBox, 0, wxEXPAND, 5 );
	
	
	bSizer17->Add( bSizer18, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer17 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_close_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditFrame::OnCloseButtonClicked ), NULL, this );
	m_apply_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditFrame::OnApplyButtonClicked ), NULL, this );
	m_load_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditFrame::OnLoadButtonClicked ), NULL, this );
	m_clear_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditFrame::OnClearButtonClicked ), NULL, this );
}

ScriptEditFrame::~ScriptEditFrame()
{
	// Disconnect Events
	m_close_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditFrame::OnCloseButtonClicked ), NULL, this );
	m_apply_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditFrame::OnApplyButtonClicked ), NULL, this );
	m_load_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditFrame::OnLoadButtonClicked ), NULL, this );
	m_clear_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditFrame::OnClearButtonClicked ), NULL, this );
	
}
