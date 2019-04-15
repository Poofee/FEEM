#include "pf_flag.h"

/** Constructor with initialisation to the given flags. */
PF_Flag::PF_Flag(unsigned f):
    flags(f)
{
}


unsigned PF_Flag::getFlags() const {
    return flags;
}

void PF_Flag::resetFlags() {
    flags=0;
}

void PF_Flag::setFlags(unsigned f) {
    flags=f;
}

void PF_Flag::setFlag(unsigned f) {
    flags |= f;
}

void PF_Flag::delFlag(unsigned f) {
    flags &= ~f;
}

void PF_Flag::toggleFlag(unsigned f) {
    flags ^= f;
}

bool PF_Flag::getFlag(unsigned f) const {
    return flags&f;
}
