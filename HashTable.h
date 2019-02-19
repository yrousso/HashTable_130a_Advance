#include <list>
#include <string>
#include <vector>

class HashTable{

	public:
		HashTable(int size);
		void insert(std::string name);
		void remove(std::string name);
		bool exists(std::string name);
		void print();
		void lookup(std::string name);
		void stat();	
		
	private:
		std::vector<std::list<std::string> > table;
		int hash(std::string name);
		int getPrime(int);	
		int size;//closest prime to raw_size
		void parseIPtoArray(std::string);//function will parse string 123.12.4.014 and store in parsedIP[123,12,4,014]
		std::vector<int> parsedIP;
		void generateCoeff();
		std::vector<int> a;
		void generateb(int);
		int b;
		int insert_success;
		int remove_success;
		int emptySlots;
		int exactly_one_item;
		int max_collision_index;
		std::vector<int> collisions;
		std::vector<int> x;
		int x_and_a_size;
		int msb_size;
		void advBitParse();
	
};
