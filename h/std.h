#ifndef STD_H_INCLUDED
#define STD_H_INCLUDED

#include <stdlib.h>
// #include <gtk.h>
#include <gtk.h>
#include <vector>

#define     GTK3

typedef unsigned short      USHORT;
typedef short               SHORT;
typedef wchar_t             WCHAR;
typedef unsigned char       BYTE;
typedef BYTE                UCHAR;
typedef char                CHAR;
typedef unsigned long       ULONG;
typedef long                LONG;
typedef ULONG               DWORD;
typedef unsigned long int   DWORD64;
typedef unsigned int        UINT;
typedef int                 INT;
typedef void                VOID;
typedef VOID*               PVOID;
typedef INT                 INT_PTR;
typedef INT                 BOOL;
typedef DWORD*              LPDWORD;
typedef LONG*               PLONG;
typedef WCHAR*              LPWSTR;
typedef CHAR*               LPSTR;
typedef UINT                LPARAM;
typedef UINT                WPARAM;
typedef const CHAR*         LPCSTR;
typedef const WCHAR*        LPCWSTR;

typedef GtkWindow*          HWND;
typedef GtkWidget*          HCTL;
typedef GSList*             HRADIOGROUP;

#ifdef UNICODE
typedef WCHAR               TCHAR;
typedef LPWSTR              LPTSTR;
typedef LPCWSTR             LPCTSTR;
#define TEXT(s)             L##s
#else
typedef CHAR                TCHAR;
typedef LPSTR               LPTSTR;
typedef LPCSTR              LPCTSTR;
#define TEXT(s)             s
#endif

#define         HIWORD(w)                   ((w) >> 16)
#define         LOWORD(w)                   ((w) & 0xFFFFL)

#define         WM_INITDIALOG               0x101
#define         WM_CLOSE                    0x102
#define         WM_DESTROY                  0x103
#define         WM_COMMAND                  0x104

#define         BN_CLICKED                  0

#define         EN_SETFOCUS                 0x100
#define         EN_CHANGE                   0x300
#define         EN_CHANGING                 0x301

#define         MB_ICON_INFORMATION         0x40
#define         MB_ICON_QUESTION            0x30
#define         MB_ICON_ERROR               0x20
#define         MB_ICON_WARNING             0x10
#define         MB_OK                       0
#define         MB_YESNO                    0x0A
#define         MB_OKCANCEL                 0x0B
#define         MB_CANCEL                   0x0C
#define         MB_CLOSE                    0x0D
#define         MB_NONE                     0x0E


#include "ccontrols.h"
#include "cdialog.h"

#include "dialogs/ctestdlg.h"

#endif // STD_H_INCLUDED
