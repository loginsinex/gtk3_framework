#ifndef CDIALOG_H_INCLUDED
#define CDIALOG_H_INCLUDED



class CWindow;

class CWindowHandlers
{
    static void gtk_cwindow_destroy( HCTL hWnd, gpointer data );
    static INT gtk_cwindow_onclose( HWND hWnd, GdkEvent  *event, gpointer data );

    friend class CWindow;
};

class CWindow
{
        std::vector<CControl*>    m_vMeList;
        std::vector<CControl*>    m_vExtList;

        HWND            m_hWnd;
        CWindow         *m_pParent;
        INT             m_retCode;
        BOOL            m_fNonModal;

        static          INT_PTR WndProc(CWindow * This, HCTL hCtl, UINT uMsg, WPARAM wParam, LPARAM lParam);
        INT             MsgBox(LPCTSTR pszMessage, LPCTSTR pszTitle, UINT uFlags);

protected:
        virtual VOID    OnInit(LPARAM lParam);
        virtual INT     OnClose();
        virtual VOID    OnDestroy();
        virtual VOID    OnCommand(USHORT uCmd, USHORT uId, HCTL hCtl);
        virtual VOID    OnButton(USHORT uId);
        virtual VOID    OnEditChange(USHORT uId);
        VOID            Return(INT ret);
        VOID            Size(UINT width, UINT height);

public:
        CWindow();

        INT             Show(CWindow * pParent, LPARAM lParam = 0);
        HWND            Create(CWindow * pParent, LPARAM lParam = 0);
        HWND            GetWindowHandle();
        BOOL            Confirm(LPCTSTR pszPrompt);
        VOID            ShowMessage(LPCTSTR pszPrompt);
        VOID            ShowError(LPCTSTR pszPrompt);
        VOID            Add(CControl * pCtl);
        VOID            Insert(CControl * pCtl);
        HCTL            Ctl(USHORT ctlId);
        VOID            Enable(BOOL fEnable);
        BOOL            Enable();
        VOID            Close(INT ret);
        VOID            Resizeable(BOOL fResizeable);

        friend          class CWindowHandlers;
        friend          class CButtonHandlers;
        friend          class CEditHandlers;
        friend          class CEditSpinHandlers;
};

#endif // CDIALOG_H_INCLUDED
