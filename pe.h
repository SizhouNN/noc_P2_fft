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

protected:
	std::list<packet> out_queue_; // output queue
	//packet packet_in_; // incoming packet from the router
	std::list<packet> in_queue_; // output queue

	int x_, y_; // location of the PE

	virtual void read_input(); // read a packet from the the router
	virtual void execute(); // abstraction of computations NOT PURE ANY MORE
	virtual void write_output(); // // send a packet to the router
	
	virtual void fire();

	void Linker_layer();
	packet linker_out[2];

	void CPU();
	dim2 CPU_out[2];

	dim2 FSM_d[8];

	//ALU inside PE, specified for Project 2 FFT-DCT
	complex_num ALU_in[2];
	complex_num ALU_out[2];
	void ALU(int);

}; // PE_base

// for PI and PO
class PE_IO : public PE_base
{
public:
	PE_IO(const char *name) : PE_base(name) {}

protected:
	void execute();

	void fire_PI();
	void fire_PO();
}; // class PE_IO

// for P1
class PE_inc : public PE_base
{
public:
	PE_inc(const char *name) : PE_base(name) {}

protected:
	void execute();

	void fire();
};

//PROJECT 2
class PE_green : public PE_base
{
public:
		PE_green(const char *name) : PE_base(name) {}

protected:
	void execute();
	void fire_green();
	void linkLayer();
};

class PE_blue : public PE_base
{
public:
		PE_blue(const char *name) : PE_base(name) {}

protected:
	void execute();
	void fire_blue();
};

class PE_orange : public PE_base
{
public:
		PE_orange(const char *name) : PE_base(name) {}

protected:
	void execute();
	void fire_orange();
};

class PE_red : public PE_base
{
public:
		PE_red(const char *name) : PE_base(name) {}

protected:
	void execute();
	void fire_red();
};
#endif // PE_H