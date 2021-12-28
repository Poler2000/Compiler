#ifndef COMPILER_V_0_1_VALUE_H
#define COMPILER_V_0_1_VALUE_H

class Value {
public:
    enum ValueType {
        TYPE_ARRAY,
        TYPE_VAR,
        TYPE_NUMBER,
    };
    explicit Value(ValueType type);
    ValueType get_type() const;
private:
    ValueType type;
};

#endif
