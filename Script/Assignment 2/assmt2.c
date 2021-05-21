/* Engineering Computation - Assignment Two
 * by Harfiyanto Dharma Santoso (772503), October 2016
 * 
 * C Programming is Fun !
 *
 * lineintersect function was adapted in 2012 from 
 * http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/pdb.c
 * (no longer available at that URL in 2013)
 * and was written in the first instance by Paul Bourke
 *
 * Modified for use in a different assignment previously by Alistair Moffat by:
 *   . changing the argument type to two structs type line_t
 *   . making sure result is TRUE if an endpoint is on the other line
 * Modified for use by Jianzhong Qi by:
 *   . testing whether the projections of the two line segments intersect first
 */


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAX_LINE_LEN 100 		/* Max number of characters per line. */
#define CH_WAP 'W'				/* Character W for WAPs. */
#define CH_OB 'P'				/* Character P for points.*/
#define CH_V 'V'				/* Character V for vertexes. */
#define MAX_WAP 99				/* Max number of WAPs. */
#define MAX_POINT 99			/* Max number of points. */
#define MAX_VERTEX 99			/* Max number of vertexess. */
#define GRID_SIZE 78			/* Length of egular square grid's side. */
#define S_THRESHOLD -50			/* Threshold for low signal. */
#define TRUE 1					/* Number representation for TRUE */	
#define FALSE 0					/* Number representation for FALSE */
#define EPS (1e-06)	
#define ABS(x) (fabs(x))
#define MIN(a,b) (a<b ? a:b)	
#define MAX(a,b) (a>b ? a:b)

/* Struct for WAPs. */
typedef struct wap_t {
	double x;
	double y;
	double power;
	double freq;
} wap_t;

/* Struct for points. */
typedef struct point_t {
	double x;
	double y;
} point_t;

/* Struct for lines. */
typedef struct line_t {
	point_t p1;
	point_t p2;
} line_t;

/* Function prototypes. */
int line_intersect(line_t l1, line_t l2);
int calc_square_grid(int side, double min, wap_t waps[], int w);
double calc_dist(double x1, double y1, double x2, double y2);
double calc_strength(double x, double y, wap_t wap);
double fspl(double d, double f);
double max_signal(double x,double y,wap_t waps[], int w);
void print_stage_one(wap_t waps[], int w);
void print_stage_two(point_t points[], int p, wap_t waps[], int w);
void print_stage_three(wap_t waps[], int w);
void print_stage_four(wap_t waps[], int w);
void print_stage_five(point_t vertexes[], line_t segments[], int v, 
					  wap_t waps[], int w);
void write_point(point_t *point, char* line);
void write_wap(wap_t *wap, char* line);
void read_line(wap_t waps[], int *w, point_t points[], int *p, 
	           point_t vertexes[], int *v);
void write_line(char* line, wap_t waps[], int *w, point_t points[], int *p, 
	            point_t vertexes[], int *v);
void vertex_to_segments(point_t vertexes[], line_t segments[], int v);
point_t find_centroid(point_t vertexes[], int v);

/* Main Program. */
int 
main(int argc, char *argv[]) {
	int w = 0, p = 0, v = 0;
	wap_t waps[MAX_WAP];
	point_t points[MAX_POINT];
	point_t vertexes[MAX_VERTEX];
	line_t segments[MAX_VERTEX];

	read_line(waps, &w, points, &p, vertexes, &v);

	print_stage_one(waps, w);
	print_stage_two(points, p, waps, w);
	print_stage_three(waps, w);
	print_stage_four(waps, w);
	print_stage_five(vertexes, segments, v, waps, w);
	return 0;
}

/* Function to count free-space path loss. */
double 
fspl(double d, double f) {
	return ((20 * log10(d)) + (20 * log10(f)) + 32.45);
}

/* Function to calculate the distance between two points */
double
calc_dist(double x1, double y1, double x2, double y2) {
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

/* Function to read the lines. */
void
read_line(wap_t waps[], int *w, point_t points[], int *p, 
	      point_t vertexes[], int *v) {
	char line[MAX_LINE_LEN];
	int c, i, cont = 1;

	while (cont) {
		i = 0;
		while ((c = getchar())!= EOF && c != '\n') {
			if (i < MAX_LINE_LEN) {
				line[i++] = c;
			} else {
				printf("Error, line too long");
				exit(0);
			}
		}
		if (i == 0) {
			cont = 0;
		}
		line[i] = '\0';
		write_line(line, waps, w, points, p, vertexes, v);
	}
}

/* Function to write information in the line accordingly. */
void
write_line(char* line, wap_t waps[], int *w, point_t points[], int *p, 
	       point_t vertexes[], int *v) {
	char label = line[0];
	/* Writing the information as WAP. */
	if (label == CH_WAP) {
		write_wap(&waps[*w], line + 2);
		(*w)++;
	/* Writing the information as point. */
	} else if (label == CH_OB) {
		write_point(&points[*p], line + 2);
		(*p)++;
	/* Writing the information as vertex. */
	} else if (label == CH_V) {
		write_point(&vertexes[*v], line + 2);
		(*v)++;
	}
}

/* Function to write WAP inside the WAPs array. */
void 
write_wap(wap_t *wap, char* line) {
	sscanf(line, "%lf %lf %lf %lf", &wap->x, &wap->y, &wap->power, &wap->freq);
}

/* Function to write point inside the points array. */
void
write_point(point_t *point, char* line) {
	sscanf(line, "%lf %lf", &point->x, &point->y);
}

/* Function to print stage one. */
void
print_stage_one(wap_t waps[], int w) {
	printf("Stage 1\n");
	printf("==========\n");
	printf("Number of WAPs: %02d\n",w);
	printf("Maximum signal strength at (00.0, 00.0): %.2lf dBm\n",
		max_signal(0,0,waps, w));
	printf("\n");
}

/* Function to print stage two. */
void
print_stage_two(point_t pts[], int p, wap_t waps[], int w) {
	printf("Stage 2\n");
	printf("==========\n");
	int i;
	for (i = 0; i < p; i++) {
		printf("Maximum signal strength at (%04.1lf, %04.1lf): %.2lf dBm\n", 
			pts[i].x, pts[i].y, max_signal(pts[i].x, pts[i].y, waps, w));
	}
	printf("\n");
}

/* Function to print stage three. */
void
print_stage_three(wap_t waps[], int w) {
	int point = calc_square_grid(GRID_SIZE, S_THRESHOLD, waps, w);
	int total_points = (GRID_SIZE - 1) * (GRID_SIZE - 1);
	printf("Stage 3\n");
	printf("==========\n");
	printf("%04d points sampled\n", total_points);
	printf("%04d points (%05.2lf%%) with maximum signal strength <= -50 dBm\n", 
		point, (double) point / total_points * 100);
	printf("\n");
}

/* Function to print stage four. */
void
print_stage_four(wap_t waps[], int w) {
	double x, y;
	printf("Stage 4\n");
	printf("==========\n");
	/* If it doesn't, print as normal. */
	for (y = GRID_SIZE - 1; y > 0; y -= 2) {
		for (x = 0.5; x < GRID_SIZE; x++) {
			if (max_signal(x, y, waps, w) > 0) {
				printf("+");
			} else if (max_signal(x, y, waps, w) > -10) {
				printf(" ");
			} else if (max_signal(x, y, waps, w) > -20) {
				printf("2");
			} else if (max_signal(x, y, waps, w) > -30) {
				printf(" ");
			} else if (max_signal(x, y, waps, w) > -40) {
				printf("4");
			} else if (max_signal(x, y, waps, w) > -50) {
				printf(" ");
			} else if (max_signal(x, y, waps, w) <= -50) {
				printf("-");
			} else {
				printf("\nsomething went wrong\n");
			}
		}
		printf("\n");
	}
	printf("\n");
}

/* Function to print stage five. */
void
print_stage_five(point_t vertexes[], line_t segments[], int v, 
	             wap_t waps[], int w) {
	int i;
	double x, y;
	point_t temp_point;
	line_t temp_line;
	int plotted;
	point_t centroid = find_centroid(vertexes, v);
	vertex_to_segments(vertexes,segments,v);

	printf("Stage 5\n");
	printf("==========\n");
	
	for (y = GRID_SIZE - 1; y > 0; y -= 2) {
		for (x = 0.5; x < GRID_SIZE; x++) {
			temp_point.x = x;
			temp_point.y = y;
			temp_line.p1 = temp_point;
			plotted = 1;
			temp_line.p2 = centroid;
			for (i = 0; i < v; i++) {
				/* Checks whether a line from the centroid of a cell to
				   the centroid of the polygon intersects any of the boundaries.
				*/
				if (line_intersect(temp_line, segments[i])) {
					plotted = 0;
				}
			}
			/* If it doesn't, print as normal. */
			if (plotted) {
				if (max_signal(x, y, waps, w) > 0) {
					printf("+");
				} else if (max_signal(x, y, waps, w) > -10) {
					printf(" ");
				} else if (max_signal(x, y, waps, w) > -20) {
					printf("2");
				} else if (max_signal(x, y, waps, w) > -30) {
					printf(" ");
				} else if (max_signal(x, y, waps, w) > -40) {
					printf("4");
				} else if (max_signal(x, y, waps, w) > -50) {
					printf(" ");
				} else if (max_signal(x, y, waps, w) <= -50) {
					printf("-");
				} else {
					printf("\nsomething went wrong\n");
				}
			/* If it does, print an '#' instead. */
			} else {
				printf("#");
			}
		}
		printf("\n");
	}
}

/* Function that calculate the maximum signal at a point. */
double
max_signal(double x,double y,wap_t waps[], int w) {
	int i = 0;
	double max = calc_strength(x,y,waps[0]);
	for (i = 1; i < w; i++) {
		if (calc_strength(x,y,waps[i]) > max) {
			max = calc_strength(x,y,waps[i]);
		}
	}
	return max;
}

/* Function to calculate the strength of signal at a point. */
double
calc_strength(double x, double y, wap_t wap) {
	return (wap.power - fspl(calc_dist(x,y,wap.x,wap.y), wap.freq));
}

/* Function to calculate number of points which have maximum signal less
   than or equal to a particular threshold. 
*/
int
calc_square_grid(int side, double min, wap_t waps[], int w) {
	int x, y, count = 0;
	for (x = 1; x < side; x++) {
		for (y = 1; y < side; y++) {
			if (max_signal(x, y, waps, w) <= min) {
				count++;
			}
		}
	}
	return count;
}

/* Function to read and write the vertexes into line segments. */
void
vertex_to_segments(point_t vertexes[], line_t segments[], int v) {
	int i;	
	for (i = 0; i < v - 1; i++) {
		segments[i].p1 = vertexes[i];
		segments[i].p2 = vertexes[i+1];
	}
	segments[i].p1 = vertexes[i];
	segments[i].p2 = vertexes[0];
}

/* Function to find the centroid of a polygon defined by their vertexes. */
point_t
find_centroid(point_t vertexes[], int v) {
	int i, total_x = 0, total_y = 0;
	point_t result;
	for (i = 0; i < v; i++) {
		total_x += vertexes[i].x;
		total_y += vertexes[i].y;
	}
	result.x = total_x / v;
	result.y = total_y / v;
	return result;
}

/* Function to check whether two lines intersect each other. */
int 
line_intersect(line_t l1, line_t l2) {
   double x1=l1.p1.x, y1=l1.p1.y,
   	  x2=l1.p2.x, y2=l1.p2.y,
   	  x3=l2.p1.x, y3=l2.p1.y,
   	  x4=l2.p2.x, y4=l2.p2.y;
   double mua,mub;
   double denom,numera,numerb;

   /* Take the projections of the two line segments */
   double xMin1, xMax1, xMin2, xMax2, yMin1, yMax1, yMin2, yMax2;
   xMin1 = MIN(x1, x2);
   xMax1 = MAX(x1, x2);
   xMin2 = MIN(x3, x4);
   xMax2 = MAX(x3, x4);

   yMin1 = MIN(y1, y2);
   yMax1 = MAX(y1, y2);
   yMin2 = MIN(y3, y4);
   yMax2 = MAX(y3, y4);
   
   /* Do the projects intersect? */
   if((xMin2-xMax1) >= EPS || (xMin1-xMax2) >= EPS || 
   	   (yMin2-yMax1) >= EPS || (yMin1-yMax2) >= EPS) {
   	   return FALSE;
   }

   denom  = (y4-y3) * (x2-x1) - (x4-x3) * (y2-y1);
   numera = (x4-x3) * (y1-y3) - (y4-y3) * (x1-x3);
   numerb = (x2-x1) * (y1-y3) - (y2-y1) * (x1-x3);

   /* Are the line coincident? */
   if (ABS(numera) < EPS && ABS(numerb) < EPS && ABS(denom) < EPS) {
      return(TRUE);
   }

   /* Are the line parallel */
   if (ABS(denom) < EPS) {
      return(FALSE);
   }

   /* Is the intersection along the the segments */
   mua = numera / denom;
   mub = numerb / denom;
   /* AM - use equality here so that "on the end" is not an
    * intersection; use strict inequality if "touching at end" is an
    * intersection */
   if (mua < 0 || mua > 1 || mub < 0 || mub > 1) {
      return(FALSE);
   }
   return(TRUE);
}