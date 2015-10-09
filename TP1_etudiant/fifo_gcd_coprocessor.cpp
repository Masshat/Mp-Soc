#include "fifo_gcd_coprocessor.h"
using namespace sc_core;

namespace soclib {
namespace caba {

////////////////////////////////////////////////////////////////
FifoGcdCoprocessor::FifoGcdCoprocessor( sc_module_name insname )
	: sc_module(insname),
      	r_fsm("r_fsm"),
      	r_opa("r_opa"),
      	r_opb("r_opb"),
        p_resetn("resetn"),
        p_clk("clk"),
		p_in("p_in"),
		p_out("p_out")
{
	SC_METHOD(transition);
	dont_initialize();
	sensitive << p_clk.pos();
	
	SC_METHOD(genMoore);
	dont_initialize();
	sensitive << p_clk.neg();
}

FifoGcdCoprocessor::~FifoGcdCoprocessor( )
{
}

/////////////////////////////////////
void FifoGcdCoprocessor::transition()
{
	if ( !p_resetn.read() ) {
		r_fsm = READ_OPA;
		return;
	}

	switch ( r_fsm.read() ) {
	case READ_OPA :
		if ( p_in.rok.read() ) {
			r_opa = p_in.data.read();
			r_fsm = READ_OPB;
		}
		break;
	case READ_OPB :
		if ( p_in.rok.read() ) {
			r_opb = p_in.data.read();
			r_fsm = COMPARE;
		}
		break;
	case COMPARE:
		if      ( r_opa.read() < r_opb.read() )  r_fsm = DECR_B;
		else if ( r_opa.read() > r_opb.read() )  r_fsm = DECR_A;
		else                                     r_fsm = WRITE_RES;
		break;
	case DECR_A :
	    r_opa = r_opa.read() - r_opb.read();
	    r_fsm = COMPARE;
	break;
	case DECR_B :
	    r_opb = r_opb.read() - r_opa.read();
	    r_fsm = COMPARE;
	break;
	case WRITE_RES :
		if ( p_out.wok.read() ) {
			r_fsm = READ_OPA;
		}
		break;
        } // end switch
} // end transition()

///////////////////////////////////
void FifoGcdCoprocessor::genMoore()
{
	switch ( r_fsm.read() ) {
	case READ_OPA : 
        case READ_OPB :
		p_in.r 		= true;
		p_out.w		= false;
                p_out.data	= 0;
	break; 
	case COMPARE :
	case DECR_A :
	case DECR_B :
		p_in.r 		= false;
		p_out.w		= false;
                p_out.data	= 0;
	break; 
	case WRITE_RES :
		p_in.r 		= false;
		p_out.w		= true;
                p_out.data	= r_opa.read();
	break; 
     } // end switch
} // end genMoore()

}}

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4

