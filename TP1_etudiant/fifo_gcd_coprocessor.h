////////////////////////////////////////////////////////////
// This component implements a simple hardware coprocessor
// performing the Larger Common Divider computation.
// It uses two FIFO interfaces :
// It reads successively two (uint32_t) operands in the
// input FIFO, computes the PGCD, and writes the
// (uint32_t) result to the output FIFO.
////////////////////////////////////////////////////////////

#ifndef FIFO_GCD_COPROCESSOR_H
#define FIFO_GCD_COPROCESSOR_H

#include <systemc>
#include <stdint.h>
#include "fifo_ports.h"

namespace soclib {
namespace caba {

////////////////////////
class FifoGcdCoprocessor
	: public sc_core::sc_module
{
	enum coprocessor_fsm_state_e {
	READ_OPA,
    READ_OPB,
    COMPARE,
    DECR_A,
    DECR_B,
    WRITE_RES
	};

	// Registers
	sc_core::sc_signal<int>	      r_fsm;  // FSM state register.
    sc_core::sc_signal<uint32_t>  r_opa;
    sc_core::sc_signal<uint32_t>  r_opb;

protected:
	SC_HAS_PROCESS(FifoGcdCoprocessor);

public:
	// ports
    sc_core::sc_in<bool>                p_clk;
    sc_core::sc_in<bool>                p_resetn;
    soclib::caba::FifoInput<uint32_t>   p_in;
    soclib::caba::FifoOutput<uint32_t>  p_out;
    
	// constructor & destructor
	FifoGcdCoprocessor(
        sc_core::sc_module_name      name
                    );
    ~FifoGcdCoprocessor();
	
private:
	// member functions
	void transition();
    void genMoore();

}; // end class FifoGcdCoprocessor

}}

#endif 

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4

