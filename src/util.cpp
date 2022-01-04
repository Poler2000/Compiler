#include "util.h"

const LValue &Util::to_lvalue(const Value &val) {
    return dynamic_cast<const LValue&>(val);
}

const RValue &Util::to_rvalue(const Value &val) {
    return dynamic_cast<const RValue&>(val);
}
