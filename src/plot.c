/**
 ******************************************************************************
 * @file 	plot.c
 * @author 	Can Gulmez
 * @brief 	Plotting sample operations of DSP.
 * 
 ******************************************************************************
 * @attention
 * 
 * Copyright (c) 2026 Can Gulmez.
 * All rights reserved.
 * 
 * This software is licensed under the MIT License.
 * 
 ******************************************************************************
 */

#include "dsp.h"

void __plot_frame(cairo_t *cr, int width, int height)
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

void __plot_grid(cairo_t *cr, int width, int height)
{
	int i;
	double middle;

	/* Draw the plot grid. */
	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
	cairo_set_line_width(cr, 0.5);

	for (i = PLOT_MARGIN + PLOT_GRID; i < width - (PLOT_MARGIN / 2);
		  i += PLOT_GRID)	/* put the vertical grid lines */
	{
		cairo_move_to(cr, i,	PLOT_MARGIN / 2);
		cairo_line_to(cr, i, height - PLOT_MARGIN);
	}
	cairo_stroke(cr);

	for (i = (PLOT_MARGIN / 2) + PLOT_GRID; i < height - PLOT_MARGIN;
		  i += PLOT_GRID)	/* put the horizontal grid lines */
	{
		cairo_move_to(cr, PLOT_MARGIN, i);
		cairo_line_to(cr, width - (PLOT_MARGIN / 2), i);
	}
	cairo_stroke(cr);	

	/* Draw the middle bold horizontal grid line. */
	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
	cairo_set_line_width(cr, 2.0);

	middle = (PLOT_MARGIN / 2) + ((height - (PLOT_MARGIN * 3 / 2)) / 2);
	cairo_move_to(cr, PLOT_MARGIN, middle);
	cairo_line_to(cr, width - PLOT_MARGIN / 2, middle);
	cairo_stroke(cr);	
}

void __plot_label_x(cairo_t *cr, DspPlot *plot, int width, int height)
{
	int temp;
	char buffer[64];
	
	/* Put the x-axis label. */
	cairo_set_source_rgb(cr,	0.0, 0.0, 0.0);
	cairo_select_font_face(cr, "Sand", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 15.0);

	snprintf(buffer, 64, "Sample Data (%d-points)", plot->sample->length);

	temp = ((width - PLOT_MARGIN * 3 / 2) / 2) - strlen(buffer);
	cairo_move_to(cr, temp, height - PLOT_MARGIN + 20);
	cairo_show_text(cr, buffer);
}

void __plot_label_y(cairo_t *cr, int width, int height)
{
	char buffer[64];
	int temp;

	/* Put the y-axis label. */
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_select_font_face(cr, "Sand", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 15.0);

	cairo_save(cr);

	snprintf(buffer, 64, "Amplitude");
	temp = ((height + PLOT_MARGIN * 3 / 2) / 2) + (strlen(buffer) / 2);

	cairo_move_to(cr, (PLOT_MARGIN - 10), temp);
	cairo_rotate(cr, -M_PI / 2);
	cairo_show_text(cr, buffer);

	cairo_restore(cr);
}

void __plot_indices(cairo_t *cr, DspPlot *plot, int width, int height)
{
	double max, min;
	char buffer[16];
	DspTime *sample;

	sample = plot->sample;

	/* Put the ranges. */
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_select_font_face(cr, "Sand", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 15.0);

	/* Get the middle point of plot. */
	max = dsp_time_max(sample);
	min = dsp_time_min(sample);

	/* Put the upper amplitude value. */
	memset(buffer, 0, 16);
	snprintf(buffer, 16, "%.4f (max)", max);
	cairo_move_to(cr, (PLOT_MARGIN + 4), (PLOT_MARGIN / 2 + 15));
	cairo_show_text(cr, buffer);

	/* Put the lower amplitude value. */
	memset(buffer, 0, 16);
	snprintf(buffer, 16, "%.4f (min)", min);
	cairo_move_to(cr, (PLOT_MARGIN + 4), (height - PLOT_MARGIN - 6));
	cairo_show_text(cr, buffer);

	/* Put the lowest index. */
	memset(buffer, 0, 16);
	snprintf(buffer, 16, "%d", 0);
	cairo_move_to(cr, PLOT_MARGIN, (height - PLOT_MARGIN + 15));
	cairo_show_text(cr, buffer);

	/* Put the highest index. */
	memset(buffer, 0, 16);
	snprintf(buffer, 16, "%d", sample->length);
	cairo_move_to(cr, width-PLOT_MARGIN, (height - PLOT_MARGIN + 15));
	cairo_show_text(cr, buffer);
}

void __plot_color_to_rgb(DspColor color, double rgb[3])
{
	switch (color)
	{
		case DSP_COLOR_BLACK:	rgb[0] = 0.0;	rgb[1] = 0.0;	rgb[2] = 0.0;	break;
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

void __plot_sample_data(cairo_t *cr, DspPlot *plot, int width, int height)
{
	int i;
	double middle, step;
	DspTime *sample, copied;
	DspColor  color;
	double rgb[3];

	sample = plot->sample;
	__plot_color_to_rgb(plot->color, rgb);

	/* Draw the sample data. */
	cairo_set_source_rgb(cr, rgb[0], rgb[1], rgb[2]);
	cairo_set_line_width(cr, plot->width);

	middle = (height - (PLOT_MARGIN * 3 / 2)) / 2;
	step = (width - (PLOT_MARGIN * 3/2)) / (sample->length - 1);

	/* Scale the sample amplitudes between [-middle, middle]. */
	dsp_time_scale(sample, middle - 10, &copied);

	cairo_move_to(cr, PLOT_MARGIN + 2, middle + (PLOT_MARGIN / 2));
	for (i = 1; i <= sample->length; i++) 
	{
		cairo_line_to(cr, PLOT_MARGIN + (step * i) + 2, 
			middle + (PLOT_MARGIN / 2) - copied.data[i - 1]);
	}
	cairo_stroke(cr);
}

void __plot_drawing_area(GtkDrawingArea *area, cairo_t *cr, int width, 
							 	 int height, gpointer data)
{
	int i;
	DspPlot *plot;
	DspTime *sample;

	/* Get the plotting features. */
	plot = (DspPlot *) data;

	/* Set the background. */
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	cairo_paint(cr);

	__plot_frame(cr, width, height);
	__plot_grid(cr, width, height);
	__plot_label_x(cr, plot, width, height);	
	__plot_label_y(cr, width, height);
	__plot_indices(cr, plot, width, height);
	__plot_sample_data(cr, plot, width, height);
}

void __plot_activate(GtkApplication *app, gpointer data)
{
	GtkWidget *window;
	DspPlot *plot;
	GtkWidget *area;
	const char *title;

	/* Get the plot name. */
	plot = (DspPlot *) data;
	title = plot->title;

	/* Create a new window and set common utilities. */
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), title);
	gtk_window_maximize(GTK_WINDOW(window));

	/* Plut the plotting widget. */
	area = gtk_drawing_area_new();
	gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), __plot_drawing_area, 
		(gpointer) data, NULL);

	gtk_widget_set_hexpand(area, TRUE);
	gtk_widget_set_vexpand(area, TRUE);

	/* Present the window. */
	gtk_window_set_child(GTK_WINDOW(window), area);
	gtk_window_present(GTK_WINDOW(window));
}

/*****************************************************************************/
/*****************************************************************************/

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
	g_signal_connect(app, "activate", G_CALLBACK(__plot_activate), (gpointer) plot);
	
	/* Run the application and then return the status. */
	status = g_application_run(G_APPLICATION(app), 0, 0);
	g_object_unref(app);

	return status;
}
