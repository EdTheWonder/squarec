#include <X11/Xlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define WINDOW_SIZE 800
#define SCALE 200

extern void geometric_construct(void);
extern double* get_points(void);
extern double* get_circles(void);

void draw_circle(Display *display, Window window, GC gc, int x, int y, int radius) {
    XDrawArc(display, window, gc,
             WINDOW_SIZE/2 + (x - radius) * SCALE,
             WINDOW_SIZE/2 - (y + radius) * SCALE,
             radius * 2 * SCALE,
             radius * 2 * SCALE,
             0, 360 * 64);
}

void draw_point(Display *display, Window window, GC gc, double x, double y) {
    int screen_x = WINDOW_SIZE/2 + x * SCALE;
    int screen_y = WINDOW_SIZE/2 - y * SCALE;
    XFillArc(display, window, gc,
             screen_x - 3, screen_y - 3,
             6, 6, 0, 360 * 64);
}

void draw_line(Display *display, Window window, GC gc, double x1, double y1, double x2, double y2) {
    int screen_x1 = WINDOW_SIZE/2 + x1 * SCALE;
    int screen_y1 = WINDOW_SIZE/2 - y1 * SCALE;
    int screen_x2 = WINDOW_SIZE/2 + x2 * SCALE;
    int screen_y2 = WINDOW_SIZE/2 - y2 * SCALE;
    XDrawLine(display, window, gc, screen_x1, screen_y1, screen_x2, screen_y2);
}

void draw_arc(Display *display, Window window, GC gc, double cx, double cy, double radius, 
              double start_angle, double end_angle) {
    int screen_x = WINDOW_SIZE/2 + (cx - radius) * SCALE;
    int screen_y = WINDOW_SIZE/2 - (cy + radius) * SCALE;
    XDrawArc(display, window, gc,
             screen_x, screen_y,
             radius * 2 * SCALE, radius * 2 * SCALE,
             start_angle * 64, (end_angle - start_angle) * 64);
}

void draw_label(Display *display, Window window, GC gc, double x, double y, char *text) {
    int screen_x = WINDOW_SIZE/2 + x * SCALE;
    int screen_y = WINDOW_SIZE/2 - y * SCALE;
    XDrawString(display, window, gc, screen_x + 5, screen_y - 5, text, strlen(text));
}

// Function to calculate intersection point of two lines
void line_intersection(double x1, double y1, double x2, double y2,
                     double x3, double y3, double x4, double y4,
                     double *ix, double *iy) {
    double denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    *ix = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denominator;
    *iy = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denominator;
}

int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) return 1;
    
    Window window = XCreateSimpleWindow(display,
                                      DefaultRootWindow(display),
                                      0, 0, WINDOW_SIZE, WINDOW_SIZE,
                                      1,
                                      BlackPixel(display, 0),
                                      WhitePixel(display, 0));
    
    XMapWindow(display, window);
    GC gc = XCreateGC(display, window, 0, NULL);
    XSelectInput(display, window, ExposureMask | ButtonPressMask);
    
    geometric_construct();
    double *points = get_points();
    double *circles = get_circles();
    
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            // Draw initial circles
            draw_circle(display, window, gc, circles[0], circles[1], circles[2]);
            draw_circle(display, window, gc, circles[3], circles[4], circles[5]);
            
            // Draw construction points
            for (int i = 0; i < 10; i++) {
                draw_point(display, window, gc, points[i*2], points[i*2+1]);
            }
            
            // Calculate extension factor
            double extension = 2.0;  // Extend lines by 2x their length
            
            // Draw the parallel lines that form the square (extended)
            // P3 to P1 extended
            double dx = points[0*2] - points[2*2];
            double dy = points[0*2+1] - points[2*2+1];
            draw_line(display, window, gc, 
                points[2*2] - dx * extension, points[2*2+1] - dy * extension,
                points[0*2] + dx * extension, points[0*2+1] + dy * extension);
            
            // P5 to P2 extended
            dx = points[1*2] - points[4*2];
            dy = points[1*2+1] - points[4*2+1];
            draw_line(display, window, gc, 
                points[4*2] - dx * extension, points[4*2+1] - dy * extension,
                points[1*2] + dx * extension, points[1*2+1] + dy * extension);
            
            // C1 to C3 extended
            dx = points[8*2] - points[6*2];
            dy = points[8*2+1] - points[6*2+1];
            draw_line(display, window, gc, 
                points[6*2] - dx * extension, points[6*2+1] - dy * extension,
                points[8*2] + dx * extension, points[8*2+1] + dy * extension);
            
            // C2 to C4 extended
            dx = points[9*2] - points[7*2];
            dy = points[9*2+1] - points[7*2+1];
            draw_line(display, window, gc, 
                points[7*2] - dx * extension, points[7*2+1] - dy * extension,
                points[9*2] + dx * extension, points[9*2+1] + dy * extension);
            
            // Calculate square vertices (intersection points)
            double ix1, iy1, ix2, iy2, ix3, iy3, ix4, iy4;
            
            // Calculate intersection points
            // Vertex A (P3P1 intersects C1C3)
            line_intersection(points[2*2], points[2*2+1], points[0*2], points[0*2+1],
                            points[6*2], points[6*2+1], points[8*2], points[8*2+1],
                            &ix1, &iy1);
                            
            // Vertex B (P3P1 intersects C2C4)
            line_intersection(points[2*2], points[2*2+1], points[0*2], points[0*2+1],
                            points[7*2], points[7*2+1], points[9*2], points[9*2+1],
                            &ix2, &iy2);
                            
            // Vertex C (P5P2 intersects C2C4)
            line_intersection(points[4*2], points[4*2+1], points[1*2], points[1*2+1],
                            points[7*2], points[7*2+1], points[9*2], points[9*2+1],
                            &ix3, &iy3);
                            
            // Vertex D (P5P2 intersects C1C3)
            line_intersection(points[4*2], points[4*2+1], points[1*2], points[1*2+1],
                            points[6*2], points[6*2+1], points[8*2], points[8*2+1],
                            &ix4, &iy4);
            
            // Draw just the square
            draw_line(display, window, gc, ix1, iy1, ix2, iy2);  // A to B
            draw_line(display, window, gc, ix2, iy2, ix3, iy3);  // B to C
            draw_line(display, window, gc, ix3, iy3, ix4, iy4);  // C to D
            draw_line(display, window, gc, ix4, iy4, ix1, iy1);  // D to A
            
            // Label the vertices
            draw_label(display, window, gc, ix1, iy1, "A");
            draw_label(display, window, gc, ix2, iy2, "B");
            draw_label(display, window, gc, ix3, iy3, "C");
            draw_label(display, window, gc, ix4, iy4, "D");
        }
        if (event.type == ButtonPress)
            break;
    }
    
    XCloseDisplay(display);
    return 0;
} 