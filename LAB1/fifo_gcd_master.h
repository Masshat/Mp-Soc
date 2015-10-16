/////////////////////////////////////////////////////////////////
// This component implements an "hardwired" master able
// to drive the "Larger Common Divider" coprocessor.
// It uses two FIFO interfaces, executing and infinite loop:
// It writes sucessively two (uint32_t) operands to the
// output fifo port, reads the result on the input fifo
// interface (uint32_t), and finally displays the results.
//
// Remark : this component emulates two complex functionnalities
// by using two stdlib functions :
// - the random generation of operands uses the rand() function
// - the result is displayed using the printf() function
//////////////////////////////////////////////////////////////////

#ifndef FIFO_GCD_MASTER_H
#define FIFO_GCD_MASTER_H

#include <systemc>
#include <stdint.h>
#include "fifo_ports.h"

namespace soclib {
namespace caba {

///////////////////
class FifoGcdMaster
	: public sc_core::sc_module
{
	enum fsm_state_e {
		RANDOM,
		WRITE_OPA,
		WRITE_OPB,
        READ_RES,
		DISPLAY,
	};

	// Registers
	sc_core::sc_signal<int>			r_fsm;
        sc_core::sc_signal<uint32_t>   		r_opa;
        sc_core::sc_signal<uint32_t>   		r_opb;
        sc_core::sc_signal<uint32_t>   		r_res;
        sc_core::sc_signal<uint32_t>   		r_cyclecount;
        sc_core::sc_signal<uint32_t>   		r_iterationcount;

protected:
	SC_HAS_PROCESS(FifoGcdMaster);

public:
	// ports
        sc_core::sc_in<bool>			p_resetn;
        sc_core::sc_in<bool> 			p_clk;
        soclib::caba::FifoInput<uint32_t> 	p_in;
        soclib::caba::FifoOutput<uint32_t> 	p_out;

	// constructor & destructor
        FifoGcdMaster( sc_core::sc_module_name insname, int seed );
    	~FifoGcdMaster();

private:
	// member functions
    	void transition();
    	void genMoore();

}; // end class FifoGcdMaster

}}

#endif 

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4

