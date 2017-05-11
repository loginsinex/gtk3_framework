#ifndef CCONTROLS_H_INCLUDED
#define CCONTROLS_H_INCLUDED

class CWindow;
class CButton;
class CEdit;
class CEditSpin;

#define     CTL_BUTTON_PUSH             0
#define     CTL_BUTTON_CHECK            1
#define     CTL_BUTTON_RADIO            2
#define     CTL_BUTTON_SWITCH           3

typedef struct _tagLV_COLUMN
{
    GtkCellRenderer *       pRenderer;
    GtkTreeViewColumn *     pColumn;
} LV_COLUMN, *PLV_COLUMN;

class CControlHandlers
{

};

class CButtonHandlers: public CControlHandlers
{
    static              void gtk_button_onclick(HCTL hButton, gpointer data);
    friend              class CButton;
};

class CEditHandlers: public CControlHandlers
{
    static              void gtk_edit_preedit_changed(HCTL hEdit, LPTSTR pszPreedit, gpointer data);
    static              void gtk_edit_paste_clipboard(HCTL hEdit, gpointer data);
    static              void gtk_edit_insert_at_cursor(HCTL hEdit, LPTSTR pszText, gpointer data);
    static              void gtk_edit_delete_from_cursor(HCTL hEdit, GtkDeleteType type, INT count, gpointer data);
    static              void gtk_edit_cut_clipboard(HCTL hEdit, gpointer data);
    static              void gtk_edit_backspace(HCTL hEdit, gpointer data);
    static              void gtk_edit_activate(HCTL hEdit, gpointer data);

    friend              class CEdit;
};

class CEditSpinHandlers: public CControlHandlers
{
    static              void gtk_spin_value_changed(HCTL hSpin, gpointer data);
    static              void gtk_spin_value_change(HCTL hSpin, GtkScrollType arg1, gpointer data);

    friend              class CEditSpin;
};

class CControl
{
    CWindow     *m_pParent;
    HCTL        m_hCtl;
    HCTL        m_hContainer;
    DWORD       m_ctlId;

protected:
    CControl(DWORD ctlId);
    ~CControl();
    VOID            CreatedContainer(HCTL hContainer);

    VOID            Created(HCTL hCtl);
    virtual VOID    InitSignals();

public:
    DWORD       CtlId();
    HCTL        Ctl();
    HCTL        Container();
    VOID        Show(BOOL fVisible);
    VOID        Enable(BOOL fEnable);
    BOOL        Show();
    BOOL        Enable();
    VOID        Size(UINT cx, UINT cy);
    VOID        SetFocus();
    CWindow     *Parent();
    VOID        Parent(CWindow * pWindow);
    VOID        Container(CWindow * pWnd, BOOL fInsert = FALSE);
    VOID        Margin(UINT width);

};

class CStatic: public CControl
{

public:
    CStatic(DWORD ctlId, LPCTSTR pszText, UINT uAlignment = 0);
};

class CButton: public CControl
{
    USHORT  m_uButtonType;
    VOID    InitSignals();

public:
    CButton(DWORD ctlId);
    CButton(DWORD ctlId, LPCTSTR pszText);
    CButton(DWORD ctlId, LPCTSTR pszText, HRADIOGROUP pGroup, BOOL fPushLike = FALSE);
    ~CButton();

    VOID Check(BOOL fCheck);
    BOOL Check();

    friend class CButtonHandlers;

};

class CEdit: public CControl
{
    HCTL        m_pBuffer;
    VOID        InitSignals();

public:
    CEdit(DWORD ctlId, LPCTSTR pszInitialText = NULL);
    ~CEdit();

    VOID        Text(LPCTSTR pszText);
    LPCTSTR     Text();

    UINT        TextLength();
    VOID        Readonly(BOOL fReadonly);
    VOID        Password(BOOL fHide);

    friend class CEditHandlers;
};

class CEditSpin: public CControl
{
    VOID        InitSignals();

public:
    CEditSpin(DWORD ctlId, UINT min=0, UINT max=0);

    VOID        Value(UINT uValue);
    UINT        Value();
    BOOL        IsError();

    friend      class CEditSpinHandlers;
};

class CMultiEdit: public CControl
{
    GtkTextBuffer   *   m_pTextBuffer;

public:
    CMultiEdit(DWORD ctlId);
    VOID                Readonly(BOOL fSet);
    LPCTSTR             Text();
    VOID                Text(LPCTSTR pszText);

    ~CMultiEdit();

};

class CComboBox: public CControl
{
    GtkCellRenderer*                    m_pCellRender;
    GtkListStore*                       m_pList;
    VOID                                InitSignals();

public:
    CComboBox(DWORD ctlId, DWORD cbType=0);
    ~CComboBox();

    VOID        AddItem(LPCTSTR pszString);
    VOID        InsertItem(LPCTSTR pszString, UINT index);
    VOID        CurSel(UINT uSel);
    UINT        CurSel();
    UINT        Count();
    VOID        Delete(UINT index);
    VOID        Reset();
};

class CLinkButton: public CControl
{

public:
    CLinkButton(DWORD ctlId, LPCTSTR pszUrl, LPCTSTR pszTitle);
};

class CProgressBar: public CControl
{

public:
    CProgressBar(DWORD ctlId);
    VOID        Pulse(BYTE bPulse=0);
    VOID        Progress(BYTE bProgress);
    BYTE        Progress();

    VOID        Tip(LPCTSTR pszText);
    VOID        ShowText(BOOL fShow);
    BOOL        ShowText();

};

class CListView: public CControl
{
    GtkListStore*           m_pList;
    GtkCellRenderer*        m_pCellRenderer;
    BOOL                    IterFromIndex(UINT index, GtkTreeIter * pIter);
    std::vector<PLV_COLUMN> m_vColList;

public:
    CListView(DWORD ctlId, UINT nColumns = 1);
    ~CListView();

    VOID AddColumn(LPCTSTR pszColumn);
    UINT AddItem(LPCTSTR pszText, PVOID lParam = NULL);
    VOID Item(UINT index, UINT column, LPCTSTR pszText);
    UINT CurSel();
    PVOID Param(UINT uItem);
    UINT Count();
    VOID Reset();
    VOID GridLines(INT iSet);

};

#ifdef GTK3
class CGrid: public CControl
{

public:
    CGrid(DWORD ctlId, UINT cColumns, UINT cRows);
    VOID Attach(CControl & pCtl, UINT uLeft, UINT uTop, UINT uWidth=1, UINT uHeight=1);
    VOID Attach(HCTL hCtl, UINT uLeft, UINT uTop, UINT uWidth=1, UINT uHeight=1);
};
#endif

#endif // CCONTROLS_H_INCLUDED
