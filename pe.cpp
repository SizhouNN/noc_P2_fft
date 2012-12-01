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

//Currently the fire_green is only for green
void PE_green::fire_green()
{
	/* FUNCTION DESCRIPTION:
	perform "green" computation described as in design
	# - 1
	green computation
	## - 1
	Get input for ALU, prepare output of ALU
	Call ALU
	# - 2
	process output queue
	*/

    //# - 1
	complex_num * source = new complex_num[2];
	complex_num * result = new complex_num[2];
		
	ALU(source, 0, result);



}

void PE_green::ALU(const complex_num (&input)[2], int powerOfw, complex_num (& output)[2])
{
	/* FUNCTION DESCRIPTION
	*As name suggest: ALU
	*The function simulates 4 input 4 output FFT element ALU with 4to1mux selected by powerOfw
	*/
	complex_num w;
	w.real = cos(PI/4);
	w.imaginary = -sin(PI/4);
	
	complex_num w2 = w * w;
	complex_num w3 = w2 * w;

	switch (powerOfw)
	{
	case 0:
		output[0] = input[0] + input[1];
		output[1] = input[0] - input[1];
		break;
	case 1:
		output[0] = input[0] + input[1] * w;
		output[1] = input[0] - input[1] * w;
		break;
	case 2:
		output[0] = input[0] + input[1] * w2;
		output[1] = input[0] - input[1] * w2;
		break;
	case 3:
		output[0] = input[0] + input[1] * w3;
		output[1] = input[0] - input[1] * w3;
		break;
	}
	


}

void PE_green::linkLayer()
{
	/* FUNCTION DESCRIPTION:
	As the name suggest, the linkLayer() determine where the output packet shall be send to.
	(or even in the future, the linkLayer() shall be responsible to identify where the input packet is from.)

	More param in packet is called for:
	data_level, data_index, source_address, destination_address



}
