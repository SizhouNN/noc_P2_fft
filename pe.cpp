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
		if(x_==2&&y_==2)
		{
			//printf("PE22: (%d,%d)&(%d,%d) %d\n", out_queue_.front().dest_x,out_queue_.front().dest_y,out_queue_.back().dest_x,out_queue_.back().dest_y, out_queue_.size());
		}


		//printf("PE_base%d_%d write: #%lf, %lf#\n", x_, y_, out_queue_.front().cplx_n.real, out_queue_.front().cplx_n.imaginary);
		data_out.write(out_queue_.front());
		out_queue_.pop_front();
		//printf("PE_base%d_%d next : #%lf, %lf#\n", x_, y_, out_queue_.front().cplx_n.real, out_queue_.front().cplx_n.imaginary);
	}
}

void PE_base::set_dest(dim2 l0, dim2 l1, dim2 l2, dim2 l3, dim2 l4, dim2 l5, dim2 l6, dim2 l7)
{
	FSM_d[0] = l0;
	FSM_d[1] = l1;
	FSM_d[2] = l2;
	FSM_d[3] = l3;
	FSM_d[4] = l4;
	FSM_d[5] = l5;
	FSM_d[6] = l6;
	FSM_d[7] = l7;
}

void PE_unit::execute()
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
	printf("  PE%d_%d fired\n", x_, y_);
	CPU();
	//Now I have CPU_out and ALU_out
	Linker_layer();
	//printf("OUT0 = %lf, %lf\n", linker_out[1].cplx_n.real, linker_out[1].cplx_n.imaginary);
	out_queue_.push_back(linker_out[0]);
	out_queue_.push_back(linker_out[1]);
	//printf("front: %lf, %lf\n", out_queue_.back().cplx_n.real, out_queue_.back().cplx_n.imaginary);
	
}

void PE_base::Linker_layer()
{
	int i;
	for (i = 0; i<2;i++)
	{
		//printf("OUT0_ALU = %lf, %lf\n", ALU_out[i].real, ALU_out[i].imaginary);
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
	//printf("ALU_IN0 = %lf, %lf\n", in_queue_.front().cplx_n.real, in_queue_.front().cplx_n.imaginary);
	//printf("ALU_IN1 = %lf, %lf\n", in_queue_.back().cplx_n.real, in_queue_.back().cplx_n.imaginary);
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
	//printf("ANS0 = %lf, %lf\n", ALU_out[0].real, ALU_out[0].imaginary);
	//printf("ANS1 = %lf, %lf\n", ALU_out[1].real, ALU_out[1].imaginary);
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
	//printf("PO fired\n");
	int i;
	int k;
	for(i=0; i<8; i++)
	{
		//printf("front: %lf, %lf INDEX:%d (%d)\n", in_queue_.front().cplx_n.real, in_queue_.front().cplx_n.imaginary, in_queue_.front().info.index, in_queue_.front().info.layer);
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
		default:
			printf("IMPOSSIBLE INDEX");
			break;

		}
		//printf("fire_out5: %lf, %lf\n", fire_out[5].real, fire_out[5].imaginary);
		
		
		in_queue_.pop_front();
	}
	for (i=0;i<8;i++)printf("Write to file: %lf, %lf \n", fire_out[i].real,  fire_out[i].imaginary);

	ofstream myfile("OUTPUT.txt", ios::app);
			
	for(i=0;i<8;i++)
	{
			
			

			if (fire_out[i].imaginary >= 0)
			{
				myfile<<fire_out[i].real << '+' << fire_out[i].imaginary << 'i'<<endl;
			}else
			{
				myfile<<fire_out[i].real <<  fire_out[i].imaginary << 'i'<<endl;
			}
			
	}
	myfile.close();

}

void PE_I::execute()
{
	int rm = existing_input%8;
	int actual = current_input + rm;
	//printf("%d, %d", current_input, actual);
	//printf("size = %d", out_queue_.size());
	if (out_queue_.size() == 0 && actual < existing_input)
	{
		fire_I();
		//fire_I();//put 8 cplx into out_queue_
		current_input += 8;


	}

}

void PE_I::fire_I2()
{
	for (int i = 0;i < 8;i++)
	{
		current_input = 0;
		pd[current_input].cplx_n.real = i;

		pd[current_input].cplx_n.imaginary = i;
		pd[current_input].dest_x = 2;
		pd[current_input].dest_y = 2;
		pd[current_input].src_x = 0;
		pd[current_input].dest_y = 0;
		pd[current_input].info.layer = 0;
		pd[current_input].info.index = i;
		pd[current_input].token = 1;
		
		out_queue_.push_back(pd[current_input]);
		
	}
}

void PE_I::fire_I()
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
		//printf("PE_I: #%d, %d#\n", tmp.dest_x, tmp.dest_y);
		printf("PE_I: #%lf, %lf#, INDEX:%d(%d)->(%d, %d)\n", tmp.cplx_n.real, tmp.cplx_n.imaginary, tmp.info.index, tmp.info.layer, tmp.dest_x, tmp.dest_y);
		out_queue_.push_back(tmp);
		//printf("outqueue size = %d\n", out_queue_.size());
	}
	//printf("######################");

}

void PE_I::init()
{
	//#-1 read file into char array of 1024 line by line
	char mem_c[MAX_INPUTS][128];
	int i;
	int j;
	int valid;
	char tmp[128];
	current_input = 0;
	existing_input = 0;
	FILE * pFile;
	pFile = fopen ("INPUT.txt" , "r");

	if (pFile == NULL) 
	{
		printf ("Error opening file %s.\n");
	}
	else
	{
		for (i=0;i<MAX_INPUTS;i++)
		{
			if ( fgets (mem_c[existing_input] , 128 , pFile) != NULL )
			{
				//trim whit space
				valid = 0;
				for (j = 0; j< 128 ; ++j)
				{
					
					if(mem_c[existing_input][j] != ' ')
					{
						tmp[valid]=mem_c[i][j];
						valid++;
					}
				}
				strcpy(mem_c[existing_input], tmp);
				existing_input++;

				//printf(" ### existing = %d, string =  %s\n", existing_input, mem_c[i]);
			}
		}
		fclose(pFile);
       
	}
	
	//#-2 convert char to complex_num 1024 line by line

	//char minus_cut[128];
	//char real_cut[128];
	//char real_c[128];
	//char imag_c[128];
	char * real_c = (char*)malloc(128);
	char * real_cut = (char*)malloc(128);
	char * minus_cut = (char*)malloc(128);
	char * imag_c = (char*)malloc(128);
	
	char * main_operator;
	char * i_operator;
	int counter = 0;
	int sizei;
	size_t sizet;
	for (i=0;i<existing_input;i++)
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
				//printf("i is %d\n", i_operator - mem_c[i]); 
				//Real part
				sizei = 0;
				while(mem_c[i] + sizei != main_operator) sizei++;
				
				sizet = sizei;
				memcpy(real_cut, mem_c[i], sizet);
				real_cut[sizei] = '\0';
				//printf("#%s#\n", real_cut);
				memcpy(real_c, real_cut, sizet+1);
				//Imag part
				sizei = 0;
				//printf("sizei = %d, i is %d\n",sizei, i_operator - main_operator); 
				while((main_operator + 1 + sizei) != i_operator)sizei++;
				//printf("sizei = %d\n",sizei); 
				sizet = sizei;
				memcpy(imag_c, main_operator+1, sizet);
				imag_c[sizei] = '\0';
				//convert
				mem_cplx[i].real = strtod(real_c, NULL);
				mem_cplx[i].imaginary = strtod(imag_c, NULL);
			}
		}
		printf(" ### i = %d, MAX_INPUTS = %d, converted =  %lf+%lfi\n", i, MAX_INPUTS, mem_cplx[i].real, mem_cplx[i].imaginary);
	}
	//printf("#########################\n");
	free(real_c);
	//printf("#########################\n");
	free(real_cut);
	//printf("#########################\n");
	free(minus_cut);
	//printf("#########################\n");
	free(imag_c);
	//printf("#########################\n");
	return;
}

void PE_unit::init()
{
	return;
}

void PE_O::init()
{
	return;
}
