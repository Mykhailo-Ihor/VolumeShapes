/*
��'���� ������ ���������� � ������ � ������ - ������ ������. �������
  VolShape ���� �������� �����䳿: ���������� ���� ������ �� ����
  �������, ��'���, ���������� ������. ϳ������ �������� - PiramidalShape
  i DirectShape - ���������� ������� ���������� ���� � ��'��� ���������
  ���� ��'����� �����: ������ � ������ �� ������� � ����� ������. � ���
  ��� ������� ���������� ��������� ��������� �������� �����, �����
  ����. ������ ����� ������� ���������� ����� ���� ������� � ��������
  PiramidalShape: ����� � ������� ����������� ����.

���������� ��'���� � ���� - ������������� "��������" ����� �����/������ Գ����
 pRintOn(����) ���� ������ ���������� ��'����
��������� ��'���� - ��������� � ���� ����� ��� ��'��� � ������, �������� ���
 ���������� ���������� � ��������� ��'����. ����� storeOn(����) ����� ���������
 ��� � ������������ �������� ����, ��������� ���������� � ����������� �����
 getClassName() ��������� ���� ��� ���� ���������� (�� ������ ������ ����������
 ������� typeid(pointer), ��� �� ���� ���� ����������).

������� ������ (������ �����) ��������� ������ ����� ���� ���������� ���������
 ���������� ���� ��'���� ��� �������� �� ���������� ���������� �� ���
 ����������� � �����
*/
#ifndef _VolumeShapeHeader_
#define _VolumeShapeHeader_

#include "..\FlatShapes\FlatShapes.h"
#include <exception>
#include <stdexcept>
// --------------------- ����������� ������� ����
class VolShape
{
protected:
	double h;
    Shape* base;
	string baseToStr() const { return base->toStr(); }
public:
	// ���� ��� ����� �������, �� �������� ��� ����������� ������� ���� �����
	class BadClassname : public std::exception
	{
	public:
		BadClassname(const char* mess) :exception(mess){};
	};
	// ������ ����� ���������� �� ��������� �� ��������� ����������
	static VolShape* CopyInstance(VolShape*);
	static VolShape* MakeInstance(std::ifstream&);
	VolShape(double high=1., Shape* s=nullptr) : h(high), base(s) {}
	virtual ~VolShape() 
    { 
        delete base; 
    }
	virtual double baseArea() const { return base->area(); }
	virtual double sideArea() const abstract;
	virtual double surfaceArea() const abstract;
	virtual double volume() const abstract;
	// ��������� � ���� ���������� ������� ��'����
	virtual void printOn(ostream&) const abstract;
	// ��������� ��'���� �� ����� � ������, ���������� ��� ���������
	virtual void storeOn(ofstream&) const;
	// ��'� ����� ������� ��� ������ ��������� �� �����
	virtual const char * getClassName() const { return typeid(*this).name(); }
    virtual VolShape* Clone() const abstract;
};

ostream& operator<<(ostream& os, const VolShape& s);

// --------------------- �������� ������ �����
// DirectShape ���� �������� ��� ������. ����� ����������� (� ���������� ������).
// � �������� ������� ����� ��������� ������������� ��'����

class DirectShape abstract : public VolShape
{
public:
	DirectShape(double high=1., Shape* s=nullptr) : VolShape(high,s) {}
	virtual double sideArea() const override
	{
		return base->perim() * h;
	}
	virtual double surfaceArea() const
	{
		return baseArea() * 2 + sideArea();
	}
	virtual double volume() const override
	{
		return base->area() * h;
	}
	virtual void printOn(ostream&) const override;
	const char * getClassName() const override { return typeid(*this).name(); }
};

// ϳ������ �������� �������� ��� ������, ��������� ��������� ��'���
class Cylinder : public DirectShape
{
public:
	Cylinder(double high=1., double radius=1.): DirectShape(high)
	{
		base = new Circle(radius);
	}
	Cylinder(const Cylinder& c): DirectShape(c)
	{
		base = new Circle(*dynamic_cast<Circle*>(c.base));
	}
	Cylinder& operator=(const Cylinder& c);
	const char * getClassName() const override { return typeid(*this).name(); }
    virtual VolShape* Clone() const override;
};

class Parallelepiped : public DirectShape
{
public:
    Parallelepiped(double high = 1., double sideA = 1., double sideB = 1.) : DirectShape(high)
    {
        base = new Rectangle(sideA, sideB);
    }
    Parallelepiped(const Parallelepiped& p) : DirectShape(p)
    {
        base = new Rectangle(*dynamic_cast<Rectangle*>(p.base));
    }
    Parallelepiped& operator=(const Parallelepiped& p);
    bool operator ==(const Parallelepiped& p) const noexcept;
	const char * getClassName() const override { return typeid(*this).name(); }
    virtual VolShape* Clone() const override;
};

class TriPrizm : public DirectShape
{
public:
	TriPrizm(double high=1., double sideA=3., double sideB=4., int angle=90): DirectShape(high)
	{
		base = new Triangle(sideA,sideB,angle);
	}
	TriPrizm(const TriPrizm& t): DirectShape(t)
	{
		base = new Triangle(*dynamic_cast<Triangle*>(t.base));
	}
	TriPrizm& operator=(const TriPrizm& t);
	const char * getClassName() const override { return typeid(*this).name(); }
    virtual VolShape* Clone() const override;
};

// --------------------- �������� ������� �����
// PiramidalShape ���� ����������� �� ��������� ��'��,
// ��� ���� �������� �������� �� ���� ������, ���� - � ��������.
// �� � � ������ �������, ������������ ������� ���������� �� ��������� ��������� ������.
// ��� ��������� ������ ��������� ���������� �������� ����� - �� ������ ����� ����� ���������� ������

class PiramidalShape : public VolShape
{
public:
	PiramidalShape(double high=1., Shape* s=nullptr) : VolShape(high,s) {}
	virtual double surfaceArea() const override
	{
		return baseArea() + sideArea();
	}
	virtual double volume() const override
	{
		return base->area() * h/3.;
	}
	virtual void printOn(ostream&) const override;
	const char * getClassName() const override { return typeid(*this).name(); }
};

class Conus : public PiramidalShape
{
public:
	Conus(double high=1., double radius=1.)
		: PiramidalShape(high, new Circle(radius)) {}
	Conus(const Conus& c): PiramidalShape(c)
	{
		base = new Circle(*dynamic_cast<Circle*>(c.base));
	}
	virtual double sideArea() const override;
	const char * getClassName() const override { return typeid(*this).name(); }
    virtual VolShape* Clone() const override;
};

// ���� Rectangle �� �� ������ ������� �� ���� ���� ����� - ����� ������������,
// � ���� ������ ��� ���������. ���� ��������� ������, ���� ���� ���������������
// �������. ����� ������ ������� ����� �����, ���� ���������� �����������
// �������� RectPiramid
class RectPiramid : public PiramidalShape
{
	class RectAB : public Rectangle
	{
	public:
		RectAB(double sideA=1., double sideB=1.) : Rectangle(sideA,sideB) {}
		double getA() const { return a; }
		double getB() const { return b; }
	};
public:
	RectPiramid(double high=1., double sideA=1., double sideB=1.)
		: PiramidalShape(high, new RectAB(sideA,sideB)) {}
	RectPiramid(const RectPiramid& p): PiramidalShape(p)
	{
		base = new Rectangle(*dynamic_cast<Rectangle*>(p.base));
	}
	virtual double sideArea() const override;
	const char * getClassName() const override { return typeid(*this).name(); }
    virtual VolShape* Clone() const override;
};

class TriPiramid : public PiramidalShape
{
public:
	TriPiramid(double high=1., double sideA=3., double sideB=4., int angle=90)
		: PiramidalShape(high, new Triangle(sideA,sideB,angle)) {}
	TriPiramid(const TriPiramid& t): PiramidalShape(t)
	{
		base = new Triangle(*dynamic_cast<Triangle*>(t.base));
	}
	virtual double sideArea() const override;
	const char * getClassName() const { return typeid(*this).name(); }
    virtual VolShape* Clone() const override;
};

class LinkedList {
public:
    struct Node {
        VolShape* data;
        Node* next;
        Node(VolShape* val,Node* p = nullptr) : next(p)
        {
            data = VolShape::CopyInstance(val);
        }
        ~Node() 
        {
            delete data; 
        }
    };
private:
    Node* head;
public:
    LinkedList(): head() {}

    LinkedList(VolShape* val, Node* next = nullptr) { head = new Node(val, next); }

    LinkedList(const LinkedList& other) : head(nullptr) {
        if (other.head == nullptr) {
            return;
        }
        head = new Node(other.head->data);
        Node* curr = head;
        Node* otherCurr = other.head->next;
        while (otherCurr != nullptr) {
            curr->next = new Node(otherCurr->data);
            curr = curr->next;
            otherCurr = otherCurr->next;
        }
    }
    VolShape& operator[] (int i)
    {
        return getShape(i);
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this == &other) {
            return *this;
        }
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }

        if (other.head == nullptr) 
        {
            return *this;
        }
        head = new Node(other.head->data);
        Node* curr = head;
        Node* otherCurr = other.head->next;
        while (otherCurr != nullptr) 
        {
            curr->next = new Node(otherCurr->data);
            curr = curr->next;
            otherCurr = otherCurr->next;
        }
        return *this;
    }

    void addtoEnd(VolShape* val)
    {
        if (head == nullptr) 
        {
            head = new Node(val);
            return;
        }
        Node* curr = head;
        while (curr->next != nullptr) 
        {
            curr = curr->next;
        }
        curr->next = new Node(val);
    }

    void printAll() const
    {

        Node* curr = head;
        if (curr == nullptr)
        {
            std::cout << "List is empty :(\n";
        }
        else
        {
            while (curr != nullptr)
            {
                
                std::cout << *curr->data << " ";
                curr = curr->next;
                std::cout << std::endl;
            }
        }
    }
    void insert(VolShape* val, int index) 
    {
        Node* newNode = new Node(val);
        Node phantom(nullptr, head);
        Node* curr = &phantom;
        for (int i = 0; i < index && curr != nullptr; ++i) 
        {
            curr = curr->next;
        }
        if (curr == nullptr) 
        {
            delete newNode;
            throw std::out_of_range("Error: Cannot insert at specified position\n");
        }
        
        newNode->next = curr->next;
        curr->next = newNode;
        head = phantom.next;
    }
    VolShape& getShape(int index) const
    {
        Node* curr = head;
        for (int i = 0; i < index && curr != nullptr; ++i) 
        {
            curr = curr->next;
        }
        if (curr == nullptr) 
        {
            throw std::out_of_range("Error: Cannot get element at specified position\n");
        }
        return *curr->data;
    }
    void remove(int index) {
        if (head == nullptr) 
        {
            return;
        }
        Node phantom(nullptr, head);
        Node* curr = &phantom;
        for (int i = 0; i < index && curr != nullptr; ++i) 
        {
            curr = curr->next;
        }
        if (curr == nullptr || curr->next == nullptr) 
        {
            throw std::out_of_range("Error: Cannot get element at specified position\n");
        }
        Node* temp = curr->next;
        curr->next = curr->next->next;
        delete temp;
        head = phantom.next;
    }
    void removeAll()
    {
        while (head != nullptr) 
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void storeOn(ofstream& os)
    {
        Node* curr = head;
        while (curr != nullptr)
        {
            curr->data->storeOn(os);
            os << std::endl;
            curr = curr->next;
        }
    }   
    Node* findFirst_if(bool(*p)(VolShape*))
    {
        Node* curr = head;
        while (curr != nullptr)
        {
            if (p(curr->data))
            {
                return curr;
            }
            curr = curr->next;
        }
    }
    void ForEach(void (*do_something)(VolShape*)) const
    {
        Node* curr = head;
        while (curr != nullptr)
        {
            do_something(curr->data);
            curr = curr->next;
        }
    }
    ~LinkedList() 
    {
        while (head != nullptr) 
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

#endif