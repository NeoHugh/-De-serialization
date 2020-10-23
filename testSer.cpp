#include "binser.h"
#include "xmlSer.h"
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
using namespace std;

struct MyStruct
{
	int x;
	double y;
	vector<int> vec;
	unique_ptr<vector<double>> ptr;
	MyStruct(int in, double dou) :x(in), y(dou){
		for (size_t i = 0; i < 100; i++)
		{
			vec.push_back(i);
			ptr.reset(new vector<double>);
		}
	}
	bool operator==(const MyStruct& latter) {
		return(latter.x == x && latter.y == y && latter.vec == vec&&*ptr==*(latter.ptr));
	}
};

template<class T>void testBasicType(string file,T& orig, T& bin, T& xml) {
	Tobin::serialize(orig, file);
	Tobin::deserialize(bin, file);
	unique_ptr<T>ptr(new T), ptrbin(new T), ptrxml(new T);
	*ptr = orig;
	Tobin::serialize(ptr, file+"_ptr");
	Tobin::deserialize(ptrbin, file+"_ptr");
	Toxml::serialize( file+".xml",orig);
	Toxml::deserialize( file+".xml",xml );
	Toxml::serialize(file + "_ptr.xml", ptr);
	Toxml::deserialize(file + "_ptr.xml", ptrxml);
	if (orig == bin && orig == xml) {
		cout << typeid(T).name() << " pass!" << endl;
	}
	if(orig!=bin)
		cout << typeid(T).name() << " bin didn't pass!" << endl;
	if (orig != bin)
		cout << typeid(T).name() << " xml didn't pass!" << endl;
	if (*ptr == *ptrbin && *ptr == *ptrxml)
		cout << "Unique ptr for "<<typeid(T).name()<<" passed!" << endl;
	if (*ptr != *ptrbin)
		cout << "Unique_ptr in bin for" << typeid(T).name() << "didn't pass!" << endl;
	if (*ptr != *ptrxml)
		cout << "Unique_ptr in xml for" << typeid(T).name() << " didn't pass!" << endl;
	remove(file.c_str());
	remove((file + ".xml").c_str());
	remove((file + ".base64").c_str());
	remove((file + "_ptr").c_str());
	remove((file + "_ptr.xml").c_str());
	remove((file + "_ptr.base64").c_str());


}
void testSTL() {
	cout << "Finished arithmetic types and string!" << endl;
	cout << "Testing STL!" << endl;
	//construct large containers to size 1w.
	vector<int> myvec, vecbin, vecxml;
	list<int> mylist, listbin, listxml;
	set <int> myset, setbin, setxml;
	pair<string, int> mypair("Test pair!", 999), pairbin, pairxml;
	map<string, int>mymap, mapbin, mapxml;
	for (size_t i = 0; i < 100; i++)
	{
		for (size_t j = 0; j < 100; j++)
		{
			myvec.push_back(i * j + j);
			mylist.push_back(i * j + j);
			myset.insert(i * j + j);
			mymap.insert(pair<string, int>("Hello!", i * j + j));
		}
	}
	unique_ptr<vector<int>>vectorptr(new vector<int>), pvectorbin(new vector<int>), pvectorxml(new vector<int>);
	unique_ptr<list<int>>listptr(new list<int>), plistbin(new list<int>), plistxml(new list<int>);
	unique_ptr<set<int>>setptr(new set<int>), psetbin(new set<int>), psetxml(new set<int>);
	unique_ptr<map <string, int >>mapptr(new map <string, int >), pmapbin(new map <string, int >), pmapxml(new map <string, int >);
	unique_ptr<pair<string, int>>pairptr(new pair<string, int>), ppairbin(new pair<string, int>), ppairxml(new pair<string, int>);
	*vectorptr = myvec, * listptr = mylist, * setptr = myset, * mapptr = mymap, * pairptr = mypair;

	Tobin::serialize(myvec, "vector");
	Tobin::deserialize(vecbin, "vector");
	Toxml::serialize("vector.xml", myvec);
	Toxml::deserialize("vector.xml", vecxml);
	if (myvec == vecbin && myvec == vecxml)
		cout << "Vector pass!" << endl;
	if (myvec != vecbin)
		cout << "vecbin didn't pass!" << endl;
	if (myvec != vecxml)
		cout << "vecxml didn't pass!" << endl;
	Tobin::serialize(vectorptr, "ptr_vector");
	Tobin::deserialize(pvectorbin, "ptr_vector");
	Toxml::serialize("ptr_vector.xml", vectorptr);
	Toxml::deserialize("ptr_vector.xml", pvectorxml);
	if (*vectorptr == *pvectorbin && *vectorptr == *pvectorxml)
		cout << "unique_ptr for vector passed!" << endl;
	if (*vectorptr != *pvectorbin)
		cout << "unique_ptr for vector bin didn't pass!" << endl;
	if (*vectorptr != *pvectorxml)
		cout << "unique_ptr for vector xml didn't pass!" << endl;
	remove("vector");
	remove("vector.xml");
	remove("vector.base64");
	remove("ptr_vector");
	remove("ptr_vector.xml");
	remove("ptr_vector.base64");



	Tobin::serialize(mylist, "list");
	Tobin::deserialize(listbin, "list");
	Toxml::serialize("list.xml", mylist);
	Toxml::deserialize("list.xml", listxml);
	if (mylist == listbin && mylist == listxml)
		cout << "list pass!" << endl;
	if (mylist != listbin)
		cout << "listbin didn't pass!" << endl;
	if (mylist != listxml)
		cout << "listxml didn't pass!" << endl;
	Tobin::serialize(listptr, "ptr_list");
	Tobin::deserialize(plistbin, "ptr_list");
	Toxml::serialize("ptr_list.xml", listptr);
	Toxml::deserialize("ptr_list.xml", plistxml);
	if (*listptr == *plistbin && *listptr == *plistxml)
		cout << "unique_ptr for list passed!" << endl;
	if (*listptr != *plistbin)
		cout << "unique_ptr for list bin didn't pass!" << endl;
	if (*listptr != *plistxml)
		cout << "unique_ptr for list xml didn't pass!" << endl;
	remove("list");
	remove("list.xml");
	remove("list.base64");
	remove("ptr_list");
	remove("ptr_list.xml");
	remove("ptr_list.base64");



	Tobin::serialize(myset, "set");
	Tobin::deserialize(setbin, "set");
	Toxml::serialize("set.xml", myset);
	Toxml::deserialize("set.xml", setxml);
	if (myset == setbin && myset == setxml)
		cout << "set pass!" << endl;
	if (myset != setbin)
		cout << "setbin didn't pass!" << endl;
	if (myset != setxml)
		cout << "setxml didn't pass!" << endl;
	Tobin::serialize(setptr, "ptr_set");
	Tobin::deserialize(psetbin, "ptr_set");
	Toxml::serialize("ptr_set.xml", setptr);
	Toxml::deserialize("ptr_set.xml", psetxml);
	if (*setptr == *psetbin && *setptr == *psetxml)
		cout << "unique_ptr for set passed!" << endl;
	if (*setptr != *psetbin)
		cout << "unique_ptr for set bin didn't pass!" << endl;
	if (*setptr != *psetxml)
		cout << "unique_ptr for set xml didn't pass!" << endl;
	remove("set");
	remove("set.xml");
	remove("set.base64");
	remove("ptr_set");
	remove("ptr_set.xml");
	remove("ptr_set.base64");




	Tobin::serialize(mypair, "pair");
	Tobin::deserialize(pairbin, "pair");
	Toxml::serialize("pair.xml", pairbin);
	Toxml::deserialize("pair.xml", pairxml);
	if (mypair == pairbin && pairbin == pairxml)
		cout << "pair pass!" << endl;
	if (mypair != pairbin)
		cout << "pairbin didn't pass!" << endl;
	if (mypair != pairxml)
		cout << "pairxml didn't pass!" << endl;
	Tobin::serialize(pairptr, "ptr_pair");
	Tobin::deserialize(ppairbin, "ptr_pair");
	Toxml::serialize("ptr_pair.xml", pairptr);
	Toxml::deserialize("ptr_pair.xml", ppairxml);
	if (*pairptr == *ppairbin && *pairptr == *ppairxml)
		cout << "unique_ptr for pair passed!" << endl;
	if (*pairptr != *ppairbin)
		cout << "unique_ptr for pair bin didn't pass!" << endl;
	if (*pairptr != *ppairxml)
		cout << "unique_ptr for pair xml didn't pass!" << endl;
	remove("pair");
	remove("pair.xml");
	remove("pair.base64");
	remove("ptr_pair");
	remove("ptr_pair.xml");
	remove("ptr_pair.base64");



	Tobin::serialize(mymap, "map");
	Tobin::deserialize(mapbin, "map");
	Toxml::serialize("map.xml", mymap);
	Toxml::deserialize("map.xml", mapxml);
	if (mymap == mapbin && mymap == mapxml)
		cout << "map pass!" << endl;
	if (mymap != mapbin)
		cout << "mapbin didn't pass!" << endl;
	if (mymap != mapxml)
		cout << "mapxml didn't pass!" << endl;
	Tobin::serialize(mapptr, "ptr_map");
	Tobin::deserialize(pmapbin, "ptr_map");
	Toxml::serialize("ptr_map.xml", mapptr);
	Toxml::deserialize("ptr_map.xml", pmapxml);
	if (*mapptr == *pmapbin && *mapptr == *pmapxml)
		cout << "unique_ptr for map passed!" << endl;
	if (*mapptr != *pmapbin)
		cout << "unique_ptr for map bin didn't pass!" << endl;
	if (*mapptr != *pmapxml)
		cout << "unique_ptr for map xml didn't pass!" << endl;
	remove("map");
	remove("map.xml");
	remove("map.base64");
	remove("ptr_map");
	remove("ptr_map.xml");
	remove("ptr_map.base64");

}
void testBasic() {

	string string_orig = "testing!", string_bin, string_xml;
	bool bool_orig = 1, bool_bin, bool_xml;
	short short_orig = 1, short_bin, short_xml;
	unsigned short ushort_orig = 1, ushort_bin, ushort_xml;
	unsigned int uint_orig = 1, uint_bin, uint_xml;
	int int_orig = 1, int_bin, int_xml;
	long long_orig = 1, long_bin, long_xml;
	unsigned long ulong_orig = 1, ulong_bin, ulong_xml;
	float float_orig = 1.567, float_bin, float_xml;
	double double_orig = 1.567, double_bin, double_xml;
	long double ldouble_orig = 1.567, ldouble_bin, ldouble_xml;
	char char_orig = 1, char_bin, char_xml;
	unsigned char uchar_orig = 1, uchar_bin, uchar_xml;
	wchar_t wchar_t_orig = 1, wchar_t_bin, wchar_t_xml;
	char16_t char16_t_orig = 1, char16_t_bin, char16_t_xml;
	char32_t char32_t_orig = 1, char32_t_bin, char32_t_xml;
	long long llong_orig = 1, llong_bin, llong_xml;
	unsigned long long ullong_orig = 1, ullong_bin, ullong_xml;

	testBasicType("string", string_orig, string_bin, string_xml);
	testBasicType("bool", bool_orig, bool_bin, bool_xml);
	testBasicType("short", short_orig, short_bin, short_xml);
	testBasicType("ushort", ushort_orig, ushort_bin, ushort_xml);
	testBasicType("uint", uint_orig, uint_bin, uint_xml);
	testBasicType("int", int_orig, int_bin, int_xml);
	testBasicType("long", long_orig, long_bin, long_xml);
	testBasicType("ulong", ulong_orig, ulong_bin, ulong_orig);
	testBasicType("float", float_orig, float_bin, float_xml);
	testBasicType("double", double_orig, double_bin, double_xml);
	testBasicType("ldouble", ldouble_orig, ldouble_bin, ldouble_xml);
	testBasicType("char", char_orig, char_bin, char_xml);
	testBasicType("uchar", uchar_orig, uchar_bin, uchar_xml);
	testBasicType("llong", llong_orig, llong_bin, llong_xml);
	testBasicType("ullong", ullong_orig, ullong_bin, ullong_xml);


	/*done*/


	

}
void testComplicatedSTL() {
	/*
	Notice:
	since vector and list is almost the same in the aspect of implementation and this function has tested complicated vector,
	complicated list is reasonably ignored.
	since comlicated map are tested in this function, and pair is just a simple map with size being 1, pairs are also ignored in this function.
	*/
	cout << "Finished simple STL testing!" << endl << "Testing for complicated STL" << endl;
	vector<int> myvec, vecbin, vecxml;
	list<int> mylist, listbin, listxml;
	set <int> myset, setbin, setxml;
	pair<string, int> mypair("Test pair!", 999), pairbin, pairxml;
	map<string, int>mymap, mapbin, mapxml;
	for (size_t i = 0; i < 100; i++)
	{
		for (size_t j = 0; j < 100; j++)
		{
			myvec.push_back(i * j + j);
			mylist.push_back(i * j + j);
			myset.insert(i * j + j);
			mymap.insert(pair<string, int>("Hello!", i * j + j));
		}
	}


	vector< map <string, int >>cvector,cvecbin,cvecxml;
	for (size_t i = 0; i < 10000; i++)
	{
		cvector.push_back(mymap);
	}

	//list  一个道理 就不重复了

	set<map<string, int>>cset,csetbin,csetxml;
	for (size_t i = 0; i < 10000; i++)
	{
		cset.insert(mymap);
	}	

	map< vector<map <string, int >>, set<map <string, int >>> cmap,cmapbin,cmapxml;
	
	for (size_t i = 0; i < 10; i++)//不是1w的原因是 。。 1w太耗时了。。。
	{
		cmap.insert(pair< vector<map <string, int >>, set<map <string, int >>>(cvector, cset));
	}

	unique_ptr<vector< map <string, int >>>vectorptr(new vector< map <string, int >>), pvectorbin(new vector< map <string, int >>), pvectorxml(new vector< map <string, int >>);
	unique_ptr<set<map<string, int>>>setptr(new set<map<string, int>>), psetbin(new set<map<string, int>>), psetxml(new set<map<string, int>>);
	unique_ptr<map < vector<map <string, int >>, set<map <string, int >>>>mapptr(new map< vector<map <string, int >>, set<map <string, int >>>), pmapbin(new map < vector<map <string, int >>, set<map <string, int >>>), pmapxml(new map < vector<map <string, int >>, set<map <string, int >>>);
	*vectorptr = cvector, * setptr = cset, * mapptr = cmap;

	Tobin::serialize(cvector, "cvector");
	Tobin::deserialize(cvecbin, "cvector");
	Toxml::serialize("cvector.xml", cvector);
	Toxml::deserialize("cvector.xml", cvecxml);
	cout << "Complicated vector:"<<(cvector == cvecbin && cvector == cvecxml) << endl;
	Tobin::serialize(vectorptr, "ptr_vector");
	Tobin::deserialize(pvectorbin, "ptr_vector");
	Toxml::serialize("ptr_vector.xml", vectorptr);
	Toxml::deserialize("ptr_vector.xml", pvectorxml);
	if (*vectorptr == *pvectorbin && *vectorptr == *pvectorxml)
		cout << "unique_ptr for cvector passed!" << endl;
	if (*vectorptr != *pvectorbin)
		cout << "unique_ptr for cvector bin didn't pass!" << endl;
	if (*vectorptr != *pvectorxml)
		cout << "unique_ptr for cvector xml didn't pass!" << endl;
	cvecbin.clear(); cvecxml.clear(); cvector.clear();
	vectorptr.release(); pvectorbin.release(); pvectorxml.release();
	remove("cvector");
	remove("cvector.xml");
	remove("cvector.base64");
	remove("ptr_vector");
	remove("ptr_vector.xml");
	remove("ptr_vector.base64");




	Tobin::serialize(cset, "cset");
	Tobin::deserialize(csetbin, "cset");
	Toxml::serialize("cset.xml", cset);
	Toxml::deserialize("cset.xml", csetxml);
	cout << "Complicated set:" << (cset == csetbin && cset == csetxml) << endl;
	Tobin::serialize(setptr, "ptr_set");
	Tobin::deserialize(psetbin, "ptr_set");
	Toxml::serialize("ptr_set.xml", setptr);
	Toxml::deserialize("ptr_set.xml", psetxml);
	if (*setptr == *psetbin && *setptr == *psetxml)
		cout << "unique_ptr for cset passed!" << endl;
	if (*setptr != *psetbin)
		cout << "unique_ptr for cset bin didn't pass!" << endl;
	if (*setptr != *psetxml)
		cout << "unique_ptr for cset xml didn't pass!" << endl;
	cset.clear(); csetxml.clear(); csetbin.clear();
	setptr.release(); psetbin.release(); psetxml.release();
	remove("cset");
	remove("cset.xml");
	remove("cset.base64");
	remove("ptr_set");
	remove("ptr_set.xml");
	remove("ptr_set.base64");


	Tobin::serialize(cmap, "cmap");
	Tobin::deserialize(cmapbin, "cmap");
	Toxml::serialize("cmap.xml", cmap);
	Toxml::deserialize("cmap.xml", cmapxml);
	cout << "Complicated map:" << (cmap == cmapbin && cmap == cmapxml) << endl;
	Tobin::serialize(mapptr, "ptr_map");
	Tobin::deserialize(pmapbin, "ptr_map");
	Toxml::serialize("ptr_map.xml", mapptr);
	Toxml::deserialize("ptr_map.xml", pmapxml);
	if (*mapptr == *pmapbin && *mapptr == *pmapxml)
		cout << "unique_ptr for cmap passed!" << endl;
	if (*mapptr != *pmapbin)
		cout << "unique_ptr for cmap bin didn't pass!" << endl;
	if (*mapptr != *pmapxml)
		cout << "unique_ptr for cmap xml didn't pass!" << endl;
	remove("cmap");
	remove("cmap.xml");
	remove("cmap.base64");
	remove("ptr_map");
	remove("ptr_map.xml");
	remove("ptr_map.base64");
}
void testUserdefined() {
	MyStruct test(1,1.45764657),testbin(2,2.2),testxml(3,3.3);
	vector<double> dummy;
	for (size_t i = 0; i < 12; i++)
	{
		dummy.push_back(i / 3.234);
	}
	*(test.ptr) = dummy;
	testbin.vec.clear();
	Tobin::Userialize("user", test.x, test.y, test.vec,test.ptr);
	Tobin::Udeserialize("user", 0, testbin.x, testbin.y, testbin.vec,testbin.ptr);
	testxml.vec.clear();
	Toxml::Userialize("user.xml", test.x, test.y, test.vec,test.ptr);
	Toxml::Udeserialize("user.xml", nullptr, testxml.x, testxml.y, testxml.vec,testxml.ptr);
	if ((test == testbin) && (test == testxml))
		cout << "userDefinedType passed!" << endl;
	if (!(test == testbin)) 
		cout << "bin for userDefiendType didn't pass!" << endl;
	if (!(test == testxml))
		cout << "xml for userDefinedType didn't pass!" << endl;
	remove("user");
	remove("user.xml");
	remove("user.base64");
}
int main() {
	int choice=0;
	do
	{
		cout << "input 1 to do basic testing, 2 to do Simple STL testing, 3 to do Complicated STL testing, 4 to do Userdefined type testing, and 0 to quit." << endl;
		cin >> choice;
		switch (choice)
		{
		case 0: 
			cout << "Bye~" << endl;
			system("pause");
			return 0;
		case 1:
			cout << "Basic types testing: arithmetic string and their unique_str" << endl;
			testBasic();
			break;
		case 2:
			cout << "Basic types testing: simple stl and their unique_str" << endl;
			testSTL();
			break;
		case 3:
			cout << "Embedded types testing: STL & their unique_ptrs" << endl;
			testComplicatedSTL();
			break;
		case 4:
			cout << "YouserDefined types testing: defualt struct contains Arithmetic, STL, unique_str" << endl;
			testUserdefined();
			break;
		}
	} while (choice);

}

