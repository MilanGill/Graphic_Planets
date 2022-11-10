#include "Main.h"
#include "Vector.h"
#include <iostream>
#include <vector>

using namespace Geometry;


void clear_screen(HDC hdc, HWND hwnd)
{
	RECT rcWin;
	RECT rcWnd;
	//HWND parWnd = GetParent(hwnd); // Get the parent window.
	HDC parDc = GetDC(0);//(parWnd); // Get its DC.

	GetWindowRect(hwnd, &rcWnd);
	//ScreenToClient(parWnd, &rcWnd); // Convert to the parent's co-ordinates

	GetClipBox(hdc, &rcWin);
	// Copy from parent DC.
	BitBlt(hdc, rcWin.left, rcWin.top, rcWin.right - rcWin.left,
		rcWin.bottom - rcWin.top, parDc, rcWnd.left, rcWnd.top, BLACKNESS);

	ReleaseDC(0, parDc);
}





class View
{
public:
	Vector ex, ey;
	View(Vector e1,Vector e2): ex(e1),ey(e2) {}

	//void rotate(int axis, double angle)
	//{
	//	switch (axis)
	//	{
	//	case 0:   // arround main x
	//		double angle0 = atan2(ey.y, ey.z);
	//		break;
	//	case 1:   // arround main y
	//		
	//		break;
	//	default:
	//		break;
	//	}
	//}
};

static View view(Vector(1, 0, 0), Vector(0, 0.1, 1));



class Solar_wind
{
	Vector zero;
public:
	double intensity;
	Vector &source_place;


	Solar_wind(): intensity(0),source_place(zero) {}
	Solar_wind(double intensity_, Vector& source) : intensity(intensity_),source_place(source) {}


	Vector v(const Vector& r) const
	{
		Vector e = r / r.norm();
		Vector v = (intensity / r.sqr()) * e;
		return v;
	}

	static Vector f(const Vector& v,const Vector& v_obj)
	{

		Vector Dv = v - v_obj;
		Vector e = Dv.norm() == 0 ? Vector() : Dv / Dv.norm();
		Vector f = 0.01 * e;
		return f;
	}
};


class Gravity
{
public:
	static const double G; // Newton's gravity constant

	static Vector f(double m1, double m2, const Vector& r1, const Vector& r2)
	{
		Vector r = r2 - r1;
		Vector e = r / r.norm();
		Vector f = G * m1 * m2 * e / r.sqr();
		return f;
	}
};

const double Gravity::G = 1; // Newton's gravity constant

double t = 0, t_ = 0;


class Planet
{
public:
	Planet(double mass,double radius,COLORREF color0,Vector r0, Vector v0)
		:m(mass),R(radius), r(r0), v(v0), color(color0) {}

	Vector F; // Force
	Vector r, v;   // Radius-vector(position),velocity
	COLORREF color;
	double m; // mass
	double R; // radius_of_planet
	Solar_wind solar_wind;

	void pre_new_time()
	{
		F = Vector();
	}

	void on_new_time()
	{
		double dt = t - t_;
		Vector dr = v * dt;
		Vector dv = F / m * dt;
		double ndr = dr.norm();
		double ndv = dv.norm();
		const double eps = 0.000001;
		if (ndr < eps)
		{
			r = r + 0.5 * v * dt;
			v = v + F / m * dt;
			r = r + 0.5 * v * dt;
		}
		else
		{
			int n = ndr / eps + 1;
			double dt_ = dt / n;
			for (int i = 0; i < n; i++)
			{
				r = r + 0.5 * v * dt_;
				v = v + F / m * dt_;
				r = r + 0.5 * v * dt_;
			}
		}
	}

	void draw(HDC hdc)
	{
		double x = r * view.ex;
		double y = r * view.ey;
		SetPixel(hdc, int(x), int(y), color);
	}
};

class Sun : public Planet
{
public:
	Sun() : Planet(332982, 109, RGB(255, 255, 50), Vector(200, 200, 100), Vector(0, 0, 0)) {
		new (&solar_wind) Solar_wind(1.0e-100, r);
	}
};


class Venus : public Planet
{
public:
	Venus() : Planet(0.815,0.95,RGB(255, 255, 255), Vector(100, 200, 100), Vector(0, 50, 0)) {

	}
};

class Mars : public Planet
{public:
	Mars(): Planet(0.107, 0.151, RGB(255, 100, 10), Vector(50, 50, 231), Vector(10, 30, 0)){
		
	}
};
template <typename Object>
class List
{
private:
	std::vector <Object*> a;
	

public:
	void add(Object * element)
	{
		a.push_back(element);
	}
	virtual ~List()
	{
		for (int i = 0; i < a.size(); i++)
		{
			if (a[i]) 
			{
				delete a[i];
			}
		}
	}
	Object& operator [] (int i)
	{
		return *(a[i]);
	}
	const Object& operator [] (int i) const
	{
		return *(a[i]);
	}
	int  n () const
	{
		return a.size();
	}
};

class List_of_planets : public List<Planet>
{
public:
	List_of_planets()
	{
		this->add(new Sun);
		this->add(new Mars);
		this->add(new Venus);
	}

};

List_of_planets list_of_planets;


double dt = 0.0001;



void MilanDraws(HDC hdc, HWND hWnd)
{
	clear_screen(hdc, hWnd);
	List_of_planets& p = list_of_planets;
	for (t = t_ = 0; t < 1300; t_ = t, t += dt)
	{
		for (int i = 0; i < p.n(); i++)
		{
			p[i].pre_new_time();
			for (int j = 0; j < p.n(); j++)
				if (j != i)
					p[i].F += Gravity::f(p[i].m, p[j].m, p[i].r, p[j].r);
			for (int j = 0; j < p.n(); j++)
				if (j != i)
					if (p[j].solar_wind.intensity != 0)
						p[i].F += Solar_wind::f(p[j].solar_wind.v(p[i].r-p[j].solar_wind.source_place), p[i].v);
			p[i].on_new_time();
			p[i].draw(hdc);
		}
	}
}






// Домашнее задание: 
// Изучить вращение в двумерной плоскости + реагирование на кнопки (именно на c++)
// главное понять в мат. смысле