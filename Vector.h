#pragma once
#include <cmath>
namespace Geometry
{
    template <typename Coord_Type>
    class Vector3D
    {
    public:
        Coord_Type x, y, z;
        Vector3D() : x(0), y(0), z(0) {}
        Vector3D(Coord_Type X, Coord_Type Y, Coord_Type Z) : x(X), y(Y), z(Z) {}
        Vector3D(const Vector3D& v) : x(v.x), y(v.y), z(v.z) {}
        Vector3D& operator = (const Vector3D& v)
        {
            x = v.x; y = v.y; z = v.z;

            return *this;
        }
        Vector3D operator - () const
        {
            return Vector3D(-x, -y, -z);

        }

        Vector3D& operator += (const Vector3D& b)
        {

            return (*this) = (*this) + b;
        }

        Coord_Type& operator [] (int k)
        {
            if (k == 0)
            {
                return x;

            }
            else
            {
                if (k == 1)
                {
                    return y;

                }
                else
                {
                    if (k == 2)
                    {
                        return z;
                    }
                    else
                    {

                    }
                }
            }
        }
        const Coord_Type& operator [] (int k) const
        {

            if (k == 0)
            {
                return x;

            }
            else
            {
                if (k == 1)
                {
                    return y;

                }
                else
                {
                    if (k == 2)
                    {
                        return z;
                    }
                    else
                    {

                    }
                }
            }
        }

        void print(const char* postsfix = " ") const
        {
           // std::cout << "( " << x << " , " << y << " , " << z << " )" << postsfix;
        }

        double norm() const
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        double sqr() const
        {
            return (x * x + y * y + z * z);
        }


    };
    
    template <typename Coord_Type>
    Vector3D<Coord_Type> operator + (const Vector3D<Coord_Type>& a, const Vector3D<Coord_Type>& b)
    {
        Vector3D<Coord_Type> c(a.x + b.x, a.y + b.y, a.z + b.z);
        return c;
    }
    template <typename Coord_Type>
    Coord_Type operator * (const Vector3D<Coord_Type>& a, const Vector3D<Coord_Type>& b) //dot product
    {
        Coord_Type res = 0;
        res = a.x * b.x + a.y * b.y + a.z * b.z;
        return res;
    }
    template <typename Coord_Type>
    Vector3D<Coord_Type> operator *(const Vector3D<Coord_Type> & a, const Coord_Type & k)
    {
        return Vector3D<Coord_Type>(k * a.x, k * a.y, k * a.z);
    }
    template <typename Coord_Type>
    Vector3D<Coord_Type> operator *(const Coord_Type& k , const Vector3D<Coord_Type>& a)
    {
        return Vector3D<Coord_Type>(k * a.x, k * a.y, k * a.z);
    }
    template <typename Coord_Type>
    Vector3D<Coord_Type> operator - (const Vector3D<Coord_Type>& a, const Vector3D<Coord_Type>& b)
    {
        return Vector3D<Coord_Type>((a.x - b.x), (a.y - b.y), (a.z - b.z));
    }
    template <typename Coord_Type>
    Vector3D<Coord_Type> operator / (const Vector3D<Coord_Type>& a, Coord_Type b)
    {
        return Vector3D<Coord_Type>((a.x / b), (a.y / b), (a.z / b));
    }


    typedef Vector3D <double> Vector;
    typedef Vector3D <int> IntVector;
}

