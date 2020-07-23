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
	#define ARITH 0x22;
	#define STRING 0x24;
	#define PTR 0x28;
	#define VEC 0x30;
	#define LI 0x31;
	#define SET 0x32;
	#define MAP 0x34;
	#define PAIR 0x38;
	#define ENDP 0x40;
	//以上为定义的标识，其实在序列化过程中没有用到
	template <class T>
	class binser {
	public:
		int ser();
		//The overall interface of serialization.
		int des(T&);
		int des(std::unique_ptr<T>&);
		//void desUserDefined(userdefined);
		//The overall interface of deserialization.
		//2 Ctor:
		binser<T>(const std::string&, const T& ,const int =0); //this one for serailization of ordinary objs.
		binser<T>(const std::string&, const std::unique_ptr<T>&, const int =0);//this one for serialization of unique_ptr.
		~binser<T>() { file.close(); }
	private:
		std::fstream file;
		std::string fileName;
		T value;
		std::unique_ptr<T> unq;
		char* ptr;
		int filePtr;
		//Recursion + Template will see to serialization easily and elegantly.
		//Thanks to my compiler!!!
		template<class Vec>void ser_parse(const std::vector<Vec>&);//Ser: vector
		template<class li>void ser_parse(const std::list<li>&);//Ser:  list
		template<class ele>void ser_parse(const std::set<ele>&);//Ser:  set
		template<class key, class key_value>void ser_parse(const std::map< key,key_value>&);//Ser: map
		template<class key, class key_value>void ser_parse(const std::pair< key, key_value>&);//Ser: pair
		template<class UPTR>void ser_parse(const std::unique_ptr<UPTR>&);//Ser: unique_ptr
		//Exit of recursion, the class type will either be C++ arithmetic or stirng. 
		inline void ser_parse(const std::string&);
		template<class Arith> inline void ser_parse(const Arith&);//Ser: arithmetic
		

		//Codes below see to binary deserialization.
		template<class Vec>void des_parse( std::vector<Vec>&);//des: vector
		template<class li>void des_parse( std::list<li>&);//des:  list
		template<class ele>void des_parse( std::set<ele>&);//des:  set
		template<class key, class key_value>void des_parse( std::map< key, key_value>&);//des: map
		template<class key, class key_value>void des_parse( std::pair< key, key_value>&);//des: pair
		template<class UPTR>void des_parse( std::unique_ptr<UPTR>&);//des: unique_ptr
		//Exit of recursion, the class type will either be C++ arithmetic or stirng. 
		inline void des_parse( std::string&);//des: string
		template<class Arith> inline void des_parse( Arith&);//des: arithmetic
	};
	//Cotrs:
	template<class T> binser<T>::binser(const std::string& filename, const T& val,const int filePos) :fileName(filename), value(val),filePtr(filePos) {
			ptr = reinterpret_cast<char*> (&value);
			unq = nullptr;
		}
	template<class T> binser<T>::binser(const std::string& filename, const std::unique_ptr<T>& unique, const int filePos) : fileName(filename), filePtr(filePos) {
		unq = std::unique_ptr<T>(new T);
		*unq = *unique;
	}
	//inner interface of serialization.
	template<class T> int binser< T >::ser() {

		using namespace std;
		file.open(fileName, ios::out | ios::binary|ios::app);
		if (file.fail())
		{
			cout << "Failed to open!" << endl;
			return -1;
		}
		file.seekp(filePtr);
		if (!unq)
			ser_parse(value);
		else
			ser_parse(unq);
		int temp = file.tellp();
		file.close();
		return temp;
	}
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
		file.seekp(std::ios::end);
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
		file.seekp(std::ios::end);
		int type = ARITH;
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		type = sizeof(value);
		file.write(reinterpret_cast<char*>(&type), sizeof(type));
		file.write((char *)(&value), sizeof(value));
	}


	template<class T>template<class Vec>void binser <T>::des_parse(std::vector<Vec>&putin) {
		int size;
		file.read(reinterpret_cast<char*>(&size), sizeof(size));//type
		file.read(reinterpret_cast<char*>(&size), sizeof(size));//confirm  size of the vector
		for (size_t i = 0; i < size; i++)
		{
			Vec temp;
			des_parse(temp);
			putin.push_back(temp);
		}
	}
	template<class T>template<class li>void binser <T>::des_parse(std::list<li>&putin) {
		int size;
		file.read(reinterpret_cast<char*>(&size), sizeof(size));//type
		file.read(reinterpret_cast<char*>(&size), sizeof(size));//confirm  size of the vector
		for (size_t i = 0; i < size; i++)
		{
			li temp;
			des_parse(temp);
			putin.push_back(temp);
		}
	}
	template<class T>template<class ele>void binser <T>::des_parse(std::set<ele>&putin) {
		int size;
		file.read(reinterpret_cast<char*>(&size), sizeof(size));//type
		file.read(reinterpret_cast<char*>(&size), sizeof(size));//confirm  size of the vector
		for (size_t i = 0; i < size; i++)
		{
			ele temp;
			des_parse(temp);
			putin.insert(temp);
		}
	}
	template<class T>template<class key, class key_value>void binser <T>::des_parse(std::map< key, key_value>&putin) {
		int size;
		file.read(reinterpret_cast<char*>(&size), sizeof(size));//type
		file.read(reinterpret_cast<char*>(&size), sizeof(size));//confirm  size of the vector
		for (size_t i = 0; i < size; i++)
		{
			std::pair<key, key_value> temp;
			des_parse(temp);
			putin.insert(temp);
		}
	}
	template<class T>template<class key, class key_value>void binser <T>::des_parse(std::pair< key, key_value>&putin) {
		int size;
		file.read(reinterpret_cast<char*>(&size), sizeof(size));//type   pai没有size
		des_parse(putin.first);
		des_parse(putin.second);
	}
	template<class T>template<class UPTR>void binser <T>::des_parse(std::unique_ptr<UPTR>&putin) {
		int check;
		file.read(reinterpret_cast<char*>(&check), sizeof(check));//指针开始标志
		des_parse(*putin);
		file.read(reinterpret_cast<char*>(&check), sizeof(check));//指针结束标志
	}
	template<class T>inline void binser <T>::des_parse(std::string&putin) {
		int length;
		file.read(reinterpret_cast<char*>(&length), sizeof(length));//读类型
		file.read(reinterpret_cast<char*>(&length), sizeof(length));//读长度
		std::unique_ptr<char> tempptr(new char[length]);
		file.read(tempptr.get(), length);
		putin = tempptr.get();
	}
	template<class T>template<class Arith> inline void binser <T>::des_parse(Arith& putin) {
		int size;
		file.read(reinterpret_cast<char*>(&size), sizeof(int));//读类型进来
		file.read(reinterpret_cast<char*>(&size), sizeof(int));//读长度
		file.read(reinterpret_cast<char*>(&putin), size);
	}//des: arithmetic
	template<class T> int binser< T >::des(T&putin) {

		using namespace std;
		file.open(fileName, ios::in | ios::binary);
		if (file.fail())
		{
			cout << "Failed to open!" << endl;
			return -1;
		}
		file.seekp(filePtr);
		des_parse(putin);
		int temp = file.tellp();
		file.close();
		return temp;
	}
	template<class T> int binser< T >::des(std:: unique_ptr<T>& putin) {
		using namespace std;
		file.open(fileName, ios::in | ios::binary);
		if (file.fail())
		{
			cout << "Failed to open!" << endl;
			return -1;
		}
		file.seekp(filePtr);
		int type;
		file.read(reinterpret_cast<char*>(&type), sizeof(type));
		des_parse(*putin);
		file.read(reinterpret_cast<char*>(&type), sizeof(type));
		type = file.tellp();
		file.close();
		return type;
	}


	template <class T> void serialize(T& value, std::string file) {
		binser<T>mybin(file, value);
		mybin.ser();
	}
	//strategy for user defined type:
	//use template with various classes and values

	template <class T> void serialize(std::unique_ptr<T>& value, std::string file) {
		binser<T>mybin(file, value);
		mybin.ser();
	}
	void Userialize(std::string file) {
		return;
	}
	template<class T1, class ... T2>void Userialize(std::string file, T1& start, T2& ... val) {
		serialize(start,file);
		Userialize(file, val...);
	}
	template <class T> int deserialize(std::unique_ptr<T>& putin, std::string file,int filepos=0) {
		binser<T>mybin(file, putin,filepos);
		return mybin.des(putin);
	}
	template <class T> int deserialize(T& putin, std::string file, int filepos = 0) {
		binser<T>mybin(file, putin,filepos);
		return mybin.des(putin);
	}
	void Udeserialize(std::string file,int filepos) {
		return;
	}
	template <class T1, class ... T2> void Udeserialize(std::string file, int filepos,T1& start, T2& ... val ) {
		int nextfilepos = deserialize(start, file,filepos);
		Udeserialize(file, nextfilepos,  val...);
	}

}
#endif
