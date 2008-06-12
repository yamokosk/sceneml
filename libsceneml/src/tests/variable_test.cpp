#include <iostream>
#include "smlMath.h"
#include "smlVariable.h"

class VarObs : public sceneml::Observer
{
public:
	VarObs(sceneml::Subject& s) : sceneml::Observer(s) {};

	virtual void notify(bool bExtended) const
	{
		std::cout << "Variable changed! Value = " << ((sceneml::Variable&)subject_).getValue() << std::endl;
	}
};

void main(void)
{
	ColumnVector x(3), y(3);
	x << 1.0 << 2.0 << 3.0;
	
	sceneml::Variable avar(x);
	
	y = avar.getValue();
	
	if (x == y) {std::cout << "x and y are equal!" << std::endl;}
	else {std::cout << "x and y are not equal!" << std::endl;}


	// Create a variable
	sceneml::Variable var(3);
	
	// Create a variable observer
	VarObs obs(var);
	
	// Change the variable data
	ColumnVector a(3);
	a << 1.0 << 2.0 << 3.0;
	var.setValue(a);
}