class Person {
};


class Room {
public:
    void add_person(Person person)
    {
        // do stuff
    }

private:
    Person* people_in_room;
};


template <class T, int N>
class Bag<T, N> {
};


#pragma semantics huihui
int my_add(int x, int y) {
	return x + y;
}

int main()
{
    Person* p = new Person();
    Bag<Person, 42> bagofpersons;

    return 0;
}
