#ifndef CTESTDLG_H_INCLUDED
#define CTESTDLG_H_INCLUDED

class CTestDlg: public CWindow
{
    CGrid       m_cGrid;
    CButton     m_cBtn;
    CButton     m_cBtn2;
    CProgressBar m_cProgress;
    CEdit       m_cEdit;
    CEditSpin   m_cSpin;
    CComboBox   m_cCombo;
    CLinkButton m_cLink;
    CListView   m_cList;
    CButton     m_cBtn3;

public:
    CTestDlg();
    BOOL    OnClose();
    VOID    OnButton(USHORT uId);
    VOID    OnEditChange(USHORT uId);
};


#endif // CTESTDLG_H_INCLUDED
