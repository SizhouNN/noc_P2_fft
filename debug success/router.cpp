#include "router.h"

std::ostream &operator<<(std::ostream &o, const packet &p)
{
	char buf[100];
	sprintf(buf, "(%d,%d)->(%d,%d)",
		p.src_x, p.src_y, p.dest_x, p.dest_y);
	return o << buf << ", " << p.token;
}

void sc_trace(sc_trace_file *tf, const packet &p, const std::string &name)
{
	sc_trace(tf, p.src_x, name+".src.x");
	sc_trace(tf, p.src_y, name+".src.y");
	sc_trace(tf, p.dest_x, name+".dest.x");
	sc_trace(tf, p.dest_y, name+".dest.y");
	sc_trace(tf, p.token, name+".token");
	sc_trace(tf, p.cplx_n.real, name+".cplx_nx");
	sc_trace(tf, p.cplx_n.imaginary, name+".cplx_ny");
	sc_trace(tf, p.info.index, name+".infoi");
	sc_trace(tf, p.info.layer, name+".infol");
}


void router::main()
{
	assert((x_ != -1) && (y_ != -1)); // to identify PE

		read_packet(0);

}

void router::set_xy(int x, int y)
{
	assert((x_ == -1) && (y_ == -1)); // set once only
	assert((x != -1) && (y != -1)); // must use a legal location
	
	x_ = x;
	y_ = y;
}

void router::read_packet(int iport)
{
	assert(iport < PORTS);

	packet packet_in_ = port_in[iport].read();
	

	if ((packet_in_.src_x == -1) && (packet_in_.src_y == -1))
	{
		return; // empty packet
	}

	//printf("ROUTER%d_%d port%d: #%lf, %lf#\n", x_, y_, iport, port_in[iport].read().cplx_n.real, port_in[iport].read().cplx_n.imaginary);
	printf("ROUTER%d_%d port%d: #%lf, %lf#, (%d, %d), (%d)\n", x_, y_, iport, packet_in_.cplx_n.real, packet_in_.cplx_n.imaginary, packet_in_.dest_x, packet_in_.dest_y, packet_in_.info.index);

	route_packet_xy(packet_in_);
}

void router::write_packet(int iport)
{
	assert(iport < PORTS);

	if (out_queue_[iport].empty())
	{
		port_out[iport].write(packet()); // write an empty packet
	}
	else
	{
		port_out[iport].write(out_queue_[iport].front());
		out_queue_[iport].pop_front();
	}
}

void router::route_packet_xy(packet p)
{

	if ((p.dest_x == -1) || (p.dest_y == -1))
	{   
		
		return;
	}


		if (p.dest_x == x_)
		{
			out_queue_[PE].push_back(p);
		}
		else if (p.dest_x < x_) 
		{
			out_queue_[WEST].push_back(p);
		}
		else 
		{
			out_queue_[EAST].push_back(p);
		}


}
