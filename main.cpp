
#include "h/std.h"


int main (int argc, char *argv[])
{
    // Initialize GTK+
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
    gtk_init (&argc, &argv);
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

    CTestDlg dlg;
    dlg.Show(NULL);
    return 0;
}


