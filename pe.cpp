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
/*
void PE_IO::execute()
{
	
	// decide if we are going to fire PI
	int r = rand()%100;
	if (r < 80)
	fire_PI();

	// fire PO if the incoming packet is valid
	if ((packet_in_.src_x != -1)
	&& (packet_in_.src_y != -1))
	fire_PO();
	
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
	
	assert((packet_in_.src_x != -1)
	&& (packet_in_.src_y != -1));

	printf("PO: receive %d from (%d,%d)\n",
	packet_in_.token, packet_in_.src_x,	packet_in_.src_y);
	
}

void PE_inc::execute()
{
	
	// fire the actor if the incoming packet is valid
	if ((packet_in_.src_x != -1)
	&& (packet_in_.src_y != -1))
	fire();
	
}
void PE_inc::fire()
{
	
	assert((packet_in_.src_x != -1)
	&& (packet_in_.src_y != -1));

	int PO_x = 0, PO_y = 0;
	packet p(x_, y_, PO_x, PO_y, packet_in_.token+1);

	printf("inc(%d,%d): receive %d from (%d,%d), send %d to (%d,%d)\n",
	x_, y_,
	packet_in_.token, packet_in_.src_x,	packet_in_.src_y,
	p.token, p.dest_x, p.dest_y);

	out_queue_.push_back(p);
	
}*/

//void PE_green::execute()
//{
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
/*
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
}*/

//Currently the fire_green is only for green
//void PE_green::fire_green()
//{
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
/*
    //# - 1
	complex_num * source = new complex_num[2];
	complex_num * result = new complex_num[2];
		
	ALU(source, 0, result);



}*/

void PE_base::set_dest(dim2 l0, dim2 l1, dim2 l2, dim2 l3, dim2 l4, dim2 l5, dim2 l6, dim2 l7)
{
	FSM_d[0] = l0;
	FSM_d[0] = l1;
	FSM_d[0] = l2;
	FSM_d[0] = l3;
	FSM_d[0] = l4;
	FSM_d[0] = l5;
	FSM_d[0] = l6;
	FSM_d[0] = l7;
}

void PE_base::execute()
{
	/* FUNCTION DESCRIPTION
	Fire PE only when the in queue size is 2. And after execute, the in queue size shall be 0 again;


	*/
	if (in_queue_.back().src_x == -1)
	{
		in_queue_.pop_back();

	} else if (in_queue_.size() == 2)
	{
		fire();//fire will call what is necessary in the PE physical structure
		in_queue_.clear();

	}

}

void PE_base::fire()
{
	CPU();
	//Now I have CPU_out and ALU_out
	Linker_layer();
	out_queue_.push_back(linker_out[0]);
	out_queue_.push_back(linker_out[1]);
	
	
}

void PE_base::Linker_layer()
{
	int i;
	for (i = 0; i<2;i++)
	{
		linker_out[i].cplx_n = ALU_out[i];
		linker_out[i].src_x = x_;
		linker_out[i].src_y = y_;
		linker_out[i].dest_x = CPU_out[i].x;
		linker_out[i].dest_y = CPU_out[i].y;
	}
	

	packet tmp = in_queue_.front();
	switch (tmp.info.layer)
	{
	case 0: //x
		linker_out[0].info.layer = 1;
		linker_out[1].info.layer = 1;
		switch (tmp.info.index)
		{
		case 0:
			linker_out[0].info.index = 0;
			linker_out[1].info.index = 1;
			break;
		case 1:
			linker_out[0].info.index = 4;
			linker_out[1].info.index = 5;
			break;
		case 2:
			linker_out[0].info.index = 2;
			linker_out[1].info.index = 3;
			break;
		case 3:
			linker_out[0].info.index = 6;
			linker_out[1].info.index = 7;
			break;
		case 4:
			linker_out[0].info.index = 0;
			linker_out[1].info.index = 1;
			break;
		case 5:
			linker_out[0].info.index = 4;
			linker_out[1].info.index = 5;
			break;
		case 6:
			linker_out[0].info.index = 2;
			linker_out[1].info.index = 3;
			break;
		case 7:
			linker_out[0].info.index = 6;
			linker_out[1].info.index = 7;
			break;
		default:
			printf("Impossible Index\n");
			break;
		}
		break;
	case 1: //y
		linker_out[0].info.layer = 2;
		linker_out[1].info.layer = 2;
		switch (tmp.info.index)
		{
		case 0:
			linker_out[0].info.index = 0;
			linker_out[1].info.index = 1;
			break;
		case 1:
			linker_out[0].info.index = 2;
			linker_out[1].info.index = 3;
			break;
		case 2:
			linker_out[0].info.index = 0;
			linker_out[1].info.index = 1;
			break;
		case 3:
			linker_out[0].info.index = 2;
			linker_out[1].info.index = 3;
			break;
		case 4:
			linker_out[0].info.index = 4;
			linker_out[1].info.index = 5;
			break;
		case 5:
			linker_out[0].info.index = 6;
			linker_out[1].info.index = 7;
			break;
		case 6:
			linker_out[0].info.index = 4;
			linker_out[1].info.index = 5;
			break;
		case 7:
			linker_out[0].info.index = 6;
			linker_out[1].info.index = 7;
			break; 
		default:
			printf("Impossible Index\n");
			break;
		}
		break;
	case 2: //z
		linker_out[0].info.layer = 3;
		linker_out[1].info.layer = 3;
		switch (tmp.info.index)
		{
		case 0:
			linker_out[0].info.index = 0;
			linker_out[1].info.index = 4;
			break;
		case 1:
			linker_out[0].info.index = 2;
			linker_out[1].info.index = 6;
			break;
		case 2:
			linker_out[0].info.index = 1;
			linker_out[1].info.index = 5;
			break;
		case 3:
			linker_out[0].info.index = 3;
			linker_out[1].info.index = 7;
			break;
		case 4:
			linker_out[0].info.index = 0;
			linker_out[1].info.index = 4;
			break;
		case 5:
			linker_out[0].info.index = 2;
			linker_out[1].info.index = 6;
			break;
		case 6:
			linker_out[0].info.index = 1;
			linker_out[1].info.index = 5;
			break;
		case 7:
			linker_out[0].info.index = 3;
			linker_out[1].info.index = 7;
			break;
		default:
			printf("Impossible Index\n");
			break;
		}
		break;
	default:
		printf("Impossible Layer\n");
		break;
	}
	
}


void PE_base::CPU()
{
	assert(!in_queue_.empty());
	assert(in_queue_.size() == 2);

	packet tmp = in_queue_.front();

	switch (tmp.info.layer)
	{
	case 0: //x
		switch (tmp.info.index)
		{
		case 0:
			ALU_in[0] = in_queue_.front().cplx_n;
			ALU_in[1] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[0];
			CPU_out[1] = FSM_d[2];
			break;
		case 1:
			ALU_in[0] = in_queue_.front().cplx_n;
			ALU_in[1] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[4];
			CPU_out[1] = FSM_d[6];
			break;
		case 2:
			ALU_in[0] = in_queue_.front().cplx_n;
			ALU_in[1] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[1];
			CPU_out[1] = FSM_d[3];
			break;
		case 3:
			ALU_in[0] = in_queue_.front().cplx_n;
			ALU_in[1] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[5];
			CPU_out[1] = FSM_d[7];
			break;
		case 4:
			ALU_in[1] = in_queue_.front().cplx_n;
			ALU_in[0] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[0];
			CPU_out[1] = FSM_d[2];
			break;
		case 5:
			ALU_in[1] = in_queue_.front().cplx_n;
			ALU_in[0] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[4];
			CPU_out[1] = FSM_d[6];
			break;
		case 6:
			ALU_in[1] = in_queue_.front().cplx_n;
			ALU_in[0] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[1];
			CPU_out[1] = FSM_d[3];
			break;
		case 7:
			ALU_in[1] = in_queue_.front().cplx_n;
			ALU_in[0] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[5];
			CPU_out[1] = FSM_d[7];
			break;
		default:
			printf("Impossible Index\n");
			break;
		}
		break;
	case 1: //y
		switch (tmp.info.index)
		{
		case 0:
			ALU_in[0] = in_queue_.front().cplx_n;
			ALU_in[1] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[0];
			CPU_out[1] = FSM_d[4];
			break;
		case 1:
			ALU_in[0] = in_queue_.front().cplx_n;
			ALU_in[1] = in_queue_.back().cplx_n;
			ALU(2);
			CPU_out[0] = FSM_d[2];
			CPU_out[1] = FSM_d[6];
			break;
		case 2:
			ALU_in[1] = in_queue_.front().cplx_n;
			ALU_in[0] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[0];
			CPU_out[1] = FSM_d[4];
			break;
		case 3:
			ALU_in[1] = in_queue_.front().cplx_n;
			ALU_in[0] = in_queue_.back().cplx_n;
			ALU(2);
			CPU_out[0] = FSM_d[2];
			CPU_out[1] = FSM_d[6];
			break;
		case 4:
			ALU_in[0] = in_queue_.front().cplx_n;
			ALU_in[1] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[1];
			CPU_out[1] = FSM_d[5];
			break;
		case 5:
			ALU_in[0] = in_queue_.front().cplx_n;
			ALU_in[1] = in_queue_.back().cplx_n;
			ALU(2);
			CPU_out[0] = FSM_d[3];
			CPU_out[1] = FSM_d[7];
			break;
		case 6:
			ALU_in[1] = in_queue_.front().cplx_n;
			ALU_in[0] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[1];
			CPU_out[1] = FSM_d[5];
			break;
		case 7:
			ALU_in[1] = in_queue_.front().cplx_n;
			ALU_in[0] = in_queue_.back().cplx_n;
			ALU(2);
			CPU_out[0] = FSM_d[3];
			CPU_out[1] = FSM_d[7];
			break; 
		default:
			printf("Impossible Index\n");
			break;
		}
		break;
	case 2: //z
		switch (tmp.info.index)
		{
		case 0:
			ALU_in[0] = in_queue_.front().cplx_n;
			ALU_in[1] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[0];
			CPU_out[1] = FSM_d[4];
			break;
		case 1:
			ALU_in[0] = in_queue_.front().cplx_n;
			ALU_in[1] = in_queue_.back().cplx_n;
			ALU(2);
			CPU_out[0] = FSM_d[2];
			CPU_out[1] = FSM_d[6];
			break;
		case 2:
			ALU_in[0] = in_queue_.front().cplx_n;
			ALU_in[1] = in_queue_.back().cplx_n;
			ALU(1);
			CPU_out[0] = FSM_d[1];
			CPU_out[1] = FSM_d[5];
			break;
		case 3:
			ALU_in[0] = in_queue_.front().cplx_n;
			ALU_in[1] = in_queue_.back().cplx_n;
			ALU(3);
			CPU_out[0] = FSM_d[3];
			CPU_out[1] = FSM_d[7];
			break;
		case 4:
			ALU_in[1] = in_queue_.front().cplx_n;
			ALU_in[0] = in_queue_.back().cplx_n;
			ALU(0);
			CPU_out[0] = FSM_d[0];
			CPU_out[1] = FSM_d[4];
			break;
		case 5:
			ALU_in[1] = in_queue_.front().cplx_n;
			ALU_in[0] = in_queue_.back().cplx_n;
			ALU(2);
			CPU_out[0] = FSM_d[2];
			CPU_out[1] = FSM_d[6];
			break;
		case 6:
			ALU_in[1] = in_queue_.front().cplx_n;
			ALU_in[0] = in_queue_.back().cplx_n;
			ALU(1);
			CPU_out[0] = FSM_d[1];
			CPU_out[1] = FSM_d[5];
			break;
		case 7:
			ALU_in[1] = in_queue_.front().cplx_n;
			ALU_in[0] = in_queue_.back().cplx_n;
			ALU(3);
			CPU_out[0] = FSM_d[3];
			CPU_out[1] = FSM_d[7];
			break;
		default:
			printf("Impossible Index\n");
			break;
		}
		break;
	default:
		printf("Impossible Layer\n");
		break;
	}

	//FSM_Controller finished. ALU_out ready for pack-up


	

}

void PE_base::ALU(int powerOfw)
{
	/* FUNCTION DESCRIPTION
	As name suggest: ALU
	The function simulates 2 input 2 output FFT element ALU with 4to1mux selected by powerOfw;
	ALU doesn't care about sequence, it just reads ALU_in[2] and generate ALU_out[2];
	Out layer manage ALU_in and and identify ALU_out;
	*/
	complex_num w;
	w.real = cos(PI/4);
	w.imaginary = -sin(PI/4);
	
	complex_num w2 = w * w;
	complex_num w3 = w2 * w;

	switch (powerOfw)
	{
	case 0:
		ALU_out[0] = ALU_in[0] + ALU_in[1];
		ALU_out[1] = ALU_in[0] - ALU_in[1];
		break;
	case 1:
		ALU_out[0] = ALU_in[0] + ALU_in[1] * w;
		ALU_out[1] = ALU_in[0] - ALU_in[1] * w;
		break;
	case 2:
		ALU_out[0] = ALU_in[0] + ALU_in[1] * w2;
		ALU_out[1] = ALU_in[0] - ALU_in[1] * w2;
		break;
	case 3:
		ALU_out[0] = ALU_in[0] + ALU_in[1] * w3;
		ALU_out[1] = ALU_in[0] - ALU_in[1] * w3;
		break;
	}
	
}

//void PE_green::linkLayer()
//{
	/* FUNCTION DESCRIPTION:
	As the name suggest, the linkLayer() determine where the output packet shall be send to.
	(or even in the future, the linkLayer() shall be responsible to identify where the input packet is from.)

	More param in packet is called for:
	data_level, data_index, source_address, destination_address
	*/


//}

void PE_O::execute()
{
	if (in_queue_.back().src_x == -1)
	{
		in_queue_.pop_back();

	} else if (in_queue_.size() == 8)
	{
		fire_O();
		in_queue_.clear();

	}
}

void PE_O::fire_O()
{
	int i;
	int k;
	for(i=0; i<8; i++)
	{
		k = in_queue_.front().info.index;
		switch(k)
		{
		case 0:
			fire_out[0] = in_queue_.front().cplx_n;
			break;
		case 1:
			fire_out[4] = in_queue_.front().cplx_n;
			break;
		case 2:
			fire_out[1] = in_queue_.front().cplx_n;
			break;
		case 3:
			fire_out[5] = in_queue_.front().cplx_n;
			break;
		case 4:
			fire_out[2] = in_queue_.front().cplx_n;
			break;
		case 5:
			fire_out[6] = in_queue_.front().cplx_n;
			break;
		case 6:
			fire_out[3] = in_queue_.front().cplx_n;
			break;
		case 7:
			fire_out[7] = in_queue_.front().cplx_n;
			break;

		}
		
		
		
		in_queue_.pop_front();
	}

	for(i=0;i<8;i++)
	{
			ofstream myfile("OUTPUT.txt", ios::app);
			if (fire_out[i].imaginary >= 0)
			{
				myfile<<fire_out[i].real << '+' << fire_out[i].imaginary << 'i'<<endl;
			}else
			{
				myfile<<fire_out[i].real <<  fire_out[i].imaginary << 'i'<<endl;
			}
			myfile.close();
	}


}

void PE_I::execute()
{
	int rm = existing_input%8;
	int actual = current_input + rm;
	if (in_queue_.size() == 0 && actual < existing_input)
	{
		fire_I();//put 8 cplx into out_queue_
		current_input += 8;


	}
}

void PE_I::fire_I()
{
	int i;
	int k;
	
	packet tmp;
	tmp.info.layer = 0;
	tmp.src_x = x_;
	tmp.src_y = y_;
	for (i =0;i<8;i++)
	{
		tmp.info.index = i;
		tmp.cplx_n = mem_cplx[current_input + i];
		switch(i)
		{
		case 0:
			tmp.dest_x = FSM_d[0].x;
			tmp.dest_y = FSM_d[0].y;
			break;
		case 1:
			tmp.dest_x = FSM_d[4].x;
			tmp.dest_y = FSM_d[4].y;
			break;
		case 2:
			tmp.dest_x = FSM_d[2].x;
			tmp.dest_y = FSM_d[2].y;
			break;
		case 3:
			tmp.dest_x = FSM_d[6].x;
			tmp.dest_y = FSM_d[6].y;
			break;
		case 4:
			tmp.dest_x = FSM_d[1].x;
			tmp.dest_y = FSM_d[1].y;
			break;
		case 5:
			tmp.dest_x = FSM_d[5].x;
			tmp.dest_y = FSM_d[5].y;
			break;
		case 6:
			tmp.dest_x = FSM_d[3].x;
			tmp.dest_y = FSM_d[3].y;
			break;
		case 7:
			tmp.dest_x = FSM_d[7].x;
			tmp.dest_y = FSM_d[7].y;
			break;
		default:
			printf("invalid index PE_I");
			break;
		}

		out_queue_.push_back(tmp);
	}

}

void PE_I::init()
{
	//#-1 read file into char array of 1024 line by line
	int i;
	existing_input = 0;
	FILE * pFile;
	pFile = fopen ("INPUT.txt" , "r");
	if (pFile == NULL) printf ("Error opening file %s.\n", filename);
	else
	{
		for (i=0;i<MAX_INPUT;i++)
		{
			if ( fgets (mem_c[existing_input] , 128 , pFile) != NULL )
			{
				existing_input++;
			}
		}
       
	}
	//#-2 convert char to complex_num 1024 line by line

	char minus_cut[128];
	char real_cut[128];
	char real_c[128];
	char imag_c[128];
	char * main_operator;
	char * i_operator;
	int counter = 0;
	int sizei;
	size_t sizet;
	for (i=0;i<existing_input++;i++)
	{
		if(mem_c[i][0] == '-') //real part is negative
		{
			strcpy(minus_cut, mem_c[i]+1);
			if(strchr(minus_cut, '-') != NULL)//imag part is negative
			{
				main_operator = strchr(minus_cut, '-');
				i_operator = strchr(minus_cut, 'i');
				//Real part
				sizei = 0;
				while(minus_cut + sizei != main_operator) sizei++;
				sizet = sizei;
				memcpy(real_cut, minus_cut, sizet);
				real_cut[sizei] = '\0';
				real_c[0] = '-';
				memcpy(real_c+1, real_cut, sizet+1);
				//Imag part
				sizei = 0;
				while(main_operator + 1 + sizei != i_operator)sizei++;
				sizet = sizei;
				memcpy(imag_c, main_operator, sizet+1);
				imag_c[sizei+1] = '\0';
				//convert
				mem_cplx[i].real = strtod(real_c, NULL);
				mem_cplx[i].imaginary = strtod(imag_c, NULL);
			}
			else if(strchr(minus_cut, '+') != NULL)//imag is positive
			{
				main_operator = strchr(minus_cut, '+');
				i_operator = strchr(minus_cut, 'i');
				//Real part
				sizei = 0;
				while(minus_cut + sizei != main_operator) sizei++;
				sizet = sizei;
				memcpy(real_cut, minus_cut, sizet);
				real_cut[sizei] = '\0';
				real_c[0] = '-';
				memcpy(real_c+1, real_cut, sizet+1);
				//Imag part
				sizei = 0;
				while(main_operator + 1 + sizei != i_operator)sizei++;
				sizet = sizei;
				memcpy(imag_c, main_operator+1, sizet);
				imag_c[sizei] = '\0';
				//convert
				mem_cplx[i].real = strtod(real_c, NULL);
				mem_cplx[i].imaginary = strtod(imag_c, NULL);
			}
		}else //real part is positive
		{
			if(strchr(mem_c[i], '-') != NULL)//imag part is negative
			{
				main_operator = strchr(mem_c[i], '-');
				i_operator = strchr(mem_c[i], 'i');
				//Real part
				sizei = 0;
				while(mem_c[i] + sizei != main_operator) sizei++;
				sizet = sizei;
				memcpy(real_cut, mem_c[i], sizet);
				real_cut[sizei] = '\0';
				memcpy(real_c, real_cut, sizet+1);
				//Imag part
				sizei = 0;
				while(main_operator + 1 + sizei != i_operator)sizei++;
				sizet = sizei;
				memcpy(imag_c, main_operator, sizet+1);
				imag_c[sizei+1] = '\0';
				//convert
				mem_cplx[i].real = strtod(real_c, NULL);
				mem_cplx[i].imaginary = strtod(imag_c, NULL);
			}
			else if(strchr(mem_c[i], '+') != NULL)//imag is positive
			{
				main_operator = strchr(mem_c[i], '+');
				i_operator = strchr(mem_c[i], 'i');
				//Real part
				sizei = 0;
				while(mem_c[i] + sizei != main_operator) sizei++;
				sizet = sizei;
				memcpy(real_cut, mem_c[i], sizet);
				real_cut[sizei] = '\0';
				memcpy(real_c, real_cut, sizet+1);
				//Imag part
				sizei = 0;
				while(main_operator + 1 + sizei != i_operator)sizei++;
				sizet = sizei;
				memcpy(imag_c, main_operator+1, sizet);
				imag_c[sizei] = '\0';
				//convert
				mem_cplx[i].real = strtod(real_c, NULL);
				mem_cplx[i].imaginary = strtod(imag_c, NULL);
			}
		}
	}
}

void PE_unit::init()
{
	return;
}