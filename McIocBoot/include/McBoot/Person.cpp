#include "Person.h"

MC_INIT(Person)
MC_REGISTER_BEAN_FACTORY(Person)
MC_REGISTER_CONTAINER_CONVERTER(DogMap)
MC_REGISTER_CONTAINER_CONVERTER(QList<DogPtr>)
MC_INIT_END

Person::Person()
{
    
}
