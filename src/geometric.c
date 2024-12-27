#include <math.h>
#include <stdlib.h>

typedef struct {
    double x, y;
} Point;

typedef struct {
    Point center;
    double radius;
} Circle;

// Global storage
static Point points[16];  // All construction points
static Circle circles[2]; // Circle A and B

void geometric_construct(void) {
    double radius = 1.0;
    
    // Initialize circles
    circles[0] = (Circle){{0, 0}, radius};     // Circle A at origin
    circles[1] = (Circle){{radius, 0}, radius}; // Circle B at (r,0)
    
    // Find circle intersections (vesica piscis)
    double h = radius / 2;  // half the distance between centers
    double a = sqrt(radius * radius - h * h);  // height to intersection point
    points[0] = (Point){h, a};    // P1
    points[1] = (Point){h, -a};   // P2
    
    // Using compass to mark equal distances
    points[2] = (Point){-h, a};   // P3 (mirror of P1)
    points[3] = (Point){3*h, a};  // P4 (extend P1 by radius)
    
    points[4] = (Point){-h, -a};  // P5 (mirror of P2)
    points[5] = (Point){3*h, -a}; // P6 (extend P2 by radius)
    
    // Find C1, C2 (intersection of OP4 line with Circle A)
    // Scale factor to reach circle A from origin through P4
    double scale = radius / sqrt(points[3].x * points[3].x + points[3].y * points[3].y);
    points[6] = (Point){points[3].x * scale, points[3].y * scale};     // C1
    points[7] = (Point){-points[3].x * scale, -points[3].y * scale};   // C2
    
    // Find C3, C4 (intersection of OP6 line with Circle A)
    scale = radius / sqrt(points[5].x * points[5].x + points[5].y * points[5].y);
    points[8] = (Point){points[5].x * scale, points[5].y * scale};     // C3
    points[9] = (Point){-points[5].x * scale, -points[5].y * scale};   // C4
}

double* get_points(void) {
    return (double*)points;
}

double* get_circles(void) {
    return (double*)circles;
} 