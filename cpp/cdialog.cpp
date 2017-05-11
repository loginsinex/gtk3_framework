
#include "../h/std.h"

void CWindowHandlers::gtk_cwindow_destroy( HCTL hWnd, gpointer data )
{
    CWindow::WndProc((CWindow*) data, hWnd, WM_DESTROY, 0, 0);
    gtk_main_quit();
}

INT CWindowHandlers::gtk_cwindow_onclose( HWND hWnd, GdkEvent  *event, gpointer data )
{
    return ( !CWindow::WndProc((CWindow*) data, (HCTL) hWnd, WM_CLOSE, 0, 0) );
}


CWindow::CWindow()
{
    m_hWnd = (HWND) gtk_window_new(GTK_WINDOW_TOPLEVEL);
    m_retCode = 0;
    m_fNonModal = TRUE;
    Resizeable(FALSE);
}

VOID CWindow::Size(UINT width, UINT height)
{
    gtk_widget_set_size_request((HCTL) m_hWnd, width, height);
}

VOID CWindow::Add(CControl * pCtl)
{
    m_vExtList.push_back(pCtl);
}

VOID CWindow::Insert(CControl * pCtl)
{
    m_vMeList.push_back(pCtl);
}

VOID CWindow::Return(INT ret)
{
    m_retCode = ret;
}

VOID CWindow::OnCommand(USHORT uCmd, USHORT uId, HCTL hCtl)
{

}

VOID CWindow::OnButton(USHORT uId)
{

}

VOID CWindow::OnEditChange(USHORT uId)
{

}

VOID CWindow::OnInit(LPARAM lParam)
{

}

INT CWindow::OnClose()
{
    return TRUE;
}

VOID CWindow::OnDestroy()
{
    return;
}

HWND CWindow::GetWindowHandle()
{
    return m_hWnd;
}

HCTL CWindow::Ctl(USHORT ctlId)
{
    for(std::vector<CControl*>::iterator v = m_vMeList.begin(); v < m_vMeList.end(); v++)
          if ( ctlId == (USHORT) (0xFFFFL & (*v)->CtlId()) )
                return (*v)->Ctl();


    for(std::vector<CControl*>::iterator v = m_vExtList.begin(); v < m_vExtList.end(); v++)
          if ( ctlId == (USHORT) (0xFFFFL & (*v)->CtlId()) )
                return (*v)->Ctl();

    return NULL;
}

VOID CWindow::Resizeable(BOOL fResizeable)
{
    gtk_window_set_resizable(m_hWnd, fResizeable);
}

VOID CWindow::Close(INT ret)
{
    Return(ret);
    gtk_widget_destroy((HCTL) m_hWnd);
}

VOID CWindow::Enable(BOOL fEnable)
{
    gtk_widget_set_sensitive((HCTL) m_hWnd, fEnable);
}

BOOL CWindow::Enable()
{
    return gtk_widget_is_sensitive((HCTL) m_hWnd);
}

INT CWindow::Show(CWindow * pParent, LPARAM lParam)
{
    m_pParent = pParent;

    // connect all needed signals
    g_signal_connect (m_hWnd, "destroy", G_CALLBACK(CWindowHandlers::gtk_cwindow_destroy), this);
    g_signal_connect (m_hWnd, "delete_event", G_CALLBACK (CWindowHandlers::gtk_cwindow_onclose), this);

    // connect controls
    for(std::vector<CControl*>::iterator v = m_vMeList.begin(); v < m_vMeList.end(); v++)
          (*v)->Container(this, TRUE);

    for(std::vector<CControl*>::iterator v = m_vExtList.begin(); v < m_vExtList.end(); v++)
          (*v)->Container(this, FALSE);

    OnInit(lParam);
    if ( pParent )
    {
        gtk_window_set_modal(m_hWnd, TRUE);
        gtk_window_set_transient_for((HWND) m_hWnd, (HWND) pParent->m_hWnd);
    }


    gtk_widget_show_all ((HCTL) m_hWnd);
    gtk_main ();

    return m_retCode;
}

HWND CWindow::Create(CWindow * pParent, LPARAM lParam)
{
    m_pParent = pParent;
    m_fNonModal = FALSE;

    // connect all needed signals
    g_signal_connect (m_hWnd, "destroy", G_CALLBACK(CWindowHandlers::gtk_cwindow_destroy), this);
    g_signal_connect (m_hWnd, "delete_event", G_CALLBACK (CWindowHandlers::gtk_cwindow_onclose), this);

    // connect controls
    for(std::vector<CControl*>::iterator v = m_vMeList.begin(); v < m_vMeList.end(); v++)
          (*v)->Container(this, TRUE);

    for(std::vector<CControl*>::iterator v = m_vExtList.begin(); v < m_vExtList.end(); v++)
          (*v)->Container(this, FALSE);

    OnInit(lParam);
    if ( pParent )
    {
        gtk_window_set_transient_for((HWND) m_hWnd, (HWND) pParent->m_hWnd);
    }


    gtk_widget_show_all ((HCTL) m_hWnd);

    return m_hWnd;
}

INT_PTR CWindow::WndProc(CWindow * This, HCTL hCtl, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
        {
            switch(HIWORD(wParam))
            {
                case BN_CLICKED: This->OnButton(LOWORD(wParam)); break;
                case EN_CHANGE: This->OnEditChange(LOWORD(wParam)); break;
                default: This->OnCommand(HIWORD(wParam), LOWORD(wParam), hCtl); break;
            }
            break;
        }
        case WM_CLOSE: return This->OnClose();
        case WM_DESTROY: This->OnDestroy(); break;
    }
    return 0;
}

INT CWindow::MsgBox(LPCTSTR pszMessage, LPCTSTR pszTitle, UINT uFlags)
{
    UINT uType = ( uFlags & 0xF0 );
    UINT uBtns = ( uFlags & 0x0F );

    GtkMessageType gType; GtkButtonsType gBtns;
    switch(uType)
    {
        case MB_ICON_INFORMATION: gType = GTK_MESSAGE_INFO; break;
        case MB_ICON_ERROR: gType = GTK_MESSAGE_ERROR; break;
        case MB_ICON_WARNING: gType = GTK_MESSAGE_WARNING; break;
        case MB_ICON_QUESTION: gType = GTK_MESSAGE_QUESTION; break;
        default: gType = GTK_MESSAGE_OTHER; break;
    }

    switch(uBtns)
    {
        case MB_YESNO: gBtns = GTK_BUTTONS_YES_NO; break;
        case MB_OKCANCEL: gBtns = GTK_BUTTONS_OK_CANCEL; break;
        case MB_CANCEL: gBtns = GTK_BUTTONS_CANCEL; break;
        case MB_CLOSE: gBtns = GTK_BUTTONS_CLOSE; break;
        case MB_NONE: gBtns = GTK_BUTTONS_NONE; break;
        default: gBtns = GTK_BUTTONS_OK; break;
    }

    HWND hDlg = (HWND) gtk_message_dialog_new (m_hWnd, GTK_DIALOG_MODAL, gType, gBtns, pszMessage, 0);

    UINT ret;
    gtk_window_set_position (hDlg, GTK_WIN_POS_CENTER_ON_PARENT);
    ret = gtk_dialog_run (GTK_DIALOG (hDlg));
    gtk_widget_destroy ((HCTL) hDlg);
    return ret;
}

BOOL CWindow::Confirm(LPCTSTR pszMessage)
{
    return ( GTK_RESPONSE_YES == MsgBox(pszMessage, NULL, MB_ICON_QUESTION | MB_YESNO) );
}

VOID CWindow::ShowError(LPCTSTR pszMessage)
{
    MsgBox(pszMessage, NULL, MB_ICON_ERROR);
}

VOID CWindow::ShowMessage(LPCTSTR pszMessage)
{
    MsgBox(pszMessage, NULL, MB_ICON_INFORMATION);
}
