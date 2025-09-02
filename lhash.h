

#include <vector>
#include <functional>

//INHERITANCE
//Base class that provides an interface the two subclasses will inherit
//This is an abstract (pure virtual) class that does nothing
//But it allows us to avoid copy/pasting code in main because the interface is the same for both hash tables
template <class T, size_t SIZE>
class Hash {
	public:
		virtual ~Hash() {}
		virtual void insert(T new_data) = 0;
		virtual void remove(T old_data) = 0;
		virtual bool search(T test_data) const = 0;
		virtual void change(T old_data, T new_data) = 0;
};

//YOU: Write three hash tables, one for linear probing, one for chaining, one using any other method you want (Quadratic, Double Hashing, or using the STL unordered_set class are all ok)

template <class T, size_t SIZE>
class LinearProbingHash final : public Hash<T,SIZE> { //This class cannot be inherited from
	//This bit of magic makes three constants: STATUS::OPEN, STATUS::FILLED, and STATUS::DELETED
	//OPEN means the bucket has never held a value in it
	//FILLED means it currently holds a valid value in it
	//DELETED means the value in it has been deleted and not replaced yet
	enum class STATUS : char {OPEN, FILLED, DELETED};
	std::vector<T> data;
	std::vector<STATUS> status; //Each element can be STATUS::OPEN, STATUS::FILLED or STATUS::DELETED
	public:
	LinearProbingHash() {
		data.resize(SIZE); //Set the vector to be SIZE big - SIZE is a template parameter
		status.resize(SIZE,STATUS::OPEN); //Set the state of every bucket to STATUS::OPEN
	}
	//RTTI Run Time Type Information - vtable
	//Final turns this off if you get a LinearProbingHash& in a function parameter, it is devirtualized
	void insert(T new_data) final override {
		size_t address = std::hash<T>{}(new_data) % SIZE;
		int firstDeleted = -1;
		int loops = SIZE;

		while (loops > 0){
			if (status.at(address) == STATUS::OPEN){
				if (firstDeleted == - 1) {
					data.at(address) = new_data;
					status.at(address) = STATUS:: FILLED;
				} else {
					data.at(firstDeleted) = new_data;
					status.at(firstDeleted) = STATUS::FILLED;
				}
				return;
			} else if (status.at(address) == STATUS::DELETED && firstDeleted == -1) {
				firstDeleted = address;
			}else if(data.at(address) == new_data && status.at(address) == STATUS::FILLED) {
					return;
			}
			address = (address+1)%SIZE; //Move one bucket to the right, looping around
			loops--;
			}
		if (firstDeleted != -1){
			data.at(address) = new_data;
		status.at(address) = STATUS::FILLED;
	}
	}
	void remove(T old_data) override {
		size_t address = std::hash<T>{}(old_data) % SIZE;
	int loops = SIZE;

	while (loops > 0){
		if (data.at(address) == old_data && status.at(address) == STATUS::FILLED) {
			status.at(address) = STATUS::DELETED;
			return;
		} else {
			address = (address + 1) % SIZE;
		}
	loops--;
	}
}
	bool search(T old_data) const override {
		int loops = SIZE;
		size_t address = std::hash<T>{}(old_data) % SIZE;
			while (true) {
				if(data.at(address) == old_data && status.at(address) == STATUS::FILLED) {
					return true;
				} else {
					address = (address+1)%SIZE;
					if(loops == 0){
						return false;
				} else {
					loops--;
				}
			}
	}
};
	void change(T old_data, T new_data) override {
	size_t address = std::hash<T>{}(old_data) % SIZE;
	int loops= SIZE;
	while (loops > 0) {
		if(data.at(address) == old_data && status.at(address) == STATUS::FILLED) {
			data.at(address) = new_data;
			return;
		} else {
			address = (address + 1) % SIZE;
		}
		loops--;
	}
	}
};
