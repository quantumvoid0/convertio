#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#define NUM_UNITS 18

const char *unit_names[NUM_UNITS] = {
    "kilometer", "hectometer", "decameter", "meter", "decimeter", "centimeter", "millimeter",
    "micrometer", "nanometer", "picometer", "mile", "yard", "foot", "inch",
    "nautical mile", "astronomical unit", "light year", "angstrom"
};

struct widgets {
    GtkWidget *inp;
    GtkWidget *combo;
    GtkWidget *out;
};

double to_meters(const char *unit, double val) {
    if (strcmp(unit, "kilometer") == 0) return val * 1000;
    if (strcmp(unit, "hectometer") == 0) return val * 100;
    if (strcmp(unit, "decameter") == 0) return val * 10;
    if (strcmp(unit, "meter") == 0) return val;
    if (strcmp(unit, "decimeter") == 0) return val / 10;
    if (strcmp(unit, "centimeter") == 0) return val / 100;
    if (strcmp(unit, "millimeter") == 0) return val / 1000;
    if (strcmp(unit, "micrometer") == 0) return val / 1e6;
    if (strcmp(unit, "nanometer") == 0) return val / 1e9;
    if (strcmp(unit, "picometer") == 0) return val / 1e12;
    if (strcmp(unit, "mile") == 0) return val * 1609.344;
    if (strcmp(unit, "yard") == 0) return val * 0.9144;
    if (strcmp(unit, "foot") == 0) return val * 0.3048;
    if (strcmp(unit, "inch") == 0) return val * 0.0254;
    if (strcmp(unit, "nautical mile") == 0) return val * 1852;
    if (strcmp(unit, "astronomical unit") == 0) return val * 149597870700.0;
    if (strcmp(unit, "light year") == 0) return val * 9.461e15;
    if (strcmp(unit, "angstrom") == 0) return val / 1e10;
    return -1;
}

double from_meters(const char *unit, double meters) {
    if (strcmp(unit, "kilometer") == 0) return meters / 1000;
    if (strcmp(unit, "hectometer") == 0) return meters / 100;
    if (strcmp(unit, "decameter") == 0) return meters / 10;
    if (strcmp(unit, "meter") == 0) return meters;
    if (strcmp(unit, "decimeter") == 0) return meters * 10;
    if (strcmp(unit, "centimeter") == 0) return meters * 100;
    if (strcmp(unit, "millimeter") == 0) return meters * 1000;
    if (strcmp(unit, "micrometer") == 0) return meters * 1e6;
    if (strcmp(unit, "nanometer") == 0) return meters * 1e9;
    if (strcmp(unit, "picometer") == 0) return meters * 1e12;
    if (strcmp(unit, "mile") == 0) return meters / 1609.344;
    if (strcmp(unit, "yard") == 0) return meters / 0.9144;
    if (strcmp(unit, "foot") == 0) return meters / 0.3048;
    if (strcmp(unit, "inch") == 0) return meters / 0.0254;
    if (strcmp(unit, "nautical mile") == 0) return meters / 1852;
    if (strcmp(unit, "astronomical unit") == 0) return meters / 149597870700.0;
    if (strcmp(unit, "light year") == 0) return meters / 9.461e15;
    if (strcmp(unit, "angstrom") == 0) return meters * 1e10;
    return -1;
}

void convert_all(const char *inp_unit, double val, char *buffer) {
    double meters = to_meters(inp_unit, val);
    if (meters < 0) {
        sprintf(buffer, "Unknown unit");
        return;
    }
    char line[128];
    buffer[0] = '\0';
    for (int i = 0; i < NUM_UNITS; i++) {
        if (strcmp(unit_names[i], inp_unit) == 0) continue;
        double converted = from_meters(unit_names[i], meters);
        snprintf(line, sizeof(line), "%.8f %s\n", converted, unit_names[i]);
        strcat(buffer, line);
    }
}

static void on_convert_clicked(GtkButton *button, gpointer user_data) {
    struct widgets *w = (struct widgets *)user_data;
    const char *input_str = gtk_entry_get_text(GTK_ENTRY(w->inp));
    const char *unit_str = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(w->combo));
    if (!input_str || !unit_str || strlen(input_str) == 0) return;
    double val = atof(input_str);
    char result[2048];
    convert_all(unit_str, val, result);
    gtk_label_set_text(GTK_LABEL(w->out), result);
    g_free((gchar *)unit_str);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *window, *combo, *inp, *out, *go;
    builder = gtk_builder_new_from_file("/usr/share/convertio/ui.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
	GtkCssProvider *provider = gtk_css_provider_new();
gtk_css_provider_load_from_path(provider, "/usr/share/convertio/style.css", NULL);
gtk_style_context_add_provider_for_screen(
    gdk_screen_get_default(),
    GTK_STYLE_PROVIDER(provider),
    GTK_STYLE_PROVIDER_PRIORITY_USER
);

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_widget_show_all(window);
    inp = GTK_WIDGET(gtk_builder_get_object(builder, "inp"));
    out = GTK_WIDGET(gtk_builder_get_object(builder, "out"));
    go = GTK_WIDGET(gtk_builder_get_object(builder, "go"));
    combo = GTK_WIDGET(gtk_builder_get_object(builder, "combo"));
    struct widgets *w = g_malloc(sizeof(struct widgets));
    w->inp = inp;
    w->combo = combo;
    w->out = out;
    g_signal_connect(go, "clicked", G_CALLBACK(on_convert_clicked), w);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    app = gtk_application_new("org.calc.conv", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
