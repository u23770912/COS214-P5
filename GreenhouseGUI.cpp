#include "GreenhouseGUI.h"
#include <iostream>

GreenhouseGUI::GreenhouseGUI(GreenhouseManager* mgr) 
    : manager(mgr), simulationRunning(FALSE), simulationTimerId(0) {
    createMainWindow();
}

GreenhouseGUI::~GreenhouseGUI() {
    if (simulationTimerId > 0) {
        g_source_remove(simulationTimerId);
    }
}

void GreenhouseGUI::createMainWindow() {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Greenhouse Management System");
    gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    
    g_signal_connect(window, "destroy", G_CALLBACK(onDestroy), this);
    
    notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(window), notebook);
    
    createStructureTab();
    createMonitorTab();
    createOrdersTab();
    createStaffTab();
    createSimulationTab();
    
    gtk_widget_show_all(window);
}

void GreenhouseGUI::createStructureTab() {
    GtkWidget* label = gtk_label_new("Structure Tab - Greenhouse Hierarchy");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), label, gtk_label_new("Structure"));
}

void GreenhouseGUI::createMonitorTab() {
    GtkWidget* label = gtk_label_new("Monitor Tab - Plant Lifecycles");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), label, gtk_label_new("Monitor"));
}

void GreenhouseGUI::createOrdersTab() {
    GtkWidget* label = gtk_label_new("Orders Tab - Customer Orders");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), label, gtk_label_new("Orders"));
}

void GreenhouseGUI::createStaffTab() {
    GtkWidget* label = gtk_label_new("Staff Tab - Management Dashboard");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), label, gtk_label_new("Staff"));
}

void GreenhouseGUI::createSimulationTab() {
    GtkWidget* label = gtk_label_new("Simulation Tab - Control Panel");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), label, gtk_label_new("Simulation"));
}

void GreenhouseGUI::run() {
    gtk_main();
}

void GreenhouseGUI::onDestroy(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}
