#include "VolumeShapes.h"

VolShape* VolShape::CopyInstance(VolShape* v)
{
	if (typeid(*v) == typeid(Cylinder))      return new Cylinder(*dynamic_cast<Cylinder*>(v));
	if (typeid(*v) == typeid(Parallelepiped))return new Parallelepiped(*dynamic_cast<Parallelepiped*>(v));
	if (typeid(*v) == typeid(TriPrizm))      return new TriPrizm(*dynamic_cast<TriPrizm*>(v));
	if (typeid(*v) == typeid(Conus))         return new Conus(*dynamic_cast<Conus*>(v));
	if (typeid(*v) == typeid(RectPiramid))   return new RectPiramid(*dynamic_cast<RectPiramid*>(v));
	if (typeid(*v) == typeid(TriPiramid))    return new TriPiramid(*dynamic_cast<TriPiramid*>(v));
	return nullptr;
}

VolShape* VolShape::MakeInstance(std::ifstream& fin)
{
	int y; double h, r, a, b;
	string name;
	fin >> name;
	if (name == "Cylinder")
	{
		fin >> h >> r;
		return new Cylinder(h,r);
	}
	if (name == "Parallelepiped")
	{
		fin >> h >> a >> b; return new Parallelepiped(h,a,b);
	}
	if (name == "TriPrizm")
	{
		fin >> h >> a >> b >> y; return new TriPrizm(h,a,b,y);
	}
	if (name == "Conus")
	{
		fin >> h >> r; return new Conus(h,r);
	}
	if (name == "RectPiramid")
	{
		fin >> h >> a >> b; return new RectPiramid(h,a,b);
	}
	if (name == "TriPiramid")
	{
		fin >> h >> a >> b >> y; return new TriPiramid(h,a,b,y);
	}
	throw BadClassname(name.c_str());
}
//-----------------------------------------------------------
void VolShape::storeOn(ofstream& fout) const
{
	const char* line = this->getClassName();
	for (int i = 6; line[i] != '\0'; ++i) fout << line[i];
	fout << ' ' << this->h << ' ';
	this->base->storeOn(fout);
}

ostream& operator<<(ostream& os, const VolShape& s)
{
	s.printOn(os);
	return os;
}

//-----------------------------------------------------------

void DirectShape::printOn(ostream& os) const
{
	os << "DirectShape of " << h << " high on " << *base;
}
Cylinder& Cylinder::operator=(const Cylinder& c)
{
	if (this != &c)
	{
		h = c.h;
		delete base;
		base = new Circle(*static_cast<Circle*>(c.base));
	}
	return *this;
}
VolShape* Cylinder::Clone() const
{
	return new Cylinder(*this);
}
Parallelepiped& Parallelepiped::operator=(const Parallelepiped& p)
{
	if (this != &p)
	{
		h = p.h;
		delete base;
		base = new Rectangle(*dynamic_cast<Rectangle*>(p.base));
	}
	return *this;
}
bool Parallelepiped::operator == (const Parallelepiped& p) const noexcept
{
	if (h == p.h && base->area() == p.base->area() && volume() == p.volume() && base->perim() == p.base->perim()) return true;
	return false;
}

VolShape* Parallelepiped::Clone() const
{
	return new Parallelepiped(*this);
}

TriPrizm& TriPrizm::operator=(const TriPrizm& t)
{
	if (this != &t)
	{
		h = t.h;
		delete base;
		base = new Triangle(*dynamic_cast<Triangle*>(t.base));
	}
	return *this;
}
VolShape* TriPrizm::Clone() const
{
	return new TriPrizm(*this);
}
//-----------------------------------------------------------

void PiramidalShape::printOn(ostream& os) const
{
	os << "PiramidalShape of " << h << " high on " << *base;
}



double Conus::sideArea() const
{
/* ����� ���� �������� = Pi R L, �� L - �����. �� ����� ��������� ����� H i R.
   ����������, ������� ����� ������ (�����) ==> ������� �������� ����������� ��� ������ �� �����������
*/
	double r = static_cast<Circle*>(base)->radius();
	return M_PI*r*sqrt(h*h+r*r);
}

VolShape* Conus::Clone() const
{
	return new Conus(*this);
}

double RectPiramid::sideArea() const
{
/* ��� ��������� ����� ���� �������� ���������� ������,
   ������� ����� ������� �����
*/
	double a = static_cast<RectAB*>(base)->getA();
	double b = static_cast<RectAB*>(base)->getB();
	return a*sqrt(h*h+b*b*0.25)+b*sqrt(h*h+a*a*0.25);
}

VolShape* RectPiramid::Clone() const
{
	return new RectPiramid(*this);
}

double TriPiramid::sideArea() const
{
/* ���������, �� ������� ������ ����������� � ����� ��������� ����,
   ���� ����� ��������� ������� ����� ����� ��������� ����
*/
	double r = 2. * base->area() / base->perim();
	return 0.5 * base->perim() * sqrt(h*h+r*r);
}

VolShape* TriPiramid::Clone() const
{
	return new TriPiramid(*this);
}

