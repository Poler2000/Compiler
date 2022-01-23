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
    [[nodiscard]] ValueType get_type() const;
    [[nodiscard]] virtual bool is_initialized() const;
    [[nodiscard]] virtual bool is_compile_time_known() const = 0;
    virtual bool operator==(const Value &rhs) const = 0;
    void inc_priority();
    void inc_priority(int n);
    long long get_priority() const;

private:
    ValueType type;
    long long priority = 0;
};

#endif
