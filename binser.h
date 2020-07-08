#ifndef __BINSER__HEADER__
#define __BINSER__HEADER__

#include <iostream>
#include <typeinfo>
#include <vector>
#include <map>
#include <list>
#include <set>
#include<string>
#include <uchar.h>
#include <fstream>
namespace Tobin {
	//#define BOOL 0x1
	//#define SHORT 0x2
	//#define INT 0x4
	//#define LONG 0x8
	//#define LONGL 0x10
	//#define FLOAT 0x11
	//#define DOUBLE 0x12
	//#define DOUBLEL 0x14
	//#define CHAR 0x18
	//#define WCHART 0x20
	//#define CHAR16 0x21
	//#define CHAR32 0x22
	#define ARITH 0x22;
	#define STRING 0x24;
	#define PTR 0x28;
	#define VEC 0x30;
	#define LI 0x31;
	#define SET 0x32;
	#define MAP 0x34;
	#define PAIR 0x38;
	#define ENDP 0x40;
	template <class T>
	class binser {
	public:
		void ser();
		//The overall interface of serialization.
		void des();
		//The overall interface of deserialization.
		//2 Ctor:
		binser<T>(const std::string&, const T& ); //this one for serailization of ordinary objs.
		binser<T>(const std::string&, const std::unique_ptr<T>&);//this one for serialization of unique_ptr.
		~binser<T>() { file.close(); }
	private:
		std::fstream file;
		std::string fileName;
		T value;
		std::unique_ptr<T> unq;
		char* ptr;
		//Recursion + Template will see to serialization easily and elegantly.
		//Thanks to my compiler!!
		template<class Vec>void ser_parse(const std::vector<Vec>&);//Ser: vector
		template<class li>void ser_parse(const std::list<li>&);//Ser:  list
		template<class ele>void ser_parse(const std::set<ele>&);//Ser:  set
		template<class key, class key_value>void ser_parse(const std::map< key,key_value>&);//Ser: map
		template<class key, class key_value>void ser_parse(const std::pair< key, key_value>&);//Ser: pair
		template<class UPTR>void ser_parse(const std::unique_ptr<UPTR>&);//Ser: unique_ptr
		//Exit of recursion, the class type will either be C++ arithmetic or stirng. 
		inline void ser_parse(const std::string&);//Ser: string
		template<class Arith> inline void ser_parse(const Arith&);//Ser: arithmetic
		
		//
		//Functions for user_defined structure: to be appended.
		//

		//The above is for binary serialization.


		//Codes below see to binary deserialization.
		void des_arith(T);
		void des_vector();
		void des_string();
		void des_map();
		void des_set();
		void des_list();
		void des_pair();
		void des_ptr();
		void des_parse();
	};
	//Cotrs:
	template<class T> binser<T>::binser(const std::string& filename, const T& val) :fileName(filename), value(val) {
			ptr = reinterpret_cast<char*> (&value);
			unq = nullptr;
		}
	template<class T> binser<T>::binser(const std::string& filename, const std::unique_ptr<T>& unique) : fileName(filename) {
		unq = std::unique_ptr<T>(new T);
		*unq = *unique;
	}
	//inner interface of serialization.
	template<class T> void binser< T >::ser() {

		using namespace std;
		file.open(fileName, ios::out | ios::binary);
		if (file.fail())
		{
			cout << "Failed to open!" << endl;
			return;
		}
		if (!unq)
			ser_parse(value);
		else
			ser_parse(unq);
		file.close();
	}
	//vector
	template<class T> template<class Vec>void binser <T>::ser_parse(const std::vector<Vec>& myvec) {
		int type = VEC;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		type = myvec.size();
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		for (auto iter : myvec)
		{
			ser_parse(iter);
		}
		
	}
	//list
	template<class T> template<class li>void binser <T>::ser_parse(const std::list<li>& mylist) {
		int type = LI;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		type = mylist.size();
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		for (auto iter : mylist)
		{
			ser_parse(iter);
		}
	}
	//set
	template<class T> template<class ele>void binser <T>::ser_parse(const std::set<ele>& myset) {
		int type = SET;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		type = myset.size();
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		for (auto iter : myset)
		{
			ser_parse(iter);
		}
		
	}
	//map
	template<class T> template<class key, class key_value> void binser<T>::ser_parse(const std::map<key, key_value>&mymap){
		int type = MAP;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		type = mymap.size();
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		for (auto iter : mymap)
			ser_parse(iter);
	}
	//pair
	template<class T> template<class key, class key_value> void binser<T>::ser_parse(const std::pair<key, key_value>&mypair) {
		int type = PAIR;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		ser_parse(mypair.first);
		ser_parse(mypair.second);
	}
	//ptr
	template<class T> template<class UPTR> void binser<T>::ser_parse(const std:: unique_ptr<UPTR> &unq) {
		int type = PTR;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		ser_parse(*unq);
		type = ENDP;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
	}
	//string
	template<class T> void binser<T>::ser_parse(const std::string& value) {
		const char* str_ptr = value.c_str();
		int length = STRING;
		file.write(reinterpret_cast<char*>(&length), sizeof(length));
		length = value.length() + 1;
		file.write(reinterpret_cast<char*>(&length), sizeof(length));
		//先写入字符串标识符，再写入字符串实际长度，最后写入字符串内容
		file.write(str_ptr, length);//'\0'也要写进去
	}
	//arithmetic type
	template<class T> template<class Arith>inline void binser< T>::ser_parse(const Arith& value) {
		int type = ARITH;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		type = sizeof(value);
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		//写入类型信息  好像真没有必要
		//int type = 0;
		//switch (typeid(TA).name())
		//{
		//case"bool":
		//case"short":
		//case "unsigned short":
		//case "int":
		//case "unsigned int":
		//	case ""

		//default:
		//	break;
		//}
		//file.write(reinterpret_cast<char*>(&type), sizeof(int));
		type = value;
		file.write(reinterpret_cast<char*>(&type), sizeof(value));
		//ptr += size;
	}
	//done for 2 serailization.
	


	//deserialization.
	template<class T> inline void binser< T >::des_arith(T value){
		int size = 0, type = 0;
		file.read(reinterpret_cast<char*>(&type), sizeof(int));
		size = sizeof(T);
		//写入类型信息
		file.read(ptr, size);
		ptr += size;
	}

	//以上为二进制序列化部分
	//以下为二进制反序列化部分
	//template<class T>template<class key,class key_value> void binser<T>::ser_map(std::pair<key, key_value>) {
	//	std::is_arithmetic<key>check;
	//	std::string type_name = typeid(key).name();
	//	if (check.value) {
	//		ser_arith(value);
	//	}
	//	else if (type_name.find("class std::basic_string") != std::string::npos && type_name.substr(11, 23) == "basic_string") {
	//		ser_string(value);
	//	}
	//	else
	//	{
	//		if (type_name.find("class std::vector" != std::string::npos) && type_name.substr(11, 16) == "vector")
	//		{
	//			for (auto iter : value)
	//				ser_vector(iter);
	//		}
	//		else if (type_name.find("class std::list") != std::string::npos && type_name.substr(11, 14) == "list")
	//		{
	//			for (auto iter : value)
	//				ser_list(iter);
	//		}
	//		else if (type_name.find("class std::set") != std::string::npos && type_name.substr(11, 13) == "set") {
	//			for (auto iter : value)
	//				ser_set(set);
	//		}
	//		else if (type_name.find("struct std::pair") != std::string::npos && type_name.substr(11, 14) == "pair") {
	//			for (auto iter : value)
	//				ser_pair(iter);
	//		}
	//		else if (type_name.find("class std::map") != std::string::npos && type_name.substr(11, 13) == "map")
	//		{
	//			for (auto i = value.begin(); i != value.end(); i++)
	//			{
	//				ser_pair(i->first(), i->second());
	//			}
	//		}
	//		else
	//		{
	//			//may be user defined structure
	//		}
	//	}
	//}

}
#endif // !__BINSER__HEADER__

//template<class T> template<class TVLS>void binser <T>::ser_parse(TVLS toparse) {
//	std::is_arithmetic<TVLS>check;
//	if (check.value) {//算数类型
//		ser_arith(value);
//	}
//	else
//	{
//		/*for set, vector, listand string whose elements are of only a unique type, recursion will solve it.
//			for map, pari, we developed corresbonding specific template type function to handle;*/
//		std::string type_name = typeid(toparse).name();
//		if (type_name.find("class std::vector") != std::string::npos && type_name.substr(11, 16) == "vector")
//		{
//			for (auto iter : toparse)
//				ser_parse(iter);
//		}
//		else if (type_name.find("class std::list") != std::string::npos && type_name.substr(11, 14) == "list")
//		{
//			for (auto iter : toparse)
//				ser_parse(iter);
//		}
//		else if (type_name.find("class std::set") != std::string::npos && type_name.substr(11, 13) == "set") {
//			for (auto iter : toparse)
//				ser_parse(iter);
//		}
//		else if (type_name.find("struct std::pair") != std::string::npos && type_name.substr(11, 14) == "pair") {
//			ser_parse(toparse);
//		}
//		else if (type_name.find("class std::basic_string") != std::string::npos && type_name.substr(11, 23) == "basic_string") {
//			ser_string(toparse);
//		}
//		else if (type_name.find("class std::map") != std::string::npos && type_name.substr(11, 13) == "map")
//		{
//			for (auto i = toparse.begin(); i != toparse.end(); i++)
//			{
//				ser_parse(i->first(), i->second());
//			}
//		}
//		else if (type_name.find("class std::unique_ptr") != std::string::npos && type_name.substr(11,20 ) == "unique_ptr")
//		{
//			ser_parse(toparse);
//		}
//		else
//		{
//			//自定义类型
//		}
//	}
//}


//template<class T> void binser<T>::ser_vector(T value) {
//	std::is_arithmetic<T>check;
//	std::string type_name = typeid(T).name();
//	if (check.value) {
//		ser_arith(value);
//	}
//	else if (type_name.find("class std::basic_string") != std::string::npos && type_name.substr(11, 23) == "basic_string") {
//		ser_string(value);
//	}
//	else
//	{
//		if (type_name.find("class std::vector" != std::string::npos) && type_name.substr(11, 16) == "vector")
//		{
//			for(auto iter:value)
//			ser_vector(iter);
//		}
//		else if (type_name.find("class std::list") != std::string::npos && type_name.substr(11, 14) == "list")
//		{
//			for (auto iter : value)
//			ser_list(iter);
//		}
//		else if (type_name.find("class std::set") != std::string::npos && type_name.substr(11, 13) == "set") {
//			for (auto iter : value)
//			ser_set(set);
//		}
//		else if (type_name.find("struct std::pair") != std::string::npos && type_name.substr(11, 14) == "pair") {
//			for (auto iter : value)
//			ser_pair(iter);
//		}
//		else if(type_name.find("class std::map") != std::string::npos && type_name.substr(11, 13) == "map")
//		{
//			for (auto i = value.begin(); i != value.end(); i++)
//			{
//				ser_pair(i->first(), i->second());
//			}
//		}
//		else
//		{
//			//may be user defined structure
//		}
//	}

//}
//template<class T> void binser<T>::ser_list(T vlaue){
//	std::is_arithmetic<T>check;
//	std::string type_name = typeid(T).name();
//	if (check.value) {
//		ser_arith(value);
//	}
//	else if (type_name.find("class std::basic_string") != std::string::npos && type_name.substr(11, 23) == "basic_string") {
//		ser_string(value);
//	}
//	else
//	{
//		if (type_name.find("class std::vector" != std::string::npos) && type_name.substr(11, 16) == "vector")
//		{
//			for (auto iter : value)
//				ser_vector(iter);
//		}
//		else if (type_name.find("class std::list") != std::string::npos && type_name.substr(11, 14) == "list")
//		{
//			for (auto iter : value)
//				ser_list(iter);
//		}
//		else if (type_name.find("class std::set") != std::string::npos && type_name.substr(11, 13) == "set") {
//			for (auto iter : value)
//				ser_set(set);
//		}
//		else if (type_name.find("struct std::pair") != std::string::npos && type_name.substr(11, 14) == "pair") {
//			ser_parse()
//		}
//		else if (type_name.find("class std::map") != std::string::npos && type_name.substr(11, 13) == "map")
//		{
//			for (auto i = value.begin(); i != value.end(); i++)
//			{
//				ser_pair(i->first(), i->second());
//			}
//		}
//		else
//		{
//			//may be user defined structure
//		}
//	}
//}
//template<class T> void binser<T>::ser_set(T vlaue) {
//	std::is_arithmetic<T>check;
//	std::string type_name = typeid(T).name();
//	if (check.value) {
//		ser_arith(value);
//	}
//	else if (type_name.find("class std::basic_string") != std::string::npos && type_name.substr(11, 23) == "basic_string") {
//		ser_string(value);
//	}
//	else
//	{
//		if (type_name.find("class std::vector" != std::string::npos) && type_name.substr(11, 16) == "vector")
//		{
//			for (auto iter : value)
//				ser_vector(iter);
//		}
//		else if (type_name.find("class std::list") != std::string::npos && type_name.substr(11, 14) == "list")
//		{
//			for (auto iter : value)
//				ser_list(iter);
//		}
//		else if (type_name.find("class std::set") != std::string::npos && type_name.substr(11, 13) == "set") {
//			for (auto iter : value)
//				ser_set(set);
//		}
//		else if (type_name.find("struct std::pair") != std::string::npos && type_name.substr(11, 14) == "pair") {
//			ser_parse()
//		}
//		else if (type_name.find("class std::map") != std::string::npos && type_name.substr(11, 13) == "map")
//		{
//			for (auto i = value.begin(); i != value.end(); i++)
//			{
//				ser_pair(i->first(), i->second());
//			}
//		}
//		else
//		{
//			//may be user defined structure
//		}
//	}
//}