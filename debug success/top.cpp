#include <time.h>
#include <systemc.h>

#include "router.h"
#include "pe.h"

SC_MODULE(top)
{
public:
	


	router *routers;
	PE_base *pes;

	sc_signal<packet> router_to_pe, pe_to_router;

	sc_signal<packet> terminal_loop_north, terminal_loop_south;
	sc_signal<packet> terminal_loop_east, terminal_loop_west;
	sc_signal<bool> clock;

	SC_CTOR(top)
	{
		
		create_pes();
		
		create_network();

	}

protected:
	void create_pes()
	{


		//set up PE_I
		//layer 0
		pes = new PE_I("PE0_0");
		pes->clock(clock);
		pes->set_xy(0, 0);

		pes->init();
		
	
	}

	void create_network()
	{



		// create router
		routers = new router("router0_0");
		routers->set_xy(0,0);
		routers->clock(clock);



		routers->port_in[router::NORTH](
			terminal_loop_north);
		routers->port_out[router::NORTH](
			terminal_loop_north);



		routers->port_in[router::SOUTH](
			terminal_loop_south);
		routers->port_out[router::SOUTH](
			terminal_loop_south);



		routers->port_out[router::WEST](
			terminal_loop_west);
		routers->port_in[router::WEST](
			terminal_loop_west);

		routers->port_out[router::EAST](
			terminal_loop_east);
		routers->port_in[router::EAST](
			terminal_loop_east);


		// connect router to PE
		routers->port_out[router::PE](router_to_pe);
		routers->port_in[router::PE](pe_to_router);
		pes->data_in(router_to_pe);
		pes->data_out(pe_to_router);


	}

}; // top

int sc_main(int argc , char *argv[])
{
	srand(0);

	top top_module("top");
	printf("FINISHED TOP CONSTRUCTOR");

	



	printf("cycle  0 ================================\n");
	sc_start(0, SC_NS);

	for(int i = 1; i < 12; i++){
	
		printf("cycle %2d ================================\n", i);

		top_module.clock.write(1);
		sc_start(10, SC_NS);
		top_module.clock.write(0);
		sc_start(10, SC_NS);
	}


	return 0;
}
