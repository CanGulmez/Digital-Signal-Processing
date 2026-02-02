/**
 ******************************************************************************
 * @file 	plot.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Plotting operations of DSP.
 * 
 ******************************************************************************
 * @attention
 * 
 * Copyright (c) 2026 Ahmet Can GULMEZ.
 * All rights reserved.
 * 
 * This software is licensed under the MIT License.
 * 
 ******************************************************************************
 */

#include "./dsp.h"

/**
 * Put the plot frame.
 */
void __plot_frame(GtkDrawingArea *area, cairo_t *cr, int width, int height)
{
	/* Draw the plot frame. */
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_set_line_width(cr, 3.0);

	cairo_move_to(cr, PLOT_MARGIN, PLOT_MARGIN / 2);
	cairo_line_to(cr, PLOT_MARGIN, height - PLOT_MARGIN);
	cairo_move_to(cr, PLOT_MARGIN, height - PLOT_MARGIN);
	cairo_line_to(cr, width - PLOT_MARGIN / 2, height - PLOT_MARGIN);
	cairo_move_to(cr, width - PLOT_MARGIN / 2, height - PLOT_MARGIN);
	cairo_line_to(cr, width - PLOT_MARGIN / 2, PLOT_MARGIN / 2);
	cairo_move_to(cr, width - PLOT_MARGIN / 2, PLOT_MARGIN / 2);
	cairo_line_to(cr, PLOT_MARGIN, PLOT_MARGIN / 2);
	cairo_stroke(cr);
}

/**
 * Put the grid of frame.
 */
void __plot_grid(GtkDrawingArea *area, cairo_t *cr, int width, int height)
{
	int i;
	double middle;

	/* Draw the plot grid. */
	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
	cairo_set_line_width(cr, 0.5);

	for (i = PLOT_MARGIN + PLOT_GRID;
		  i < width - (PLOT_MARGIN / 2);
		  i += PLOT_GRID)	/* put the vertical grid lines */
	{
		cairo_move_to(cr, i,	PLOT_MARGIN / 2);
		cairo_line_to(cr, i, height - PLOT_MARGIN);
	}
	cairo_stroke(cr);

	for (i = (PLOT_MARGIN / 2) + PLOT_GRID;
		  i < height - PLOT_MARGIN;
		  i += PLOT_GRID)	/* put the horizontal grid lines */
	{
		cairo_move_to(cr, PLOT_MARGIN, i);
		cairo_line_to(cr, width - (PLOT_MARGIN / 2), i);
	}
	cairo_stroke(cr);	

	/* Draw the middle bold horizontal grid line. */
	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
	cairo_set_line_width(cr, 2.0);

	middle = (PLOT_MARGIN/2) + ((height - (PLOT_MARGIN*3/2)) / 2);
	cairo_move_to(cr, PLOT_MARGIN, middle);
	cairo_line_to(cr, width - PLOT_MARGIN / 2, middle);
	cairo_stroke(cr);	
}

/**
 * Plot the x-axis label.
 */
void __plot_label_x(GtkDrawingArea *area, cairo_t *cr, len_t length, 
						  int width, int height)
{
	int temp;
	char buffer[64];
	
	/* Put the x-axis label. */
	cairo_set_source_rgb(cr,	0.0, 0.0, 0.0);
	cairo_select_font_face(cr, "Sand", CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 15.0);

	snprintf(buffer, 64, "Sample Data (%d-points)", length);

	temp = ((width - PLOT_MARGIN * 3 / 2) / 2) - strlen(buffer);
	cairo_move_to(cr, temp, height - PLOT_MARGIN + 20);
	cairo_show_text(cr, buffer);
}

/**
 * Plot the y-axis label.
 */
void __plot_label_y(GtkDrawingArea *area, cairo_t *cr, int width, 
						  int height)
{
	char buffer[64];
	int temp;

	/* Put the y-axis label. */
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_select_font_face(cr, "Sand", CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 15.0);

	cairo_save(cr);

	snprintf(buffer, 64, "Amplitude");
	temp = ((height + PLOT_MARGIN * 3 / 2) / 2) + (strlen(buffer) / 2);

	cairo_move_to(cr, (PLOT_MARGIN - 10), temp);
	cairo_rotate(cr, -M_PI / 2);
	cairo_show_text(cr, buffer);

	cairo_restore(cr);
}

void __plot_indices(GtkDrawingArea *area, cairo_t *cr, 
						  DspPlot *plot, int width, int height)
{
	int max_value, min_value;
	char buffer[16];
	DspTime *sample;
	DspTime *copied;

	sample = plot->sample;

	/* Put the ranges. */
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_select_font_face(cr, "Sand", CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 15.0);

	/* Get the middle point of plot. */
	max_value = (int) dsp_time_max(sample);
	min_value = (int) dsp_time_min(sample);

	/* Put the upper amplitude value. */
	memset(buffer, 0, 16);
	snprintf(buffer, 16, "%d (max)", max_value);
	cairo_move_to(cr, (PLOT_MARGIN + 4), (PLOT_MARGIN / 2 + 15));
	cairo_show_text(cr, buffer);

	/* Put the lower amplitude value. */
	memset(buffer, 0, 16);
	snprintf(buffer, 16, "%d (min)", min_value);
	cairo_move_to(cr, (PLOT_MARGIN + 4), (height-PLOT_MARGIN - 5));
	cairo_show_text(cr, buffer);

	/* Put the lowest index. */
	memset(buffer, 0, 16);
	snprintf(buffer, 16, "%d", 0);
	cairo_move_to(cr, PLOT_MARGIN, (height-PLOT_MARGIN + 15));
	cairo_show_text(cr, buffer);

	/* Put the highest index. */
	memset(buffer, 0, 16);
	snprintf(buffer, 16, "%d", sample->length);
	cairo_move_to(cr, width-PLOT_MARGIN, (height-PLOT_MARGIN + 15));
	cairo_show_text(cr, buffer);
}

/**
 * Convert the data line color to rgb format.
 */
void __plot_color_to_rgb(DspColor  color, double rgb[3])
{
	switch (color)
	{
		case DSP_COLOR_BLACK:	rgb[0] = 0.0;	rgb[1] = 0.0;	rgb[2] = 0.0;	break;
		case DSP_COLOR_WHITE:	rgb[0] = 1.0;	rgb[1] = 1.0;	rgb[2] = 1.0;	break;
		case DSP_COLOR_RED:		rgb[0] = 1.0;	rgb[1] = 0.0;	rgb[2] = 0.0;	break;
		case DSP_COLOR_GREEN:	rgb[0] = 0.0;	rgb[1] = 1.0;	rgb[2] = 0.0;	break;
		case DSP_COLOR_BLUE:		rgb[0] = 0.0;	rgb[1] = 0.0;	rgb[2] = 1.0;	break;
		case DSP_COLOR_YELLOW:	rgb[0] = 1.0;	rgb[1] = 1.0;	rgb[2] = 0.0;	break;
		case DSP_COLOR_MAGENTA:	rgb[0] = 1.0;	rgb[1] = 0.0;	rgb[2] = 1.0;	break;
		case DSP_COLOR_CYAN:		rgb[0] = 0.0;	rgb[1] = 1.0;	rgb[2] = 1.0;	break;
		case DSP_COLOR_ORANGE:	rgb[0] = 1.0;	rgb[1] = 0.5; 	rgb[2] = 0.0;	break;
		case DSP_COLOR_PINK:		rgb[0] = 1.0;	rgb[1] = 0.75;	rgb[2] = 0.8;	break;
		case DSP_COLOR_BROWN: 	rgb[0] = 0.65;	rgb[1] = 0.16;	rgb[2] = 0.16;	break;
		case DSP_COLOR_PURPLE: 	rgb[0] = 0.5; 	rgb[1] = 0.0; 	rgb[2] = 0.5;	break;
		case DSP_COLOR_SILVER: 	rgb[0] = 0.75; rgb[1] = 0.75; rgb[2] = 0.75; break;
		case DSP_COLOR_GOLD: 	rgb[0] = 1.0; 	rgb[1] = 0.84; rgb[2] = 0.0;	break;
	}
}

/**
 * Draw the sample data itself.
 */
void __plot_sample_data(GtkDrawingArea *area, cairo_t *cr, 
								DspPlot *plot, int width, int height)
{
	int i;
	double middle, step;
	DspTime *sample;
	double linewidth;
	DspColor  color;
	double rgb[3];
	DspTime copied;

	sample = plot->sample;
	linewidth = plot->width;
	color = plot->color;
	__plot_color_to_rgb(color, rgb);

	/* Draw the sample data. */
	cairo_set_source_rgb(cr, rgb[0], rgb[1], rgb[2]);
	cairo_set_line_width(cr, linewidth);

	middle = (height - (PLOT_MARGIN * 3 / 2)) / 2;
	step = (width - (PLOT_MARGIN * 3/2)) / (sample->length - 1);

	/* Scale the sample amplitudes between [-middle, middle]. */
	dsp_time_scale(sample, middle - 10, sample);

	cairo_move_to(cr, PLOT_MARGIN + 2, middle + (PLOT_MARGIN / 2));
	for (i = 1; i <= sample->length; i++) 
	{
		cairo_line_to(cr, PLOT_MARGIN + (step * i) + 2, 
			middle + (PLOT_MARGIN / 2) - sample->data[i - 1]);
	}
	cairo_stroke(cr);
}

/**
 * Draw the plot itself.
 */
void __plot_drawing(GtkDrawingArea *area, cairo_t *cr, int width, 
							 int height, gpointer data)
{
	int i;
	DspPlot *plot;
	DspTime *sample;
	double step, middle, value;
	len_t length;

	/* Get the plotting features. */
	plot = (DspPlot *)data;
	sample = plot->sample;
	length = sample->length;

	/* Set the background. */
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	cairo_paint(cr);

	__plot_frame(area, cr, width, height);
	__plot_grid(area, cr, width, height);
	__plot_label_x(area, cr, length, width, height);	
	__plot_label_y(area, cr, width, height);
	__plot_indices(area, cr, plot, width, height);
	__plot_sample_data(area, cr, plot, width, height);
}

/**
 * Save the cairo plot as PNG image.
 */
void __plot_button_clicked(GtkWidget *button, gpointer data)
{
	GtkWindow *window;
	GtkWidget *area;
	DspPlot *plotting;
	int width, height;
	cairo_surface_t *surface;
	cairo_t *cr;
	time_t now;
	struct tm *t;
	char filepath[128];
	char hostname[8];

	gethostname(hostname, 8);
	/* Get the broken-dowm date to make unique paths. */
	now = time(NULL);
	t = localtime(&now);

	snprintf(filepath, 128, 
		"/home/%s/Downloads/sample_data_%d%d%d_%d%d%d.png",
		hostname, t->tm_year, t->tm_mon, t->tm_mday, t->tm_hour,
		t->tm_min, t->tm_sec);

	/* Get the required widgets and structures. */
	window = GTK_WINDOW(data);
	area = gtk_window_get_child(window);
	plotting = (DspPlot *) g_object_get_data(G_OBJECT(button), 
		"plot-data");

	if (area && GTK_IS_DRAWING_AREA(area))
	{
		/* Get the width and height values of current window. */
		width = gtk_widget_get_width(area);
		height = gtk_widget_get_height(area);

		/* Create a surface and then draw onto it. */
		surface = cairo_image_surface_create(
			CAIRO_FORMAT_ARGB32, width, height
		);
		cr = cairo_create(surface);
		__plot_drawing(NULL, cr, width, height, plotting);

		/* Save the plot as PNG. */
		cairo_surface_write_to_png(surface, filepath);

		cairo_destroy(cr);
		cairo_surface_destroy(surface);
	}
}

/**
 * Activate the plotting application fully.
 */
void __plot_activate(GtkApplication *app, gpointer data)
{
	GtkWidget *window, *headerbar;
	DspPlot *plot;
	GtkWidget *area;
	len_t width;
	const char *title;
	DspTime *sample;
	GtkWidget *button;
	int factor;

	/* Get the plot structure. */
	plot = (DspPlot *) data;

	/* Validate the inputs. */
	assert_plot(plot);

	/* Get the plot properties. */
	factor = plot->factor;
	title = plot->title;
	sample = plot->sample;
	width = (len_t) sample->length * factor + (PLOT_MARGIN*3/2) + 8;
	width = (width < PLOT_WIDTH) ? PLOT_WIDTH : width;

	/* Create a new window and set common utilities. */
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), title);
	gtk_window_set_default_size(GTK_WINDOW(window), width, PLOT_HEIGHT);

	/* Put the header bar. */
	headerbar = gtk_header_bar_new();
	gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(headerbar), TRUE);
	gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);

	/* Plut the plotting widget. */
	area = gtk_drawing_area_new();
	gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area),
		__plot_drawing, (gpointer) plot, NULL);

	gtk_widget_set_hexpand(area, TRUE);
	gtk_widget_set_vexpand(area, TRUE);

	/* Put the buttons. */
	button = gtk_button_new_from_icon_name("document-save-as-symbolic");
	g_object_set_data(G_OBJECT(button), "plot-data", (gpointer) plot);
	
	g_signal_connect(button, "clicked", G_CALLBACK(__plot_button_clicked),
		(gpointer) window);

	gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), button);
	gtk_widget_set_tooltip_text(button, "Save As PNG");

	/* Put toast overlay to notify the saved PNG image. */

	/* Present the window. */
	gtk_window_set_child(GTK_WINDOW(window), area);
	gtk_window_present(GTK_WINDOW(window));
}

/**
 * Plot the `plotting` features which include the sample
 * data itself and some metadata in time domain.
 */
int dsp_plot_sample(const DspPlot *plot)
{
	int status;
	GtkApplication *app;

	/* Create a new application then link it with activation function. */
	app = gtk_application_new(PLOT_ID, G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(__plot_activate), 
		(gpointer) plot);
	
	/* Run the application and then return the status. */
	status = g_application_run(G_APPLICATION(app), 0, 0);
	g_object_unref(app);

	return status;
}
