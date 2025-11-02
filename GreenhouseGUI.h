#ifndef GREENHOUSEGUI_H
#define GREENHOUSEGUI_H

#include <gtk/gtk.h>
#include "GreenhouseManager.h"

class GreenhouseGUI {
private:
    GreenhouseManager* manager;
    GtkWidget* window;
    GtkWidget* notebook;
    gboolean simulationRunning;
    guint simulationTimerId;

public:
    GreenhouseGUI(GreenhouseManager* mgr);
    ~GreenhouseGUI();
    void run();

private:
    void createMainWindow();
    void createStructureTab();
    void createMonitorTab();
    void createOrdersTab();
    void createStaffTab();
    void createSimulationTab();
    
    static void onDestroy(GtkWidget* widget, gpointer data);
};

#endif
