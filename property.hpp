#ifndef PROPERTY_HPP
#define PROPERTY_HPP

struct PropertyOfObject
{
	PropertyOfObject(int propertyNumber, int objectNumber, int propertyValue):
		propertyNumber_(propertyNumber),
    objectNumber_(objectNumber), 
    propertyValue_(propertyValue)
  {}

	int propertyNumber_;
	int objectNumber_;
	int propertyValue_;
};

#endif