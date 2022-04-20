#include <iostream> 
#include <vector>

const int MAX = 100; 

class Entry { 
protected:
	std::string m_identifier;
	std::string m_scope;
	std::string m_type; 
	std::string m_value;
	int m_offset;
	Entry* next; 

public: 
	Entry() { next = nullptr; } 
	Entry(std::string key, std::string scope, std::string type, std::string value, int offset) : m_identifier(key), m_scope(scope), m_type(type), m_value(value), m_offset(offset) { next = nullptr; } 

	void print() { 
		std::cerr << " " << m_identifier << "\t     " << m_type << "\t\t  " << m_scope << " \t    " << m_value << " \t\t" << m_offset << std::endl; 
	} 
	friend class Symbol_Table; 
}; 

class Symbol_Table { 
	Entry* head[MAX]; 

public: 
	Symbol_Table() { 
		for (int i=0; i<MAX; i++) 
			head[i] = nullptr; 
	} 
	int hashf(std::string id); // hash function 
	bool insert(std::string id, std::string scope, std::string type, std::string value, int offset); 
	std::string find(std::string id); 
	std::vector<std::string> find_global();
	std::vector<std::string> find_function();
	std::string what_type(std::string id);
	std::string what_scope(std::string id);
	std::string what_value(std::string id);
	int what_offset(std::string id);
	std::string offset_fp(std::string id);
	int count_local_var();
	bool delete_entry(std::string id); 
	bool modify(std::string id, std::string scope);
	bool modify(std::string id, std::string scope, std::string type);
	bool modify(std::string id, std::string scope, std::string type, std::string value);
	bool modify(std::string id, std::string scope, std::string type, std::string value, int offset);
	void display();
}; 

// Hashing function for indexing entries by id
int Symbol_Table::hashf(std::string id) { 
	int asciiSum = 0; 
	for (unsigned int i = 0; i < id.length(); i++) { 
		asciiSum = asciiSum + id[i]; 
	} 
	return (asciiSum % 100); 
} 

// Function to modify an identifier 
bool Symbol_Table::modify(std::string id, std::string scope) {
	int index = hashf(id); 
	Entry* start = head[index]; 
	if (start == nullptr) 
		return "-1"; 
	while (start != nullptr) { 
		if (start->m_identifier == id) { 
			start->m_scope = scope; 
			return true; 
		} 
		start = start->next; 
	}
	return false; // id not found 
} 
bool Symbol_Table::modify(std::string id, std::string scope, std::string type) { 
	int index = hashf(id); 
	Entry* start = head[index]; 
	if (start == nullptr) 
		return "-1"; 
	while (start != nullptr) { 
		if (start->m_identifier == id) { 
			start->m_scope = scope; 
			start->m_type = type;  
			return true; 
		} 
		start = start->next; 
	}
	return false;
} 
bool Symbol_Table::modify(std::string id, std::string scope, std::string type, std::string value) {
	int index = hashf(id); 
	Entry* start = head[index]; 
	if (start == nullptr) 
		return "-1"; 
	while (start != nullptr) { 
		if (start->m_identifier == id) { 
			start->m_value = value;  
			return true; 
		} 
		start = start->next; 
	}
	return false;
}
bool Symbol_Table::modify(std::string id, std::string scope, std::string type, std::string value, int offset) {
	int index = hashf(id); 
	Entry* start = head[index]; 
	if (start == nullptr) 
		return "-1"; 
	while (start != nullptr) { 
		if (start->m_identifier == id) { 
			start->m_offset = offset;  
			return true; 
		} 
		start = start->next; 
	}
	return false;
}

// Accessing private member variables
std::string Symbol_Table::what_type(std::string id) {
	int index = hashf(id); 
	Entry* start = head[index]; 
	if (start == nullptr) 
		return "-1"; 
	while (start != nullptr) { 
		if (start->m_identifier == id) { 
			return start->m_type;
		} 
		start = start->next; 
	}
	return "-1"; // id not found 
}
std::string Symbol_Table::what_scope(std::string id) {
	int index = hashf(id); 
	Entry* start = head[index]; 
	if (start == nullptr) 
		return "-1"; 
	while (start != nullptr) { 
		if (start->m_identifier == id) { 
			return start->m_scope;
		} 
		start = start->next; 
	}
	return "-1"; // id not found 
}
std::string Symbol_Table::what_value(std::string id) {
	int index = hashf(id); 
	Entry* start = head[index]; 
	if (start == nullptr) 
		return "-1"; 
	while (start != nullptr) { 
		if (start->m_identifier == id) { 
			return start->m_value;
		} 
		start = start->next; 
	}
	return "-1"; // id not found 
}
int Symbol_Table::what_offset(std::string id) {
	int index = hashf(id); 
	Entry* start = head[index]; 
	if (start == nullptr) 
		return -1; 
	while (start != nullptr) { 
		if (start->m_identifier == id) { 
			return start->m_offset;
		} 
		start = start->next; 
	}
	return -1; // id not found 
}

// Function to delete an identifier 
bool Symbol_Table::delete_entry(std::string id)  { 
	int index = hashf(id); 
	Entry* tmp = head[index]; 
	Entry* par = head[index]; 
	if (tmp == nullptr) { return false; } 
	if (tmp->m_identifier == id && tmp->next == nullptr) { 
		tmp->next = nullptr; 
		delete tmp; 
		return true; 
	} 
	while (tmp->m_identifier != id && tmp->next != nullptr) { 
		par = tmp; 
		tmp = tmp->next; 
	} 
	if (tmp->m_identifier == id && tmp->next != nullptr) { 
		par->next = tmp->next; 
		tmp->next = nullptr; 
		delete tmp; 
		return true; 
	} 
	else { 
		par->next = nullptr; 
		tmp->next = nullptr; 
		delete tmp; 
		return true; 
	} 
	return false; 
} 

// Finding entry by identifier
std::string Symbol_Table::find(std::string id) { 
	int index = hashf(id); 
	Entry* start = head[index]; 
	if (start == nullptr) 
		return "-1"; 
	while (start != nullptr) { 
		if (start->m_identifier == id) { 
			//start->print(); 
			return "1"; 
		} 
		start = start->next; 
	} 
	return "-1";
} 
std::vector<std::string> Symbol_Table::find_global() {
	std::vector<std::string> global_vars;
	std::string global = "global var";
	Entry* start;
	for(unsigned i=0; i<MAX; i++) {
		start = head[i];
		if(start != nullptr) {
			if((start->m_scope) == global) {
				global_vars.push_back(start->m_identifier);
			}
		}
	} 
	return global_vars;
}
std::vector<std::string> Symbol_Table::find_function() {
	std::vector<std::string> functions;
	std::string function = "function";
	Entry* start;
	for(unsigned i=0; i<MAX; i++) {
		start = head[i];
		if(start != nullptr) {
			if((start->m_scope) == function) {
				functions.push_back(start->m_identifier);
			}
		}
	} 
	return functions;
}
int Symbol_Table::count_local_var() {
	int sum = 0;
	Entry* start;
	for(unsigned i=0; i<MAX; i++) {
		start = head[i];
		if(start != nullptr) {
			if((start->m_scope) == "local var") {
				sum++;
			}
		}
	}
	return sum;
}

// Inserting an entry
bool Symbol_Table::insert(std::string id, std::string scope, std::string type, std::string value, int offset) { 
	int index = hashf(id); 
	Entry* p = new Entry(id, scope, type, value, offset); 
	if (head[index] == nullptr) { 
		head[index] = p; 
		//std::cerr << "\n" << id << " inserted"; 
		return true; 
	} 
	else { 
		Entry* start = head[index]; 
		while (start->next != nullptr) 
			start = start->next; 
		start->next = p; 
		//std::cerr << "\n" << id << " inserted"; 
		return true; 
	} 
	return false; 
} 


// Printing the table out
void Symbol_Table::display() {
	std::cerr << "============================================================================" << std::endl;
	std::cerr << "|                                 Symbol Table                             |" << std::endl;
	std::cerr << "============================================================================" << std::endl;
	std::cerr << "|    ID     |      Type      |        Scope        |   Value   |   Offset  |" << std::endl;
	std::cerr << "============================================================================" << std::endl;
	for (unsigned i=0; i<MAX; i++) {
		Entry* start = head[i]; 
		if (start == nullptr) 
			;
		else { 
			start->print(); 
		} 
	}
}


