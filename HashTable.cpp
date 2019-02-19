#include <iostream>
#include "HashTable.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cmath> //for sqrt()
#include <bitset>

HashTable::HashTable(int raw_size){

	//initialize data 
	this->insert_success = 0;
	this->remove_success = 0;
	this->emptySlots = 0;
	this->exactly_one_item = 0;
	this->max_collision_index = 0;
	this->x_and_a_size = 0;
	this->msb_size = 0;

	this->size = getPrime(raw_size);
	table.resize(size);
	collisions.resize(size);
	generateb(raw_size);
	generateCoeff();

}

int HashTable::getPrime(int raw_size)
{
	//neccassary primes close to powers of 2
	unsigned long primes[] = { 2, 3, 5, 11, 17, 37, 67, 131, 257, 521, 1031, 2053, 4099, 8209,
		16411, 32771, 65537, 131101, 262147, 524309, 1048583,
		2097169, 4194319, 8388617, 16777259, 33554467, 67108879,
		134217757, 268435459, 536870923, 1073741827, 2147483659,
		4294967311 };
	for (unsigned long i = 0; i < sizeof(primes); i++)
		if( primes[i] > raw_size)
			return primes[i];	
	std::cout<< "Invalid table size" << std::endl;
	return 0;
}
int HashTable::hash(std::string name){
	parseIPtoArray(name); // parse ip to array
	advBitParse();
	int val_advance = 0;
	for(int i = 0 ; i< x.size() ; i++)
		val_advance += a[x.size()-1-i]*x[i];//[x1,x2,...xn] [an,...a2,a1], a1 coeff is for lsb of bit packets
	return val_advance % size;
}

void HashTable::parseIPtoArray(std::string ip_stream)
{   
	if(!parsedIP.empty())
	{
		parsedIP.clear(); //clear any previously stored ip 
	}
	std::string ip; 
	for(int i = 0 ; i<ip_stream.length(); i++)
	{	    
		if(ip_stream[i] == '.')
		{   
			parsedIP.push_back (std::stoi(ip)); //save ip sectioned in array of ints
			ip = ""; //reset ip for next section
			i++; //move past '.'
		}    
		ip += ip_stream[i]; //save digits in ip as string

		if(i == ip_stream.length() - 1)
		{   
			parsedIP.push_back (std::stoi(ip));  //save last ip sectioned in array of ints
			ip = ""; 
		}   
	}   
}
void HashTable::advBitParse()
{
	// parsedIP[0] ... parsedIP[3]
	std::string ip = std::bitset<8>(parsedIP[0]).to_string() + std::bitset<8>(parsedIP[1]).to_string() +
		std::bitset<8>(parsedIP[2]).to_string() + std::bitset<8>(parsedIP[3]).to_string();
	
		int i = 0;
		int r = 0;
		if(msb_size !=0)
		{
			x[i++] =std::stoi(ip.substr(r,msb_size),0,2);//parse ip for msb on first iteration, then for the concurent size b bit packets; store int in base 2
			r += msb_size;
		}
	while( i < x_and_a_size)
	{
			x[i] =std:: stoi(ip.substr(r,b),0,2); // if msb is same size as all other bit packets
			r += b;
			i++;
	}
}

void HashTable::generateCoeff(){
	msb_size = 32 % b; //get bit size for msb
	if(msb_size % b == 0)
		x_and_a_size = 32/b;
	else
		x_and_a_size = 32/b + 1;

	x.resize(x_and_a_size);
	a.resize(x_and_a_size);

	srand (time(0));
	int t;
	//	for(std::vector<int>::iterator it = a.begin(); it != a.end(); ++it) {
	//	a.push_back (rand() % size + 1);   }
	for(int i = 0; i< a.size() ; i++)
	{
		t = rand() % size + 1;
		a[i] = t;
	}
	//HARDCODE TESTING DATA:
	// BASIC
	 /*
       	  a[0] = 10;
	  a[1] = 89;
	  a[2] = 210;
	  a[3] = 162;
	 */ 
	
	//Advanced Diff WORKING!
	/*	
	a[0] = 9;
	a[1] = 24;
	a[2] = 55;
	a[3] = 9;
	a[4] = 20;
	a[5] = 58;
	*/
	//Advance Delete before insert WORKING! 
	/*
	   a[0] = 127;
	   a[1] = 159;
	   a[2] = 124;
	   a[3] = 136;
	   */
	// Advance Delete after Delete WORKING! 
	/*	
	   a[0] = 104;
	   a[1] = 643;
	   a[2] = 910;
	   a[3] = 96;
	*/ 
	//Advance Delete after Delete out 2 WORKING!
	/*	
	a[0] = 10;
	a[1] = 11;
	a[2] = 5;
	a[3] = 14;
	a[4] = 3;
	a[5] = 8;
	a[6] = 2;
	a[7] = 13;
	*/
}
void HashTable::generateb(int raw_size)
{
	b = log2 (raw_size);
}

bool HashTable::exists(std::string name){
	int index = hash(name);
	std::list<std::string>::iterator it;
	for(it = table[index].begin(); it != table[index].end(); it++){
		if(name.compare(*it) == 0){
			return true;
		}
	}
	return false;
}

void HashTable::insert(std::string name){
	if(exists(name)){
		std::cout << name << " already exists" << std::endl;
		return;
	}
	int index = hash(name);
	table[index].push_back(name);
	insert_success++;
	collisions[index] += 1;	
	return;
}

void HashTable::remove(std::string name){
	int index = hash(name);
	std::list<std::string>::iterator it;
	for(it = table[index].begin(); it != table[index].end(); it++){
		if(name.compare(*it) == 0){
			table[index].erase(it);
			remove_success++;
			return;
		}
	}
	std::cout << "Error : could not delete " << name << " because it does not exist" << std::endl;
	return;
}

void HashTable::print(){
	for(int i = 0; i < size; i++){
		std::list<std::string>::iterator it;
		std::cout << i << ": ";
		for(it = table[i].begin(); it != table[i].end(); it++){
			std::cout << *it << ' ';
		}
		std::cout << std::endl;
	}
}

void HashTable::lookup(std::string name)
{
	if(exists(name))
		std::cout << name << ": found" << std::endl;
	else
		std::cout << name << ": not found" << std::endl;
}

void HashTable::stat(){
	max_collision_index = 0;
	for(int i = 0; i < size; i++)
	{
		if(table[i].size() == 0)		
			emptySlots++;
		if(table[i].size() == 1)		
			exactly_one_item++;

		if(collisions[i] > collisions[max_collision_index])
			max_collision_index = i;
		else if(collisions[i] == collisions[max_collision_index])
			if(i >  max_collision_index)
				max_collision_index = i;
	}
	for(std::vector<int>::iterator it = a.begin(); it != a.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;	
	std::cout << insert_success << std::endl;
	std::cout << remove_success << std::endl;
	std::cout << emptySlots << std::endl;
	std::cout << exactly_one_item << std::endl;
	std::cout << collisions[max_collision_index] << " " << max_collision_index << std::endl;
}
