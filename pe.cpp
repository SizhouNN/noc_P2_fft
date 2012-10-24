#include "pe.h"

#define PI 3.14159265

void PE_base::set_xy(int x, int y)
{
	assert((x_ == -1) && (y_ == -1)); // set once only
	assert((x != -1) && (y != -1)); // must use a legal location

	x_ = x;
	y_ = y;
}

void PE_base::read_input()
{
	in_queue_.push_back(data_in.read());
}

void PE_base::write_output()
{
	if (out_queue_.empty())
	{
		data_out.write(packet());
	}
	else
	{
		data_out.write(out_queue_.front());
		out_queue_.pop_front();
	}
}

void PE_IO::execute()
{
	/*
	// decide if we are going to fire PI
	int r = rand()%100;
	if (r < 80)
	fire_PI();

	// fire PO if the incoming packet is valid
	if ((packet_in_.src_x != -1)
	&& (packet_in_.src_y != -1))
	fire_PO();
	*/
}

void PE_IO::fire_PI()
{
	int P1_x = 1, P1_y = 0;

	packet p(x_, y_, P1_x, P1_y, rand()%100);

	printf("PI: send %d to (%d,%d)\n",
		p.token, p.dest_x, p.dest_y);

	out_queue_.push_back(p);
}

void PE_IO::fire_PO()
{
	/*
	assert((packet_in_.src_x != -1)
	&& (packet_in_.src_y != -1));

	printf("PO: receive %d from (%d,%d)\n",
	packet_in_.token, packet_in_.src_x,	packet_in_.src_y);
	*/
}

void PE_inc::execute()
{
	/*
	// fire the actor if the incoming packet is valid
	if ((packet_in_.src_x != -1)
	&& (packet_in_.src_y != -1))
	fire();
	*/
}

void PE_inc::fire()
{
	/*
	assert((packet_in_.src_x != -1)
	&& (packet_in_.src_y != -1));

	int PO_x = 0, PO_y = 0;
	packet p(x_, y_, PO_x, PO_y, packet_in_.token+1);

	printf("inc(%d,%d): receive %d from (%d,%d), send %d to (%d,%d)\n",
	x_, y_,
	packet_in_.token, packet_in_.src_x,	packet_in_.src_y,
	p.token, p.dest_x, p.dest_y);

	out_queue_.push_back(p);
	*/
}

void PE_green::execute()
{
	/* FUNCTION DESCRIPTION:
	execute PE-green
	fire PE_green under the condition:
	(1) incomming packet valid (src_x, src_y not -1)
	(2) input queue size is 2
	clean input queue for another round of computation
	# - 1
	monitor the input queue
	discard input if the incomming packet is invalid 
	(RULE: queue size must not be ZERO)
	(RULE: if there has been a valid packet in input queue, the next packet shall not be invalid either)
	# - 2
	fire PE_green if the input queue size could accumulate to 2
	## - 1
	NOTE:fire_green()
	## - 2
	clean input queue (behavior)
	*/

	//# - 1
	assert(!in_queue_.empty());
	if (in_queue_.back().src_x == -1)
		in_queue_.pop_back();
	//# - 2
	if (in_queue_.size() == 2)
	{
		//## - 1
		fire_green();
		//## - 2
		in_queue_.clear();
	}
}

void PE_green::fire_green()
{
	/* FUNCTION DESCRIPTION:
	perform "green" computation described as in design
	# - 1
	green computation
	# - 2
	process output queue
	*/

    //# - 1


	double param_former_real = in_queue_.front().real;
	double param_former_imaginary = in_queue_.front().imaginary;
	double param_later_real = in_queue_.back().real;
	double param_later_imaginary = in_queue_.back().imaginary;

	double result_add_real = param_former_real + param_later_real;
	double result_add_imaginary = param_former_imaginary + param_later_imaginary;
	double result_sub_real = param_former_real - param_later_real;
	double result_sub_imaginary = param_former_imaginary - param_later_imaginary;


}

double * PE_green::ALU(double param_former_real, double param_former_imaginary, double param_later_real, double param_later_imaginary, int powerOfw)
{
	/* FUNCTION DESCRIPTION
	As name suggest: ALU
	The function simulates 4 input 4 output FFT element ALU with mux selected by powerOfw
	*/
	const double w_real = cos(PI/4), w_imaginary = -sin(PI/4);



}

void PE_green::linkLayer()
{
}
