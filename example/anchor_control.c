#include "example.h"

typedef enum {
    ANCHOR_EDGE_LEFT = 0,
    ANCHOR_EDGE_RIGHT,
    ANCHOR_EDGE_TOP,
    ANCHOR_EDGE_BOTTOM,
    ANCHOR_EDGE_ENUM_COUNT,
} AnchorEdge;

void (*const anchor_edge_setters[]) (GtkWindow *window, gboolean anchor) = {
    gtk_layer_set_anchor_left,
    gtk_layer_set_anchor_right,
    gtk_layer_set_anchor_top,
    gtk_layer_set_anchor_bottom,
};

typedef struct {
    gboolean edges[ANCHOR_EDGE_ENUM_COUNT];
    WindowOrientation orientation;
    GtkWindow *layer_window;
} AnchorEdges;

typedef struct {
    AnchorEdge edge;
    AnchorEdges *window_edges;
} AnchorButtonData;

static void
anchor_edges_update_orientation (AnchorEdges *edges)
{
    gboolean horiz = edges->edges[ANCHOR_EDGE_LEFT] == edges->edges[ANCHOR_EDGE_RIGHT];
    gboolean vert = edges->edges[ANCHOR_EDGE_TOP] == edges->edges[ANCHOR_EDGE_BOTTOM];
    WindowOrientation orientation = WINDOW_ORIENTATION_NONE;
    if (horiz && (!vert || (edges->edges[ANCHOR_EDGE_LEFT] && !edges->edges[ANCHOR_EDGE_TOP]))) {
        orientation = WINDOW_ORIENTATION_HORIZONTAL;
    } else if (vert && (!horiz || (edges->edges[ANCHOR_EDGE_TOP] && !edges->edges[ANCHOR_EDGE_LEFT]))) {
        orientation = WINDOW_ORIENTATION_VERTICAL;
    }
    if (orientation != edges->orientation) {
        edges->orientation = orientation;
        g_signal_emit_by_name(edges->layer_window, "orientation-changed", orientation);
    }
}

static void
on_anchor_toggled (GtkToggleButton *button, AnchorButtonData *data)
{
    gboolean is_anchored = gtk_toggle_button_get_active (button);
    data->window_edges->edges[data->edge] = is_anchored;
    anchor_edges_update_orientation (data->window_edges);
    g_return_if_fail (data->edge < sizeof (anchor_edge_setters) / sizeof (anchor_edge_setters[0]));
    anchor_edge_setters[data->edge] (data->window_edges->layer_window, is_anchored);
}

static GtkWidget *
anchor_edge_button_new (AnchorEdges *window_edges, AnchorEdge edge, const char *icon_name, const char *tooltip)
{
    GtkWidget *button = gtk_toggle_button_new ();
    gtk_button_set_image (GTK_BUTTON (button), gtk_image_new_from_icon_name (icon_name, GTK_ICON_SIZE_BUTTON));
    gtk_widget_set_tooltip_text (button, tooltip);
    AnchorButtonData *data = g_new0 (AnchorButtonData, 1);
    *data = (AnchorButtonData) {
        .edge = edge,
        .window_edges = window_edges,
    };
    g_signal_connect_data (button,
                           "clicked",
                           G_CALLBACK (on_anchor_toggled),
                           data,
                           (GClosureNotify)g_free,
                           (GConnectFlags)0);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), data->window_edges->edges[data->edge]);
    return button;
}

GtkWidget *
anchor_control_new (GtkWindow *layer_window,
                    gboolean default_left,
                    gboolean default_right,
                    gboolean default_top,
                    gboolean default_bottom)
{
    AnchorEdges *anchor_edges = g_new0 (AnchorEdges, 1);
    anchor_edges->edges[ANCHOR_EDGE_LEFT] = default_left;
    anchor_edges->edges[ANCHOR_EDGE_RIGHT] = default_right;
    anchor_edges->edges[ANCHOR_EDGE_TOP] = default_top;
    anchor_edges->edges[ANCHOR_EDGE_BOTTOM] = default_bottom;
    anchor_edges->layer_window = layer_window;
    anchor_edges->orientation = -1; // invalid value will force anchor_edges_update_orientation to update
    anchor_edges_update_orientation (anchor_edges);
    // This is never accessed, but is set for memeory management
    g_object_set_data_full (G_OBJECT (layer_window), "anchor_edges", anchor_edges, g_free);

    GtkWidget *outside_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    {
        GtkWidget *hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
        gtk_box_pack_start (GTK_BOX (outside_hbox), hbox, TRUE, FALSE, 0);
        {
            GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
            gtk_container_add (GTK_CONTAINER (hbox), vbox);
            {
                GtkWidget *button = anchor_edge_button_new (anchor_edges, ANCHOR_EDGE_LEFT, "go-first", "Anchor left");
                gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, FALSE, 0);
            }
        }{
            GtkWidget *center_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 16);
            gtk_container_add (GTK_CONTAINER (hbox), center_vbox);
            {
                GtkWidget *button = anchor_edge_button_new (anchor_edges, ANCHOR_EDGE_TOP, "go-top", "Anchor top");
                gtk_box_pack_start (GTK_BOX (center_vbox), button, FALSE, FALSE, 0);
            }{
                GtkWidget *button = anchor_edge_button_new (anchor_edges, ANCHOR_EDGE_BOTTOM, "go-bottom", "Anchor bottom");
                gtk_box_pack_end (GTK_BOX (center_vbox), button, FALSE, FALSE, 0);
            }
        }{
            GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
            gtk_container_add (GTK_CONTAINER (hbox), vbox);
            {
                GtkWidget *button = anchor_edge_button_new (anchor_edges, ANCHOR_EDGE_RIGHT, "go-last", "Anchor right");
                gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, FALSE, 0);
            }
        }
    }

    return outside_hbox;
}