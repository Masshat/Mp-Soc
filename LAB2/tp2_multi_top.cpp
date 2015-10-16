#include <systemc>
#include <limits>
#include <stdlib.h>

#include "vci_gcd_master.h"
#include "vci_gcd_coprocessor.h"
#include "vci_signals.h"
#include "vci_param.h"
#include "mapping_table.h"
#include "vci_vgsb.h"

#define GCD_SIZE	0x10/* ACOMPLETER */
#define GCD_BASE1	0x2000000
#define GCD_BASE2	0x3000000
#define GCD_BASE3	0x4000000
	
int sc_main(int argc, char *argv[])
{
        using namespace sc_core;
	using namespace soclib::caba;
	using namespace soclib::common;

	// VCI fields width definition
	//	cell_size	= 4;
	// 	plen_size	= 8;
	// 	addr_size	= 32;
	// 	rerror_size	= 1;
	// 	clen_size	= 1;
	// 	rflag_size	= 1;
	// 	srcid_size	= 12;
	// 	trdid_size	= 1;
	// 	pktid_size	= 1;
	// 	wrplen_size	= 1;

	typedef VciParams<4, 8, 32, 1, 1, 1, 12, 1, 1, 1> vci_param;

	///////////////////////////////////////////////////////////////////////////
	// simulation arguments : number of cycles & seed for the random generation
	///////////////////////////////////////////////////////////////////////////
        int ncycles = std::numeric_limits<int>::max();
        int seed    = 123456789;
        if (argc > 1) ncycles = atoi(argv[1]) ;
        if (argc > 2) seed = atoi(argv[2]) ;

	//////////////////////////////////////////////////////////////////////////
	// Mapping Table
	//////////////////////////////////////////////////////////////////////////
	MappingTable maptab(32, IntTab(8), IntTab(8), 0x03000000);
	maptab.add(soclib::common::Segment("GCD1", GCD_BASE1, GCD_SIZE, IntTab(0), true));
	std::cout << std::endl << maptab << std::endl;
	maptab.add(soclib::common::Segment("GCD2", GCD_BASE2, GCD_SIZE, IntTab(1), true));
	std::cout << std::endl << maptab << std::endl;
	maptab.add(soclib::common::Segment("GCD3", GCD_BASE3, GCD_SIZE, IntTab(2), true));
	std::cout << std::endl << maptab << std::endl;
	//////////////////////////////////////////////////////////////////////////
        // Signals
	//////////////////////////////////////////////////////////////////////////
        sc_clock               		signal_clk("signal_clk", sc_time( 1, SC_NS ), 0.5 );
        sc_signal<bool> 			signal_resetn("signal_resetn");
        VciSignals<vci_param> 		signal_vci1("signal_vci1");
        VciSignals<vci_param> 		signal_vci2("signal_vci2");
        VciSignals<vci_param> 		signal_vci3("signal_vci3");
        VciSignals<vci_param> 		signal_vci4("signal_vci4");
        VciSignals<vci_param> 		signal_vci5("signal_vci5");
        VciSignals<vci_param> 		signal_vci6("signal_vci6");

	//////////////////////////////////////////////////////////////////////////
	// Components
	//////////////////////////////////////////////////////////////////////////
	
	VciVgsb<vci_param>		vgsb("vgsb", maptab, 3,3);
	
	VciGcdMaster<vci_param> 		master1("master1",IntTab(0),maptab,seed,GCD_BASE1);
	VciGcdCoprocessor<vci_param>	coproc1("coproc1",IntTab(0),maptab); 

    VciGcdMaster<vci_param> 		master2("master2",IntTab(1),maptab,seed,GCD_BASE2);
	VciGcdCoprocessor<vci_param>	coproc2("coproc2",IntTab(1),maptab); 

    VciGcdMaster<vci_param> 		master3("master3",IntTab(2),maptab,seed,GCD_BASE3);
	VciGcdCoprocessor<vci_param>	coproc3("coproc3",IntTab(2),maptab); 
	//////////////////////////////////////////////////////////////////////////
	// Net-List
	//////////////////////////////////////////////////////////////////////////
	master1.p_clk(signal_clk);
	master1.p_resetn(signal_resetn);
	master1.p_vci(signal_vci1);

	master2.p_clk(signal_clk);
	master2.p_resetn(signal_resetn);
	master2.p_vci(signal_vci2);

	master3.p_clk(signal_clk);
	master3.p_resetn(signal_resetn);
	master3.p_vci(signal_vci3);


	coproc1.p_clk(signal_clk);
	coproc1.p_resetn(signal_resetn);
	coproc1.p_vci(signal_vci4);

	coproc2.p_clk(signal_clk);
	coproc2.p_resetn(signal_resetn);
	coproc2.p_vci(signal_vci5);

	coproc3.p_clk(signal_clk);
	coproc3.p_resetn(signal_resetn);
	coproc3.p_vci(signal_vci6);

	vgsb.p_clk(signal_clk);
	vgsb.p_resetn(signal_resetn);
	
	vgsb.p_to_target[0](signal_vci4);
	vgsb.p_to_target[1](signal_vci5);
	vgsb.p_to_target[2](signal_vci6);
	
	vgsb.p_to_initiator[0](signal_vci1);
	vgsb.p_to_initiator[1](signal_vci2);
	vgsb.p_to_initiator[2](signal_vci3);
	//////////////////////////////////////////////////////////////////////////
	// simulation
	//////////////////////////////////////////////////////////////////////////
        sc_start( sc_time( 1, SC_NS ) ) ;
        signal_resetn = false;
        sc_start( sc_time( 1, SC_NS ) ) ;

        signal_resetn = true;
        for ( size_t n=1 ; n<ncycles ; n++) sc_start( sc_time( 1, SC_NS ) ) ;

        return(0);

} // end sc_main

