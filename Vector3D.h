#pragma once
#include <glut.h>
#include <math.h>

class Vector3D
{
public:
	// Data
	GLfloat x, y, z;

	// Constructors
	Vector3D( GLfloat _x, GLfloat _y, GLfloat _z ) : x( _x ), y( _y ), z( _z )
		{
		}
	Vector3D( ) : x(0), y(0), z(0)
		{
		}

	// Operator Overloads
	inline bool operator== (const Vector3D& V2) const 
		{
		return (x == V2.x && y == V2.y && z == V2.z);
		}

	inline Vector3D operator+ (const Vector3D& V2) const 
		{
		return Vector3D( x + V2.x,  y + V2.y,  z + V2.z);
		}
	inline Vector3D operator- (const Vector3D& V2) const
		{
		return Vector3D( x - V2.x,  y - V2.y,  z - V2.z);
		}
	inline Vector3D operator- ( ) const
		{
		return Vector3D(-x, -y, -z);
		}

	inline Vector3D operator/ (GLfloat S ) const
		{
		GLfloat fInv = 1.0f / S;
		return Vector3D (x * fInv , y * fInv, z * fInv);
		}
	inline Vector3D operator/ (const Vector3D& V2) const
		{
		return Vector3D (x / V2.x,  y / V2.y,  z / V2.z);
		}
	inline Vector3D operator* (const Vector3D& V2) const
		{
		return Vector3D (x * V2.x,  y * V2.y,  z * V2.z);
		}
	inline Vector3D operator* (GLfloat S) const
		{
		return Vector3D (x * S,  y * S,  z * S);
		}

	inline void operator+= ( const Vector3D& V2 )
		{
		x += V2.x;
		y += V2.y;
		z += V2.z;
		}
	inline void operator-= ( const Vector3D& V2 )
		{
		x -= V2.x;
		y -= V2.y;
		z -= V2.z;
		}

	inline GLfloat operator[] ( int i )
		{
		if ( i == 0 ) return x;
		if ( i == 1 ) return y;
		return z;
		}

	// Functions
	inline GLfloat Dot( const Vector3D &V1 ) const
		{
		return V1.x*x + V1.y*y + V1.z*z;
		}

	inline Vector3D CrossProduct( const Vector3D &V2 ) const
		{
		return Vector3D(
			y * V2.z  -  z * V2.y,
			z * V2.x  -  x * V2.z,
			x * V2.y  -  y * V2.x 	);
		}

	// Vector Length
	GLfloat Length( ) const
		{
		return sqrtf( x*x + y*y + z*z );
		}

	GLfloat Distance( const Vector3D &V1 ) const
		{
		return ( *this - V1 ).Length();	
		}

	inline Vector3D Normalize()
		{
		GLfloat fMag = ( x*x + y*y + z*z );
		if (fMag == 0) {return Vector3D(0,0,0);}

		GLfloat fMult = 1.0f/sqrtf(fMag);            
		x *= fMult;
		y *= fMult;
		z *= fMult;
		return Vector3D(x,y,z);
		}

	inline double Angle(Vector3D v2){
		const double epsilon = 1.0e-6;
		const double PI = acos(-1.0);
		double angle = 0;	

		Vector3D norVec1 = Normalize();
		Vector3D norVec2 = v2.Normalize();

		//doar pe x si z
		double dotProd = (norVec1.x * norVec2.x) + (norVec1.z * norVec2.z);
							
		if (abs(dotProd - 1.0) <= epsilon)
			angle = 0;
		else if ( abs(dotProd + 1.0) <= epsilon )
			angle = PI;
		else {
		double cross = 0;
		angle = acos(dotProd);
		//in sensul acelor de ceasornic sau invers
	
		cross = (norVec1.x * norVec2.z) - (norVec2.x * norVec1.z);
 
		if (cross < 0)
			angle = 2 * PI - angle;
		}
 
		return angle*(180 / PI);
	}

	static float *arr;

	float *Array()
	{
		arr[0] = x;
		arr[1] = y;
		arr[2] = z;
		return arr;
	}
};
