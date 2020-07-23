#pragma once
#ifndef __XML__HEADER__
#define __XML__HEADER__

#include <iostream>
#include <typeinfo>
#include <vector>
#include <map>
#include <list>
#include <set>
#include<string>
#include <uchar.h>
#include <fstream>
#include <sstream>
#include "tinyxml2.h"
namespace Toxml {
	template <class T>
	class xmlser {
	public:
		void ser(bool =0);
		tinyxml2::XMLElement* des(T& );//开始的
		tinyxml2::XMLElement* des(std::unique_ptr<T>&);
		
		//2 Ctor:
		xmlser<T>(const std::string&, const T&, std::stringstream&, tinyxml2::XMLDocument* =nullptr,tinyxml2::XMLElement* =nullptr); //this one for serailization of ordinary objs.
		xmlser<T>(const std::string&, const std::unique_ptr<T>&, std::stringstream&, tinyxml2::XMLDocument* = nullptr, tinyxml2::XMLElement* = nullptr);//this one for serialization of unique_ptr.
	private:
		std::string fileName;
		T value;
		std::unique_ptr<T> unq;
		tinyxml2::XMLDocument* xml;
		tinyxml2::XMLElement* rootNode;
		tinyxml2::XMLElement* lastVisit;
		std::stringstream& mystream;
		//Userdefined type

		//Recursion + Template will see to serialization easily and elegantly.
		/*
		Overall method:with the following STL specified and unique_ptr specified template functions,
		we can simply use recursion to serialize the elements of a certain STL container.
		don't need to consider whatever the type the element is because that's what compilers do.
		Thanks to all coworkers of my compilers!!!Sincerely!!!
		!(*╹▽╹*)!!    !(*╹▽╹*)!!		!(*╹▽╹*)!!
		As for unique_ptr, we construct a unique_ptr of the same type inside the object and reconstruct elements into that unique_ptr,
		before returning, we simply do *outter_unique_ptr = *insider_uniqe_ptr. and that's done.(deep copy required.
		*/
		template<class Vec>void ser_parse(const std::vector<Vec>&, tinyxml2::XMLElement*);//Ser: vector
		template<class li>void ser_parse(const std::list<li>&, tinyxml2::XMLElement*);//Ser:  list
		template<class ele>void ser_parse(const std::set<ele>&, tinyxml2::XMLElement*);//Ser:  set
		template<class key, class key_value>void ser_parse(const std::map< key, key_value>&, tinyxml2::XMLElement*);//Ser: map
		template<class key, class key_value>void ser_parse(const std::pair< key, key_value>&, tinyxml2::XMLElement*);//Ser: pair
		template<class UPTR>void ser_parse(const std::unique_ptr<UPTR>&, tinyxml2::XMLElement*);//Ser: unique_ptr
		
																								//Exit of recursion, the class type will either be C++ arithmetic or stirng. 
		inline void ser_parse(const std::string&, tinyxml2::XMLElement* );//Ser: string
		inline void ser_parse(const bool&, tinyxml2::XMLElement* );
		inline void ser_parse(const short&, tinyxml2::XMLElement* );
		inline void ser_parse(const unsigned short&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void ser_parse(const unsigned int&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void ser_parse(const int&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void ser_parse(const long&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void ser_parse(const unsigned long&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void ser_parse(const float&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void ser_parse(const double&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void ser_parse(const long double&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void ser_parse(const char&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void ser_parse(const unsigned char&, tinyxml2::XMLElement* );
		inline void ser_parse(const wchar_t&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void ser_parse(const char16_t&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void ser_parse(const char32_t&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void ser_parse(const long long&, tinyxml2::XMLElement* );
		inline void ser_parse(const unsigned long long&, tinyxml2::XMLElement* );

		template<class userdefined>void ser_parse(const userdefined);//Ser: userdefinedtype
		//
		//done for original type xml serialization.
		//no user defiend type support yet.
		//brilliant !

		//Codes below see to binary deserialization.
		template<class Vec>void des_parse(std::vector<Vec>&, tinyxml2::XMLElement*);//des: vector
		template<class li>void des_parse(std::list<li>&, tinyxml2::XMLElement*);//des:  list
		template<class ele>void des_parse(std::set<ele>&, tinyxml2::XMLElement*);//des:  set
		template<class key, class key_value>void des_parse(std::map< key, key_value>&, tinyxml2::XMLElement*);//des: map
		template<class key, class key_value>void des_parse(std::pair< key, key_value>&, tinyxml2::XMLElement*);//des: pair
		template<class UPTR>void des_parse(std::unique_ptr<UPTR>&, tinyxml2::XMLElement*);//des: unique_ptr

		//Exit of recursion, the class type will either be C++ arithmetic or stirng. 
		inline void des_parse(std::string&, tinyxml2::XMLElement*);//des: string
		inline void des_parse( bool&, tinyxml2::XMLElement* );
		inline void des_parse( short&, tinyxml2::XMLElement* );
		inline void des_parse( unsigned short&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void des_parse( unsigned int&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void des_parse( int&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void des_parse( long&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void des_parse( unsigned long&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void des_parse( float&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void des_parse( double&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void des_parse( long double&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void des_parse( char&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void des_parse( unsigned char&, tinyxml2::XMLElement* );
		inline void des_parse( wchar_t&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void des_parse( char16_t&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void des_parse( char32_t&, tinyxml2::XMLElement* );//Ser: arithmetic
		inline void des_parse( long long&, tinyxml2::XMLElement* );
		inline void des_parse( unsigned long long&, tinyxml2::XMLElement* );
	};
	//Cotrs:
	template<class T> xmlser<T>::xmlser(const std::string& filename, const T& val, std::stringstream& Tool, tinyxml2::XMLDocument* Overall, tinyxml2::XMLElement* last) :fileName(filename), value(val), mystream(Tool), xml(Overall),lastVisit(last) {
		unq = nullptr;
	}
	template<class T> xmlser<T>::xmlser(const std::string& filename, const std::unique_ptr<T>& unique, std::stringstream&Tool, tinyxml2::XMLDocument* Overall, tinyxml2::XMLElement* last) : fileName(filename), mystream(Tool),xml(Overall),lastVisit(last) {
		unq = std::unique_ptr<T>(new T);
		*unq = *unique;
	}
	
	
	
	//outter interface of serialization.
	template<class T> void xmlser< T >::ser(bool firstTime) {
		using namespace tinyxml2;
		xml->LoadFile(fileName.c_str());
		if (firstTime && !(rootNode = xml->RootElement())) {

			XMLDeclaration* declaration = xml->NewDeclaration();
			xml->InsertFirstChild(declaration);
			rootNode = xml->NewElement("Serailization");
			xml->InsertEndChild(rootNode);
		}
		else 
			rootNode = xml->RootElement();
			std::string nextVisit;
			if (!unq)
			{
				nextVisit = rootNode->Name();
				ser_parse(value, rootNode);
			}
			else {
				tinyxml2::XMLElement* uniq_ptr = xml->NewElement("Unique_ptr");
				rootNode->InsertEndChild(uniq_ptr);
				nextVisit = uniq_ptr->Name();
				ser_parse(*unq, uniq_ptr);
			}
			xml->SaveFile(fileName.c_str());
		
	}
	
	//vector
	template<class T> template<class Vec>void xmlser <T>::ser_parse(const std::vector<Vec>& myvec, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Vect = xml->NewElement("Vector");
		for (auto iter : myvec)
			ser_parse(iter, Vect);
		Vect->SetAttribute("Size", myvec.size());
		father->InsertEndChild(Vect);
	}
	//list
	template<class T> template<class li>void xmlser <T>::ser_parse(const std::list<li>& mylist, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* List = xml->NewElement("List");
		for (auto iter : mylist)
			ser_parse(iter, List);
		List->SetAttribute("Size", mylist.size());
		father->InsertEndChild(List);
	}
	//set
	template<class T> template<class ele>void xmlser <T>::ser_parse(const std::set<ele>& myset, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Set = xml->NewElement("Set");
		for (auto iter : myset)
			ser_parse(iter, Set);
		Set->SetAttribute("Size", myset.size());
		father->InsertEndChild(Set);

	}
	//map
	template<class T> template<class key, class key_value> void xmlser<T>::ser_parse(const std::map<key, key_value>& mymap, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Map = xml->NewElement("Map");
		Map->SetAttribute("Size", mymap.size());
		for (auto iter : mymap)
			ser_parse(iter, Map);
		father->InsertEndChild(Map);

	}
	//pair is also one of the exits for a certain perspective.
	template<class T> template<class key, class key_value> void xmlser<T>::ser_parse(const std::pair<key, key_value>& mypair, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Pair = xml->NewElement("Pair");
		tinyxml2::XMLElement* First = xml->NewElement("First");
		tinyxml2::XMLElement* Second = xml->NewElement("Second");
		ser_parse(mypair.first,First);
		ser_parse(mypair.second,Second);
		Pair->InsertFirstChild(First);
		Pair->InsertAfterChild(First, Second);
		father->InsertEndChild(Pair);
	}
	//ptr
	template<class T> template<class UPTR> void xmlser<T>::ser_parse(const std::unique_ptr<UPTR>& unq, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Uni_ptr = xml->NewElement("Unique_Ptr");
		ser_parse(*unq,Uni_ptr);
		father->InsertEndChild(Uni_ptr);
	}
	//string
	template<class T> void xmlser<T>::ser_parse(const std::string& value, tinyxml2::XMLElement* father) {
		const char* str_ptr = value.c_str();
		tinyxml2::XMLElement* String = xml->NewElement("String");
		String->SetAttribute("val", str_ptr);
		father->InsertEndChild(String);
	}
	//In  arithmetic type
	template<class T> inline void xmlser< T>::ser_parse(const bool& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Bool = xml->NewElement("Bool");
		Bool->SetAttribute("val", value);
		father->InsertEndChild(Bool);
	}
	template<class T> inline void xmlser< T>::ser_parse(const short& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Short = xml->NewElement("Short");
		std::string temp = std::to_string(value);
		Short->SetAttribute("val", temp.c_str());
		father->InsertEndChild(Short);
	}
	template<class T> inline void xmlser< T>::ser_parse(const unsigned short& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Ushort = xml->NewElement("Ushort");
		int temp = static_cast<int>(value);
		temp &= 0x0000ffff;
		std::string tempstr = std::to_string(temp);
		Ushort->SetAttribute("val", tempstr.c_str());
		father->InsertEndChild(Ushort);
	}
	template<class T> inline void xmlser< T>::ser_parse(const int& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Int = xml->NewElement("Int");
		Int->SetAttribute("val", value);
		father->InsertEndChild(Int);
	}
	template<class T> inline void xmlser< T>::ser_parse(const unsigned int& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* UInt = xml->NewElement("UInt");
		UInt->SetAttribute("val", value);
		father->InsertEndChild(UInt);
	}
	template<class T> inline void xmlser< T>::ser_parse(const unsigned long& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Ulong = xml->NewElement("Ulong");
		std::string temp = std::to_string(value);
		Ulong->SetAttribute("val", temp.c_str());
		father->InsertEndChild(Ulong);
	}
	template<class T> inline void xmlser< T>::ser_parse(const  long& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Long = xml->NewElement("Long");
		Long->SetAttribute("val", value);
		father->InsertEndChild(Long);
	}
	template<class T> inline void xmlser< T>::ser_parse(const  float& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Float = xml->NewElement("Float");
		Float->SetAttribute("val", value);
		father->InsertEndChild(Float);
	}
	template<class T> inline void xmlser< T>::ser_parse(const  double& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Double = xml->NewElement("Double");
		Double->SetAttribute("val", value);
		father->InsertEndChild(Double);
	}
	template<class T> inline void xmlser< T>::ser_parse(const  char& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Char = xml->NewElement("Char");
		char temp[2] = {value,0};
		Char->SetAttribute("val", temp);
		father->InsertEndChild(Char);
	}
	template<class T> inline void xmlser< T>::ser_parse(const unsigned char& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Uchar = xml->NewElement("Uchar");
		std::string temp;
		mystream << value;
		mystream >> temp;
		Uchar->SetAttribute("val", temp.c_str());
		father->InsertEndChild(Uchar);
	}
	template<class T> inline void xmlser< T>::ser_parse(const long double& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Ldouble = xml->NewElement("Ldouble");
		std::string temp(std::to_string(value));
		Ldouble->SetAttribute("val", temp.c_str());
		father->InsertEndChild(Ldouble);
	}
	template<class T> inline void xmlser< T>::ser_parse(const long long& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* LLong = xml->NewElement("LLong");
		std::string temp(std::to_string(value));
		LLong->SetAttribute("val", temp.c_str());
		father->InsertEndChild(LLong);
	}
	template<class T> inline void xmlser< T>::ser_parse(const unsigned long long& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Ullong = xml->NewElement("Ullong");
		std::string temp(std::to_string(value));
		Ullong->SetAttribute("val", temp.c_str());
		father->InsertEndChild(Ullong);
	}
	template<class T> inline void xmlser< T>::ser_parse(const wchar_t& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Wchar = xml->NewElement("Wchar");
		mystream << value;
		std::string temp;
		mystream >> temp;
		Wchar->SetAttribute("val", temp.c_str());
		father->InsertEndChild(Wchar);
	}
	template<class T> inline void xmlser< T>::ser_parse(const char16_t& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Char16_t = xml->NewElement("Char16_t");
		mystream << value;
		std::string temp;
		mystream >> temp;
		Char16_t->SetAttribute("val", temp.c_str());
		father->InsertEndChild(Char16_t);
	}
	template<class T> inline void xmlser< T>::ser_parse(const char32_t& value, tinyxml2::XMLElement* father) {
		tinyxml2::XMLElement* Char32_t = xml->NewElement("Char32_t");
		std::string temp;
		mystream << value;
		mystream >> temp;
		Char32_t->SetAttribute("val", temp.c_str());
		father->InsertEndChild(Char32_t);
	}
	
	template<class T> tinyxml2::XMLElement* xmlser< T >::des(T& putin) {
		using namespace tinyxml2;
		XMLElement* nextNode;
		if (!lastVisit)
		nextNode = xml->RootElement()->FirstChildElement();
		else 
		nextNode = lastVisit->NextSiblingElement();
		des_parse(putin, nextNode);
		return nextNode;
	}
	template<class T> tinyxml2::XMLElement* xmlser< T >::des(std::unique_ptr<T>& putin) {
		using namespace tinyxml2;
		XMLElement* nextNode;
		if(!lastVisit)
		nextNode = xml->RootElement()->FirstChildElement()->FirstChildElement();
		else 
		nextNode = lastVisit->NextSiblingElement()->FirstChildElement();
		des_parse(*putin, nextNode);
		return nextNode;
	}

	//inner interface of all kinds of data structures.
	template<class T>template<class Vec>void xmlser <T>::des_parse(std::vector<Vec>& putin,tinyxml2::XMLElement* me) {
		int size = atoi(me->Attribute("Size"));
		tinyxml2::XMLElement* nextToGo = me->FirstChildElement();
		for (size_t i = 0; i < size; i++)
		{
			Vec temp;
			if(!i)
			des_parse(temp,nextToGo);
			else
			des_parse(temp, nextToGo=nextToGo->NextSiblingElement());
			putin.push_back(temp);
		}
	}
	template<class T>template<class li>void xmlser <T>::des_parse(std::list<li>& putin, tinyxml2::XMLElement* me) {
		int size = atoi(me->Attribute("Size"));
		tinyxml2::XMLElement* nextToGo = me->FirstChildElement();
		for (size_t i = 0; i < size; i++)
		{
			li temp;
			if (!i)
				des_parse(temp, nextToGo);
			else
				des_parse(temp, nextToGo = nextToGo->NextSiblingElement());
			putin.push_back(temp);
		}
	}
	template<class T>template<class ele>void xmlser <T>::des_parse(std::set<ele>& putin, tinyxml2::XMLElement* me) {
		int size = atoi(me->Attribute("Size"));
		tinyxml2::XMLElement* nextToGo = me->FirstChildElement();
		for (size_t i = 0; i < size; i++)
		{
			ele temp;
			if (!i)
				des_parse(temp, nextToGo);
			else
				des_parse(temp, nextToGo = nextToGo->NextSiblingElement());
			putin.insert(temp);
		}
	}
	template<class T>template<class key, class key_value>void xmlser <T>::des_parse(std::map< key, key_value>& putin, tinyxml2::XMLElement* me) {
		int size = atoi(me->Attribute("Size"));
		tinyxml2::XMLElement* nextToGo = me->FirstChildElement();
		for (size_t i = 0; i < size; i++)
		{
			std::pair<key, key_value> temp;
			if (!i)
				des_parse(temp, nextToGo);
			else
				des_parse(temp, nextToGo = nextToGo->NextSiblingElement());
			putin.insert(temp);
		}
	}
	template<class T> template<class key, class key_value>void xmlser <T>::des_parse(std::pair< key, key_value>& putin, tinyxml2::XMLElement* me) {
		tinyxml2::XMLElement* first = me->FirstChildElement();
		des_parse(putin.first, first->FirstChildElement());
		first = first->NextSiblingElement();
		des_parse(putin.second, first->FirstChildElement());
	}
	template<class T>template<class UPTR>void xmlser <T>::des_parse(std::unique_ptr<UPTR>& putin, tinyxml2::XMLElement* me) {
		des_parse(*putin,me->FirstChildElement());
		//指针只能指向一种数据类型
	}
	//The followings are the EXIT of recursion.
	
	
	template<class T> inline void xmlser <T>::des_parse(std::string& putin, tinyxml2::XMLElement* me) {
		putin = me->Attribute("val");
	}
	template<class T> inline void xmlser< T>::des_parse( bool& value, tinyxml2::XMLElement* me) {
		mystream << me->Attribute("val");
		std::string temp;
		mystream >> temp;
		if (temp.find("true") != std::string::npos)
			value = 1;
		else
			value = 0;
	}
	template<class T> inline void xmlser< T>::des_parse( short& value, tinyxml2::XMLElement* me) {
		value = static_cast<short>(std:: atoi(me->Attribute("val")));
	}
	template<class T> inline void xmlser< T>::des_parse( unsigned short& value, tinyxml2::XMLElement* me) {
		value = static_cast<unsigned short>(std:: atoi(me->Attribute("val")));

	}
	template<class T> inline void xmlser< T>::des_parse( int& value, tinyxml2::XMLElement* me) {
		value = std:: atoi(me->Attribute("val"));

	}
	template<class T> inline void xmlser< T>::des_parse( unsigned int& value, tinyxml2::XMLElement* me) {
		value = static_cast<unsigned int>(std:: atoi(me->Attribute("val")));

	}
	template<class T> inline void xmlser< T>::des_parse( unsigned long& value, tinyxml2::XMLElement* me) {
		value = static_cast<unsigned long>(std:: atol(me->Attribute("val")));
	}
	template<class T> inline void xmlser< T>::des_parse(  long& value, tinyxml2::XMLElement* me) {
		value = std:: atol(me->Attribute("val"));

	}
	template<class T> inline void xmlser< T>::des_parse(  float& value, tinyxml2::XMLElement* me) {
		mystream << me->Attribute("val");
		mystream >> value;
	}
	template<class T> inline void xmlser< T>::des_parse(  double& value, tinyxml2::XMLElement* me) {
		value = std:: atof(me->Attribute("val"));

	}
	template<class T> inline void xmlser< T>::des_parse(  char& value, tinyxml2::XMLElement* me) {
		value =  (me->Attribute("val"))[0];
	}
	template<class T> inline void xmlser< T>::des_parse( unsigned char& value, tinyxml2::XMLElement* me) {
		value = static_cast<unsigned char>((me->Attribute("val"))[0]);
	}
	template<class T> inline void xmlser< T>::des_parse( long double& value, tinyxml2::XMLElement* me) {
		value = static_cast<long double>(std::atof(me->Attribute("val")));
	}
	template<class T> inline void xmlser< T>::des_parse( long long& value, tinyxml2::XMLElement* me) {
		value =std::atoll(me->Attribute("val"));
	}
	template<class T> inline void xmlser< T>::des_parse( unsigned long long& value, tinyxml2::XMLElement* me) {
		value = static_cast<unsigned long long>(std::atoll(me->Attribute("val")));
	}
	template<class T> inline void xmlser< T>::des_parse( wchar_t& value, tinyxml2::XMLElement* me) {
		mystream << me->Attribute("val");
		mystream >> &value;
	}
	template<class T> inline void xmlser< T>::des_parse( char16_t& value, tinyxml2::XMLElement* me) {
		mystream << me->Attribute("val");
		mystream >> &value;
	}
	template<class T> inline void xmlser< T>::des_parse( char32_t& value, tinyxml2::XMLElement* me) {
		mystream << me->Attribute("val");
		mystream >> &value;
	}

	//Codes to convert plain text into and from base64.
	void To64(std::string fileName) {
		std::string outfileName(fileName);
		outfileName.replace(outfileName.find(".xml"), 4, ".base64");
		std::fstream infile(fileName, std::ios::in | std::ios::binary);
		std::fstream outfile(outfileName, std::ios::out | std::ios::binary);
		char in[3], out[4];
		char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		do
		{
			in[0] = in[1] = in[2] = out[0] = out[1] = out[2] = out[3] = 0;
			infile.read(in, 1);
			if (!infile.eof())
				infile.read(in + 1, 1);
			else;
			if (!infile.eof())
				infile.read(in + 2, 1);
			else;

			//每次读入三个字节 没有的话0补足并且记录不足的位数。

			out[0] = alpha[in[0] >> 2];
			out[1] = alpha[((in[0] & 0x03) << 4) + ((in[1] & 0xf0) >> 4)];
			out[2] = alpha[((in[1] & 0x0f) << 2) + ((in[2] & 0xc0) >> 6)];
			out[3] = alpha[in[2] & 0x3f];
			outfile.write(out, 4);
		} while (!infile.eof());
		infile.close();
		outfile.close();
	}
	void ChangeChar(char* input, char* output) {
		for (size_t i = 0; i < 4; i++)
		{
			switch (input[i]) {
			case'A':
			case'B':
			case'C':
			case 'D':
			case 'E':
			case 'F':
			case'G':
			case'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
				input[i] = input[i] - 'A';
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				input[i] = input[i] - '0' + 52;
				break;
			case '+':
				input[i] = 62;
				break;
			case '/':
				input[i] = 63;
			default:
				input[i] = input[i] - 'a' + 26;
			}
		}
		output[0] = (input[0] << 2) + ((input[1] & 0x30) >> 4);
		output[1] = ((input[1] & 0xf) << 4) + ((input[2] & 0x3c) >> 2);
		output[2] = ((input[2] & 0x3) << 6) + input[3];
	}
	void From64(std::string outfileName) {
		std::string infileName(outfileName);
		infileName.replace(infileName.find(".xml"), 4, ".base64");
		std::fstream testfile(outfileName, std::ios::in | std::ios::binary);
		if (!testfile.fail())
			return;
		std::fstream outfile(outfileName, std::ios::out | std::ios::binary);
		std::fstream infile(infileName, std::ios::in | std::ios::binary);
		char in[4], out[3];
		do
		{
			int count = 0;
			in[0] = in[1] = in[2] = out[0] = out[1] = out[2] = in[3] = 0;
			infile.read(in, 4);
			if (!in[0] && !in[1] && !in[2] && !in[3])
				break;
			//每次读入三个字节 没有的话0补足并且记录不足的位数。
			ChangeChar(in, out);
			for (size_t i = 0; i < 3; i++)
			{
				if (!out[i])
					count++;
			}
			outfile.write(out, 3 - count);
		} while (!infile.eof());
		infile.close();
		outfile.close();
	}




	//overall wrapper!
	//Userdefined ser to xml.
	template <class T> void serialize( std::string file,T& value ) {
		std::stringstream mystream;
		tinyxml2::XMLDocument xml;
		xmlser<T>myxml(file, value, mystream, &xml);		
		myxml.ser(1);
	}
	template <class T> void serialize( std::string file, std::unique_ptr<T>& value) {
		std::stringstream mystream;
		tinyxml2::XMLDocument xml;
		xmlser<T>myxml(file, value, mystream, &xml);
		myxml.ser(1);
	}
	void Userialize(std::string file) {
		To64(file);
	}
	template<class T1, class ... T2>void Userialize(std::string file, T1& start, T2& ... val) {
		serialize(file, start);
		Userialize(file, val ...);
	}


	template <class T> tinyxml2::XMLElement* deserialize( std::string file, T& putin,tinyxml2::XMLElement* lastVisit=nullptr) {
		From64(file);
		std::stringstream mystream;
		tinyxml2::XMLDocument xml;
		xml.LoadFile(file.c_str());
		xmlser<T>myxml(file, putin, mystream, &xml,lastVisit);
		tinyxml2::XMLElement* nextVisit= myxml.des(putin);
		return nextVisit;
	}
	template <class T> tinyxml2::XMLElement* deserialize( std::string file, std::unique_ptr<T>& putin, tinyxml2::XMLElement* lastVisit = nullptr) {
		From64(file);
		std::stringstream mystream;
		tinyxml2::XMLDocument xml;
		xml.LoadFile(file.c_str());
		xmlser<T>myxml(file, putin, mystream, &xml, lastVisit);
		tinyxml2::XMLElement* nextVisit = myxml.des(putin);
		return nextVisit;
	}
	//functions to shift files to base64 and back to utf-8.


	void Udeserialize(std::string file, tinyxml2::XMLElement* lastVisit) {
		return;
	}
	template<class T1, class ... T2>void Udeserialize(std::string file, tinyxml2::XMLElement* lastVisit, T1& start, T2& ... val) {
		tinyxml2::XMLElement* nextVisit = deserialize(file, start, lastVisit);
		Udeserialize(file, nextVisit, val ...);
	}

}
#endif