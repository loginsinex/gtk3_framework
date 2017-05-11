#include "../../h/std.h"

CTestDlg::CTestDlg() :
    m_cGrid(-1, 10, 5),
    m_cBtn(1, TEXT("Click me"), NULL),
    m_cBtn2(2, TEXT("Click me too"), NULL),
    m_cProgress(3),
    m_cEdit(5, TEXT("Something right!!!")),
    m_cSpin(6, 0, 100),
    m_cCombo(7),
    m_cLink(8, TEXT("https://github.com/"), TEXT("GITHUB")),
    m_cList(9,3),
    m_cBtn3(10, TEXT("Dlg"))
{
    Insert(&m_cGrid);
    Add(&m_cBtn);
    Add(&m_cBtn2);
    Add(&m_cProgress);
    Add(&m_cEdit);
    Add(&m_cSpin);
    Add(&m_cCombo);
    Add(&m_cLink);
    Add(&m_cList);
    Add(&m_cBtn3);

    Size(650, 490);

    m_cGrid.Size(640, 480);
    m_cGrid.Attach(m_cBtn, 1, 0, 5);
    m_cGrid.Attach(m_cBtn2, 1, 1, 1);
    m_cGrid.Attach(m_cProgress, 1, 2, 5);
    m_cGrid.Attach(m_cEdit, 3, 1, 3);
    m_cGrid.Attach(m_cSpin, 6, 2, 5);
    m_cGrid.Attach(m_cCombo, 1, 3, 7);
    m_cGrid.Attach(m_cLink, 1, 4);
    m_cGrid.Attach(m_cList, 1, 5, 4, 3);
    m_cBtn.Size(100,20);
    m_cGrid.Attach(m_cBtn3, 1, 9, 2);

    m_cProgress.Progress(127);
    m_cProgress.Tip(TEXT("Sample of progress bar"));
    m_cProgress.ShowText(TRUE);

    for(int i=0; i<20; ++i)
    {
        TCHAR p[16];
        sprintf(p, TEXT("Something %d"), i);
        m_cCombo.AddItem(p);
    }

    m_cCombo.CurSel(5);

    m_cBtn.Check(TRUE);

    printf("Count cb: %d\n", m_cCombo.Count());

    m_cCombo.Delete(10);

    m_cCombo.InsertItem(TEXT("Not something"), 10);

    m_cList.AddColumn(TEXT("Column #1"));
    m_cList.AddColumn(TEXT("Column #2"));
    m_cList.AddColumn(TEXT("Column #3"));

    m_cList.AddItem(TEXT("Col1.Item1"));
    m_cList.AddItem(TEXT("Col1.Item2"));
    m_cList.AddItem(TEXT("Col1.Item3"));


    m_cList.Item(0, 1, TEXT("Col2.Item1"));
    m_cList.Item(1, 1, TEXT("Col2.Item2"));
    m_cList.Item(2, 1, TEXT("Col2.Item3"));

    m_cList.Item(0, 2, TEXT("Col3.Item1"));
    m_cList.Item(1, 2, TEXT("Col3.Item2"));
    m_cList.Item(2, 2, TEXT("Col3.Item3"));

}

BOOL CTestDlg::OnClose()
{
    Return(5);
    return Confirm(TEXT("Are you sure that you want to exit?"));
}

VOID CTestDlg::OnButton(USHORT uId)
{
    if ( 1 == uId || 2 == uId )
    {
        TCHAR szText[16];
        sprintf(szText, TEXT("Button is: %d"), 1==uId ? m_cBtn.Check() : m_cBtn2.Check());
        ShowMessage(szText);
    }
    else if ( 10 == uId )
    {
        CTestDlg dlg;
        TCHAR p[15];
        sprintf(p, TEXT("Return: %d"), dlg.Show(this));
        ShowMessage(p);
    }
}

VOID CTestDlg::OnEditChange(USHORT uId)
{
    if ( 6 == uId )
    {
        m_cProgress.Pulse(1);
    }
}
