#include "pix-internal.h"

// fill in the rest of your equiv stuff.

void equiv_print(const char *msg) {
    pix_equiv_t c = pix_get_curproc()->ctx;

    if(!c.n_inst)
        return;
    printk("%s\n", msg);
    if(c.n_inst)
        trace("EQUIV:\tnumber instructions = %d\n", c.n_inst);
    if(c.reg_hash)
        trace("EQUIV:\treg hash = %x\n", c.reg_hash);
    // don't print pc hash if we print reg hash.
    else if(c.pc_hash)
        trace("EQUIV:\tpc hash = %x\n", c.pc_hash);
}

