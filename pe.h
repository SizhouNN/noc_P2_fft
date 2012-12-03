#ifndef PE_H
#define PE_H

#include "router.h"
#define MAX_INPUTS 1024

SC_MODULE(PE_base)
{
	sc_in<bool> clock; // port for the global clock

	sc_in<packet> data_in; // from router
	sc_out<packet> data_out; // to router

	void main() // specify the functionality of PE per clock cycle
	{
		read_input();
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

	void set_dest(dim2 l0, dim2 l1, dim2 l2, dim2 l3, dim2 l4, dim2 l5, dim2 l6, dim2 l7);

	virtual ~PE_base() {}

	virtual void init() = 0;
protected:
	std::list<packet> out_queue_; // output queue
	//packet packet_in_; // incoming packet from the router
	std::list<packet> in_queue_; // output queue

	int x_, y_; // location of the PE

	virtual void read_input(); // read a packet from the the router
	virtual void execute() = 0; // abstraction of computations NOT PURE ANY MORE
	virtual void write_output(); // // send a packet to the router
	
	virtual void fire();

	void Linker_layer();
	

	void CPU();
	

	

	//ALU inside PE, specified for Project 2 FFT-DCT

	void ALU(int);
	
public : 
	
	packet pd[1024];
	
	complex_num ALU_in[2];
	complex_num ALU_out[2];
	dim2 CPU_out[2];
	packet linker_out[2];
	dim2 FSM_d[8];
	complex_num mem_cplx[MAX_INPUTS];
	//char mem_c[MAX_INPUT][128];
	int existing_input;
	int current_input;
	complex_num fire_out[8];
	//char filename[128];
}; // PE_base

class PE_unit : public PE_base
{
public:
	PE_unit(const char *name) : PE_base(name) {}
	void init();
protected:
	void execute();

};


// for PI and PO
class PE_I : public PE_base
{
public:
	
	PE_I(const char *name) : PE_base(name) {}
	
	void init();

protected:
	void execute();


	void fire_I2();
	void fire_I();

	

}; // class PE_I


class PE_O : public PE_base
{
public:
	PE_O(const char *name) : PE_base(name) {}
	void init();
protected:
	void execute();
	
	void fire_O();

}; // class PE_O


#endif // PE_H