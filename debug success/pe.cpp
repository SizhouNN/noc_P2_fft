#include "pe.h"



void PE_base::set_xy(int x, int y)
{
	assert((x_ == -1) && (y_ == -1)); // set once only
	assert((x != -1) && (y != -1)); // must use a legal location

	x_ = x;
	y_ = y;
}



void PE_base::write_output()
{
	if (out_queue_.empty())
	{
		data_out.write(packet());
	}
	else
	{
		printf("PE_base%d_%d write: #%lf, %lf#\n", x_, y_, out_queue_.front().cplx_n.real, out_queue_.front().cplx_n.imaginary);
		data_out.write(out_queue_.front());
		out_queue_.pop_front();
		//printf("PE_base%d_%d next : #%lf, %lf#\n", x_, y_, out_queue_.front().cplx_n.real, out_queue_.front().cplx_n.imaginary);
	}
}











void PE_I::execute()
{

	int actual = current_input;
	//printf("%d, %d", current_input, actual);
	//printf("size = %d", out_queue_.size());
	if (out_queue_.size() == 0 && actual < existing_input)
	{
		fire();
		//fire_I();//put 8 cplx into out_queue_
		current_input += 8;


	}

}


void PE_I::fire()
{
	//printf("PI fired");
	int i;
	
	
	packet tmp;
	tmp.info.layer = 0;
	tmp.src_x = x_;
	tmp.src_y = y_;
	for (i =0;i<8;i++)
	{
		tmp.info.index = i;
		tmp.cplx_n = mem_cplx[current_input + i];
		tmp.dest_x = 2;
		tmp.dest_y = 2;

		printf("PE_I: #%lf, %lf#\n", tmp.cplx_n.real, tmp.cplx_n.imaginary);
		
		out_queue_.push_back(tmp);

	}
	

}

void PE_I::init()
{
	existing_input = 8;
	current_input = 0;
	mem_cplx[0].real = 1;
	mem_cplx[1].real = 2;
	mem_cplx[2].real = 3;
	mem_cplx[3].real = 4;
	mem_cplx[4].real = 5;
	mem_cplx[5].real = 6;
	mem_cplx[6].real = 7;
	mem_cplx[7].real = 8;
	mem_cplx[0].imaginary = 1;
	mem_cplx[1].imaginary = 2;
	mem_cplx[2].imaginary = 3;
	mem_cplx[3].imaginary = 4;
	mem_cplx[4].imaginary = 5;
	mem_cplx[5].imaginary = 6;
	mem_cplx[6].imaginary = 7;
	mem_cplx[7].imaginary = 8;

	return;
}

