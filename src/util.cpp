#include "util.h"

const LValue &Util::to_lvalue(const Value &val) {
    return dynamic_cast<const LValue&>(val);
}

const RValue &Util::to_rvalue(const Value &val) {
    return dynamic_cast<const RValue&>(val);
}

LValue &Util::to_mut_lvalue(Value &val) {
    return dynamic_cast<LValue&>(val);
}
