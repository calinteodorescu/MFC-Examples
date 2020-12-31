#include <functional>
#include <algorithm>
#include <list>
#include <set>
#include <numeric> 
using namespace std;
template<typename InputIterator,                // һ��copy_if��
    typename OutputIterator,        // ��ȷʵ��
    typename Predicate>
    OutputIterator copy_if(InputIterator begin,
    InputIterator end,
    OutputIterator destBegin,
    Predicate p) {
        while (begin != end) {
            if (p(*begin))*destBegin++ = *begin;
            ++begin;
        }

        return destBegin;
}


///==
struct Point {
    double x; 
    double y;
    Point(double a,double b):x(a),y(b){}
    Point():x(0.0),y(0.0){}
};                    // ͬ��
class PointAverage:
    public unary_function<Point, void> {        // �μ�����40
public:
    PointAverage(): xSum(0), ySum(0), numPoints(0) {}
    void operator()(const Point& p)
    {
        ++numPoints;
        xSum += p.x;
        ySum += p.y;
    }
    Point result() const
    {
        return Point(xSum/numPoints, ySum/numPoints);
    }

private:
    size_t numPoints;
    double xSum;
    double ySum;
};

void listPoind()
{
    list<Point> lp;
    ////...
    Point avg = (for_each(lp.begin(), lp.end(), PointAverage())).result();
}
///==
string::size_type                    // string::size_type�������뿴����
    stringLengthSum(string::size_type sumSoFar,         
    const string& s)
{
    return sumSoFar + s.size();
}
void setPoind()
{
    set<string> ss;                    // �����ַ���������������һЩ����
    ////...                         
    string::size_type lengthSum =            // ��lengthSum��Ϊ�� ss�е�ÿ��Ԫ�ص���
        accumulate(ss.begin(), ss.end(), 0, stringLengthSum);         
            // stringLengthSum�Ľ����ʹ��0��Ϊ��ʼͳ��ֵ

    ///=
}
template<typename T>                    // �����޸ĵ�BPFC  ����ʵ����
class BPFCImpl:
    public unary_function<T, void> {        
private:
    //Widget w;                    // ��ǰ��BPFC�����������
    int x;                        // ����������
    ////...
    virtual ~BPFCImpl();                // ��̬����Ҫ
    // ����������
    virtual void operator()(const T& val) const;
    template<class U> friend class BPFC;                // ��BPFC���Է�����Щ����
};

template<typename T>
class BPFC:                        // С�ģ���̬���BPFC
    public unary_function<T, void> {
private:
    BPFCImpl<T> *pImpl;                // ����BPFCΨһ������

public:
    void operator()(const T& val) const        // ���ڷ��飻
    {                        
        pImpl->operator() (val);// ����BPFCImpl��
    }
    ////...
};
