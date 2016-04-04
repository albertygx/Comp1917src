/*
 * Test if a point is inside a triangle.
 * Julian Saknussemm
 *
 * Given Three points of a triangle, and another arbitrary point
 * this program determines if that point lies inside the triangle.
 *
 * This is determined by satisfying the following rule:
 * A point P(x,y) is inside triangle A(x0,y0), B(x1,y1), C(x2,y2)
 * if
 * P is on the same side of the line AB as C
 * P is on the same side of the line BC as A
 * P is on the same side of the line AC as B
 *
 * A special case exits for a vertical line (inf gradient) when
 * testing the side of the line
 */
//Guanxiong Ye z5095134
//tue09-sitar Jade Giacoppo
//Modified on 17-Mar-2016
//This tiny program is refactored version of the original.
#include <stdio.h>

double pointLineRelationship(double xA, double yA,
		double xB, double yB, double xP, double yP);
int isInside(double x0, double y0, double x1, double y1, double x2,
		double y2, double px, double py);

int main(int argc, char* argv[]) {
	//(x,y)for point triangle ABC and point P.
	double xA, yA, xB, yB, xC, yC, xP, yP;
	//disable output buffer, We don't need it.
	setbuf(stdout, NULL);
    //get input
	//return -1 if fail to read any numeric value.
	printf("Triangle Vertex Ax,Ay: ");
	if(scanf("%lf,%lf", &xA, &yA) != 2){
		printf("Invalid value, Program terminated.\n");
		return -1;
	}
	printf("Triangle Vertex Bx,By: ");
	if(scanf("%lf,%lf", &xB, &yB) != 2){
		printf("Invalid value, Program terminated.\n");
		return -1;
	}
	printf("Triangle Vertex Cx,Cy: ");
	if(scanf("%lf,%lf", &xC, &yC) != 2){
		printf("Invalid value, Program terminated.\n");
		return -1;
	}
	printf("Test Point Px,Py: ");
	if(scanf("%lf,%lf", &xP, &yP) != 2){
		printf("Invalid value, Program terminated.\n");
		return -1;
	}
    //Test if ABC is a triangle:
	//Calculate [the area of triangle * 2] using the
	//area formula with coordinate
	//Halt the program and return 0 if ABC is collinear.
	if (xA*(yB-yC)+xB*(yC-yA)+xC*(yA-yB)==0){
		printf("Point A, B, C are collinear!");
		return 0;
	}
    // print answer
	printf("Point (%.2lf,%.2lf) is ", xP, yP);
	if (isInside(xA, yA, xB, yB, xC, yC, xP, yP) == 1){
		printf("inside");
	}else if (isInside(xA, yA, xB, yB, xC, yC, xP, yP) == -1){
		printf("outside");
	}else{
		printf("on");
	}
	printf(" the Triangle\n");

    // end point
	return 0;
}

//Line AB Separate the plane into two half planes.
//The sign of vertical signed distance(same orientation with y axis)
//and have faith that this is true enough in this case.
//from point P(xP, yP) to the line: yP - yA - slope*(xP - xA)
//indicates which half the point is in.
//@return double difference
double pointLineRelationship(double xA, double yA,
		double xB, double yB, double xP, double yP) {
	double slope;
	double distance;
	//vertical line?
	if((xA - xB) == 0){
		distance = xP - xA;
	}else{
		slope = (yA - yB)/(xA - xB);
		distance = yP - yA - slope*(xP - xA);
	}
	return distance;
}

//Check pointLineRelationship for all three line and the point P
//to determine if P is inside the triangle.
//return integer -1 if P is outside
//                0 if P is on the triangle
//                1 if P is inside
int isInside(double xA, double yA, double xB, double yB, double xC,
		double yC, double xP, double yP) {
	//if it is outside the triangle because there is at least one
	//point in A, B, or C are at different side with P.
	//(The product of their signed distance is negative.)
	if( pointLineRelationship(xA, yA, xB, yB, xP, yP) *
	    pointLineRelationship(xA, yA, xB, yB, xC, yC) < 0||
	    pointLineRelationship(xA, yA, xC, yC, xP, yP) *
	    pointLineRelationship(xA, yA, xC, yC, xB, yB) < 0||
        pointLineRelationship(xB, yB, xC, yC, xP, yP) *
	    pointLineRelationship(xB, yB, xC, yC, xA, yA < 0)){
		return -1;
	}
	//if P is on one of line AB, AC, or BC.
	else if(pointLineRelationship(xA, yA, xB, yB, xP, yP) == 0 ||
			 pointLineRelationship(xA, yA, xC, yC, xP, yP) == 0 ||
			 pointLineRelationship(xB, yB, xC, yC, xP, yP) == 0){
		return 0;
	}
	//Else, Then it must be inside!
	else{
		return 1;
	}
}


