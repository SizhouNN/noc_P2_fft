#ifndef PE_H
#define PE_H

#include "router.h"


SC_MODULE(PE_base)
{
	sc_in<bool> clock; // port for the global clock

	sc_in<packet> data_in; // from router
	sc_out<packet> data_out; // to router

	void main() // specify the functionality of PE per clock cycle
	{

		execute();
		write_output();
	}

	SC_CTOR(PE_base)
		: x_(-1), y_(-1)
	{
		SC_METHOD(main);
		sensitive << clock.pos();
	}

	// use this function to set the coordinates of the PE
	void set_xy(int x, int y);



	virtual ~PE_base() {}

	virtual void init() = 0;
protected:
	std::list<packet> out_queue_; // output queue
	
	std::list<packet> in_queue_; // output queue

	int x_, y_; // location of the PE

	
	virtual void execute() = 0; // abstraction of computations NOT PURE ANY MORE
	virtual void write_output(); // // send a packet to the router
	



	
public : 
	
	


	complex_num mem_cplx[1024];

	int existing_input;
	int current_input;


}; // PE_base




// for PI and PO
class PE_I : public PE_base
{
public:
	
	PE_I(const char *name) : PE_base(name) {}
	
	void init();

protected:
	void execute();


	void fire();

	

}; // class PE_I





#endif // PE_H