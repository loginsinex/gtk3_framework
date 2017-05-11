
#include "../h/std.h"


CControl::CControl(DWORD ctlId)
{
    m_pParent = NULL;
    m_hCtl = NULL;
    m_hContainer = NULL;
    m_ctlId = ( ctlId & 0xFFFFL );
}

CControl::~CControl()
{
    if ( m_hCtl )
        gtk_widget_destroy(m_hCtl);

    m_hCtl = NULL;
}

VOID CControl::Created(HCTL hCtl)
{
    m_hCtl = hCtl;
    g_object_set_data(G_OBJECT(hCtl), TEXT("this-ptr"), this);
    Margin(5);
}

VOID CControl::CreatedContainer(HCTL hContainer)
{
    m_hContainer = hContainer;
}

VOID CControl::Show(BOOL fVisible)
{
    if ( fVisible )
        gtk_widget_show_now(m_hCtl);
    else
        gtk_widget_hide(m_hCtl);
}

VOID CControl::Enable(BOOL fEnable)
{
    gtk_widget_set_sensitive(m_hCtl, fEnable);
}

BOOL CControl::Show()
{
    return gtk_widget_get_visible(m_hCtl);
}

BOOL CControl::Enable()
{
    return gtk_widget_is_sensitive(m_hCtl);
}

VOID CControl::Margin(UINT width)
{
    GValue val = { 0 };
    g_value_init(&val, G_TYPE_INT);
    g_value_set_int(&val, width);
    g_object_set_property((GObject*) ( m_hContainer ? m_hContainer : m_hCtl ), "margin", &val);
    g_value_unset(&val);
}

VOID CControl::Size(UINT cx, UINT cy)
{
#ifdef GTK2
    gtk_widget_set_usize(( m_hContainer ? m_hContainer : m_hCtl ), cx, cy);
#endif
#ifdef GTK3
    gtk_widget_set_size_request(( m_hContainer ? m_hContainer : m_hCtl ), cx, cy);
#endif
}

VOID CControl::SetFocus()
{
    gtk_widget_grab_focus(m_hCtl);
}

CWindow * CControl::Parent()
{
    return m_pParent;
}

VOID CControl::Parent(CWindow * pWindow)
{
    HWND hParent = pWindow->GetWindowHandle();
    gtk_container_remove(GTK_CONTAINER(hParent), m_hCtl);
    m_pParent = NULL;
    gtk_container_add(GTK_CONTAINER(hParent), m_hCtl);
    m_pParent = pWindow;
}

VOID CControl::Container(CWindow * pWindow, BOOL fInsert)
{
    if ( m_pParent )
        return;

    HWND hWnd = pWindow->GetWindowHandle();

    if ( fInsert )
        gtk_container_add(GTK_CONTAINER(hWnd), (m_hContainer ? m_hContainer : m_hCtl) );

    m_pParent = pWindow;
    InitSignals();
}

DWORD CControl::CtlId()
{
    return m_ctlId;
}

HCTL CControl::Ctl()
{
    return m_hCtl;
}

HCTL CControl::Container()
{
    return m_hContainer;
}

VOID CControl::InitSignals()
{

}

// ---- BUTTON ----

void CButtonHandlers::gtk_button_onclick(HCTL hButton, gpointer data)
{
    CButton * This = (CButton*) g_object_get_data(G_OBJECT(hButton), TEXT("this-ptr"));

    CWindow::WndProc((CWindow*) data, hButton, WM_COMMAND, This->CtlId(), 0);
}

CButton::CButton(DWORD ctlId) : CControl(ctlId)
{
    m_uButtonType = CTL_BUTTON_PUSH;
    Created(gtk_button_new());
}

CButton::CButton(DWORD ctlId, LPCTSTR pszText) : CControl(ctlId)
{
    m_uButtonType = CTL_BUTTON_PUSH;
    Created(gtk_button_new_with_label(pszText));
}

CButton::CButton(DWORD ctlId, LPCTSTR pszText, HRADIOGROUP pRadioGroup, BOOL fPushLike) : CControl(ctlId)
{
#ifdef GTK2
    m_uButtonType = ( pRadioGroup ? CTL_BUTTON_RADIO : CTL_BUTTON_CHECK );
#endif

#ifdef GTK3
    m_uButtonType = ( pRadioGroup ? (1 == (DWORD) pRadioGroup ? CTL_BUTTON_SWITCH : CTL_BUTTON_RADIO ) : CTL_BUTTON_CHECK );
#endif

    Created( pRadioGroup ?
#ifdef GTK2
            gtk_radio_button_new_with_label(pRadioGroup, pszText)
#endif

#ifdef GTK3
            ( 1 == (DWORD) pRadioGroup
             ?
             gtk_switch_new()
             :
             gtk_radio_button_new_with_label(pRadioGroup, pszText)
             )
#endif
            :
            (
                fPushLike
                ?
                gtk_toggle_button_new_with_label(pszText)
                :
                gtk_check_button_new_with_label(pszText)
            )
            );

}

CButton::~CButton()
{

}

VOID CButton::InitSignals()
{
    switch(m_uButtonType)
    {
        case CTL_BUTTON_PUSH:
        {
            g_signal_connect (G_OBJECT (Ctl()), "clicked", G_CALLBACK (CButtonHandlers::gtk_button_onclick), (gpointer) Parent());
            break;
        }
        case CTL_BUTTON_SWITCH:
        {
            g_signal_connect(G_OBJECT( Ctl() ), "activate", G_CALLBACK(CButtonHandlers::gtk_button_onclick), (gpointer) Parent());
            break;
        }
        case CTL_BUTTON_CHECK:
        {
            g_signal_connect(G_OBJECT( Ctl() ), "toggled", G_CALLBACK(CButtonHandlers::gtk_button_onclick), (gpointer) Parent());
            break;
        }
        case CTL_BUTTON_RADIO:
        {
            g_signal_connect(G_OBJECT( Ctl() ), "group-changed", G_CALLBACK(CButtonHandlers::gtk_button_onclick), (gpointer) Parent());
            break;
        }
    }
}

BOOL CButton::Check()
{
    switch(m_uButtonType)
    {
        case CTL_BUTTON_PUSH: return FALSE;
#ifdef GTK3
        case CTL_BUTTON_SWITCH:
#endif
        case CTL_BUTTON_CHECK:
        case CTL_BUTTON_RADIO: return gtk_toggle_button_get_active((GtkToggleButton*) Ctl());
    }
    return FALSE;
}

VOID CButton::Check(BOOL fCheck)
{
    switch(m_uButtonType)
    {
        case CTL_BUTTON_PUSH: return;
#ifdef GTK3
        case CTL_BUTTON_SWITCH:
#endif
        case CTL_BUTTON_CHECK:
        case CTL_BUTTON_RADIO: return gtk_toggle_button_set_active((GtkToggleButton*) Ctl(), fCheck);
    }
    return;
}

/* ------------ GTK STATIC EDIT ------------ */

CStatic::CStatic(DWORD ctlId, LPCTSTR pszText, UINT uAlignment) : CControl(ctlId)
{
    GtkJustification j;

    switch(uAlignment)
    {
        case 1: j=GTK_JUSTIFY_RIGHT; break;
        case 2: j=GTK_JUSTIFY_CENTER; break;
        case 3: j=GTK_JUSTIFY_FILL; break;
        default: j=GTK_JUSTIFY_LEFT; break;
    }
    Created( gtk_label_new(pszText) );
    gtk_label_set_justify((GtkLabel*) Ctl(), j);
}


/* ------------ GTK SIGNLE-LINE EDIT ------------ */

void CEditHandlers::gtk_edit_preedit_changed(HCTL hEdit, LPTSTR pszPreedit, gpointer data)
{
    CEdit * This = (CEdit*) g_object_get_data(G_OBJECT(hEdit), TEXT("this-ptr"));

    CWindow::WndProc((CWindow*) data, hEdit, WM_COMMAND, This->CtlId() | ( EN_CHANGE << 16 ), (DWORD) This->Ctl());
}

void CEditHandlers::gtk_edit_paste_clipboard(HCTL hEdit, gpointer data)
{
    CEdit * This = (CEdit*) g_object_get_data(G_OBJECT(hEdit), TEXT("this-ptr"));

    CWindow::WndProc((CWindow*) data, hEdit, WM_COMMAND, This->CtlId() | ( EN_CHANGING << 16 ), (DWORD) This->Ctl());
}

void CEditHandlers::gtk_edit_insert_at_cursor(HCTL hEdit, LPTSTR pszText, gpointer data)
{
    CEdit * This = (CEdit*) g_object_get_data(G_OBJECT(hEdit), TEXT("this-ptr"));

    CWindow::WndProc((CWindow*) data, hEdit, WM_COMMAND, This->CtlId() | ( EN_CHANGE << 16 ), (DWORD) This->Ctl());
}

void CEditHandlers::gtk_edit_delete_from_cursor(HCTL hEdit, GtkDeleteType type, INT count, gpointer data)
{
    CEdit * This = (CEdit*) g_object_get_data(G_OBJECT(hEdit), TEXT("this-ptr"));

    CWindow::WndProc((CWindow*) data, hEdit, WM_COMMAND, This->CtlId() | ( EN_CHANGE << 16 ), (DWORD) This->Ctl());
}

void CEditHandlers::gtk_edit_cut_clipboard(HCTL hEdit, gpointer data)
{
    CEdit * This = (CEdit*) g_object_get_data(G_OBJECT(hEdit), TEXT("this-ptr"));

    CWindow::WndProc((CWindow*) data, hEdit, WM_COMMAND, This->CtlId() | ( EN_CHANGING << 16 ), (DWORD) This->Ctl());
}

void CEditHandlers::gtk_edit_backspace(HCTL hEdit, gpointer data)
{
    CEdit * This = (CEdit*) g_object_get_data(G_OBJECT(hEdit), TEXT("this-ptr"));

    CWindow::WndProc((CWindow*) data, hEdit, WM_COMMAND, This->CtlId() | ( EN_CHANGING << 16 ), (DWORD) This->Ctl());
}

void CEditHandlers::gtk_edit_activate(HCTL hEdit, gpointer data)
{
    CEdit * This = (CEdit*) g_object_get_data(G_OBJECT(hEdit), TEXT("this-ptr"));

    CWindow::WndProc((CWindow*) data, hEdit, WM_COMMAND, This->CtlId() | ( EN_SETFOCUS << 16 ), (DWORD) This->Ctl());
}


CEdit::CEdit(DWORD ctlId, LPCTSTR pszText) : CControl(ctlId)
{
    m_pBuffer = (HCTL) gtk_entry_buffer_new(pszText, -1);
    if ( !m_pBuffer )
        throw TEXT("Buffer creation failed");

    Created( gtk_entry_new_with_buffer( (GtkEntryBuffer*) m_pBuffer ));
}

CEdit::~CEdit()
{
    gtk_widget_destroy(m_pBuffer);
    m_pBuffer = NULL;
}

VOID CEdit::InitSignals()
{
    g_signal_connect (G_OBJECT (Ctl()), "preedit-changed", G_CALLBACK (CEditHandlers::gtk_edit_preedit_changed), (gpointer) Parent());
    g_signal_connect (G_OBJECT (Ctl()), "paste-clipboard", G_CALLBACK (CEditHandlers::gtk_edit_paste_clipboard), (gpointer) Parent());
    g_signal_connect (G_OBJECT (Ctl()), "insert-at-cursor", G_CALLBACK (CEditHandlers::gtk_edit_insert_at_cursor), (gpointer) Parent());
    g_signal_connect (G_OBJECT (Ctl()), "delete-from-cursor", G_CALLBACK (CEditHandlers::gtk_edit_delete_from_cursor), (gpointer) Parent());
    g_signal_connect (G_OBJECT (Ctl()), "cut-clipboard", G_CALLBACK (CEditHandlers::gtk_edit_cut_clipboard), (gpointer) Parent());
    g_signal_connect (G_OBJECT (Ctl()), "backspace", G_CALLBACK (CEditHandlers::gtk_edit_backspace), (gpointer) Parent());
    g_signal_connect (G_OBJECT (Ctl()), "activate", G_CALLBACK (CEditHandlers::gtk_edit_activate), (gpointer) Parent());
}

VOID CEdit::Text(LPCTSTR pszText)
{
    gtk_entry_set_text((GtkEntry*) Ctl(), pszText);
}

LPCTSTR CEdit::Text()
{
    return gtk_entry_get_text((GtkEntry*) Ctl());
}

UINT CEdit::TextLength()
{
    return gtk_entry_get_text_length((GtkEntry*) Ctl());
}

VOID CEdit::Readonly(BOOL fSet)
{
    GValue val = { 0 };
    g_value_init(&val, G_TYPE_BOOLEAN);
    g_value_set_boolean(&val, !fSet);
    g_object_set_property((GObject*) Ctl(), "editable", &val);
}

VOID CEdit::Password(BOOL fSet)
{
    gtk_entry_set_visibility((GtkEntry*) Ctl(), !fSet);
}

/* ------------ GTK SPIN ------------ */

void CEditSpinHandlers::gtk_spin_value_changed(HCTL hEditSpin, gpointer data)
{
    CEditSpin * This = (CEditSpin*) g_object_get_data(G_OBJECT(hEditSpin), TEXT("this-ptr"));
    CWindow::WndProc((CWindow*) data, hEditSpin, WM_COMMAND, This->CtlId() | ( EN_CHANGE << 16 ), (DWORD) This->Ctl());
}

void CEditSpinHandlers::gtk_spin_value_change(HCTL hEditSpin, GtkScrollType arg1, gpointer data)
{
    CEditSpin * This = (CEditSpin*) g_object_get_data(G_OBJECT(hEditSpin), TEXT("this-ptr"));
    CWindow::WndProc((CWindow*) data, hEditSpin, WM_COMMAND, This->CtlId() | ( EN_CHANGING << 16 ), (DWORD) This->Ctl());
}

CEditSpin::CEditSpin(DWORD ctlId, UINT min, UINT max) : CControl(ctlId)
{
    Created( gtk_spin_button_new_with_range(min, max, 1) );
}

VOID CEditSpin::Value(UINT uValue)
{
    gtk_spin_button_set_value((GtkSpinButton*) Ctl(), (gdouble) uValue);
}

UINT CEditSpin::Value()
{
    return gtk_spin_button_get_value_as_int((GtkSpinButton*) Ctl());
}

VOID CEditSpin::InitSignals()
{
    g_signal_connect (G_OBJECT (Ctl()), "change-value", G_CALLBACK (CEditSpinHandlers::gtk_spin_value_change), (gpointer) Parent());
    g_signal_connect (G_OBJECT (Ctl()), "value-changed", G_CALLBACK (CEditSpinHandlers::gtk_spin_value_changed), (gpointer) Parent());
}

/* ------------ GTK MULTIEDIT ------------ */

CMultiEdit::CMultiEdit(DWORD ctlId) : CControl(ctlId)
{
    m_pTextBuffer = gtk_text_buffer_new(NULL);
    if ( !m_pTextBuffer )
        throw(TEXT("Failed to create text buffer"));

    CreatedContainer( gtk_scrolled_window_new(NULL, NULL) );
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(Container()),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    Created( gtk_text_view_new_with_buffer( m_pTextBuffer ));
    gtk_container_add(GTK_CONTAINER(Container()), (HCTL) Ctl());
}

VOID CMultiEdit::Readonly(BOOL fSet)
{
    GValue val = { 0 };
    g_value_init(&val, G_TYPE_BOOLEAN);
    g_value_set_boolean(&val, !fSet);
    g_object_set_property((GObject*) Ctl(), "editable", &val);
}

LPCTSTR CMultiEdit::Text()
{
    GtkTextIter Iter1 = { 0 }, Iter2 = { 0 };

    gtk_text_buffer_get_start_iter(m_pTextBuffer, &Iter1);
    gtk_text_buffer_get_end_iter(m_pTextBuffer, &Iter2);

    return gtk_text_buffer_get_text(m_pTextBuffer, &Iter1, &Iter2, TRUE);
}

VOID CMultiEdit::Text(LPCTSTR pszText)
{
    GValue val = { 0 };
    g_value_init(&val, G_TYPE_STRING);
    g_value_set_string(&val, pszText);
    g_object_set_property((GObject*) m_pTextBuffer, "text", &val);
    g_value_unset(&val);
}

CMultiEdit::~CMultiEdit()
{

}

/* ------------ GTK SPIN ------------ */

CComboBox::CComboBox(DWORD ctlId, DWORD cbType) : CControl(ctlId)
{
    m_pList =  gtk_list_store_new(1, G_TYPE_STRING, G_TYPE_STRING);
    if ( !m_pList )
        throw TEXT("Creation area failed");

    m_pCellRender = gtk_cell_renderer_text_new();
    if ( !m_pCellRender )
        throw TEXT("Creating renderer failed");

    Created( gtk_combo_box_new_with_model((GtkTreeModel*) m_pList) );

    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(Ctl()), m_pCellRender, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(Ctl()), m_pCellRender, "text", 0, NULL);
}

CComboBox::~CComboBox()
{
    if ( m_pCellRender )
        gtk_widget_destroy((HCTL) m_pCellRender);

    if ( m_pList )
        gtk_widget_destroy((HCTL) m_pList);
}

VOID CComboBox::AddItem(LPCTSTR pszText)
{
    GtkTreeIter iter = { 0 };
    GValue val = { 0 };
    g_value_init(&val, G_TYPE_STRING);
    g_value_set_string(&val, pszText);

    gtk_list_store_append(m_pList, &iter );
    gtk_list_store_set_value( m_pList, &iter, 0, &val );
    g_value_unset(&val);
}

VOID CComboBox::InsertItem(LPCTSTR pszText, UINT index)
{

    if ( index >= Count() )
    {
        AddItem(pszText);
        return;
    }

    GtkTreeIter iter = { 0 };
    GValue val = { 0 };
    g_value_init(&val, G_TYPE_STRING);
    g_value_set_string(&val, pszText);

    gtk_list_store_insert(m_pList, &iter, index);
    gtk_list_store_set_value( m_pList, &iter, 0, &val );
    g_value_unset(&val);
}

VOID CComboBox::CurSel(UINT uSel)
{
    gtk_combo_box_set_active((GtkComboBox*) Ctl(), uSel);
}

UINT CComboBox::CurSel()
{
    return gtk_combo_box_get_active((GtkComboBox*) Ctl());
}

UINT CComboBox::Count()
{
    return gtk_tree_model_iter_n_children(
                                          gtk_combo_box_get_model((GtkComboBox*) Ctl())
                                          ,
                                          NULL);
}

VOID CComboBox::Delete(UINT index)
{
    if ( index >= Count() )
        return;

    GtkTreeIter iter = { 0 };
    GtkTreeModel * model = gtk_combo_box_get_model((GtkComboBox*) Ctl());

    gtk_tree_model_get_iter_first(model, &iter);

    while(index>0)
    {
        gtk_tree_model_iter_next(model, &iter);
        index--;
    }

    gtk_list_store_remove(m_pList, &iter);
}

VOID CComboBox::Reset()
{
    gtk_list_store_clear(m_pList);
}

VOID CComboBox::InitSignals()
{

}

/* ------------ GTK LISTVIEW ------------ */

CListView::CListView(DWORD ctlId, UINT nColumns) : CControl(ctlId)
{
    if ( !nColumns || nColumns > 1024 )
        throw TEXT("Allocating more than 1024 columns or no columns failed");

    nColumns++;

    GType * gtp = new GType[nColumns];
    gtp[0]=G_TYPE_POINTER;

    for(UINT i=1; i < nColumns; ++i)
        gtp[i] = G_TYPE_STRING;

    m_pList =  gtk_list_store_newv(nColumns, gtp);
    delete gtp;

    if ( !m_pList )
        throw TEXT("Creation area failed");


    m_pCellRenderer = gtk_cell_renderer_text_new();
    if ( !m_pCellRenderer )
        throw TEXT("Creating renderer failed");


    CreatedContainer( gtk_scrolled_window_new(NULL, NULL) );
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(Container()),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    Created( gtk_tree_view_new_with_model((GtkTreeModel*) m_pList) );
    gtk_container_add(GTK_CONTAINER(Container()), (HCTL) Ctl());

    // create userdata column
    PLV_COLUMN plvc = new LV_COLUMN;
    plvc->pRenderer = NULL;
    plvc->pColumn = gtk_tree_view_column_new();
    gtk_tree_view_append_column((GtkTreeView*) Ctl(), plvc->pColumn);

    GValue val = { 0 };
    g_value_init(&val, G_TYPE_BOOLEAN);
    g_value_set_boolean(&val, FALSE);
    g_object_set_property((GObject*) plvc->pColumn, "visible", &val);
    g_value_unset(&val);

    m_vColList.push_back(plvc);
}

UINT CListView::Count()
{
    return gtk_tree_model_iter_n_children(
                                          (GtkTreeModel*) m_pList
                                          ,
                                          NULL);
}

VOID CListView::Reset()
{
    gtk_list_store_clear(m_pList);
}

BOOL CListView::IterFromIndex(UINT index, GtkTreeIter * pIter)
{
    if ( index >= Count())
        return FALSE;

    gtk_tree_model_get_iter_first((GtkTreeModel*) m_pList, pIter);
    while(index>0)
    {
        gtk_tree_model_iter_next((GtkTreeModel*) m_pList, pIter);
        index--;
    }

    return TRUE;
}

VOID CListView::AddColumn(LPCTSTR pszColumn)
{
    PLV_COLUMN plvc = new LV_COLUMN;
    plvc->pRenderer = m_pCellRenderer; // gtk_cell_renderer_text_new();
    plvc->pColumn = gtk_tree_view_column_new_with_attributes(pszColumn, plvc->pRenderer, "text", m_vColList.size(), NULL);
    gtk_tree_view_append_column((GtkTreeView*) Ctl(), plvc->pColumn);
    m_vColList.push_back(plvc);

    GValue val = { 0 };
    g_value_init(&val, G_TYPE_BOOLEAN);
    g_value_set_boolean(&val, TRUE);
    g_object_set_property((GObject*) plvc->pColumn, "resizable", &val);
    g_object_set_property((GObject*) plvc->pColumn, "clickable", &val);
    g_value_unset(&val);
}

VOID CListView::GridLines(INT iSet)
{
    GValue val = { 0 };
    g_value_init(&val, G_TYPE_INT);
    g_value_set_int(&val, GTK_TREE_VIEW_GRID_LINES_BOTH);
    g_object_set_property((GObject*) Ctl(), TEXT("enable-grid-lines"), &val);
    g_value_unset(&val);
}

CListView::~CListView()
{
    if ( m_pCellRenderer )
        gtk_widget_destroy((GtkWidget*) m_pCellRenderer);

    if ( m_pList )
        gtk_widget_destroy((GtkWidget*) m_pList);

    for(std::vector<PLV_COLUMN>::iterator v = m_vColList.begin(); v < m_vColList.end(); v++)
        delete (*v);

}

UINT CListView::AddItem(LPCTSTR pszText, PVOID lParam)
{
    UINT c = Count();
    GtkTreeIter Iter = { 0 };
    GValue val = { 0 };

    gtk_list_store_append((GtkListStore*) m_pList, &Iter);

    g_value_init(&val, G_TYPE_POINTER);
    g_value_set_pointer(&val, lParam);
    gtk_list_store_set_value((GtkListStore*) m_pList, &Iter, 0, &val);
    g_value_unset(&val);

    g_value_init(&val, G_TYPE_STRING);
    g_value_set_string(&val, pszText);
    gtk_list_store_set_value((GtkListStore*) m_pList, &Iter, 1, &val);
    g_value_unset(&val);
    return c;
}

VOID CListView::Item(UINT index, UINT column, LPCTSTR pszText)
{
    GtkTreeIter Iter = { 0 };
    if ( (UINT) ~0 == column ) column = 0;

    if ( IterFromIndex(index, &Iter) )
    {
        GValue val = { 0 };
        g_value_init(&val, G_TYPE_STRING);
        g_value_set_string(&val, pszText);
        gtk_list_store_set_value((GtkListStore*) m_pList, &Iter, column+1, &val);
        g_value_unset(&val);
    }
}

PVOID CListView::Param(UINT uItem)
{
    GtkTreeIter Iter = { 0 };

    if ( gtk_tree_model_iter_nth_child((GtkTreeModel*) m_pList, &Iter, NULL, uItem) )
    {
        GValue val = { 0 };
        gtk_tree_model_get_value((GtkTreeModel*) m_pList, &Iter, 0, &val);
        PVOID pParam = NULL;
        pParam = g_value_get_pointer(&val);
        g_value_unset(&val);
        return pParam;
    }
    return NULL;
}

UINT CListView::CurSel()
{
    GtkTreeSelection * tsel = gtk_tree_view_get_selection ((GtkTreeView*) Ctl());
    GtkTreeIter iter ;
    GtkTreeModel * tm ;
    if ( gtk_tree_selection_get_selected ( tsel , &tm , &iter ) )
    {
        INT * i ;
        GtkTreePath * path = gtk_tree_model_get_path ( tm , &iter ) ;
        i = gtk_tree_path_get_indices ( path ) ;
        return i [ 0 ] ;
    }
    return -1;
}

/* ------------ GTK LINKBUTTON ------------ */

CLinkButton::CLinkButton(DWORD ctlId, LPCTSTR pszUrl, LPCTSTR pszTitle) : CControl(ctlId)
{
    Created( gtk_link_button_new_with_label(pszUrl, pszTitle) );
}

/* ------------ GTK PROGRESSBAR ------------ */

CProgressBar::CProgressBar(DWORD ctlId) : CControl(ctlId)
{
    Created( gtk_progress_bar_new() );
}

VOID CProgressBar::Pulse(BYTE bPulse)
{
    gtk_progress_bar_pulse((GtkProgressBar*) Ctl());
    if ( bPulse ) gtk_progress_bar_set_pulse_step((GtkProgressBar*) Ctl(), ((gdouble) bPulse)/255);
}

VOID CProgressBar::Progress(BYTE bProgress)
{
    gtk_progress_bar_set_fraction((GtkProgressBar*) Ctl(), ((gdouble) bProgress)/255);
}

BYTE CProgressBar::Progress()
{
    return (BYTE) (255*gtk_progress_bar_get_fraction((GtkProgressBar*) Ctl()));
}

VOID CProgressBar::Tip(LPCTSTR pszText)
{
    gtk_progress_bar_set_text((GtkProgressBar*) Ctl(), pszText);
}

VOID CProgressBar::ShowText(BOOL fShow)
{
#ifdef GTK3
    gtk_progress_bar_set_show_text((GtkProgressBar*) Ctl(), fShow);
#endif
}

BOOL CProgressBar::ShowText()
{
#ifdef GTK3
    return gtk_progress_bar_get_show_text((GtkProgressBar*) Ctl());
#else
    return TRUE;
#endif
}

/* ------------ GTK GRID ------------ */

CGrid::CGrid(DWORD ctlId, UINT cColumns, UINT cRows) : CControl(ctlId)
{
    Created( gtk_grid_new() );
    for(UINT y=0; y<cColumns; ++y)
        gtk_grid_insert_column((GtkGrid*) Ctl(), y);

    for(UINT x=0; x<cColumns; ++x)
        gtk_grid_insert_row((GtkGrid*) Ctl(), x);
}

VOID CGrid::Attach(CControl & pCtl, UINT uLeft, UINT uTop, UINT uWidth, UINT uHeight)
{
    Attach(( pCtl.Container() ? pCtl.Container() : pCtl.Ctl() ), uLeft, uTop, uWidth, uHeight);
}

VOID CGrid::Attach(HCTL hCtl, UINT uLeft, UINT uTop, UINT uWidth, UINT uHeight)
{
    gtk_grid_attach((GtkGrid*) Ctl(), hCtl, uLeft, uTop, uWidth, uHeight);
}

