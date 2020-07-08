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
	#define BOOL 0x1
	#define SHORT 0x2
	//#define USHORT 0x4;
	#define INT 0x4
	#define LONG 0x8
	#define LONGL 0x10
	#define FLOAT 0x11
	#define DOUBLE 0x12
	#define DOUBLEL 0x14
	#define CHAR 0x18
	#define WCHART 0x20
	#define CHAR16 0x21
	#define CHAR32 0x22
	#define STRING 0x24;
	#define PTR 0x28;
	template <class T>
	class binser {
	public:
		void ser();
		void des();
		binser<T>(std::string&, T ); 
	private:
		std::fstream file;
		std::string fileName;
		T value;
		char* ptr;
		inline void openfile(int opentype);
		inline void closefile();
		//inline void readByte();
		//inline void writeByte();
		template <class TA>void ser_arith(TA);//递归到最后的出口 要么是算数类型
		void ser_string(std::string);//要么是字符串
		template<class TVLS>void ser_parse(TVLS);//序列化 vector list set

		template<class Vec>void ser_parse(std::vector<Vec>);//序列化 vector
		template<class li>void ser_parse(std::list<li>);//序列化 list
		template<class ele>void ser_parse(std::set<ele>);//序列化 set
		template<class key, class key_value>void ser_parse(std::map< key,key_value>);//序列化map
		template<class key, class key_value>void ser_parse(std::pair< key, key_value>);//序列化pair
		template<class UPTR>void ser_parse(std::unique_ptr<UPTR>);//序列化智能指针
		inline void ser_parse(std::string);//序列化 字符串
		template<class Arith> inline void ser_parse(Arith);//序列化算数类型

		//以上为二进制序列化部分
		//以下为二进制反序列化部分
		void des_arith(T);
		void des_vector();
		void des_string();
		void des_map();
		void des_set();
		void des_list();
		void des_pair();
		void des_ptr();
		void des_parse();
		//template <class key, class key_value>
		//void ser_map(std::pair<key, key_value>);
		//void ser_set(T);
		//void ser_list(T);
		//void ser_vector(T);
		//template <class key, class key_value>
		//void ser_pair(key,key_value);
	};
	template<class T> binser<T>::binser(std::string& filename, T  val) :fileName(filename), value(val) {
			ptr = reinterpret_cast<char*> (&value);
		}
	template<class T> inline void binser< T>::openfile(int opentype) {
		file.open(fileName, opentype);
	}
	template<class T> inline void binser< T>::closefile() {
		file.close();
	}
	//vector
	template<class T> template<class Vec>void binser <T>::ser_parse(std::vector<Vec> myvec) {
		std::is_arithmetic<Vec>check;
		std::string type_name = typeid(Vec).name();
		if (check.value) {//算数类型
			for (auto iter : myvec)
			{
				ser_arith(iter);
			}
		}
		else
		{
			/*for set, vector, listand string whose elements are of only a unique type, recursion will do well.
				for map, pari, we developed corresbonding specific template type function to handle;*/
			if (type_name.find("class std::vector") != std::string::npos && type_name.substr(11, 16) == "vector")
			{
				for (auto iter : myvec)
					ser_parse(iter);
			}
			else if (type_name.find("class std::list") != std::string::npos && type_name.substr(11, 14) == "list")
			{
				for (auto iter : myvec)
					ser_parse(iter);
			}
			else if (type_name.find("class std::set") != std::string::npos && type_name.substr(11, 13) == "set") {
				for (auto iter : myvec)
					ser_parse(iter);
			}
			else if (type_name.find("struct std::pair") != std::string::npos && type_name.substr(11, 14) == "pair") {
				for (auto iter : myvec)
					ser_parse(iter);
			}
			else if (type_name.find("class std::basic_string") != std::string::npos && type_name.substr(11, 23) == "basic_string") {
				for (auto iter : myvec)
					ser_string(iter);
			}
			else if (type_name.find("class std::map") != std::string::npos && type_name.substr(11, 13) == "map")
			{
				for (auto i = myvec.begin(); i != myvec.end(); i++)
				{
					ser_parse(i->first(), i->second());
				}
			}
			else if (type_name.find("class std::unique_ptr") != std::string::npos && type_name.substr(11, 20) == "unique_ptr")
			{
				for (auto iter : myvec)
					ser_parse(iter);
			}
			else
			{
				//自定义类型
			}
		}
	}
	//list
	template<class T> template<class li>void binser <T>::ser_parse(std::list<li> mylist) {
		std::is_arithmetic<li>check;
		std::string type_name = typeid(li).name();
		if (check.value) {//算数类型
			for (auto iter : mylist)
			{
				ser_arith(iter);
			}
		}
		else
		{
			/*for set, vector, listand string whose elements are of only a unique type, recursion will do well.
				for map, pari, we developed corresbonding specific template type function to handle;*/
			if (type_name.find("class std::vector") != std::string::npos && type_name.substr(11, 16) == "vector")
			{
				for (auto iter : mylist)
					ser_parse(iter);
			}
			else if (type_name.find("class std::list") != std::string::npos && type_name.substr(11, 14) == "list")
			{
				for (auto iter : mylist)
					ser_parse(iter);
			}
			else if (type_name.find("class std::set") != std::string::npos && type_name.substr(11, 13) == "set") {
				for (auto iter : mylist)
					ser_parse(iter);
			}
			else if (type_name.find("struct std::pair") != std::string::npos && type_name.substr(11, 14) == "pair") {
				for (auto iter : mylist)
					ser_parse(iter);
			}
			else if (type_name.find("class std::basic_string") != std::string::npos && type_name.substr(11, 23) == "basic_string") {
				for (auto iter : mylist)
					ser_string(iter);
			}
			else if (type_name.find("class std::map") != std::string::npos && type_name.substr(11, 13) == "map")
			{
				for (auto i = mylist.begin(); i != mylist.end(); i++)
				{
					ser_parse(i->first(), i->second());
				}
			}
			else if (type_name.find("class std::unique_ptr") != std::string::npos && type_name.substr(11, 20) == "unique_ptr")
			{
				for (auto iter : mylist)
					ser_parse(iter);
			}
			else
			{
				//自定义类型
			}
		}
	}
	//set
	template<class T> template<class ele>void binser <T>::ser_parse(std::set<ele> myset) {
		std::is_arithmetic<ele>check;
		std::string type_name = typeid(ele).name();
		if (check.value) {//算数类型
			for (auto iter : myset)
			{
				ser_arith(iter);
			}
		}
		else
		{
			/*for set, vector, listand string whose elements are of only a unique type, recursion will do well.
				for map, pari, we developed corresbonding specific template type function to handle;*/
			if (type_name.find("class std::vector") != std::string::npos && type_name.substr(11, 16) == "vector")
			{
				for (auto iter : myset)
					ser_parse(iter);
			}
			else if (type_name.find("class std::list") != std::string::npos && type_name.substr(11, 14) == "list")
			{
				for (auto iter : myset)
					ser_parse(iter);
			}
			else if (type_name.find("class std::set") != std::string::npos && type_name.substr(11, 13) == "set") {
				for (auto iter : myset)
					ser_parse(iter);
			}
			else if (type_name.find("struct std::pair") != std::string::npos && type_name.substr(11, 14) == "pair") {
				for (auto iter : myset)
					ser_parse(iter);
			}
			else if (type_name.find("class std::basic_string") != std::string::npos && type_name.substr(11, 23) == "basic_string") {
				for (auto iter : myset)
					ser_string(iter);
			}
			else if (type_name.find("class std::map") != std::string::npos && type_name.substr(11, 13) == "map")
			{
				for (auto i = myset.begin(); i != myset.end(); i++)
				{
					ser_parse(i->first(), i->second());
				}
			}
			else if (type_name.find("class std::unique_ptr") != std::string::npos && type_name.substr(11, 20) == "unique_ptr")
			{
				for (auto iter : myset)
					ser_parse(iter);
			}
			else
			{
				//自定义类型
			}
		}
	}
	//map
	template<class T> template<class key, class key_value> void binser<T>::ser_parse(std::map<key, key_value>mymap){
		for (auto iter : mymap)
			ser_parse(iter);
	}
	//pair
	template<class T> template<class key, class key_value> void binser<T>::ser_parse(std::pair<key, key_value>mypair) {
		ser_parse(mypair.first());
		ser_parse(mypair.second());
	}
	//ptr
	template<class T> template<class UPTR> void binser<T>::ser_parse(std:: unique_ptr<UPTR> ptr) {
		int type = PTR;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		ser_parse(*ptr);
	}
	//string
	template<class T> void binser<T>::ser_parse(std::string value) {
		const char* str_ptr = value.c_str();
		int length = STRING;
		file.write(reinterpret_cast<char*>(&length), sizeof(length));
		length = value.length() + 1;
		file.write(reinterpret_cast<char*>(&length), sizeof(length));
		//先写入字符串标识符，再写入字符串实际长度，最后写入字符串内容
		file.write(str_ptr, length);//'\0'也要写进去
	}
	//arith
	template<class T> template<class Arith>inline void binser< T>::ser_parse(Arith value) {
		int size = sizeof(value);
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
		file.write(ptr, size);
		ptr += size;
	}
	template<class T> inline void binser< T >::des_arith(T value){
		int size = 0, type = 0;
		file.read(reinterpret_cast<char*>(&type), sizeof(int));
		size = sizeof(T);
		//写入类型信息
		file.read(ptr, size);
		ptr += size;
	}
	template<class T> void binser< T >:: ser() {
		openfile(std::ios::in | std::ios::out | std::ios::binary);
		ser_parse(value);
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